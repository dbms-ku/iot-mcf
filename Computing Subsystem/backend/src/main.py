from os import sys, path
sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))
import threading

from database import Database
from api import API
from sensor_processor.serial_communicator import SerialCommunicator
# from sensor_processor.radio_communicator import RadioCommunicator
# from sensor_processor.radio_communicator_mock import RadioCommunicatorMock


def run():
    comm = SerialCommunicator()    
    # comm = RadioCommunicator()    
    # comm = RadioCommunicatorMock()
    database = Database()
         
    comm.register_observer("SENSOR_DATA_UPDATE", database.process_data)

    th = threading.Thread(target=comm.start_main_loop)    
    th.start()

    api = API()
    api.inject_database_service(database)
    api.inject_radio_communicator(comm)
    api.start()    
    

if __name__ == "__main__":
    run()