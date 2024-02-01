#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "pds.h"

void bst_print_custom( struct BST_Node *root )
{
	if( root == NULL )
		return;
	else{
		// printf("%d ", root->key);
		fwrite(root->data, sizeof(root->data), 1, repo_handle.pds_ndx_fp);
		bst_print( root->left_child );
		bst_print( root->right_child );
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
// Load the index entries into the BST ndx_root by calling bst_add_node repeatedly for each
// index entry. Unlike array, for BST, you need read the index file one by one in a loop
int pds_load_ndx()
{
	int read_NDX = fread(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
	struct PDS_NdxInfo * temp_Ndx_Array = (struct PDS_NdxInfo *)malloc(read_NDX * sizeof(struct PDS_NdxInfo));
	fread(temp_Ndx_Array, sizeof(struct PDS_NdxInfo), read_NDX, repo_handle.pds_ndx_fp);
	if (read_NDX == 0)
	{
		free(temp_Ndx_Array);
		return PDS_NDX_SAVE_FAILED;
	}
	for (int i = 0; i < read_NDX; i++)
	{
		bst_add_node(&repo_handle.ndx_root, temp_Ndx_Array[i].key, &(temp_Ndx_Array[i]));
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
			struct BST_Node * temp_BST_Struct;
			if ((temp_BST_Struct = bst_search(repo_handle.ndx_root, key)) == NULL)
			{
				int offset = ftell(repo_handle.pds_data_fp);
				struct PDS_NdxInfo *temp_Struct = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
				temp_Struct->key = key;
				temp_Struct->offset = offset;
				repo_handle.rec_count++;
				fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
				int status = bst_add_node(&repo_handle.ndx_root, key, temp_Struct);
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
int get_rec_by_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		struct BST_Node *temp_BST_Struct;
		if ((temp_BST_Struct = bst_search(repo_handle.ndx_root, key)) != NULL)
		{
			struct PDS_NdxInfo * temp_Struct = (struct PDS_NdxInfo *)(temp_BST_Struct->data);
			if (fseek(repo_handle.pds_data_fp, temp_Struct->offset, SEEK_SET) == 0)
			{
				fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
				return PDS_SUCCESS;
			}
			return PDS_FILE_ERROR;
		}
		return PDS_REC_NOT_FOUND;
	}
	return PDS_REPO_NOT_OPEN;
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
		bst_print_custom(repo_handle.ndx_root);
		fclose(repo_handle.pds_data_fp);
		fclose(repo_handle.pds_ndx_fp);
		repo_handle.repo_status = PDS_REPO_CLOSED;
		free(temp);
		return PDS_SUCCESS;
	}
	
	return PDS_FILE_ERROR;
}
