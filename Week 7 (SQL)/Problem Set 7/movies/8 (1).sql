SELECT name
FROM (
    SELECT *
    FROM movies JOIN stars
    ON movies.id = stars.movie_id WHERE title LIKE "Toy Story"
) AS movie_stars JOIN people
ON people.id = movie_stars.person_id;
