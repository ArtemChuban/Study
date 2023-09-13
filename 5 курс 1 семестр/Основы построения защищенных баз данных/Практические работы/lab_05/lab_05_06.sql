SELECT
    LAST_NAME,
    DECODE(JOB_ID, 'AD_PRES', 'A',
        DECODE(JOB_ID, 'ST_MAN', 'B',
            DECODE(JOB_ID, 'IT_PROG', 'C',
                DECODE(JOB_ID, 'SA_REP', 'D',
                    DECODE(JOB_ID, 'ST_CLERK', 'E', '0')
                )
            )
        )
    )
FROM
    EMPLOYEES;