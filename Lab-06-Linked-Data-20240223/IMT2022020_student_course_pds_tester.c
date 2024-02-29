#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pds.h"
#include "student_course.h"

#define TREPORT(a1, a2)                        \
	{                                          \
		printf("Status: %s - %s\n\n", a1, a2); \
		fflush(stdout);                        \
	}

void process_line(char *test_case);

int array_Matcher(int arr_1[], int arr_2[], int counter)
{
	for (int i = 0; i < counter; i++)
	{
		if (arr_1[i] != arr_2[i])
		{
			return 0;
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{
	FILE *cfptr;
	char test_case[50];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s testcasefile\n", argv[0]);
		exit(1);
	}

	cfptr = (FILE *)fopen(argv[1], "r");
	while (fgets(test_case, sizeof(test_case) - 1, cfptr))
	{
		// printf("line:%s",test_case);
		if (!strcmp(test_case, "\n") || !strcmp(test_case, ""))
			continue;
		process_line(test_case);
	}
}

void process_line(char *test_case)
{
	char repo_name[30];
	char linked_repo_name[30];
	char command[20], param1[20], param2[20], param3[20], info[10000];
	int student_id, course_id, status, rec_size, linked_rec_size, expected_status;
	struct Course testCourse;
	struct Student testStudent;

	// strcpy(testContact.contact_name, "dummy name");
	// strcpy(testContact.phone, "dummy number");

	rec_size = sizeof(struct Student);
	linked_rec_size = sizeof(struct Course);

	sscanf(test_case, "%s%s%s", command, param1, param2);
	printf("Test case: %s", test_case);
	fflush(stdout);
	if (!strcmp(command, "CREATE"))
	{
		strcpy(repo_name, param1);
		if (!strcmp(param2, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		char * linked_repo_name = NULL;
		status = pds_create(repo_name, linked_repo_name);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status == expected_status)
		{
			TREPORT("PASS", "");
		}
		else
		{
			sprintf(info, "pds_create returned status %d", status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "CREATE-L"))
	{
		sscanf(test_case, "%s%s%s%s", command, param1, param2, param3);
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		if (!strcmp(param3, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		status = pds_create(repo_name, linked_repo_name);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status == expected_status)
		{
			TREPORT("PASS", "");
		}
		else
		{
			sprintf(info, "pds_create returned status %d", status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "OPEN"))
	{
		strcpy(repo_name, param1);
		if (!strcmp(param2, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		char * linked_repo_name = NULL;
		status = pds_open(repo_name, linked_repo_name, rec_size, linked_rec_size);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status == expected_status)
		{
			TREPORT("PASS", "");
		}
		else
		{
			sprintf(info, "pds_open returned status %d", status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "OPEN-L"))
	{
		sscanf(test_case, "%s%s%s%s", command, param1, param2, param3);
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		if (!strcmp(param3, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		status = pds_open(repo_name, linked_repo_name, rec_size, linked_rec_size);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status == expected_status)
		{
			TREPORT("PASS", "");
		}
		else
		{
			sprintf(info, "pds_open returned status %d", status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "STORE"))
	{
		if (!strcmp(param2, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		sscanf(param1, "%d", &student_id);
		testStudent.student_ID = student_id;
		sprintf(testStudent.phone, "Phone-of-%d", student_id);
		sprintf(testStudent.name, "Name-of-%d", student_id);
		status = add_student(&testStudent);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status == expected_status)
		{
			TREPORT("PASS", "");
		}
		else
		{
			sprintf(info, "add_student returned status %d", status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "STORE-COURSE"))
	{
		if (!strcmp(param2, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		sscanf(param1, "%d", &course_id);
		testCourse.course_ID = course_id;
		sprintf(testCourse.course_Name, "Course-of-%d", course_id);
		status = add_course(&testCourse);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status == expected_status)
		{
			TREPORT("PASS", "");
		}
		else
		{
			sprintf(info, "add_course returned status %d", status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "NDX_SEARCH"))
	{
		if (!strcmp(param2, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		sscanf(param1, "%d", &student_id);
		testStudent.student_ID = -1;
		status = search_student(student_id, &testStudent);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status != expected_status)
		{
			sprintf(info, "search key: %d; Got status %d", student_id, status);
			TREPORT("FAIL", info);
		}
		else
		{
			// Check if the retrieved values match
			char expected_phone[30];
			sprintf(expected_phone, "Phone-of-%d", student_id);
			char expected_name[30];
			sprintf(expected_name, "Name-of-%d", student_id);
			if (expected_status == 0)
			{
				if (testStudent.student_ID == student_id &&
					strcmp(testStudent.name, expected_name) == 0 &&
					strcmp(testStudent.phone, expected_phone) == 0)
				{
					TREPORT("PASS", "");
				}
				else
				{
					sprintf(info, "Contact data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
							student_id, expected_name, expected_phone,
							testStudent.student_ID, testStudent.name, testStudent.phone);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if (!strcmp(command, "SEARCH-COURSE"))
	{
		if (!strcmp(param2, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		sscanf(param1, "%d", &course_id);
		testCourse.course_ID = course_id;
		status = search_course(course_id, &testCourse);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status != expected_status)
		{
			sprintf(info, "search key: %d; Got status %d", course_id, status);
			TREPORT("FAIL", info);
		}
		else
		{
			// Check if the retrieved values match
			char expected_name[30];
			sprintf(expected_name, "Course-of-%d", course_id);
			if (expected_status == 0)
			{
				if (testCourse.course_ID == course_id &&
					strcmp(testCourse.course_Name, expected_name) == 0)
				{
					TREPORT("PASS", "");
				}
				else
				{
					sprintf(info, "Contact data not matching... Expected:{%d,%s} Got:{%d,%s}\n",
							course_id, expected_name,
							testCourse.course_ID, testCourse.course_Name);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if (!strcmp(command, "NON_NDX_SEARCH"))
	{
		char name[30], expected_name[30], expected_phone[30];
		int expected_io, actual_io;

		if (strcmp(param2, "-1") == 0)
			expected_status = CONTACT_FAILURE;
		else
			expected_status = CONTACT_SUCCESS;

		sscanf(param1, "%s", name);
		sscanf(param2, "%d", &expected_io);
		// testContact.contact_id = -1;
		testStudent.student_ID = -1;
		actual_io = 0;
		// status = search_contact_by_phone(phone_num, &testStudent, &actual_io);
		status = search_student_by_name(name, &testStudent, &actual_io);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status != expected_status)
		{
			sprintf(info, "search key: %d; Got status %d", student_id, status);
			TREPORT("FAIL", info);
		}
		else
		{
			// Check if the retrieved values match
			// Check if num block accesses match too
			// Extract the expected contact_id from the phone number
			sscanf(name + sizeof("Name-of"), "%d", &student_id);
			sprintf(expected_name, "Name-of-%d", student_id);
			sprintf(expected_phone, "Phone-of-%d", student_id);
			if (expected_status == 0)
			{
				if (testStudent.student_ID == student_id &&
					strcmp(testStudent.name, expected_name) == 0 &&
					strcmp(testStudent.phone, expected_phone) == 0)
				{
					if (expected_io == actual_io)
					{
						TREPORT("PASS", "");
					}
					else
					{
						sprintf(info, "Num I/O not matching for contact %d... Expected:%d Got:%d\n",
								student_id, expected_io, actual_io);
						TREPORT("FAIL", info);
					}
				}
				else
				{
					sprintf(info, "Contact data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
							student_id, expected_name, expected_phone,
							testStudent.student_ID, testStudent.name, testStudent.phone);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if (!strcmp(command, "NDX_DELETE"))
	{
		if (strcmp(param2, "0") == 0)
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		sscanf(param1, "%d", &student_id);
		testStudent.student_ID = -1;
		status = delete_student(student_id);
		if (status != expected_status)
		{
			sprintf(info, "delete key: %d; Got status %d", student_id, status);
			TREPORT("FAIL", info);
		}
		else
		{
			TREPORT("PASS", "");
		}
	}
	else if (!strcmp(command, "CLOSE"))
	{
		if (!strcmp(param1, "0"))
			expected_status = CONTACT_SUCCESS;
		else
			expected_status = CONTACT_FAILURE;

		status = pds_close();
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status == expected_status)
		{
			TREPORT("PASS", "");
		}
		else
		{
			sprintf(info, "pds_close returned status %d", status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "LINK"))
	{
		int key1, key2;
		sscanf(test_case, "%s%s%s%s", command, param1, param2, param3);
		if (!strcmp(param3, "0"))
		{
			expected_status = CONTACT_SUCCESS;
		}
		else
		{
			expected_status = CONTACT_FAILURE;
		}
		sscanf(param1, "%d", &key1);
		sscanf(param2, "%d", &key2);
		status = pds_link_rec(key1, key2);
		if (status == PDS_SUCCESS)
		{
			status = CONTACT_SUCCESS;
		}
		else
		{
			status = CONTACT_FAILURE;
		}
		if (status != expected_status)
		{
			sprintf(info, "Got status %d", status);
			TREPORT("FAIL", info);
		}
		else
		{
			TREPORT("PASS", "");
		}
	}
	else if (!strcmp(command, "GETLINK"))
	{
		// command student_id expected_result_size expected_result_elements expected_status
		int parent_key;
		int expected_result_set_size;
		sscanf(param1, "%d", &parent_key);
		sscanf(param2, "%d", &expected_result_set_size);
		int actual_result_set_size;
		int expected_linked_keys_result[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int actual_linked_key_results[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		char children_Keys_Comma_Separated[1000];
		sscanf(test_case, "%s%s%s%s%s", command, param1, param2, children_Keys_Comma_Separated, param3);
		int counter = 0;
		char *token = strtok(children_Keys_Comma_Separated, ",");
		while (token != NULL)
		{
			expected_linked_keys_result[counter++] = atoi(token); // Convert token to integer and store in array
			token = strtok(NULL, ",");							  // Get the next token
		}
		if (!strcmp(param3, "-1"))
		{
			expected_status = CONTACT_FAILURE;
		}
		else
		{
			expected_status = CONTACT_SUCCESS;
		}
		status = pds_get_linked_rec(parent_key, actual_linked_key_results, &actual_result_set_size);
		if (status == PDS_SUCCESS)
			status = CONTACT_SUCCESS;
		else
			status = CONTACT_FAILURE;
		if (status != expected_status)
		{
			if (!(array_Matcher(expected_linked_keys_result, actual_linked_key_results, counter)))
			{
				if (actual_result_set_size == counter)
				{
					TREPORT("PASS", "");
				}
				else
				{
					sprintf(info, "Child keys array not matching. Expected: ");
					for (int i = 0; i < counter; i++)
					{
						if (i == (counter - 1))
						{
							sprintf(info + strlen(info), "%d ", expected_linked_keys_result[i]);
						}
						else
						{
							sprintf(info + strlen(info), "%d, ", expected_linked_keys_result[i]);
						}
					}
					sprintf(info + strlen(info), "Got: ");
					for (int i = 0; i < counter; i++)
					{
						if (i == (counter - 1))
						{
							sprintf(info + strlen(info), "%d\n", actual_linked_key_results[i]);
						}
						else
						{
							sprintf(info + strlen(info), "%d, ", actual_linked_key_results[i]);
						}
					}
					TREPORT("FAIL", info);
				}
			}
			else
			{
				sprintf(info, "Parent key: %d; Got status %d", parent_key, status);
				TREPORT("FAIL", info);
			}
		}
		else
		{
			if (expected_status == 0)
			{
				if (actual_result_set_size == expected_result_set_size)
				{
					if (array_Matcher(expected_linked_keys_result, actual_linked_key_results, counter))
					{
						TREPORT("PASS", "");
					}
					else
					{
						sprintf(info, "Child keys array not matching. Expected: ");
						for (int i = 0; i < counter; i++)
						{
							if (i == (counter - 1))
							{
								sprintf(info + strlen(info), "%d ", expected_linked_keys_result[i]);
							}
							else
							{
								sprintf(info + strlen(info), "%d, ", expected_linked_keys_result[i]);
							}
						}
						sprintf(info + strlen(info), "Got: ");
						for (int i = 0; i < counter; i++)
						{
							if (i == (counter - 1))
							{
								sprintf(info + strlen(info), "%d\n", actual_linked_key_results[i]);
							}
							else
							{
								sprintf(info + strlen(info), "%d, ", actual_linked_key_results[i]);
							}
						}
						TREPORT("FAIL", info);
					}
				}
				else
				{
					sprintf(info, "Expected result size : %d, Got result size : %d", expected_result_set_size, actual_result_set_size);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
}
