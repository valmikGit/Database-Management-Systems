CC = gcc
all:
	$(CC) -o IMT2022020_student_course_pds_tester IMT2022020_student_course.c IMT2022020_pds.c bst.c IMT2022020_student_course_pds_tester.c

clean:
	rm IMT2022020_student_course_pds_tester newdemo_course.dat newdemo_student.dat newdemo_student.ndx newdemo_student_newdemo_course.dat interactive student.dat course.dat student_course.dat student.ndx

check_testcases_with_link:
	./IMT2022020_student_course_pds_tester my_Testcases_Link.in

check_testcases_without_link:
	./IMT2022020_student_course_pds_tester my_Testcases_Without_Link.in

interactive:
	$(CC) -o interactive IMT2022020_pds.c bst.c linked_data_demo.c

run_interactive:
	./interactive