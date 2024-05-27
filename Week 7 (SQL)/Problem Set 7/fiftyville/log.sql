-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Queries serving only the purspose of me getting familiar with the table ARE NOT included
-- Assuming people are always truthful
-- Assuming car's not stolen
-- Assuming flight wasn't delayed

-- Looking for the other crimes in the same place
SELECT * FROM crime_scene_reports WHERE street = "Humphrey Street";
-- | 295 | 2023 | 7     | 28  | Humphrey Street |
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time –
-- each of their interview transcripts mentions the bakery. |


-- Scanning through ATM transactions on the same day
SELECT * FROM atm_transactions WHERE year = 2023 AND month = 7 AND day = 28;


-- Looking for additional info in interviews
SELECT * FROM interviews WHERE year = 2023 AND month = 7 AND (day = 27 OR day = 28 OR day = 29);
-- Fact 1.
-- Found some on the same day
-- Sometime within ten minutes of the theft,
-- I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot,
-- you might want to look for cars that left the parking lot in that time frame.

-- Fact 2.
-- I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery,
-- I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- Fact 3.
-- As the thief was leaving the bakery,
-- they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the
-- earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Checking fact 1
SELECT * FROM bakery_security_logs WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10;
-- | 260 | 2023 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 261 | 2023 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 262 | 2023 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 263 | 2023 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 264 | 2023 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 265 | 2023 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 266 | 2023 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 267 | 2023 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |

-- Reducing the set of suspects. If the car enters afterwards then unclear. Analyzing more activity.
SELECT * FROM bakery_security_logs WHERE license_plate IN
    (SELECT license_plate FROM bakery_security_logs WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 16 AND minute <= 23);

-- Suspects based on fact 1:
SELECT * FROM people WHERE license_plate IN
    (SELECT DISTINCT license_plate FROM bakery_security_logs WHERE license_plate IN
        (SELECT license_plate FROM bakery_security_logs WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 16 AND minute <= 23));
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+---------+----------------+-----------------+---------------+

-- People who made transactions by means of an ATM on the same day.
-- Suspect based on fact 2
SELECT * FROM people WHERE id IN (
    SELECT person_id FROM bank_accounts WHERE account_number IN (
        SELECT account_number FROM atm_transactions WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
    )
);
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+---------+----------------+-----------------+---------------+

-- People who INITIATED calls on the same day within the specified 10-minute window
-- Suspect based on fact 3
SELECT * FROM people WHERE phone_number IN (
    SELECT caller FROM phone_calls WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
);
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 907148 | Carina  | (031) 555-6622 | 9628244268      | Q12B3Z3       |
-- +--------+---------+----------------+-----------------+---------------+

-- People who RECEIVED calls on the same day within the specified 10-minute window
-- Suspect based on fact 3
SELECT * FROM people WHERE phone_number IN (
    SELECT receiver FROM phone_calls WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
);
-- +--------+------------+----------------+-----------------+---------------+
-- |   id   |    name    |  phone_number  | passport_number | license_plate |
-- +--------+------------+----------------+-----------------+---------------+
-- | 250277 | James      | (676) 555-6554 | 2438825627      | Q13SVG6       |
-- | 251693 | Larry      | (892) 555-8872 | 2312901747      | O268ZZ0       |
-- | 484375 | Anna       | (704) 555-2131 | NULL            | NULL          |
-- | 567218 | Jack       | (996) 555-8899 | 9029462229      | 52R0Y8U       |
-- | 626361 | Melissa    | (717) 555-1342 | 7834357192      | NULL          |
-- | 712712 | Jacqueline | (910) 555-3251 | NULL            | 43V0R5D       |
-- | 847116 | Philip     | (725) 555-3243 | 3391710505      | GW362R6       |
-- | 864400 | Robin      | (375) 555-8161 | NULL            | 4V16VO0       |
-- | 953679 | Doris      | (066) 555-9701 | 7214083635      | M51FA04       |
-- +--------+------------+----------------+-----------------+---------------+

-- Searching for the earliest flight on the 29th, per fact 3
SELECT * FROM flights WHERE year = 2023 AND month = 7 AND day = 29;
-- See id 36
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 18 | 8                 | 6                      | 2023 | 7     | 29  | 16   | 0      |
-- | 23 | 8                 | 11                     | 2023 | 7     | 29  | 12   | 15     |
-- | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
-- | 43 | 8                 | 1                      | 2023 | 7     | 29  | 9    | 30     |
-- | 53 | 8                 | 9                      | 2023 | 7     | 29  | 15   | 20     |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+

