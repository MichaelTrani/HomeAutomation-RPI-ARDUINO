CREATE TABLE public.sensor_data (
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    initialconnection bool,
    cycle INT,
    temperature REAL,
    humidity INT,
    light INT,
    magnetic INT,
    gas INT,
    noiselevel INT
);
