SELECT
    EMPLOYEE_ID,
    LAST_NAME,
    SALARY
FROM
    EMPLOYEES
WHERE
    SALARY > (
        SELECT
            SUM(SALARY) / COUNT(*)
        FROM
            EMPLOYEES
    )
ORDER BY
    SALARY;