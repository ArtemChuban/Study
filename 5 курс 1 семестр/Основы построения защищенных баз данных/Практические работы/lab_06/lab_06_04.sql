ACCEPT JOB_ID CHAR PROMPT 'JOB_ID';

SELECT
    JOB_ID,
    COUNT(*)
FROM
    EMPLOYEES
WHERE
    JOB_ID = '&JOB_ID'
GROUP BY
    JOB_ID;