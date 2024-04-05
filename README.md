# Database Management Systems
### Learning Database Management and Systems.

## Lab 1 A:
### Description:
- This lab is about saving our data in a .txt file for storage purpose, like a very raw database. Then we perform file I/O to read from and write to the .txt files.
- Firstly, we store 20 random integers in a text file and then read them and print them on the terminal.
- Secondly, we store student details in the form: roll number, name and age in a .txt file. Then, we read from this file and print the content read on the terminal.
- Thus, it is a very basic database which we are trying to create.
### How to run the programs:
- Navigate to the Session 1 folder.
    ```bash
    cd "Session 1"
    ```
- Start WSL.
    ```bash
    wsl
    ```
- Run the command:
    ```bash
    make clean
    ```
- Then run the command:
    ```bash
    make
    ```
- Now, to create and write into a .txt file:
    ```bash
    make write_a
    ```
- To read from the created .txt file.
    ```bash
    make read_a
    ```
- Similarly:
    ```bash
    make write_c
    ```
- Now to read:
    ```bash
    make read_c
    ```
    
## Lab 1 B:
### Description:
- This lab is about saving our data in a .bin file for storage purpose, like a very raw database. Then we perform file I/O to read from and write to the .bin files.
- Firstly, we store 20 random integers in a binary file and then read them and print them on the terminal.
- Secondly, we store student details in the form: roll number, name and age in a .bin file. Then, we read from this file and print the content read on the terminal.
- Thus, it is a very basic database which we are trying to create.
### How to run the programs:
- Navigate to the Session 2 folder.
    ```bash
    cd "Session 2"
    ```
- Start WSL.
    ```bash
    wsl
    ```
- Run the command:
    ```bash
    make clean
    ```
- Then run the command:
    ```bash
    make
    ```
- Now, to create and write into a .bin file:
    ```bash
    make write_a
    ```
- To read from the created .bin file.
    ```bash
    make read_a
    ```
- Similarly:
    ```bash
    make write_c
    ```
- Now to read:
    ```bash
    make read_c
    ```

## Lab 2 - Non Indexed Sequential Datafile:
### Description:
- We are beginning with storing fixed length records in the database.
- In this lab, we begin with a basic implementation of Personal Data Store (PDS). This does not contain any index.
- We must note that the database operations do not know what kind of data is getting stored in the data because the reference pointer to the data to be read to written is of void type. Thus, this is a generic database. It can store whatever we want it to store. Our database is flexible in terms of what type of records to store but is not flexible in terms of length of the records to be stored.
- To store a record in the .dat file, we navigate to the end of the .dat file and write the record there.
- To get a record from the .dat file, we iterate through all the records in the .dat file and check whether the key given to us matches the key of the current record. If it does, we copy this record to the void reference pointer provided to us otherwise we return that the record was not found.
- Thus, in the worst case our time to read a record is proportional to the number of records in the .dat file.
### How to run the program:
- Navigate to the Lab 02 Non-indexed Sequential Datafile folder:
  ```bash
  cd ".\Lab 02 - Non-Indexed Sequential Datafile (pds_v1)-20240112\"
  ```
- Start WSL.
  ```bash
  wsl
  ```
- Run the command:
  ```bash
  make clean
  ```
- Then run the command:
  ```bash
  make
  ```
- Now, to run the driver code, run the command:
  ```bash
  make run_contact
  ```
- You will see the result of the test cases, i.e. "SUCCESS" or "FAIL".

## Lab 3A - Indexed Read Write:
### Description:
- In this lab, we modify our initial implementation of the database from a non indexed datafile to a indexed datafile. We maintain a .ndx file separately to make the search of an entry in the .dat file faster than before. The format of the .ndx file is: First entry in the .ndx file is an integer storing the count of records in the repository till the most recent update of the .ndx file.
- In the previous version, to put a record into the database i.e. our .dat file, we used to directly write at the end of the .dat file. However, now in the latest version we will maintain an array of Index structures. Each Index structure will have a key and an offset. This tells the key of the record at that offset in the .dat file. Now, while making a new entry, we will write the record to the .dat file and add the corresponding key-offset pair to the array of Index structures. We will not write this key-offset pair to the .ndx file. We will just update the Index structure array.
- Initially, we used to iterate through each of the entries in the .dat file then we used to compare the given key to us and then compare it to the key of the current record. If the keys are same then we copy the record to the void reference pointer given to us, otherwise we return that the desired key was not found. However, now what we do is that we iterate over the Index structure array and check whether the key is present or not. If yes, we go to the corresponding offset in the .dat file and read the record at that offset into the void reference pointer given to us.
- While closing the repository, we will rewrite the number of records present and completely rewrite the .ndx file by writing the contents of the Index structure array in the .ndx file. Thus, what we did was, we wrote the new record in the .dat file but did not update the .ndx file. While putting and getting records between the period when the repository is opened and closed, we use the Index array structure. After we close the repository, we update the .ndx file.
- The .ndx file helps to directly the access the location in the the .dat file by reading the offset orresponding to the required key. Thus, our access time to get a record is significantly reduced as compared to the previous version.
- Thus, in the worst case, the time required to read a record from the .dat file is proportional to the total number of elements in the Index structure array.
### How to run the program:
- Navigate to the "Lab03-PDS-Indexed-Read-Write-20240119" folder:
  ```bash
  cd ".\Lab03-PDS-Indexed-Read-Write-20240119\"
  ```
