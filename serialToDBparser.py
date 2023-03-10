import serial
import psycopg2
import datetime

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

            # Truncate the milliseconds of the timestamp to two decimal places
            now = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]

            # Insert the data point into the database
            cur.execute("INSERT INTO sensor_data (date, cycle, temperature, humidity, light, magnetic, gas, noiselevel) VALUES (%s, %s, %s, %s, %s, %s, %s, %s)",
                        (now, cycle, sensor_data.get('temperature', 0.0), sensor_data.get('humidity', 0.0), round(sensor_data.get('light', 0.0), 2), round(sensor_data.get('magnetic', 0.0), 2), round(sensor_data.get('gas', 0.0), 2), float(sensor_data.get('noiselevel', 0.0))))
            conn.commit()

            # Reset the sensor data dictionary and cycle number
            sensor_data = {}
            cycle = None
