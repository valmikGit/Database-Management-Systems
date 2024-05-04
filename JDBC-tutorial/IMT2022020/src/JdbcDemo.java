//STEP 1. Import required packages

import java.sql.*;
import java.util.Scanner;

public class JdbcDemo {

   // Set JDBC driver name and database URL
   static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
   static final String DB_URL = "jdbc:mysql://localhost:3306/collegedb?useSSL=false";

   // Database credentials
   static final String USER = "root";// add your user
   static final String PASSWORD = "valmiksql";// add password

   public static void main(String[] args) {
      Connection conn = null;
      Statement stmt = null;

      // STEP 2. Connecting to the Database
      try {
         // STEP 2a: Register JDBC driver
         Class.forName(JDBC_DRIVER);
         // STEP 2b: Open a connection
         System.out.println("Connecting to database...");
         conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
         conn.setAutoCommit(false);
         // STEP 2c: Execute a query
         System.out.println("Creating statement...");
         stmt = conn.createStatement();

         Scanner scanner = new Scanner(System.in);
         while (true) {
            System.out.println("Select an option:");
            System.out.println("1. Retrieve all students.");
            System.out.println("2. Retrieve all courses.");
            System.out.println("3. Retrieve all instructors.");
            System.out.println("4. Retrieve which student has enrolled into which course.");
            System.out.println("5. Retrieve which course is taught by which instructor.");
            System.out.println("6. Add a student.");
            System.out.println("7. Add a course.");
            System.out.println("8. Add an instructor.");
            System.out.println("9. Add a student enrollment into a course.");
            System.out.println("10. Add an instructor-course teaching pair.");
            System.out.println("11. Update a student.");
            System.out.println("12. Update a course.");
            System.out.println("13. Update an instructor");
            System.out.println("14. Delete a student.");
            System.out.println("15. Delete a course.");
            System.out.println("16. Delete an instructor.");
            System.out.println("17. Exit.");
            int input = scanner.nextInt();
            if (input == 17) {
               break;
            } else if (input == 1) {
               String query = "SELECT * FROM STUDENT";
               ResultSet rSet = stmt.executeQuery(query);
               while (rSet.next()) {
                  System.out.println("ID : " + rSet.getString("student_ID"));
                  System.out.println("Name : " + rSet.getString("Fname") + " " + rSet.getString("Mname") +
                        " " + rSet.getString("Lname"));
               }
               rSet.close();
               conn.commit();
            } else if (input == 2) {
               String query = "SELECT * FROM COURSE";
               ResultSet rSet = stmt.executeQuery(query);
               while (rSet.next()) {
                  System.out.println("ID : " + rSet.getString("course_ID"));
                  System.out.println("Name : " + rSet.getString("course_Name"));
               }
               rSet.close();
               conn.commit();
            } else if (input == 3) {
               String query = "SELECT * FROM INSTRUCTOR";
               ResultSet rSet = stmt.executeQuery(query);
               while (rSet.next()) {
                  System.out.println("ID : " + rSet.getString("instructor_ID"));
                  System.out.println("Name : " + rSet.getString("Fname") + " " + rSet.getString("Lname"));
               }
               rSet.close();
               conn.commit();
            } else if (input == 4) {
               String query = "SELECT STUDENT.*, COURSE.* FROM ENROLL INNER JOIN STUDENT ON ENROLL.student_ID = STUDENT.student_ID INNER JOIN COURSE ON ENROLL.course_ID = COURSE.course_ID";
               ResultSet rSet = stmt.executeQuery(query);
               while (rSet.next()) {
                  String full_Student_Name = rSet.getString("Fname") + " " + rSet.getString("Mname") + " "
                        + rSet.getString("Lname");
                  String course_Name = rSet.getString("course_Name");
                  System.out.println("Student : " + full_Student_Name + " enrolled in Course : " + course_Name);
               }
               rSet.close();
               conn.commit();
            } else if (input == 5) {
               String query = "SELECT INSTRUCTOR.*, COURSE.* FROM TEACHES INNER JOIN INSTRUCTOR ON TEACHES.instructor_ID = INSTRUCTOR.instructor_ID INNER JOIN COURSE ON TEACHES.course_ID = COURSE.course_ID";
               ResultSet rSet = stmt.executeQuery(query);
               while (rSet.next()) {
                  String instructor_Full_Name = rSet.getString("Fname") + " " + rSet.getString("Lname");
                  String course_Name = rSet.getString("course_Name");
                  System.out.println("Instructor : " + instructor_Full_Name + " teaches Course : " + course_Name);
               }
               rSet.close();
               conn.commit();
            } else if (input == 6) {
               System.out.println("Enter the ID of the student (5 characters) : ");
               scanner.nextLine();
               String ID = scanner.nextLine();
               System.out.println("Enter the first name : ");
               String fname = scanner.nextLine();
               System.out.println("Enter the middle name : ");
               String mname = scanner.nextLine();
               System.out.println("Enter the last name : ");
               String lname = scanner.nextLine();
               String query = "INSERT INTO STUDENT (student_ID, Fname, Mname, Lname) VALUES (" + "\"" + ID + "\",\""
                     + fname + "\",\"" + mname + "\",\"" + lname + "\")";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 7) {
               System.out.println("Enter the ID of the course (1 character) : ");
               scanner.nextLine();
               String ID = scanner.nextLine();
               System.out.println("Enter the name : ");
               String name = scanner.nextLine();
               String query = "INSERT INTO COURSE (course_ID, course_Name) VALUES (" + "\"" + ID + "\",\"" + name
                     + "\")";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 8) {
               System.out.println("Enter the ID of the instructor: ");
               int ID = scanner.nextInt();
               System.out.println("Enter the first name: ");
               scanner.nextLine();
               String fname = scanner.nextLine();
               System.out.println("Enter the last name: ");
               String lname = scanner.nextLine();
               String query = "INSERT INTO INSTRUCTOR (instructor_ID, Fname, Lname) VALUES (" + ID + ",\"" + fname
                     + "\",\"" + lname + "\")";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 9) {
               System.out.println("Enter the ID: ");
               int ID = scanner.nextInt();
               System.out.println("Enter the student ID : ");
               scanner.nextLine();
               String student_ID = scanner.nextLine();
               System.out.println("Enter the course ID : ");
               String course_ID = scanner.nextLine();
               String query = "INSERT INTO ENROLL (id, student_ID, course_ID) VALUES (" + ID + ",\"" + student_ID
                     + "\",\"" + course_ID + "\")";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 10) {
               System.out.println("Enter the ID : ");
               int ID = scanner.nextInt();
               System.out.println("Enter the instructor ID : ");
               int instructor_ID = scanner.nextInt();
               System.out.println("Enter the course ID : ");
               scanner.nextLine();
               String course_ID = scanner.nextLine();
               String query = "INSERT INTO TEACHES (id, instructor_ID, course_ID) VALUES (" + ID + "," + instructor_ID
                     + ",\"" + course_ID + "\")";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 11) {
               System.out.println("Enter the ID of the student to be updated.");
               scanner.nextLine();
               String student_ID = scanner.nextLine();
               System.out.println("Enter the updated ID : ");
               String updated_ID = scanner.nextLine();
               System.out.println("Enter the updated first name : ");
               // scanner.nextLine();
               String Fname = scanner.nextLine();
               System.out.println("Enter the updated middle name : ");
               // scanner.nextLine();
               String Mname = scanner.nextLine();
               System.out.println("Enter the updated last name : ");
               // scanner.nextLine();
               String Lname = scanner.nextLine();
               String query = "UPDATE STUDENT SET student_ID = \"" + updated_ID + "\", Fname = \"" + Fname
                     + "\", Mname = \"" + Mname + "\", Lname = \"" + Lname + "\" WHERE student_ID = \"" + student_ID
                     + "\"";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 12) {
               System.out.println("Enter the ID of the course to be updated.");
               scanner.nextLine();
               String course_ID = scanner.nextLine();
               System.out.println("Enter the updated ID: ");
               String updated_ID = scanner.nextLine();
               System.out.println("Enter the updated name : ");
               String name = scanner.nextLine();
               String query = "UPDATE COURSE SET course_ID = \"" + updated_ID + "\", course_Name = \"" + name
                     + "\" WHERE course_ID = \"" + course_ID + "\"";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 13) {
               System.out.println("Enter the ID of the instructor to be updated.");
               int instructor_ID = scanner.nextInt();
               System.out.println("Enter the updated ID : ");
               int updated_ID = scanner.nextInt();
               System.out.println("Enter the updated first name : ");
               scanner.nextLine();
               String Fname = scanner.nextLine();
               System.out.println("Enter the updated last name : ");
               String Lname = scanner.nextLine();
               String query = "UPDATE INSTRUCTOR SET instructor_ID = " + updated_ID + ", Fname = \"" + Fname
                     + "\", Lname = \"" + Lname + "\" WHERE instructor_ID = " + instructor_ID;
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 14) {
               System.out.println("Enter the ID of the student to be deleted.");
               scanner.nextLine();
               String student_ID = scanner.nextLine();
               String query = "DELETE FROM STUDENT WHERE student_ID = \"" + student_ID + "\"";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 15) {
               System.out.println("Enter the ID of the course to be deleted.");
               scanner.nextLine();
               String course_ID = scanner.nextLine();
               String query = "DELETE FROM COURSE WHERE course_ID = \"" + course_ID + "\"";
               stmt.executeUpdate(query);
               conn.commit();
            } else if (input == 16) {
               System.out.println("Enter the ID of the instructor to be deleted.");
               int instructor_ID = scanner.nextInt();
               String query = "DELETE FROM INSTRUCTOR WHERE instructor_ID = " + instructor_ID;
               stmt.executeUpdate(query);
               conn.commit();
            }
         }
         scanner.close();
         stmt.close();
         conn.close();
      } catch (SQLException se) {
         // Handle errors for JDBC
         se.printStackTrace();
         // If there is an error then rollback the changes.
         System.out.println("Rolling back data here....");
         try {
            if (conn != null)
               conn.rollback();
         } catch (SQLException se2) {
            System.out.println("Rollback failed....");
            se2.printStackTrace();
         }
      } catch (Exception e) { // Handle errors for Class.forName
         e.printStackTrace();
      } finally { // finally block used to close resources regardless of whether an exception was
                  // thrown or not
         try {
            if (stmt != null)
               stmt.close();
         } catch (SQLException se2) {
         }
         try {
            if (conn != null)
               conn.close();
         } catch (SQLException se) {
            se.printStackTrace();
         } // end finally try
      } // end try
      System.out.println("End of Code");
   } // end main
} // end class

// Note : By default autocommit is on. you can set to false using
// con.setAutoCommit(false)
