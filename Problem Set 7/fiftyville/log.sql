-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT description FROM crime_scene_reports WHERE month=7 AND day=28 AND street="Chamberlin Street";
-- Theft took place 10:15am
-- Find out what interviewes reported on this crime
.schema
SELECT name,transcript FROM interviews WHERE month=7 AND day=28;
-- Ruth: Thief license_plate in courthouse_security_logs
-- Eugene: Thief withdraw at atm same day on Fifer Street
-- Raymond: Thief on phone just after theft
-- Raymond: Thief on passenger flight list for day after
SELECT * FROM people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE month=7
AND day=28
AND hour = 10
AND minute > 15
AND minute < 30
AND activity="exit"
AND phone_number
IN (SELECT caller FROM phone_calls WHERE month=7 AND day=28 AND duration < 60)
AND people.id IN (SELECT person_id FROM bank_accounts JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE year=2020 AND month=7 AND day=28 AND atm_location="Fifer Street" AND transaction_type="withdraw"
AND people.passport_number
IN (SELECT passport_number FROM passengers JOIN flights ON passengers.flight_id = flights.id
WHERE day=29 AND month=7 AND origin_airport_id=(SELECT DISTINCT id FROM airports WHERE city="Fiftyville")));

-- Ernest and Russel is the only ones who exited the courtyard with their car around that time, made a phonecall on that day, withdrew cash on Fifer Street that day and did a fligth for the day after.

-- All receivers of Ernest and Russels calls
SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM people JOIN phone_calls ON people.phone_number = phone_calls.caller WHERE name IN ("Ernest","Russell"));

-- All receivers of Ernest and Russells calls on raymonds witness timespan
SELECT name FROM people WHERE phone_number IN (
SELECT receiver FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
AND phone_calls.month=7 AND phone_calls.day=28 AND phone_calls.duration < 60 AND name IN ("Ernest","Russell"));
-- Philip, Berthold

-- All people on flight 36 that made phone calls at the timespan within Raymonds testimony
SELECT * FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
JOIN flights ON flight_id = flights.id
JOIN airports ON origin_airport_id = airports.id
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE flight_id IN ("36","18","24","54")
AND phone_calls.month=7 AND phone_calls.day=28 AND phone_calls.duration < 60;

-- City Ernest escaped to
SELECT city FROM airports WHERE id=(SELECT destination_airport_id FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
JOIN flights ON flight_id = flights.id
WHERE name="Ernest");

-- People who flew from fiftyville the day after to same destination as Ernest
SELECT * FROM people WHERE passport_number IN (
SELECT passport_number FROM passengers JOIN flights ON passengers.flight_id = flights.id
WHERE day=29 AND month=7 AND origin_airport_id=(SELECT DISTINCT id FROM airports WHERE city="Fiftyville") AND destination_airport_id=(SELECT DISTINCT id FROM airports WHERE city="London"));