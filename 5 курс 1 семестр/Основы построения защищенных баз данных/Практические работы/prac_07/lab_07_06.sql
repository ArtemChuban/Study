SELECT
    EMPLOYEES.LAST_NAME,
    EMPLOYEES.JOB_ID,
    DEPARTMENTS.DEPARTMENT_NAME,
    EMPLOYEES.SALARY,
    JOB_GRADES.GRADE_LEVEL
FROM
    EMPLOYEES
        NATURAL JOIN
    DEPARTMENTS
        NATURAL JOIN
    JOB_GRADES
WHERE
    EMPLOYEES.SALARY > JOB_GRADES.LOWEST_SAL
        AND
    EMPLOYEES.SALARY < JOB_GRADES.HIGHEST_SAL;