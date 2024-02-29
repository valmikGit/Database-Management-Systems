#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pds.h"
#include "student_course.h"

void bst_print_custom(struct BST_Node *root)
{
	if (root == NULL)
		return;
	else
	{
		// printf("%d ", root->key);
		fwrite(root->data, sizeof(root->data), 1, repo_handle.pds_ndx_fp);
		bst_print_custom(root->left_child);
		bst_print_custom(root->right_child);
	}
}

struct PDS_RepoInfo repo_handle;

// pds_create
// Open the data file and index file in "wb" mode
// Initialize index file by storing "0" to indicate there are zero entries in index file
// close the files
int pds_create(char *repo_name, char *linked_repo_name)
{
	char *temp_dat = (char *)malloc(strlen(repo_name));
	char *temp_ndx = (char *)malloc(strlen(repo_name));
	strcpy(temp_dat, repo_name);
	strcpy(temp_ndx, repo_name);
	strcat(temp_dat, ".dat");
	strcat(temp_ndx, ".ndx");
	char *temp_linked_dat = NULL;
	char *temp_link = NULL;
	int flag = 0;
	if (linked_repo_name != NULL)
	{
		flag = 1;
		temp_linked_dat = (char *)malloc(strlen(linked_repo_name));
		temp_link = (char *)malloc(strlen(repo_name) + strlen(linked_repo_name) + 1);
		strcpy(temp_linked_dat, linked_repo_name);
		strcpy(temp_link, repo_name);
		strcat(temp_link, "_");
		strcat(temp_link, temp_linked_dat);
		strcat(temp_linked_dat, ".dat");
		strcat(temp_link, ".dat");
		repo_handle.pds_linked_data_fp = fopen(temp_linked_dat, "wb");
		repo_handle.pds_link_fp = fopen(temp_link, "wb");
	}
	else
	{
		printf("In pds create, linked_repo_name is NULL\n");
		repo_handle.pds_data_fp = NULL;
		repo_handle.pds_linked_data_fp = NULL;
	}
	repo_handle.pds_data_fp = fopen(temp_dat, "wb");
	repo_handle.pds_ndx_fp = fopen(temp_ndx, "wb");
	if (repo_handle.pds_data_fp && repo_handle.pds_ndx_fp)
	{
		int zero = 0;
		fwrite(&zero, sizeof(int), 1, repo_handle.pds_ndx_fp);
		fclose(repo_handle.pds_data_fp);
		fclose(repo_handle.pds_ndx_fp);
		if (flag)
		{
			fclose(repo_handle.pds_link_fp);
			fclose(repo_handle.pds_linked_data_fp);
		}
		repo_handle.repo_status = PDS_REPO_CLOSED;
		free(temp_dat);
		free(temp_ndx);
		if (flag)
		{
			free(temp_linked_dat);
			free(temp_link);
		}
		return PDS_SUCCESS;
	}
	free(temp_dat);
	free(temp_ndx);
	if (flag)
	{
		free(temp_linked_dat);
		free(temp_link);
	}
	return PDS_FILE_ERROR;
}

// pds_open - CHANGED
// Open the main data file and index file in rb+ mode
// If linked_repo_name is NOT NULL
//     Open the linked data file in rb+ mode (there is no index file for linked data)
//     Open the link file in rb+ mode
// end if
// Update the fields of PDS_RepoInfo appropriately
// Build BST and store in pds_bst by reading index entries from the index file
// Close only the index file

int pds_open(char *repo_name, char *linked_repo_name, int rec_size, int linked_rec_size)
{
	strcpy(repo_handle.pds_name, repo_name);
	char *temp_dat = (char *)malloc(strlen(repo_name));
	strcpy(temp_dat, repo_name);
	strcat(temp_dat, ".dat");
	char *temp_ndx = (char *)malloc(strlen(repo_name));
	strcpy(temp_ndx, repo_name);
	strcat(temp_ndx, ".ndx");
	char *temp_linked_dat = NULL;
	char *temp_link = NULL;
	int flag = 0;
	if (linked_repo_name != NULL)
	{
		flag = 1;
		temp_linked_dat = (char *)malloc(strlen(linked_repo_name));
		temp_link = (char *)malloc(strlen(repo_name) + strlen(linked_repo_name) + 1);
		strcpy(temp_linked_dat, linked_repo_name);
		strcpy(temp_link, repo_name);
		strcat(temp_link, "_");
		strcat(temp_link, linked_repo_name);
		strcat(temp_linked_dat, ".dat");
		strcat(temp_link, ".dat");
	}
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
	{
		repo_handle.pds_data_fp = fopen(temp_dat, "rb+");
		repo_handle.pds_ndx_fp = fopen(temp_ndx, "rb+");
		if (linked_repo_name != NULL)
		{
			repo_handle.pds_linked_data_fp = fopen(temp_linked_dat, "rb+");
			repo_handle.pds_link_fp = fopen(temp_link, "rb+");
			repo_handle.linked_rec_size = linked_rec_size;
		}
		else
		{
			repo_handle.pds_linked_data_fp = NULL;
			repo_handle.pds_link_fp = NULL;
			repo_handle.linked_rec_size = 0;
		}
		repo_handle.rec_size = rec_size;
		if (pds_load_ndx() == PDS_SUCCESS)
		{
			fclose(repo_handle.pds_ndx_fp);
			repo_handle.repo_status = PDS_REPO_OPEN;
			free(temp_dat);
			free(temp_ndx);
			if (flag)
			{
				free(temp_linked_dat);
				free(temp_link);
			}
			return PDS_SUCCESS;
		}
		else
		{
			repo_handle.repo_status = PDS_REPO_OPEN;
			free(temp_dat);
			free(temp_ndx);
			if (flag)
			{
				free(temp_linked_dat);
				free(temp_link);
			}

			return PDS_NDX_SAVE_FAILED;
		}
	}
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		free(temp_dat);
		free(temp_ndx);
		if (flag)
		{
			free(temp_linked_dat);
			free(temp_link);
		}
		return PDS_REPO_ALREADY_OPEN;
	}
	free(temp_dat);
	free(temp_ndx);
	if (flag)
	{
		free(temp_linked_dat);
		free(temp_link);
	}
	return PDS_FILE_ERROR;
}

