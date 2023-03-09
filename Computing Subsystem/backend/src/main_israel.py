from os import sys, path
sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))
import threading

from database import Database
from api import API
from sensor_processor.radio_communicator import RadioCommunicator


def run():
    comm = RadioCommunicator()    
    th = threading.Thread(target=comm.start_main_loop)    
    th.start()

    database = Database()

    api = API()
    api.inject_database_service(database)
    api.inject_radio_communicator(comm)
    api.start()    
    

if __name__ == "__main__":
    run()