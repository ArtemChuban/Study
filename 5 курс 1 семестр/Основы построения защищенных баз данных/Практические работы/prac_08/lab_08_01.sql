ACCEPT EMP CHAR PROMPT 'EMP';

SELECT
    LAST_NAME,
    HIRE_DATE
FROM
    EMPLOYEES
WHERE DEPARTMENT_ID IN (
            SELECT
                DEPARTMENT_ID
            FROM
                EMPLOYEES
            WHERE LAST_NAME = '&emp'
    )
AND LAST_NAME != '&emp';