// pds_load_ndx - Internal function
// Load the index entries into the BST pds_bst by calling bst_add_node repeatedly for each
// index entry. Unlike array, for BST, you need read the index file one by one in a loop
int pds_load_ndx()
{
	int read_NDX = fread(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
	if (read_NDX == 0)
	{
		return PDS_NDX_SAVE_FAILED;
	}
	if (repo_handle.rec_count == 0)
	{
		return PDS_SUCCESS;
	}
	struct PDS_NdxInfo *temp_Ndx_Array = (struct PDS_NdxInfo *)malloc(repo_handle.rec_count * sizeof(struct PDS_NdxInfo));
	fread(temp_Ndx_Array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, repo_handle.pds_ndx_fp);
	for (int i = 0; i < repo_handle.rec_count; i++)
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

// put_linked_rec_by_key
// Seek to the end of the linked data file
// No need to create index entry
// Write the key at the current data file location
// Write the record after writing the key
int put_linked_rec_by_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		if (fseek(repo_handle.pds_linked_data_fp, 0, SEEK_END) == 0)
		{
			fwrite(&key, sizeof(int), 1, repo_handle.pds_linked_data_fp);
			fwrite(rec, repo_handle.linked_rec_size, 1, repo_handle.pds_linked_data_fp);
			return PDS_SUCCESS;
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

// get_linked_rec_by_key - NEW
// Do a linear search of the given key in the linked data file
int get_linked_rec_by_key(int key, void *rec)
{
	int *temp_Key = (int *)malloc(sizeof(int));
	if (fseek(repo_handle.pds_linked_data_fp, 0, SEEK_SET) == 0)
	{
		while (fread(temp_Key, sizeof(int), 1, repo_handle.pds_linked_data_fp) > 0)
		{
			if (*temp_Key == key)
			{
				fread(rec, repo_handle.linked_rec_size, 1, repo_handle.pds_linked_data_fp);
				return PDS_SUCCESS;
			}
			fseek(repo_handle.pds_linked_data_fp, repo_handle.linked_rec_size, SEEK_CUR);
		}
		return PDS_REC_NOT_FOUND;
	}
	return PDS_FILE_ERROR;
}

// NOTE: When we delete a key, we need to actually delete the linked records too.
// But we will ignore that part for now.
// This function will now delete only from the main data
int delete_rec_by_ndx_key(int key)
{
	struct BST_Node *temp_BST_Struct;
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

// pds_link_rec - NEW
// Create PDS_link_info instance based on key1 and key2
// Go to the end of the link file
// Store the PDS_link_info record
int pds_link_rec(int key1, int key2)
{
	struct Student *temp_Student = (struct Student *)malloc(sizeof(struct Student));
	int find_Student = get_rec_by_ndx_key(key1, temp_Student);
	if (find_Student == PDS_SUCCESS)
	{
		struct PDS_link_info *new_Link_Record = (struct PDS_link_info *)malloc(sizeof(struct PDS_link_info));
		new_Link_Record->parent_key = key1;
		new_Link_Record->child_key = key2;
		if (fseek(repo_handle.pds_link_fp, 0, SEEK_END) == 0)
		{
			fwrite(new_Link_Record, sizeof(new_Link_Record), 1, repo_handle.pds_link_fp);
			return PDS_SUCCESS;
		}
		return PDS_FILE_ERROR;
	}
	return PDS_LINK_FAILED;
}

// pds_get_linked_rec - NEW
// Go to the beginning of the link file
// Reset result_set_size to 0
// Do a linear search of all link_info records for matching the given parent_key
// Store the matching linked key in linked_keys_result array
// Keep updating the result_set_size
int pds_get_linked_rec(int parent_key, int linked_keys_result[], int *result_set_size)
{
	*result_set_size = 0;
	if (fseek(repo_handle.pds_link_fp, 0, SEEK_SET) == 0)
	{
		struct PDS_link_info *read_Link_Record = (struct PDS_link_info *)malloc(sizeof(struct PDS_link_info));
		while (fread(read_Link_Record, sizeof(read_Link_Record), 1, repo_handle.pds_link_fp) > 0)
		{
			if (read_Link_Record->parent_key == parent_key)
			{
				linked_keys_result[*result_set_size] = read_Link_Record->child_key;
				(*result_set_size)++;
			}
		}
		return PDS_SUCCESS;
	}
	return PDS_FILE_ERROR;
}

// pds_close - CHANGE
// Open the index file in wb mode (write mode, not append mode)
// Unload the BST into the index file by traversing it in PRE-ORDER (overwrite the entire index file)
// Free the BST by call bst_destroy()
// Close the index file, data file and linked data file
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
		if (repo_handle.pds_link_fp != NULL)
		{
			fclose(repo_handle.pds_link_fp);
		}
		if (repo_handle.pds_linked_data_fp != NULL)
		{
			fclose(repo_handle.pds_linked_data_fp);
		}
		repo_handle.repo_status = PDS_REPO_CLOSED;
		free(temp);
		return PDS_SUCCESS;
	}
	return PDS_FILE_ERROR;
}