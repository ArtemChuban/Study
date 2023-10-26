SELECT
    EMPLOYEE_ID,
    LAST_NAME,
    SALARY,
    SALARY * 1.155 AS "New Salary",
    SALARY * 0.155 AS "Increase"
FROM
    EMPLOYEES;