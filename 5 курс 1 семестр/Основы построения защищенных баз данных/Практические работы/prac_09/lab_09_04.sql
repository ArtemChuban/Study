SELECT
    EMPLOYEE_ID,
    FIRST_NAME,
    LAST_NAME
FROM
    EMPLOYEES
WHERE
    DEPARTMENT_ID = (
        SELECT
            DEPARTMENT_ID
        FROM
            DEPARTMENTS
        WHERE
            DEPARTMENTS.DEPARTMENT_NAME = 'Sales'
    )
        AND
    JOB_ID = 'SA_REP';