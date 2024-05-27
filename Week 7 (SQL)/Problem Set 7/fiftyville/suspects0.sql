SELECT * FROM people WHERE id IN (
    -- Suspects based on fact 1 (security logs)
    SELECT p0.id FROM (
        SELECT id
        FROM people
        WHERE license_plate IN (
            SELECT DISTINCT license_plate
            FROM bakery_security_logs
            WHERE license_plate IN (
                SELECT license_plate
                FROM bakery_security_logs
                WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 16 AND 23
            )
        )
    ) AS p0
    -- Suspect based on fact 2 (atm)
    JOIN (
        SELECT id
        FROM people
        WHERE id IN (
            SELECT person_id
            FROM bank_accounts
            WHERE account_number IN (
                SELECT account_number
                FROM atm_transactions
                WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
            )
        )
    ) AS p1
    ON p0.id = p1.id
    -- Suspects based on fact 3 (calls)
    JOIN (
        SELECT id
        FROM people
        WHERE phone_number IN (
            SELECT caller
            FROM phone_calls
            WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
        )
    ) AS p2
    ON p1.id = p2.id
    -- Suspects based on fact 3 (passengers)
    JOIN (
        SELECT id
        FROM people
        WHERE passport_number IN (
            SELECT passport_number
            FROM passengers
            WHERE flight_id = 36
        )
    ) AS p3
    ON p3.id = p2.id
);
