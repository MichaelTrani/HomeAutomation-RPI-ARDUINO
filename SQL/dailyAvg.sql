SELECT date_trunc('day', date) AS date,
       ROUND(AVG(temperature)::numeric, 2) AS avg_temperature,
       ROUND(AVG(humidity)::numeric, 2) AS avg_humidity,
       ROUND(AVG(light)::numeric, 2) AS avg_light,
       ROUND(AVG(magnetic)::numeric, 2) AS avg_magnetic,
       ROUND(AVG(gas)::numeric, 2) AS avg_gas,
       ROUND(AVG(noiselevel)::numeric, 2) AS avg_noiselevel,
       MAX(cycle) AS last_cycle
FROM sensor_data
WHERE date >= NOW() - INTERVAL '24 HOURS'
GROUP BY date_trunc('day', date);

INSERT INTO daily_sensor_data
SELECT date_trunc('day', date) AS date,
       AVG(temperature) AS avg_temperature,
       AVG(humidity) AS avg_humidity,
       AVG(light) AS avg_light,
       AVG(magnetic) AS avg_magnetic,
       AVG(gas) AS avg_gas,
       AVG(noiselevel) AS avg_noiselevel,
       MAX(cycle) AS last_cycle
FROM sensor_data
GROUP BY date_trunc('day', date);
