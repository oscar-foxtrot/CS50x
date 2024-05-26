SELECT title
FROM (
    SELECT title, COUNT(*) AS counts
    FROM stars
    JOIN movies
    ON stars.movie_id = movies.id
    JOIN people
    ON people.id = stars.person_id
    WHERE people.name = "Jennifer Lawrence" OR people.name = "Bradley Cooper"
    GROUP BY movie_id
) WHERE counts = 2;
