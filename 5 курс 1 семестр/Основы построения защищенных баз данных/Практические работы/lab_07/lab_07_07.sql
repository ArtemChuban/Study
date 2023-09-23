SELECT
    LAST_NAME,
    HIRE_DATE
FROM
    EMPLOYEES
WHERE
    HIRE_DATE > (
        SELECT
            HIRE_DATE
        FROM
            EMPLOYEES
        WHERE
            LAST_NAME = 'Davies'
    );