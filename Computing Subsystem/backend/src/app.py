import os

import json
import flask
from flask import Flask, Response
from flask import request, make_response, send_file
from datetime import datetime
from models.DBDevice import DBDevice
from models.DBSensorData import DBSensorData
from models.DBPumpStatusUpdateRequest import DBPumpStatusUpdateRequest
# from models.DBSensor import DBSensor
# from dummy_utils import generate_dummy_data
import mongo_setup as mongo_setup
import mongoengine
from os import sys, path
sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))
import threading


from models.Device import Device
from models.Sensor import Sensor

# from sensor_processor.radio_communicator import RadioCommunicator
from lib_nrf24 import NRF24

app = flask.Flask(__name__)
app.config["DEBUG"] = True

class EndpointAction(object):

    def __init__(self, action):
        self.action = action
        self.response = Response(status=200, headers={})

    def __call__(self, *args):
        self.action()        
        return self.response

class API():
    def __init__(self,host="0.0.0.0",port=5000):
        self.host = host
        self.port = port
        self.api = Flask(__name__)

    def start(self):
        self.__init_endpoints()
        self.api.run(self.host,self.port)

    def inject_radio_communicator(self,radio_communicator):
        self.radio_communicator = radio_communicator

    def inject_database_communicator(self, database_communicator):
        self.database_communicator = database_communicator

    def __add_endpoint(self, endpoint=None, endpoint_name=None, handler=None, methods=["GET"]):
        self.api.add_url_rule(endpoint, endpoint_name, EndpointAction(handler), methods=methods)

    def __init_endpoints(self):
        self.__add_endpoint(endpoint="/request-pump-status", endpoint_name="request-pump-status", handler=self.request_pump_status, methods=["POST"])
        self.__add_endpoint(endpoint="/turn-pump", endpoint_name="turn-pump", handler=self.turn_pump, methods=["POST"])
        self.__add_endpoint(endpoint="/get-sensor-data", endpoint_name="get-sensor-data", handler=self.home)


# def get_api():

#     app = Flask(__name__)

# @app.route("/turn-pump",methods=["POST"])
# @app.route('/aa', methods=['GET'])
# def home():
#     res = {}
#     res["result"] = "Success"
#     response = flask.jsonify(res)
#     response.headers.add("Access-Control-Allow-Origin", "*")
#     return response


def turn_pump(self):
    contents = request.json
    
    if contents["pumpOn"] == "True":
        print("Turning Pump On")
        self.radio_communicator.schedule_instruction("TURN_PUMP_ON")
        
    elif contents["pumpOn"] == "False":
        print("Turning Pump OFF")
        self.radio_communicator.schedule_instruction("TURN_PUMP_OFF")
    res = {}
    res["result"] = "Success"
    response = flask.jsonify(res)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response

# @app.route("/request-pump-status",methods=["POST"])
def request_pump_status(self):
    print("Requesting Pump Status")
    self.radio_communicator.schedule_instruction("REQUEST_PUMP_STATUS")

@app.route("/get-sensor-data", methods=["GET"])
def home():
    mongo_setup.global_init()

    res  = {}  
    res["sensor_data"] = DBSensorData.objects(value__gte=1)
    print(res)

    response = flask.jsonify(res)

    # Enable Access-Control-Allow-Origin
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response

# @app.route("/store-sensor-data",methods=["POST"])
def store_sensor_data():
    contents = request.json
    print(contents)
    mongo_setup.global_init()

    for content in contents:
        DBDevice(
            name=content["name"], 
            field=content["field"], 
            sensors=content["sensors"],
            timestamp=content["timestamp"]
        ).save()

    return json.dumps({'success':True}), 200, {'ContentType':'application/json', 'Access-Control-Allow-Origin':'*'} 

# @app.route("/update-pump-status-data",methods=["POST"])
def update_pump_status_data():
    contents = request.json

    mongo_setup.global_init()

    DBPumpStatusUpdateRequest(
        value=contents["value"], 
        timestamp=datetime.now()
    ).save()

    return json.dumps({'success':True}), 200, {'ContentType':'application/json'} 

    # return app

# def run():
#     comm = RadioCommunicator()    
#     th = threading.Thread(target=comm.start_main_loop)    
#     th.start()

#     api = API()
#     api.inject_radio_communicator(comm)
#     api.start()    
    

if __name__ == "__main__":
    # run()
    # app = Flask(__name__)
    app.run(host="0.0.0.0",port=5000)
