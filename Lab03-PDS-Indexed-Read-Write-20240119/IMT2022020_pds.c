#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pds.h"

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
		return PDS_SUCCESS;
	}
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
			return PDS_SUCCESS;
		}
		else
		{
			repo_handle.repo_status = PDS_REPO_OPEN;
			return PDS_NDX_SAVE_FAILED;
		}
	}
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		printf("Repo was already open.\n");
		return PDS_REPO_ALREADY_OPEN;
	}
	return PDS_FILE_ERROR;
}

// pds_load_ndx
// Internal function used by pds_open to read index entries into ndx_array
int pds_load_ndx()
{
	int read_NDX = fread(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
	if (read_NDX == 0)
	{
		return PDS_NDX_SAVE_FAILED;
	}
	fread(repo_handle.ndx_array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, repo_handle.pds_ndx_fp);
	return PDS_SUCCESS;
}

// put_rec_by_key
// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to ndx_array using offset returned by ftell
// Increment record count
// Write the record at the end of the file
// Return failure in case of duplicate key
int put_rec_by_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		if (fseek(repo_handle.pds_data_fp, 0, SEEK_END) == 0)
		{
			int offset = ftell(repo_handle.pds_data_fp);
			struct PDS_NdxInfo *temp_Struct = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
			temp_Struct->key = key;
			temp_Struct->offset = offset;
			repo_handle.ndx_array[repo_handle.rec_count] = *(temp_Struct);
			repo_handle.rec_count++;
			fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
			return PDS_SUCCESS;
		}
		printf("Function put rec by key is returning PDS_ADD_FAILED = %d\n", PDS_ADD_FAILED);
		return PDS_ADD_FAILED;
	}
	else
	{
		printf("Function put rec by key is returning PDS_REPO_NOT_OPEN = %d\n", PDS_REPO_NOT_OPEN);
		return PDS_REPO_NOT_OPEN;
	}
}

// get_rec_by_key
// Search for index entry in ndx_array
// Seek to the file location based on offset in index entry
// Read the key at the current location
// Read the record from the current location
int get_rec_by_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		for (int i = 0; i < repo_handle.rec_count; i++)
		{
			if (repo_handle.ndx_array[i].key == key)
			{
				fseek(repo_handle.pds_data_fp, repo_handle.ndx_array[i].offset, SEEK_SET);
				fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
				return PDS_SUCCESS;
			}
		}
		return PDS_REC_NOT_FOUND;
	}
	else
	{
		return PDS_REPO_NOT_OPEN;
	}
}

// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Store the number of records
// Unload the ndx_array into the index file by traversing it (overwrite the entire index file)
// Close the index file and data file
int pds_close()
{
	char *temp = (char *)malloc(sizeof(repo_handle.pds_name));
	strcpy(temp, repo_handle.pds_name);
	strcat(temp, ".ndx");
	repo_handle.pds_ndx_fp = fopen(temp, "wb");
	// repo_handle.pds_data_fp = fopen(temp, "wb");
	printf("Opened the file with the name : %s in the wb mode.\n", temp);
	if (repo_handle.pds_ndx_fp != NULL)
	{
		// fwrite(repo_handle.ndx_array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, repo_handle.pds_ndx_fp);
		fwrite(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
		fwrite(repo_handle.ndx_array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, repo_handle.pds_ndx_fp);
		fclose(repo_handle.pds_ndx_fp);
		fclose(repo_handle.pds_data_fp);
		repo_handle.repo_status = PDS_REPO_CLOSED;
		return PDS_SUCCESS;
	}
	return PDS_FILE_ERROR;
}
