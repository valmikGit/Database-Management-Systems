#ifndef CONTACT_H
#define CONTACT_H

#define CONTACT_SUCCESS 0
#define CONTACT_FAILURE 1
 
struct Contact{
	int contact_id;
	char contact_name[30];
	char phone[15];
};

extern struct PDS_RepoInfo *repoHandle;

// Add the given contact into the repository by calling put_rec_by_key
int add_contact( struct Contact *c );

// Display contact info in a single line as a CSV without any spaces
void print_contact( struct Contact *c );

// Use get_rec_by_key function to retrieve contact
int search_contact( int contact_id, struct Contact *c );

// Load all the contacts from a CSV file
int store_contacts( char *contact_data_file );

int search_contact_by_phone( char *phone, struct Contact *c, int *io_count );

/* Return 0 if phone of the contact matches with phone parameter */
/* Return 1 if phone of the contact does NOT match */
/* Return > 1 in case of any other error */
int match_contact_phone( void *rec, void *key );

#endif
