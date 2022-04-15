'''
Serial port starter code
EE445L Lab 10 option
April 12, 2022
Jon Valvano
'''
#from command line
#py -m pip install pyserial
#py -m pip install matplotlib

import serial
import matplotlib.pyplot as plt
import time
from time import sleep
from serial import Serial
ser = serial.Serial('COM5', 115200)
print('Connected to',ser.name)             # check which port was really used
print('Baud rate',ser.baudrate,'bits/sec') # baud rate

serialString = ""                          # Used to hold data coming over UART
command = ""
valuestr = ""
value = 0
ser.timeout = 1
print("COMMAND GUIDE:")
print("M -> change motor speed")
print("P -> change proportional constants")
print("I -> change integral constants")
while command != 'q': #Main loop
    command = input("Command>")
    if command == "M":
        motor_speed = input("Value=")
        motor_dec = float(motor_speed)
        motor_dec *= 10
        motor_int = int(motor_dec)
        motor_str = str(motor_int)
        ser.write('M'.encode('ASCII'))
        ser.write(motor_str.encode('ASCII'))
        ser.write('\r'.encode('ASCII'))
    if command == "P":
        kp1 = input("kp1=")
        kp2 = input("kp2=")
        ser.write('P'.encode('ASCII'))
        ser.write(kp1.encode('ASCII'))
        ser.write('\r'.encode('ASCII'))
        ser.write(kp2.encode('ASCII'))
        ser.write('\r'.encode('ASCII'))
    if command == "I":
        ki1 = input("ki1=")
        ki2 = input("ki2=")
        ser.write('I'.encode('ASCII'))
        ser.write(ki1.encode('ASCII'))
        ser.write('\r'.encode('ASCII'))
        ser.write(ki2.encode('ASCII'))
        ser.write('\r'.encode('ASCII'))








