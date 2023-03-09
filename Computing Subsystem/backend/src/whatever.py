import pigpio
from nrf24 import *
from datetime import datetime
import struct

import sys
import time
import traceback

def run():
    hostname = "localhost"
    port = 8888
    address = "radio"
    pi = pigpio.pi(hostname, port)
    if not pi.connected:
        print("Not connected to Raspberry Pi ... goodbye.")
        sys.exit()

    nrf = NRF24(pi, ce=11, payload_size=RF24_PAYLOAD.DYNAMIC, channel=100, data_rate=RF24_DATA_RATE.RATE_250KBPS, pa_level=RF24_PA.LOW)
    nrf.set_address_bytes(len(address))
    nrf.open_writing_pipe(address)
    # nrf.open_reading_pipe(RF24_RX_ADDR.P1, address)
    # nrf.show_registers()
    nrf.show_registers()
    try:
        print(f'Send from {address}')
        count = 0
        while True:
            # payload = struct.pack("<Bff", 0x01, 130)
            payload = 130

            # Send the payload to the address specified above.
            nrf.reset_packages_lost()
            nrf.send(payload)
            print("Sent Payload",payload)
            # nrf.send(130)
            # Sleep 100 ms.
            try:
                nrf.wait_until_sent()
            except TimeoutError:
                print('Timeout waiting for transmission to complete.')
                # Wait 10 seconds before sending the next reading.
                time.sleep(10)
                continue
            
            if nrf.get_packages_lost() == 0:
                print(f"Success: lost={nrf.get_packages_lost()}, retries={nrf.get_retries()}")
            else:
                print(f"Error: lost={nrf.get_packages_lost()}, retries={nrf.get_retries()}")

            # Wait 10 seconds before sending the next reading.
            time.sleep(10)
    except:
        traceback.print_exc()
        nrf.power_down()
        pi.stop()    

if __name__ == "__main__":
    run()
