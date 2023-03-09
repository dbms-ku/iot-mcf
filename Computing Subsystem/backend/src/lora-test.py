import utime
from machine import UART
from machine import Pin

lora = UART(0,9600)
switch1 = Pin(20, Pin.IN, Pin.PULL_DOWN)
switch2 = Pin(19, Pin.IN, Pin.PULL_DOWN)
switch3 = Pin(18, Pin.IN, Pin.PULL_DOWN)
while True:
    if switch1.value() and switch2.value() and switch3.value():
        lora.write("SECRET111")
    elif switch1.value() and switch2.value():
        lora.write("SECRET110")
    elif switch1.value() and switch3.value():
        lora.write("SECRET101")
    elif switch2.value() and switch3.value():
        lora.write("SECRET011")
    elif switch1.value():
        lora.write("SECRET100")
    elif switch2.value():
        lora.write("SECRET010")
    elif switch3.value():
        lora.write("SECRET001")
    else:
        lora.write("SECRET000")
    utime.sleep(0.2)
#Transmitter code Ends here