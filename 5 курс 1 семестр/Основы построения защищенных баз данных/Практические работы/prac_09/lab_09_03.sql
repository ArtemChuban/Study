SELECT
    EMPLOYEE_ID,
    JOB_ID,
    DEPARTMENT_ID
FROM
    EMPLOYEES
WHERE
    DEPARTMENT_ID = 20

UNION

SELECT
    EMPLOYEE_ID,
    JOB_ID,
    DEPARTMENT_ID
FROM
    EMPLOYEES
WHERE
    DEPARTMENT_ID = 30;