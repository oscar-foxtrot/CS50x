SELECT title
FROM (
    SELECT *
    FROM stars
    JOIN movies
    ON stars.movie_id = movies.id
    JOIN people
    ON people.id = stars.person_id
    WHERE people.name = "Chadwick Boseman"
) AS temp
JOIN ratings
ON temp.movie_id = ratings.movie_id
ORDER BY rating DESC
LIMIT 5;
