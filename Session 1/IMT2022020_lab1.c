#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct student
{
  int rollnum;
  char name[30];
  int age;
} Student;

int save_num_text(char *filename)
{
  FILE *file_Pointer = fopen(filename, "w");
  for (int i = 0; i < 20; i++)
  {
    fprintf(file_Pointer, "%d ", i);
  }
  fclose(file_Pointer);
  return 0;
}

int read_num_text(char *filename)
{
  char *read_From_Text_File = (char *)malloc(50 * sizeof(char));
  FILE *file_Pointer = fopen(filename, "r");
  int temp;
  for (int i = 0; i < 20; i++)
  {
    fscanf(file_Pointer, "%d", &temp);
    printf("%d ", temp);
  }
  printf("\n");
  fclose(file_Pointer);
  free(read_From_Text_File);
  return 0;
}

int save_struct_text(char *filename)
{
  Student *students = (Student *)malloc(5 * sizeof(Student));
  char *names[5] = {"Valmik", "Vedant", "Mohit", "Krish", "Ketan"};
  int roll_Numbers[5] = {1, 2, 3, 4, 5};
  int ages[5] = {4, 6, 7, 9, 2};
  for (int i = 0; i < 5; i++)
  {
    strcpy(students[i].name, names[i]);
    students[i].age = ages[i];
    students[i].rollnum = roll_Numbers[i];
  }
  FILE *file_Pointer = fopen(filename, "w");
  for (int i = 0; i < 5; i++)
  {
    fprintf(file_Pointer, "%s %d %d\n", students[i].name, students[i].age, students[i].rollnum);
  }
  fclose(file_Pointer);
  free(students);
}

int read_struct_text(char *filename)
{
  FILE *file_Pointer = fopen(filename, "r");
  // char *read_Student_From_File = (char *)malloc(52 * sizeof(char));
  int age, rollnum;
  char * name;
  for (int i = 0; i < 5; i++)
  {
    fscanf(file_Pointer, "%s %d %d", name, &age, &rollnum);
    printf("%s,%d,%d ", name, age, rollnum);
  }
  printf("\n");
  fclose(file_Pointer);
  // free(read_Student_From_File);
  return 0;
}

int save_num_binary(char *filename)
{
  FILE *file_Pointer = fopen(filename, "wb");
  int numbers[20];
  for (int i = 0; i < 20; i++)
  {
    numbers[i] = i;
  }
  fwrite(numbers, sizeof(int), 20, file_Pointer);
  fclose(file_Pointer);
  return 0;
}

int read_num_binary(char *filename)
{
  FILE *file_Pointer = fopen(filename, "rb");
  int *read_From_File = (int *)malloc(20 * sizeof(int));
  int bytes_Read = fread(read_From_File, sizeof(int), 20, file_Pointer);
  if (bytes_Read == 0)
  {
    printf("Error reading from this file.\n");
  }
  else
  {
    for (int i = 0; i < 20; i++)
    {
      printf("%d ", read_From_File[i]);
    }
    printf("\n");
  }
  fclose(file_Pointer);
  free(read_From_File);
  return 0;
}

int save_struct_binary(char *filename)
{
  Student *students = (Student *)malloc(5 * sizeof(Student));
  char *names[5] = {"Valmik", "Vedant", "Mohit", "Krish", "Ketan"};
  int roll_Numbers[5] = {1, 2, 3, 4, 5};
  int ages[5] = {4, 6, 7, 9, 2};
  for (int i = 0; i < 5; i++)
  {
    strcpy(students[i].name, names[i]);
    students[i].age = ages[i];
    students[i].rollnum = roll_Numbers[i];
  }
  FILE *file_Pointer = fopen(filename, "wb");
  fwrite(students, sizeof(Student), 5, file_Pointer);
  fclose(file_Pointer);
  return 0;
}

int read_struct_binary(char *filename)
{
  FILE *file_Pointer = fopen(filename, "rb");
  Student *read_From_File = (Student *)malloc(5 * sizeof(Student));
  int bytes_Read = fread(read_From_File, sizeof(Student), 5, file_Pointer);
  if (bytes_Read == 0)
  {
    printf("Error reading from this file.");
  }
  else
  {
    for (int i = 0; i < 5; i++)
    {
      printf("%s,%d,%d ", read_From_File[i].name, read_From_File[i].age, read_From_File[i].rollnum);
    }
    printf("\n");
  }
  fclose(file_Pointer);
  free(read_From_File);
  return 0;
}
