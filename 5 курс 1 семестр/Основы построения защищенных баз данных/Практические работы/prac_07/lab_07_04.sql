SELECT
    EMP.LAST_NAME,
    EMP.EMPLOYEE_ID,
    MNG.LAST_NAME,
    MNG.EMPLOYEE_ID
FROM
    EMPLOYEES EMP
    INNER JOIN
    EMPLOYEES MNG
    ON EMP.MANAGER_ID = MNG.EMPLOYEE_ID;