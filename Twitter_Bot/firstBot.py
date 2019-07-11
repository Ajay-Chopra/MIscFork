# This is a Twitter Bot that allows the user to automatically either retweet
# or respond to the specified user's Tweets whenever they post a new Tweet

# In order to use this application, the user must have current Twitter developer
# account and create a Twitter app
# They must also generate their own API keys

import tweepy
import time
import datetime


## *********************** BEGIN GLOBAL VARIABLE DEFINITIONS ********************
acconutName = '@mPinoe' # Set the account you would like to respond to or retweet





CONSUMER_KEY = ''
CONSUMER_SECRET = ''
ACCESS_KEY = ''
ACCESS_SECRET = ''

auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
auth.set_access_token(ACCESS_KEY, ACCESS_SECRET)
api = tweepy.API(auth)

FILE_NAME = 'lastDate.txt'


def retrieve_date(file_name):
    f_read = open(file_name, 'r')
    last_seen_id = int(f_read.read().strip())
    f_read.close()
    return last_seen_id


def store_date(last_date, file_name):
    f_write = open(file_name, 'w')
    f_write.write(str(last_date))
    f_write.close()
    return

def to_integer(dt_time):
    return 10000*dt_time.year + 100*dt_time.month + dt_time.day




def performRetweet():
    print('**************************START**********************', flush=True)
    last_date = retrieve_date(FILE_NAME)
    print('AFTER READ IN')
    print(last_date)

    statuses = api.user_timeline(acconutName)
    statsRecent = []

    for tweets in statuses:
        if (to_integer(tweets.created_at) > last_date):
            statsRecent.append(tweets)


    statuses = statsRecent

    for tweets in reversed(statuses):
        print(tweets.text)
        print(str(to_integer(tweets.created_at)))
        api.retweet(tweets.id)
        new_last_seen = to_integer(tweets.created_at)
        store_date(new_last_seen, FILE_NAME)


while True:
    date = retrieve_date(FILE_NAME)
    print('BEFORE READ IN: ')
    print(str(date))
    performRetweet()
    time.sleep(3)
