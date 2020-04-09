import serial
import time
import subprocess

def mh_z19():
  ser = serial.Serial('/dev/serial0',
                      baudrate=9600,
                      bytesize=serial.EIGHTBITS,
                      parity=serial.PARITY_NONE,
                      stopbits=serial.STOPBITS_ONE,
                      timeout=1.0)
  while 1:
    result=ser.write(b"\xff\x01\x86\x00\x00\x00\x00\x00\x79")
    s=ser.read(9)
    if len(s) >= 4 and s[0] == 0xff and s[1] == 0x86:
      co2 = s[2]*256 + s[3]
      print("co2 : %d ppm" % (co2))
#      print("%d" % (co2))
      time.sleep(1)
      result=ser.write(b"\xff\x01\x79\x00\x00\x00\x00\x00\x86")
      time.sleep(1)
      return
    break

if __name__ == '__main__':
  mh_z19()
