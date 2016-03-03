from pyGreeney import pyGreeney
import time

b = pyGreeney('/dev/ttyACM0')
pin = 13
#day = 1
#night = 1

b.output([pin])
#b.output([pin], day, night)

b.setHigh(pin)
time.sleep(10)
print b.getState(pin)
b.setLow(pin)
print b.getState(pin)
time.sleep(10)

b.close()

