#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "student_course.h"

// Load all the students from a CSV file
int store_students( char *student_data_file )
{
	FILE *cfptr;
	char student_line[500], token;
	struct Student s, dummy;

	cfptr = (FILE *) fopen(student_data_file, "r");
	while(fgets(student_line, sizeof(student_line)-1, cfptr)){
		//printf("line:%s",contact_line);
		sscanf(student_line, "%d%s%s", &(s.student_ID),s.name,s.phone);
		print_student(&s);
		add_student(&s);
	}
}

// Load all the courses from a CSV file
int store_courses( char *course_data_file )
{
	FILE *cfptr;
	char student_line[500], token;
	struct Course c, dummy;

	cfptr = (FILE *) fopen(course_data_file, "r");
	while(fgets(student_line, sizeof(student_line)-1, cfptr)){
		//printf("line:%s",contact_line);
		sscanf(student_line, "%d%s", &(c.course_ID),c.course_Name);
		print_course(&c);
		add_course(&c);
	}
}

void print_student( struct Student *s )
{
	printf("%d,%s,%s\n", s->student_ID,s->name,s->phone);
}

void print_course( struct Course *c )
{
	printf("%d,%s\n", c->course_ID, c->course_Name);
}

// Use get_rec_by_key function to retrieve student
int search_student( int student_id, struct Student *s )
{
	return get_rec_by_ndx_key( student_id, s );
}

// Use get_linked_rec_by_key function to retrieve course
int search_course( int course_id, struct Course *c )
{
	return get_linked_rec_by_key(course_id, c);
}

// Add the given student into the repository by calling put_rec_by_key
int add_student( struct Student *s )
{
	int status;

	status = put_rec_by_key( s->student_ID, s );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add contact with key %d. Error %d", s->student_ID, status );
		return CONTACT_FAILURE;
	}
	return status;
}

// Add the given course into the repository by calling put_linked_rec_by_key
int add_course( struct Course *c )
{
	int status;

	status = put_linked_rec_by_key(c->course_ID, c);

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add contact with key %d. Error %d", c->course_ID, status );
		return CONTACT_FAILURE;
	}
	return status;
}

// Use get_rec_by_non_ndx_key function to retrieve student
// Hint: get_rec_by_non_ndx_key(name, s, &match_student_name, io_count);
// Return CONTACT_SUCCESS if student is found, CONTACT_FAILURE if not found
int search_student_by_name( char *name, struct Student *s, int *io_count )
{
	if (get_rec_by_non_ndx_key(name, s, &match_student_name, io_count) == 1)
	{
		return PDS_SUCCESS;
	}
	else
	{
		return PDS_REC_NOT_FOUND;
	}
	
}

/* Return 0 if name of the student matches with name parameter */
/* Return 1 if name of the student does NOT match */
/* Return > 1 in case of any other error */
int match_student_name( void *rec, void *key )
{
	// Store the rec in a struct student pointer
    // Store the key in a char pointer
    // Compare the name values in key and record
    // Return 0,1,>1 based on above condition
	struct Student * temp_Contact_Pointer = (struct Student *)(rec);
	char * temp_Char = (char *)(key);
	if (strcmp(temp_Char, temp_Contact_Pointer->name) == 0)
	{
		return 0;
	}
	else if (strcmp(temp_Char, temp_Contact_Pointer->name) != 0)
	{
		return 1;
	}
	else
	{
		return 5;
	}
	
}

int delete_student(int key) {
	if (delete_rec_by_ndx_key(key) == PDS_DELETE_FAILED)
	{
		return CONTACT_FAILURE;
	}
	else
	{
		return CONTACT_SUCCESS;
	}
}

// Return CONTACT_SUCCESS if the student is successfully linked to the course.
// Else return CONTACT_FAILURE.
int link_student_to_course(int student_key, int course_key) {
    if (pds_link_rec(student_key, course_key) == PDS_SUCCESS)
    {
        return CONTACT_SUCCESS;
    }
    return CONTACT_FAILURE;
}

// Return CONTACT_SUCCESS if the list of courses the given student has enrolled in are correctly found.
// Else return CONTACT_FAILURE.
int get_courses_of_student(int parent_key, int linked_keys_result[], int *result_set_size) {
	if (pds_get_linked_rec(parent_key, linked_keys_result, result_set_size) == PDS_SUCCESS)
	{
		return CONTACT_SUCCESS;
	}
	return CONTACT_FAILURE;
}