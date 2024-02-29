#include <stdio.h>
#include<string.h>
#include<stdlib.h>

// Include the header file that contains the struct declarations for parent and child records
#include "student_course.h"
#include "pds.h"

// Declare global variables for 10 parent and 10 child records
struct Student students[10];
struct Course courses[10];

void show_menu();
void setup_data();
void process_option(int option);
void process_option1();
void process_option2();

int main()
{
    int option;
    setup_data();
    do
    {
        show_menu();
        scanf("%d", &option);
        process_option(option);
    } while (option != 0);
    return 0;
}

void show_menu()
{
    printf("\nLINKED DATA DEMO\n\n");
    printf("0. Exit\n");
    printf("1. Add linked data\n");
    printf("2. Get linked data\n");
    printf("\nEnter option: ");
}

void setup_data()
{
    // statically create 10 parent and 10 child records individually
    pds_create("newdemo_student", "newdemo_course");
    pds_open("newdemo_student", "newdemo_course", sizeof(struct Student), sizeof(struct Course));
    char student_Name[50];
    char student_Phone[50];
    char course_Name[100];
    for (int i = 0; i < 10; i++)
    {
        sprintf(student_Name, "Name-of-%d", (i + 1));
        sprintf(student_Phone, "Phone-of-%d", (i + 1));
        sprintf(course_Name, "Course-%d", (i + 1));
        students[i].student_ID = i + 1;
        strcpy(students[i].name, student_Name);
        strcpy(students[i].phone, student_Phone);
        courses[i].course_ID = i + 1;
        strcpy(courses[i].course_Name, course_Name);
        put_rec_by_key(students[i].student_ID, &(students[i]));
        put_linked_rec_by_key(courses[i].course_ID, &(courses[i]));
    }
}

void process_option(int option)
{
    switch (option)
    {
    case 0:
        pds_close();
        break;
    case 1:
        process_option1();
        break;
    case 2:
        process_option2();
        break;
    default:
        printf("Invalid option\n");
        break;
    }
}

void process_option1()
{
    int parent_key, child_key;
    printf("Enter parent key and child key for linking: ");
    scanf("%d%d", &parent_key, &child_key);
    // TODO: Call appropriate PDS functions here to link the two records
    pds_link_rec(parent_key, child_key);
}

void process_option2()
{
    int parent_key;
    printf("Enter parent key of which you need the children: ");
    // printf("Enter the parent key of which the children you want to fetch: ");
    scanf("%d", &parent_key);
    // TODO: Call appropriate PDS functions here to fetch arrach of child keys
    // Call your PDS functions to display the list of child keys retrieved for the given parent key
    // Display all field values of each child record in a nicely formatted manner
    int children_Keys_List[10];
    int children_List_Size = 0;
    pds_get_linked_rec(parent_key, children_Keys_List, &children_List_Size);
    struct Course * c = (struct Course *)malloc(sizeof(struct Course));
    if (children_List_Size == 0)
    {
        printf("Student with student ID %d has not enrolled in any course.\n", parent_key);
        return;
    }
    for (int i = 0; i < children_List_Size; i++)
    {
        get_linked_rec_by_key(children_Keys_List[i], c);
        printf("Course ID: %d, Course Name: %s\n", c->course_ID, c->course_Name);
    }
}
