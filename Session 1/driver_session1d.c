#include<stdio.h>

/** Compilation:
 * gcc -o driver_session1d driver_session1d.c [other .c files]
 */

// Implement the following function in a separate file named as per the roll number convention
/**
 * Implement your function as per the given requirements
 * @param filename: Name of the text file from which the data is to be read
 * @returns 0 on success, non-zero on error
 */
int read_struct_text( char *filename );

int main( int argc, char *argv[] )
{
	int status = 0;
	if( argc != 2 ){
		fprintf(stderr, "Usage: %s filename", argv[0]);
		status = 1;
		return status;
	}
	status = read_struct_text(argv[1]);
	if( status != 0 ){
		fprintf(stderr, "Error, read_struct_text failed with status %d %s\n", status, argv[1]);
		return status;
	}
	return status;
}

