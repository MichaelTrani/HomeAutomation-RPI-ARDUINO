SELECT EXTRACT(HOUR FROM sensor_data.date) AS hour,
       AVG(sensor_data.light) AS average
FROM sensor_data
GROUP BY hour
ORDER BY hour;
