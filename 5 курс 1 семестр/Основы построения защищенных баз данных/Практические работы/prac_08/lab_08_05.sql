SELECT
    LAST_NAME,
    SALARY
FROM
    EMPLOYEES
WHERE
    MANAGER_ID = (
        SELECT
            EMPLOYEE_ID
        FROM
            EMPLOYEES
        WHERE
            LAST_NAME = 'King'
                AND
            FIRST_NAME = 'Steven'
    )