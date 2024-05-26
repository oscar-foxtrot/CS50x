SELECT DISTINCT name
FROM movies JOIN stars
ON movies.id = stars.movie_id
JOIN people
ON people.id = stars.person_id
WHERE movies.year = 2004
ORDER BY birth;
