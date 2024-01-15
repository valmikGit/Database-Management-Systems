# Database Management Systems
### Learning Database Management and Systems.

## Lab 1:
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
    
## Lab 2:
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

## Non Indexed Sequential Datafile:
### Description:
- In this lab, we begin with a basic implementation of Personal Data Store (PDS). This does not contain any index.
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
