SELECT
    LAST_NAME
FROM
    EMPLOYEES
WHERE
    SALARY > (
        SELECT
            MAX(SALARY)
        FROM
            EMPLOYEES
        WHERE
            DEPARTMENT_ID = 60
    );