SELECT
    LAST_NAME,
    DEPARTMENT_ID,
    JOB_ID
FROM
    EMPLOYEES
WHERE
    DEPARTMENT_ID IN (
        SELECT
            DEPARTMENT_ID
        FROM
            DEPARTMENTS
        WHERE
            LOCATION_ID = 1700
    );