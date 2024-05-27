SELECT * FROM people WHERE id IN (
    -- Suspects based on fact 3 (received calls)
    SELECT id FROM (
        SELECT id
        FROM people
        WHERE phone_number IN (
            SELECT receiver
            FROM phone_calls
            WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60 AND caller = "(367) 555-5533"
        )
    )
);
