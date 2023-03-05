import serial
import psycopg2

# Connect to the database
conn = psycopg2.connect(dbname='environmentsensors', user='', password='', host='localhost')
cur = conn.cursor()

# Connect to the serial port
ser = serial.Serial('/dev/ttyACM0', 9600)


cycle = None
# Read data from the serial port
while True:
    # Read a line from the serial port
    line = ser.readline().decode().strip()
    if line.startswith('-'):
        print("Arduino has reported: Initialized");
        initialconnection = True
        continue

    if line.startswith('#'):
        # Remove the '#' character from the line
        line = line.replace('#', '')
        # Get the cycle number
        cycle = int(line.split(':')[1])
        continue

    else:
        # Split the line by the colon character
        name, value = line.strip().split(':')

        # Strip whitespace from the name and value
        name = name.strip()
        value = value.strip()

        # Insert the data point into the database
        cur.execute(f"INSERT INTO sensor_data (date, cycle, \"{name.lower().replace(' ', '')}\") VALUES (CURRENT_TIMESTAMP, %s, %s::float)", (cycle, float(value)))
        conn.commit()