#include<stdio.h>

/** Compilation:
 * gcc -o driver_session2a driver_session2a.c [other .c files]
 */

// Implement the following function in a separate file named as per the roll number convention
/**
 * Implement your function as per the given requirements
 * @param filename: Name of the binary file into which the data is to be saved
 * @returns 0 on success, non-zero on error
 */
int save_num_binary( char *filename );

int main( int argc, char *argv[] )
{
	int status = 0;
	if( argc != 2 ){
		fprintf(stderr, "Usage: %s filename", argv[0]);
		status = 1;
		return status;
	}
	status = save_num_binary(argv[1]);
	if( status != 0 ){
		fprintf(stderr, "Error, save_num_binary failed with status %d\n", status);
		return status;
	}
	return status;
}

