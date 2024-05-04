INSERT INTO STUDENT (student_ID, Fname, Mname, Lname) VALUES ("IMT20", "Valmik", "Anant", "Belgaonkar");
INSERT INTO STUDENT (student_ID, Fname, Mname, Lname) VALUES ("IMT58", "Ketan", "Raman", "Ghungralekar");
INSERT INTO STUDENT (student_ID, Fname, Mname, Lname) VALUES ("IMT43", "Krish", "Rakesh", "Dave");
INSERT INTO STUDENT (student_ID, Fname, Mname, Lname) VALUES ("IMT19", "Vedant", "Vinit", "Mangrulkar");
INSERT INTO STUDENT (student_ID, Fname, Mname, Lname) VALUES ("IMT70", "Mohit", "Avinash", "Naik");

INSERT INTO COURSE (course_ID, course_Name) VALUES ("A", "DAA");
INSERT INTO COURSE (course_ID, course_Name) VALUES ("B", "OS");
INSERT INTO COURSE (course_ID, course_Name) VALUES ("C", "DBMS");
INSERT INTO COURSE (course_ID, course_Name) VALUES ("D", "SPIT");

INSERT INTO INSTRUCTOR (instructor_ID, Fname, Lname) VALUES (1, "Sachin", "Tendulkar");
INSERT INTO INSTRUCTOR (instructor_ID, Fname, Lname) VALUES (2, "Virat", "Kohli");

INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (1, "IMT20", "C");
INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (2, "IMT58", "A");
INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (3, "IMT43", "B");
INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (4, "IMT19", "D");
INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (5, "IMT70", "C");
INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (6, "IMT20", "B");
INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (7, "IMT19", "D");
INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (8, "IMT70", "A");

INSERT INTO TEACHES (id, instructor_ID, course_ID) VALUES (1, 1, "A");
INSERT INTO TEACHES (id, instructor_ID, course_ID) VALUES (2, 2, "B");
INSERT INTO TEACHES (id, instructor_ID, course_ID) VALUES (3, 1, "C");
INSERT INTO TEACHES (id, instructor_ID, course_ID) VALUES (4, 2, "D");