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
spirit = 50

# GPIO pin number of LED
#LED = 22

# Twitter application authentication
TWITTER_CONSUMER_KEY = 'xxxx'
TWITTER_CONSUMER_SECRET = 'xxxx'
TWITTER_ACCESS_TOKEN = 'xxxx'
TWITTER_ACCESS_TOKEN_SECRET = 'xxxx'

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
            spirit = spirit + 1
            if spirit > 100 :
                spirit = 100

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

def spirit_fader(): # reduces spirit over time
    global spirit
    while True:
        time.sleep(1)
        if spirit > 0 :
            spirit = spirit - 1

# Setup GPIO as output
#GPIO.setmode(GPIO.BOARD)
#GPIO.setup(LED, GPIO.OUT)
#GPIO.output(LED, GPIO.LOW)

# Create streamer
try:
    Thread(target=start_twitter_monitoring).start()
    Thread(target=spirit_fader).start()
    while True:
        print ('spirit = ', spirit)
        time.sleep(1)
    
except KeyboardInterrupt:
    stopping = True
    print ('Exiting app')
    #GPIO.cleanup()

