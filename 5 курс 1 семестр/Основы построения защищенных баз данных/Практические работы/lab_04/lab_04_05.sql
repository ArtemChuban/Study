accept letter char prompt 'Enter character';

SELECT
     INITCAP(LAST_NAME) AS "Name",
     LENGTH(LAST_NAME) AS "Length"
FROM
    EMPLOYEES
WHERE
    UPPER(LAST_NAME) LIKE UPPER('&letter%')
ORDER BY
    LAST_NAME;