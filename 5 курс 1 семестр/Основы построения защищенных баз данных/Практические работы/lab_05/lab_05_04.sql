SELECT
    LAST_NAME,
    CASE JOB_ID
        WHEN 'AD_PRES' THEN
            'A'
        WHEN 'ST_MAN' THEN
            'B'
        WHEN 'IT_PROG' THEN
            'C'
        WHEN 'SA_REP' THEN
            'D'
        WHEN 'ST_CLERK' THEN
            'E'
        ELSE '0'
        END AS GRADE
FROM
    EMPLOYEES;