SELECT
    COUNT(*)
FROM
    EMPLOYEES
GROUP BY
    EXTRACT(YEAR FROM HIRE_DATE);