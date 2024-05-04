alter table employee add constraint fk_super_ssn FOREIGN KEY employee(super_ssn) REFERENCES employee(ssn);
ALTER TABLE employee add constraint fk_emp_to_dep FOREIGN KEY employee(dno) REFERENCES department(dnumber);
alter table department add constraint fk_manager FOREIGN KEY department(mgr_ssn) REFERENCES employee(ssn);
alter table dept_locations add constraint fk_dept_num FOREIGN KEY dept_locations(dept_number) REFERENCES department(dnumber);
alter table project add constraint fk_dept FOREIGN KEY department(dnum) REFERENCES department(dnumber);
alter table works_on add constraint fk_emp FOREIGN KEY works_on(essn) REFERENCES employee(ssn);
ALTER TABLE works_on add constraint fk_project FOREIGN KEY works_on(pno) REFERENCES project(pnumber);
alter table dependent add constraint fk_employee FOREIGN KEY dependent(emp_ssn) REFERENCES employee(ssn);