- Start WSL:
  ```bash
  wsl
  ```
- Run the command:
  ```bash
  make clean
  ```
- Run the command:
  ```bash
  make
  ```
- Now, run the command:
  ```bash
  make check_testcases
  ```
- You will see the result of the test cases, i.e. "PASS" or "FAIL".
## Lab 3B - BST Indexed Read Write:
### Description:
- In this lab, we will try to reduce the search time of a record from O(n) to O(log n).
- In the earlier lab, we were maintaining an index array of key-offset pairs. Thus, in the worst case the time complexity is O(n).
- In this lab, in place of the array, we will maintain a binary search tree. Thus, in the worst case, the time complexity is O(log n).
### How to run the program:
- Navigate to the "Lab03-B-PDS-BST-Indexed-Read-Write-20240130" folder:
  ```bash
  cd ".\Lab03-B-PDS-BST-Indexed-Read-Write-20240130\"
  ```
- Start WSL:
  ```bash
  wsl
  ```
- Run the command:
  ```bash
  make clean
  ```
- Run the command:
  ```bash
  make
  ```
- Now, run the command:
  ```bash
  make check_testcases
  ```
## Lab 4 - Non Index Search:
### Description:
- Till now, we searched a record based on its key attribute. However, we may want to search a record on the basis of some other attribute.
- For example, if the records in our database are contacts, then we may want to search a contact on the basis of the phone number attribute.
- Thus, we need to have a separate function which searches a record on the basis of some attribute other than the key.
- For now, we will use brute force approach to search through the records, i.e. we will read a record from the .dat file and check whether the value of its required attribute matches the one queried for. If yes, we will return the record. If we do not find the record even after reaching the end of the .dat file, we will return that the record was not found.
- Thus, now we can search for a record using the key attribute in O(log n) time and using some other attribute by brute force approach.
- A generic matcher function is defined so that this function should be able to tell whether the record given to it has the required value of the specified attribute given to it. For example, a Contact record will be given to it and a phone number will be given to it. It should be able to return success or failure depending on whether the Contact record has a matching phone number.
- Thus, after reading each record we use this matcher function to compare the values of the required attribute of the record and the value given to us for querying.
### How to run the program:
- Navigate to the "Lab-04-Non-Index-Search-20240205" folder:
  ```bash
  cd ".\Lab-04-Non-Index-Search-20240205\"
  ```
- Start WSL:
  ```bash
  wsl
  ```
- Run the command:
  ```bash
  make clean
  ```
- Run the command:
  ```bash
  make
  ```
- Now, run the command:
  ```bash
  make check_testcases
  ```
## Lab 5 - Index delete:
### Description:
- In this lab, we will add a delete operation based on the ndx key.
- We can consider this option, once we delete a record, we can shift all the records below it up by 1 record. However, for large number of records, this will be very time consuming. The solution to this is discussed in the next point.
- For this, we will additionally add a is_deleted flag in the PDS_NdxInfo struct. We will toggle this flag of each record to consider its status as deleted or not. Now, we won't need to shift all the records any time we delete a record. Thus, a lot of time is saved.
- As we are not actually deleting a record and just toggling its is_deleted flag, we do not have to decrement the repo_handle.rec_count by 1 if we delete a record. 
- Once we delete a record, we have to just set the is_deleted to 1. If we want to add a record which is already present in the .dat file but has its is_deleted flag equal to 1, we just need to toggle is_deleted flag and set the offset as the end of the file. Now, we will write to the .dat file.
### How to run the program:
- Navigate to the "Lab05-Delete-20240213 folder:
  ```bash
  cd ".\Lab05-Delete-20240213\
  ```
- Start WSL:
  ```bash
  wsl
  ```
- Run the command:
  ```bash
  make clean
  ```
- Run the command:
  ```bash
  make
  ```
- Now, run the command:
  ```bash
  make check_testcases_part_2
  ```
## Lab 6 - Linked Data(Relationship), Non Interactive Tester and Interactive Tester:
### Description -
- In this lab, we will define a relationship between two entities, make an interactive tester and a non interactive tester.
- I have implemented this database : Student(Parent entity), Course(Child entity) and Enrolls(Relationship).
- Student has student.dat and student.ndx files. Functionalities available for the Student entity are: delete by ndx key, search by ndx key, search by non ndx key and insertion of a new record.
- Course has only course.dat file. Functionalities available for the Course entity are: insertion of a new record and search by course ID attribute.
- NOTE : Course entity can also have a .ndx file to make search by ndx key and non ndx key possible for it.
- We also require to maintain a student_course.dat file to store the student ID and the corresponding course ID. For this we have to define a PDS_link_info structure in the pds.h file which has 2 attributes : parent_key and child_key. We write the instances of this structure into the student_course.dat file.
- All the pds.c functions remain the same except the pds_open() and pds_close() functions. There can be 2 cases which we need to take care of : first is when we need to make a database which does not have relations among models (single model database) and second, when we want to establish a relation between 2 models present in our database.
- I have also made a non interactive tester to automate the testing process. A non interactive tester helps us to automate testing. 
