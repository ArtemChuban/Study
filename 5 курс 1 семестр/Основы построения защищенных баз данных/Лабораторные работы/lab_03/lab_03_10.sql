accept x number prompt 'Salary';
SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE SALARY > &x;