-- Searching for potenital suspects among the passengers
SELECT * FROM people WHERE passport_number IN (
    SELECT passport_number FROM passengers WHERE flight_id = 36
);
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 651714 | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 953679 | Doris  | (066) 555-9701 | 7214083635      | M51FA04       |
-- +--------+--------+----------------+-----------------+---------------+

-- Finally, let's combine the information the queries and
-- See suspects0.sql | the perp
-- See accomplices0.sql | the accomplice

-- Perp
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+
-- +----------------+-----------+---------------+
-- | account_number | person_id | creation_year |
-- +----------------+-----------+---------------+
-- | 49610011       | 686048    | 2010          |
-- +----------------+-----------+---------------+
--Outgoing calls
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 9   | (367) 555-5533 | (113) 555-7544 | 2023 | 7     | 25  | 469      |
-- | 104 | (367) 555-5533 | (238) 555-5554 | 2023 | 7     | 26  | 84       |
-- | 122 | (367) 555-5533 | (660) 555-3095 | 2023 | 7     | 26  | 399      |
-- | 133 | (367) 555-5533 | (286) 555-0131 | 2023 | 7     | 26  | 444      |
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2023 | 7     | 28  | 45       |
-- | 236 | (367) 555-5533 | (344) 555-9601 | 2023 | 7     | 28  | 120      |
-- | 245 | (367) 555-5533 | (022) 555-4052 | 2023 | 7     | 28  | 241      |
-- | 285 | (367) 555-5533 | (704) 555-5790 | 2023 | 7     | 28  | 75       |
-- | 395 | (367) 555-5533 | (455) 555-5315 | 2023 | 7     | 30  | 31       |
-- | 418 | (367) 555-5533 | (841) 555-3728 | 2023 | 7     | 30  | 511      |
-- | 488 | (367) 555-5533 | (696) 555-9195 | 2023 | 7     | 31  | 261      |
-- +-----+----------------+----------------+------+-------+-----+----------+
--Incoming calls
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 39  | (020) 555-6715 | (367) 555-5533 | 2023 | 7     | 25  | 414      |
-- | 170 | (695) 555-0348 | (367) 555-5533 | 2023 | 7     | 27  | 218      |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- Car activity: nothing other than on the 28th

-- Doris is the one who received phone calls and took the same flight
-- But Bruce DIDNT CALL HER, instead she's unrelated
SELECT * FROM phone_calls WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60;
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 221 | (130) 555-0289 | (996) 555-8899 | 2023 | 7     | 28  | 51       |
-- | 224 | (499) 555-9472 | (892) 555-8872 | 2023 | 7     | 28  | 36       |
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2023 | 7     | 28  | 45       |
-- | 251 | (499) 555-9472 | (717) 555-1342 | 2023 | 7     | 28  | 50       |
-- | 254 | (286) 555-6063 | (676) 555-6554 | 2023 | 7     | 28  | 43       |
-- | 255 | (770) 555-1861 | (725) 555-3243 | 2023 | 7     | 28  | 49       |
-- | 261 | (031) 555-6622 | (910) 555-3251 | 2023 | 7     | 28  | 38       |
-- | 279 | (826) 555-1652 | (066) 555-9701 | 2023 | 7     | 28  | 55       |
-- | 281 | (338) 555-6650 | (704) 555-2131 | 2023 | 7     | 28  | 54       |
-- +-----+----------------+----------------+------+-------+-----+----------+

-- Accomplice
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+
-- +----------------+-----------+---------------+
-- | account_number | person_id | creation_year |
-- +----------------+-----------+---------------+
-- | 94751264       | 864400    | 2019          |
-- +----------------+-----------+---------------+

SELECT * FROM airports;
-- +----+--------------+-----------------------------------------+---------------+
-- | id | abbreviation |                full_name                |     city      |
-- +----+--------------+-----------------------------------------+---------------+
-- | 1  | ORD          | O'Hare International Airport            | Chicago       |
-- | 2  | PEK          | Beijing Capital International Airport   | Beijing       |
-- | 3  | LAX          | Los Angeles International Airport       | Los Angeles   |
-- | 4  | LGA          | LaGuardia Airport                       | New York City |
-- | 5  | DFS          | Dallas/Fort Worth International Airport | Dallas        |
-- | 6  | BOS          | Logan International Airport             | Boston        |
-- | 7  | DXB          | Dubai International Airport             | Dubai         |
-- | 8  | CSF          | Fiftyville Regional Airport             | Fiftyville    |
-- | 9  | HND          | Tokyo International Airport             | Tokyo         |
-- | 10 | CDG          | Charles de Gaulle Airport               | Paris         |
-- | 11 | SFO          | San Francisco International Airport     | San Francisco |
-- | 12 | DEL          | Indira Gandhi International Airport     | Delhi         |
-- +----+--------------+-----------------------------------------+---------------+
-- The thief escaped to NYC
