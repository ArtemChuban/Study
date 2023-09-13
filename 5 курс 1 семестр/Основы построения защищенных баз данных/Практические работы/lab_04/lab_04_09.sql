SELECT
    LAST_NAME,
    ROUND(((SELECT SYSDATE FROM SYS.DUAL) - HIRE_DATE) / 7)
FROM
    EMPLOYEES
WHERE
    DEPARTMENT_ID = 90
ORDER BY
    HIRE_DATE;