SELECT LAST_NAME, HIRE_DATE
FROM EMPLOYEES
WHERE EXTRACT(YEAR FROM HIRE_DATE) = 2006;