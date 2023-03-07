#!/usr/bin/env python
import serial
import psycopg2

# Connect to the database
conn = psycopg2.connect(dbname='environmentsensors', user='arduino', password='arduino', host='localhost')
cur = conn.cursor()

# Connect to the serial port
ser = serial.Serial('/dev/ttyACM0', 9600)

# create an empty dictionary to hold the sensor data
sensor_data = {}
cycle = None

# Read data from the serial port
while True:
    # Read a line from the serial port
    line = ser.readline().decode().strip()

    if line.startswith('-'):
        print("Arduino has reported: Initialized")
        continue

    if line.startswith('#'):
        # Get the cycle number
        cycle = int(line.split(':')[1])
        continue

    else:
        # Split the line by the colon character
        name, value = line.strip().split(':')

        # Strip whitespace from the name and value
        name = name.strip()
        value = value.strip()

        # Add the data point to the dictionary
        sensor_data[name.lower().replace(' ', '')] = float(value)
        if len(sensor_data) == 6:
            print(sensor_data)

            # Insert the data point into the database
            cur.execute(f"INSERT INTO sensor_data (date, cycle, temperature, humidity, light, magnetic, gas, noiselevel) VALUES (CURRENT_TIMESTAMP, %s, %s, %s, %s, %s, %s, %s)", (cycle, sensor_data['temperature'], sensor_data['humidity'], sensor_data['light'], sensor_data['magnetic'], sensor_data['gas'], sensor_data['noiselevel']))
            conn.commit()

            # Reset the sensor data dictionary and cycle number
            sensor_data = {}
            cycle = None
