#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include"pds.h"


// Define the global variable
struct PDS_RepoInfo repo_handle;

int pds_create(char * repo_name) {
    char filename[30];
    strcpy(filename, repo_name);
    strcat(filename, ".dat");
    FILE * fp = fopen(filename, "wb+");
    if (fp == NULL)
    {
        return PDS_FILE_ERROR;
    }
    else
    {
        fclose(fp);
        return PDS_SUCCESS;
    }
}

int pds_open(char *repo_name, int rec_size)
{
    // Open data files as per the following convention
    // If repo_name is "demo", then data file should be "demo.dat"
    // Initialize other members of PDS_RepoInfo
    char * filename = (char *)malloc(sizeof(repo_name));
    strcpy(filename, repo_name);
    strcpy(repo_handle.pds_name, filename);
    strcat(filename, ".dat");
    repo_handle.pds_data_fp = fopen(filename, "ab+");
    if (repo_handle.pds_data_fp == NULL)
    {
        fclose(repo_handle.pds_data_fp);
        repo_handle.repo_status = PDS_REPO_CLOSED;
        return PDS_FILE_ERROR;
    }
    repo_handle.rec_size = rec_size;
    repo_handle.repo_status = PDS_REPO_OPEN;
    return PDS_SUCCESS;
}
int put_rec_by_key(int key, void *rec)
{
    // Seek to the end of the data file
    // Write the record at the current file location
    if (fseek(repo_handle.pds_data_fp, 0, SEEK_END) != 0)
    {
        repo_handle.repo_status = PDS_REPO_ALREADY_OPEN;
        return PDS_ADD_FAILED;
    }
    fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp);
    fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
    return PDS_SUCCESS;
}

int get_rec_by_key(int key, void *rec)
{
    // Read record-record-by record from data file
    // Compare key of the record with the given key
    // Return success status if record is found else return failure status
    int temp;
    if (fseek(repo_handle.pds_data_fp, 0, SEEK_SET) != 0)
    {
        repo_handle.repo_status = PDS_REPO_ALREADY_OPEN;
        return PDS_FILE_ERROR;
    }
    while (fread(&temp, sizeof(int), 1, repo_handle.pds_data_fp) > 0)
    {   
        if (temp == key)
        {   
            repo_handle.repo_status = PDS_REPO_ALREADY_OPEN;
            //rec -> store data corresponding to key
            fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
            return PDS_SUCCESS;
        }
        fseek(repo_handle.pds_data_fp, repo_handle.rec_size, SEEK_CUR);
    }
    repo_handle.repo_status = PDS_REPO_ALREADY_OPEN;
    return PDS_REC_NOT_FOUND;
}

int pds_close()
{
    // Close the repo file
    // Update file pointer and status in global struct
    fclose(repo_handle.pds_data_fp);
    // char * temp = (char *)malloc(sizeof(repo_handle.pds_name));
    // strcpy(temp, repo_handle.pds_name);
    // strcat(temp, ".dat");
    // repo_handle.pds_data_fp = fopen(temp, "wb+");
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}
