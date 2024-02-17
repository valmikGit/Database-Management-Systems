#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pds.h"

void bst_print_custom(struct BST_Node *root)
{
	if (root == NULL)
		return;
	else
	{
		// printf("%d ", root->key);
		fwrite(root->data, sizeof(root->data), 1, repo_handle.pds_ndx_fp);
		bst_print(root->left_child);
		bst_print(root->right_child);
	}
}

struct PDS_RepoInfo repo_handle;

// pds_create
// Open the data file and index file in "wb" mode
// Initialize index file by storing "0" to indicate there are zero entries in index file
// close the files
int pds_create(char *repo_name)
{
	char *temp_dat = (char *)malloc(sizeof(repo_name));
	char *temp_ndx = (char *)malloc(sizeof(repo_name));
	strcpy(temp_dat, repo_name);
	strcpy(temp_ndx, repo_name);
	strcat(temp_dat, ".dat");
	strcat(temp_ndx, ".ndx");
	repo_handle.pds_data_fp = fopen(temp_dat, "wb");
	repo_handle.pds_ndx_fp = fopen(temp_ndx, "wb");
	if (repo_handle.pds_data_fp && repo_handle.pds_ndx_fp)
	{
		int zero = 0;
		fwrite(&zero, sizeof(int), 1, repo_handle.pds_ndx_fp);
		fclose(repo_handle.pds_data_fp);
		fclose(repo_handle.pds_ndx_fp);
		repo_handle.repo_status = PDS_REPO_CLOSED;
		free(temp_dat);
		free(temp_ndx);
		return PDS_SUCCESS;
	}
	free(temp_dat);
	free(temp_ndx);
	return PDS_FILE_ERROR;
}

// pds_open
// Open the data file and index file in rb+ mode
// Update the fields of PDS_RepoInfo appropriately
// Read the number of records form the index file
// Load the index into the array and store in ndx_array by reading index entries from the index file
// Close only the index file
int pds_open(char *repo_name, int rec_size)
{
	strcpy(repo_handle.pds_name, repo_name);
	char *temp_dat = (char *)malloc(sizeof(repo_name));
	strcpy(temp_dat, repo_name);
	strcat(temp_dat, ".dat");
	char *temp_ndx = (char *)malloc(sizeof(repo_name));
	strcpy(temp_ndx, repo_name);
	strcat(temp_ndx, ".ndx");
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
	{
		repo_handle.pds_data_fp = fopen(temp_dat, "rb+");
		repo_handle.pds_ndx_fp = fopen(temp_ndx, "rb+");
		repo_handle.rec_size = rec_size;
		if (pds_load_ndx() == PDS_SUCCESS)
		{
			fclose(repo_handle.pds_ndx_fp);
			repo_handle.repo_status = PDS_REPO_OPEN;
			free(temp_dat);
			free(temp_ndx);
			return PDS_SUCCESS;
		}
		else
		{
			repo_handle.repo_status = PDS_REPO_OPEN;
			free(temp_dat);
			free(temp_ndx);
			return PDS_NDX_SAVE_FAILED;
		}
	}
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		// printf("Repo was already open.\n");
		free(temp_dat);
		free(temp_ndx);
		return PDS_REPO_ALREADY_OPEN;
	}
	free(temp_dat);
	free(temp_ndx);
	return PDS_FILE_ERROR;
}

