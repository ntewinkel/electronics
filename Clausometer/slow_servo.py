#
# Servo Control
# see https://learn.adafruit.com/adafruits-raspberry-pi-lesson-8-using-a-servo-motor/software
#
# For twitter monitoring,
# see https://learn.sparkfun.com/tutorials/raspberry-pi-twitter-monitor
#
# Provided under MIT licence. (mostly meaning: use it for anything, don't sue me for anything)

import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)

# set GPIO18 to be a PWM output = pin 12
GPIO.setup(12, GPIO.OUT)

p = GPIO.PWM(12, 50)

p.start(7.5)

# 0 degrees = 2.5
# 90 degrees = 7.5
# 180 degrees = 12.5
previousDutyCycle = 3.0
def setServoPercent(percent):
    global previousDutyCycle
    newDutyCycle = 3.0 + percent / 14.3
    #print('New duty cycle = ', newDutyCycle)
    increment = (newDutyCycle - previousDutyCycle) / 100.0
    for step in range(0, 100, 1):
        p.ChangeDutyCycle(previousDutyCycle + increment*step)
        time.sleep(0.02)
    previousDutyCycle = newDutyCycle

try:
    while True:
        setServoPercent(50.0)
        time.sleep(1) # sleep 1 second
        setServoPercent(100.0)
        time.sleep(1)
        setServoPercent(0.0)
        time.sleep(1)
except KeyboardInterrupt:
    p.stop()
    GPIO.cleanup()
