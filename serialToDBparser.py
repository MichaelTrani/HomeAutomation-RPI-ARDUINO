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

# create an empty dictionary to hold the plant data
plant_data = {}

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
        if name.lower() == 'plantmoisture':
            plant_num, moisture = value.split(',')
            plant_data[f'p{plant_num}'] = int(moisture)
        else:
            sensor_data[name.lower().replace(' ', '')] = float(value)



        if len(sensor_data) == 6 and len(plant_data) == 5:
            print(sensor_data)
            print(plant_data)

            # Insert the data point into sensor_data table
            cur.execute(f"INSERT INTO sensor_data (date, cycle, temperature, humidity, light, magnetic, gas, noiselevel) VALUES (CURRENT_TIMESTAMP, %s, %s, %s, %s, %s, %s, %s)", (cycle, sensor_data['temperature'], sensor_data['humidity'], sensor_data['light'], sensor_data['magnetic'], sensor_data['gas'], sensor_data['noiselevel']))
            conn.commit()

            # Insert the data point into plant_data table
            cur.execute(f"INSERT INTO plant_data (date, cycle, p0, p1, p2, p3, p4) VALUES (CURRENT_TIMESTAMP, %s, %s, %s, %s, %s, %s)", (cycle, plant_data.get('p0', None), plant_data.get('p1', None), plant_data.get('p2', None), plant_data.get('p3', None), plant_data.get('p4', None)))
            conn.commit()

            # Reset the sensor data and plant data dictionaries, and cycle number
            sensor_data = {}
            plant_data = {}
            cycle = None
