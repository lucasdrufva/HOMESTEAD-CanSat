import serial

ser = serial.Serial(port="/dev/tty.usbmodem2101",
                                  timeout=1,
                                  baudrate=9600)


mission = 0

file1 = open(f"raw/data{mission}.txt", 'w')

while True:
    rec = ser.readline().decode('utf-8')[8:]
    if rec[0:2] == "0b" and rec[10:12]=="01":
        print("New mission")
        mission += 1
        file1.close()
        file1 = open(f"raw/data{mission}.txt", 'w') 

    file1.write(rec)