// pds_load_ndx - Internal function
// Load the index entries into the BST pds_bst by calling bst_add_node repeatedly for each
// index entry. Unlike array, for BST, you need read the index file one by one in a loop
int pds_load_ndx()
{
	int read_NDX = fread(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
	struct PDS_NdxInfo *temp_Ndx_Array = (struct PDS_NdxInfo *)malloc(read_NDX * sizeof(struct PDS_NdxInfo));
	fread(temp_Ndx_Array, sizeof(struct PDS_NdxInfo), read_NDX, repo_handle.pds_ndx_fp);
	if (read_NDX == 0)
	{
		free(temp_Ndx_Array);
		return PDS_NDX_SAVE_FAILED;
	}
	for (int i = 0; i < read_NDX; i++)
	{
		bst_add_node(&repo_handle.pds_bst, temp_Ndx_Array[i].key, &(temp_Ndx_Array[i]));
	}
	free(temp_Ndx_Array);
	return PDS_SUCCESS;
}

// put_rec_by_key
// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to BST by calling bst_add_node
// Increment record count
// Write the record at the end of the file
// Return failure in case of duplicate key
int put_rec_by_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		if (fseek(repo_handle.pds_data_fp, 0, SEEK_END) == 0)
		{
			struct BST_Node *temp_BST_Struct;
			if ((temp_BST_Struct = bst_search(repo_handle.pds_bst, key)) == NULL)
			{
				int offset = ftell(repo_handle.pds_data_fp);
				struct PDS_NdxInfo *temp_Struct = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
				temp_Struct->key = key;
				temp_Struct->offset = offset;
				repo_handle.rec_count++;
				temp_Struct->is_deleted = 0;
				fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
				int status = bst_add_node(&repo_handle.pds_bst, key, temp_Struct);
				if (status == BST_SUCCESS)
				{
					// free(temp_Struct);
					return PDS_SUCCESS;
				}
				else
				{
					// free(temp_Struct);
					return PDS_ADD_FAILED;
				}
			}
			else
			{
				struct PDS_NdxInfo *temp_Struct = (struct PDS_NdxInfo *)(temp_BST_Struct->data);
				if (temp_Struct->is_deleted)
				{
					temp_Struct->is_deleted = 0;
					repo_handle.rec_count++;
					temp_Struct->offset = ftell(repo_handle.pds_data_fp);
					fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
					// temp_Struct->key = key;
					return PDS_SUCCESS;
				}
				return PDS_ADD_FAILED;
			}
			return PDS_ADD_FAILED;
		}
		return PDS_FILE_ERROR;
	}
	else
	{
		return PDS_REPO_NOT_OPEN;
	}
	return PDS_ADD_FAILED;
}

// get_rec_by_key
// Search for index entry in BST by calling bst_search
// Seek to the file location based on offset in index entry
// Read the record from the current location
int get_rec_by_ndx_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		struct BST_Node *temp_BST_Struct;
		if ((temp_BST_Struct = bst_search(repo_handle.pds_bst, key)) != NULL)
		{
			struct PDS_NdxInfo *temp_Struct = (struct PDS_NdxInfo *)(temp_BST_Struct->data);
			if (temp_Struct->is_deleted == 0)
			{
				if (fseek(repo_handle.pds_data_fp, temp_Struct->offset, SEEK_SET) == 0)
				{
					fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
					return PDS_SUCCESS;
				}
				return PDS_FILE_ERROR;
			}
			return PDS_REC_NOT_FOUND;
		}
		return PDS_REC_NOT_FOUND;
	}
	return PDS_REPO_NOT_OPEN;
}

/*
This function returns the corresponding ndx key for the given record at a particular io_count.
Let us say we know that we find a particular record at io_count = k.
Then, in the .dat file the location of this record will be = k * repo_handle.rec_size.
Thus, using fseek() we can set the repo_handle.pds_data_fp at this point.
Now we will read only the key/id (ndx key) of the record and return it.
If fseek() does not return 0 then we return PDS_FILE_ERROR.
*/

int get_key_for_given_non_ndx_key(int *io_count)
{
	if (fseek(repo_handle.pds_data_fp, (*io_count) * repo_handle.rec_size, SEEK_SET) == 0)
	{
		int key;
		fread(&key, sizeof(int), 1, repo_handle.pds_data_fp);
		return key;
	}
	return PDS_FILE_ERROR;
}

