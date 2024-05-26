SELECT DISTINCT people.name
FROM directors
JOIN movies
ON directors.movie_id = movies.id
JOIN ratings
ON ratings.movie_id = movies.id
JOIN people
ON people.id = directors.person_id
WHERE ratings.rating >= 9.0;
