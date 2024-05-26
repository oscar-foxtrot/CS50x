SELECT DISTINCT name
FROM people
JOIN (
    SELECT *
    FROM stars
    WHERE movie_id
    IN (
        SELECT movie_id
        FROM people
        JOIN stars
        ON stars.person_id = people.id
        JOIN movies
        ON stars.movie_id = movies.id
        WHERE name = "Kevin Bacon" AND birth = 1958
    )
) AS movs ON movs.person_id = people.id
WHERE name != "Kevin Bacon";