int get_rec_by_non_ndx_key(void *non_ndx_key, void *rec, int (*matcher)(void *rec, void *non_ndx_key), int *io_count)
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		if (fseek(repo_handle.pds_data_fp, 0, SEEK_SET) == 0)
		{
			void *temp_Pointer = malloc(repo_handle.rec_size);
			for (int i = 0; i < repo_handle.rec_count; i++)
			{
				(*io_count)++;
				fread(temp_Pointer, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
				if (matcher(temp_Pointer, non_ndx_key) == 0)
				{
					(*io_count)--;
					/*
					We need to decrement the io_count by 1 before paasing it to the get_key_for_given_non_ndx_key() function. Because we have post increment operator on line 241 which will increment the io_count by 1 in the next occurence of the io_count.
					*/
					int key = get_key_for_given_non_ndx_key(io_count);
					(*io_count)++;
					/*
					Here, we need to increment the io_count by 1 because we have decremented it initially by 1 for passing it to the get_key_for_given_non_ndx_key() function. However, for the testcases checker we need to update the value of the io_count to its original one.
					*/
					if (key == PDS_FILE_ERROR)
					{
						// Return file error.
						return PDS_FILE_ERROR;
					}
					struct BST_Node *temp_BST_Struct = bst_search(repo_handle.pds_bst, key);
					struct PDS_NdxInfo *temp_Struct = (struct PDS_NdxInfo *)(temp_BST_Struct->data);
					if (temp_Struct->is_deleted)
					{
						// If is_deleted is 1 then the record is deleted and thus, not found.
						return PDS_REC_NOT_FOUND;
					}
					fseek(repo_handle.pds_data_fp, temp_Struct->offset, SEEK_SET);
					// Set the .dat file pointer at the offset of the record and read the record and return 1.
					fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
					return 1;
				}
			}
			return PDS_REC_NOT_FOUND;
		}
		return PDS_FILE_ERROR;
	}
	return PDS_REPO_NOT_OPEN;
}

int delete_rec_by_ndx_key(int key)
{
	struct BST_Node * temp_BST_Struct;
	if ((temp_BST_Struct = bst_search(repo_handle.pds_bst, key)) != NULL)
	{
		struct PDS_NdxInfo *temp_Struct = (struct PDS_NdxInfo *)(temp_BST_Struct->data);
		if (temp_Struct->is_deleted)
		{
			return PDS_DELETE_FAILED;
		}
		/*
		NOTE - We will not decrement the repo_handle.rec_count by 1. Let us understand the reason behind by this through an example.
		Assume the key-offset-is_deleted in the .ndx file looks like this:
		10000 10 0
		10001 20 0
		10002 30 0
		10003 40 0
		10004 50 0
		Currently, repo_handle.rec_count is 5. Now, let us say when we want to delete the record with key 10001. Now, if after toggling the is_deleted attribute the record with key 10001 to 1, we decrement the repo_handle.rec_count by 1. Let us say in the next query, we want to read the record with key 10004. Now our repo_handle.rec_count is 4. Now, we would never be able to reach the end of the file because our loop will run 4 times to search but our required record can only be accessed in the 5th iteration. Thus, we should not decrement the repo_handle.rec_count.
		*/
		temp_Struct->is_deleted = 1;
		return PDS_SUCCESS;
	}
	return PDS_DELETE_FAILED;
}

// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Store the number of records
// Unload the ndx_array into the index file by traversing the BST in pre-order mode (overwrite the entire index file)
// Think why should it NOT be in-order?
// Close the index file and data file
int pds_close()
{
	char *temp = (char *)malloc(sizeof(repo_handle.pds_name));
	strcpy(temp, repo_handle.pds_name);
	strcat(temp, ".ndx");
	repo_handle.pds_ndx_fp = fopen(temp, "wb");
	if (repo_handle.pds_ndx_fp != NULL)
	{
		fwrite(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
		bst_print_custom(repo_handle.pds_bst);
		fclose(repo_handle.pds_data_fp);
		fclose(repo_handle.pds_ndx_fp);
		repo_handle.repo_status = PDS_REPO_CLOSED;
		free(temp);
		return PDS_SUCCESS;
	}

	return PDS_FILE_ERROR;
}