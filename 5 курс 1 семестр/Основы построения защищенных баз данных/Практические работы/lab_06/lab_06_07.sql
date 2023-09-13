SELECT
    MANAGER_ID,
    MIN(SALARY)
FROM
    EMPLOYEES
WHERE
    MANAGER_ID IS NOT NULL
        AND
    SALARY > 6000
GROUP BY
    MANAGER_ID
ORDER BY
    MIN(SALARY) DESC;