#
# Python Twitter monitor for Raspberry Pi
#
# See https://learn.sparkfun.com/tutorials/raspberry-pi-twitter-monitor
#
# For Twitter keys, see:
#   https://developer.twitter.com/
#   https://apps.twitter.com/
#
# Provided under MIT licence. (mostly meaning: use it for anything, don't sue me for anything)

import time
import RPi.GPIO as GPIO
from twython import TwythonStreamer
from threading import Thread

# Search terms
TERMS = '#xmas, #santa, #christmas, #navidad'  # separate terms with commas. '#xmas, #holiday, #christmas'

# Christmas spirit tracker: 0 to 100
spirit = 50.0

GPIO.setmode(GPIO.BOARD)
# set GPIO18 to be a PWM output = pin 12
GPIO.setup(12, GPIO.OUT)
servo = GPIO.PWM(12, 50)
servo.start(7.5)

# Twitter application authentication
TWITTER_CONSUMER_KEY = 'xxxxxxx'
TWITTER_CONSUMER_SECRET = 'xxxxxxx'
TWITTER_ACCESS_TOKEN = 'xxxxxxx'
TWITTER_ACCESS_TOKEN_SECRET = 'xxxxxxx'

# To stop twitter thread on app exit
stopping = False

# Setup callbacks from Twython Streamer
class ClausometerStreamer(TwythonStreamer):
    def on_success(self, data):
        global spirit, stopping
        if stopping:
            #print('disconnecting from twitter...')
            self.disconnect()
        if 'text' in data:
            #print (data['text'].encode('utf-8'))
            #print ('spirit++')
            spirit = spirit + 1.0
            if spirit > 100.0 :
                spirit = 100.0

def start_twitter_monitoring():
    global stream
    stream = ClausometerStreamer(TWITTER_CONSUMER_KEY, TWITTER_CONSUMER_SECRET, TWITTER_ACCESS_TOKEN, TWITTER_ACCESS_TOKEN_SECRET)
    while True:
        try:
            stream.statuses.filter(track=TERMS)
        except:
            print("Twitter oops. carrying on.")
            # this can happen when connection is lost
            # the while loop will start it up again

# 0 degrees = 2.5
# 90 degrees = 7.5
# 180 degrees = 12.5
previousDutyCycle = 3.0
def setServoPercent(percent):
    global previousDutyCycle, servo
    newDutyCycle = 3.0 + percent / 14.3
    #print('New duty cycle = ', newDutyCycle)
    increment = (newDutyCycle - previousDutyCycle) / 100.0
    for step in range(0, 100, 1):
        servo.ChangeDutyCycle(previousDutyCycle + increment*step)
        time.sleep(0.02)
    previousDutyCycle = newDutyCycle

def spirit_fader(): # reduces spirit over time
    global spirit
    while True:
        time.sleep(1)
        if spirit > 0 :
            spirit = spirit - 1.0

# Start the Clausometer
try:
    Thread(target=start_twitter_monitoring).start()
    Thread(target=spirit_fader).start()
    while True:
        print ('spirit = ', spirit)
        setServoPercent(spirit)
    
except KeyboardInterrupt:
    stopping = True
    print ('Exiting app')
    #GPIO.cleanup()

