#include<stdio.h>

/** Compilation:
 * gcc -o driver_session1c driver_session1c.c [other .c files]
 */

/**
 * Implement your function in a separate file as per the given requirements
 * @param filename: Name of the text file into which the data is to be saved
 * @returns 0 on success, non-zero on error
 */
int save_struct_text( char *filename );

int main( int argc, char *argv[] )
{
	int status = 0;
	if( argc != 2 ){
		fprintf(stderr, "Usage: %s filename", argv[0]);
		status = 1;
		return status;
	}
	status = save_struct_text(argv[1]);
	if( status != 0 ){
		fprintf(stderr, "Error, save_struct_text failed with status %d\n", status);
		return status;
	}
	return status;
}

