SELECT title FROM stars
JOIN people ON stars.person_id = people.id
JOIN movies ON stars.movie_id = movies.id
WHERE name="Helena Bonham Carter" OR name="Johnny Depp"
GROUP BY movie_id
HAVING COUNT(movie_id) > 1;