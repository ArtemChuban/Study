SELECT
    DEPARTMENT_ID,
    LAST_NAME,
    JOB_ID
FROM
    EMPLOYEES
WHERE
    DEPARTMENT_ID = (
        SELECT
            DEPARTMENT_ID
        FROM
            DEPARTMENTS
        WHERE
            DEPARTMENT_NAME = 'Executive'
    );