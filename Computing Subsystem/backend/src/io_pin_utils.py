
from flask import Flask, render_template
import time
from RF24 import *
import RPi.GPIO as GPIO
irq_gpio_pin = None
########### USER CONFIGURATION ###########
# See https://github.com/TMRh20/RF24/blob/master/pyRF24/readme.md
# CE Pin, CSN Pin, SPI Speed
# Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 8Mhz
radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)
#RPi B
# Setup for GPIO 15 CE and CE1 CSN with SPI Speed @ 8Mhz
#radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)
#RPi B+
# Setup for GPIO 22 CE and CE0 CSN for RPi B+ with SPI Speed @ 8Mhz
#radio = RF24(RPI_BPLUS_GPIO_J8_15, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ)
# RPi Alternate, with SPIDEV - Note: Edit RF24/arch/BBB/spi.cpp and  set 'this->device = "/dev/spidev0.0";;' or as listed in /dev
#radio = RF24(22, 0);
# Setup for connected IRQ pin, GPIO 24 on RPi B+; uncomment to activate
#irq_gpio_pin = RPI_BPLUS_GPIO_J8_18
#irq_gpio_pin = 24
def try_read_data(channel=0):
    if radio.available():
        while radio.available():
            len = radio.getDynamicPayloadSize()
            receive_payload = radio.read(len)
            receive_payload.decode('utf-8')
            # First, stop listening so we can talk
            print(receive_payload)
            #print("Sent the message: {}".format(receive_payload))
            radio.stopListening()
            # Send the final one back.
            radio.write(receive_payload)
            print('Got response.')
            # Now, resume listening so we catch the next packets.
            radio.startListening()
pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]
print('pyRF24/examples/pingpair_dyn/')
radio.begin()
radio.setAutoAck(False);
radio.enableDynamicPayloads()
radio.setRetries(5,15)
radio.printDetails()
inp_role = '0'
if inp_role == '0':
    print('Role: Pong Back, awaiting transmission')
    if irq_gpio_pin is not None:
        # set up callback for irq pin
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(irq_gpio_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.add_event_detect(irq_gpio_pin, GPIO.FALLING, callback=try_read_data)
    radio.openWritingPipe(pipes[1])
    radio.openReadingPipe(1,pipes[0])
    radio.startListening()
# forever loop
while 1: 
       # if there is data ready
        if irq_gpio_pin is None:
            # no irq pin is set up -> poll it
            try_read_data()
        else:
            # callback routine set for irq pin takes care for reading -
            # do nothing, just sleeps in order not to burn cpu by looping
            time.sleep(1000)