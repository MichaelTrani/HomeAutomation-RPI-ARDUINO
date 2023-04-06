CREATE TABLE public.sensor_data (
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    cycle INT,
    temperature REAL,
    humidity INT,
    light INT,
    magnetic INT,
    gas INT,
    noiselevel INT
);
