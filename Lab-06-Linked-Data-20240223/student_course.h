#define CONTACT_SUCCESS 0
#define CONTACT_FAILURE 1

struct Student {
    int student_ID;
    char name[50];
    char phone[50];
};

struct Course {
    int course_ID;
    char course_Name[100];
};

extern struct PDS_RepoInfo *repoHandle;

// Add the given student into the repository by calling put_rec_by_key
int add_student(struct Student * s);

// Add the given course into the repository by calling put_linked_rec_by_key
int add_course(struct Course * c);

// Display student info in a single line as a CSV without any spaces
void print_student( struct Student *s );

// Display course info in a single line as a CSV without any spaces
void print_course( struct Course *c );

// Use get_rec_by_key function to retrieve student
int search_student( int student_id, struct Student *s );

// Use get_linked_rec_by_key function to retrieve course
int search_course(int course_id, struct Course * c);

// Given the name to be searched, if found store the record in the student pointer sent as an argument.
int search_student_by_name(char *name, struct Student *c, int *io_count);

/* Return 0 if name of the student matches with name parameter */
/* Return 1 if name of the student does NOT match */
/* Return > 1 in case of any other error */
int match_student_name( void *rec, void *key );

// Load all the students from a CSV file
int store_students( char *student_data_file );

// Load all the course from a CSV file
int store_courses( char *course_data_file );

// Return CONTACT_SUCCESS if the student is successfully deleted.
// Else return CONTACT_FAILURE.
int delete_student(int key);

// Return CONTACT_SUCCESS if the student is successfully linked to the course.
// Else return CONTACT_FAILURE.
int link_student_to_course(int student_key, int course_key);

// Return CONTACT_SUCCESS if the list of courses the given student has enrolled in are correctly found.
// Else return CONTACT_FAILURE.
int get_courses_of_student(int parent_key, int linked_keys_result[], int *result_set_size);