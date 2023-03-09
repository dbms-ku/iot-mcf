import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(9, GPIO.IN, pull_up_down=GPIO.PUD_UP)
# GPIO.setup(27, GPIO.OUT)

while True:
    input_state = GPIO.input(9)
    if input_state == False:
        print('Button Pressed')
        # GPIO.output(27, 0)
        time.sleep(0.2)
    # else:
    #     GPIO.output(27, 1)
        






# import RPi.GPIO as GPIO
# import time

# GPIO.setmode(GPIO.BCM)

# GPIO.setup(20, GPIO.IN, pull_up_down=GPIO.PUD_UP)
# # GPIO.setup(16, GPIO.OUT)

# while True:
#     input_state = GPIO.input(20)
#     # output_state = GPIO.output(16, 1)
#     if input_state == False:
#         print('Button Pressed')
#         # GPIO.output(16, 1)
#         time.sleep(0.2)
#     # else:
#     #     GPIO.output(16, 0)
