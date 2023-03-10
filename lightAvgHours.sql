SELECT EXTRACT(HOUR FROM sensor_data.date) AS hour,
       AVG(sensor_data.light) AS average
FROM sensor_data
WHERE sensor_data.date BETWEEN '2023-03-05' AND '2023-03-08'
GROUP BY hour
ORDER BY hour;
