create table department(
	dname varchar(30) NOT NULL,
	dnumber smallint,
	mgr_ssn char(6),
	mgr_start_date date,
	constraint pk_department PRIMARY KEY (dnumber)
);
create table employee(
	fname varchar(50) NOT NULL,
	minit varchar(50) NOT NULL,
	lname varchar(50) NOT NULL,
	ssn char(6),
	bdate date,
	address varchar(200),
	sex char(1),
	salary smallint,
	super_ssn char(6),
	dno smallint,
	constraint pk_employee PRIMARY KEY (ssn)
);
create table project(
	pname varchar(50) NOT NULL,
	pnumber smallint,
	plocation varchar(100) NOT NULL,
	dnum smallint,
	constraint pk_project PRIMARY KEY (pnumber)
);
create table dept_locations(
	dept_number smallint,
	dlocation varchar(50),
	constraint pk_dept_locations PRIMARY KEY (dept_number, dlocation)
);
create table works_on(
	essn char(6),
	pno smallint,
	hours smallint,
	constraint pk_works_on PRIMARY KEY (essn, pno)
);
create table dependent(
	emp_ssn char(6),
	dependent_name varchar(20),
	sex char(1),
	birth_date date,
	relationship varchar(20),
	constraint pk_dependent PRIMARY KEY (emp_ssn, dependent_name)
);
