import os

import json
import flask
from flask import Flask, Response
from flask import request, make_response, send_file
from datetime import datetime
from flask_cors import CORS, cross_origin

import src.api_utils as au
import src.dummy_utils as du

import random

class EndpointAction(object):

    def __init__(self, action):
        self.action = action
        self.response = Response(status=200, headers={'ContentType':'application/json'})

    def __call__(self, *args):
        return self.action()        

class API():
    def __init__(self,host="0.0.0.0",port=5000):
        self.host = host
        self.port = port
        self.api = Flask(__name__)
        cors = CORS(self.api)
        self.api.config['CORS_HEADERS'] = 'Content-Type'

        self._pumpOn = False
        self._lastPumpModified = datetime.now()
        self._field_names = ["field1","field2","field3"]

    def start(self):
        self.__init_endpoints()
        self.api.run(self.host,self.port)

    def inject_radio_communicator(self,radio_communicator):
        self.radio_communicator = radio_communicator

    def inject_database_service(self, database_service):
        self._database_service = database_service

    def __add_endpoint(self, endpoint=None, endpoint_name=None, handler=None, methods=["GET"]):
        self.api.add_url_rule(endpoint, endpoint_name, EndpointAction(handler), methods=methods)

    def __init_endpoints(self):        
        self.__add_endpoint(endpoint="/get-sensor-data", endpoint_name="get_sensor_data", handler=self.get_sensor_data)
        self.__add_endpoint(endpoint="/get-house-data", endpoint_name="get_house_data", handler=self.get_house_data)
        self.__add_endpoint(endpoint="/get-fields-data", endpoint_name="get_fields_data", handler=self.get_fields_data)
        self.__add_endpoint(endpoint="/get-water-sensor-data", endpoint_name="get_water_sensor_data", handler=self.get_water_sensor_data)
        self.__add_endpoint(endpoint="/toggle-pump", endpoint_name="toggle_pump", handler=self.toggle_pump, methods=["POST"])
        self.__add_endpoint(endpoint="/get-pump-status", endpoint_name="get_pump_status", handler=self.get_pump_status)   

    def get_house_data(self):
        res = {}
        res["temperature"] = random.randint(25,35)
        res["humidity"] = random.randint(40,90)
        res = au.decorate_message(res)
        response = flask.jsonify(res)
        return response

    def get_sensor_data(self, field_name = "Field 01", property_name="water_level", from_date = '2018-04-30', to_date = '2023-04-30'):
        # contents = request.args.to_dict()
        contents = request.get_json( )
        field_name = contents.get("field_name")
        property_name = contents.get("property_name")
        from_date = contents.get("from_date")
        to_date = contents.get("to_date")
        
        data = self._database_service.get_sensor_data(field_name, property_name, from_date, to_date)

        aux = {
            f"Resource Description": "sensor_data records between from_date (${from_date}) and 'to_date (${to_date}) for field_name (${field_name}) and property_name (${property_name})",
            "Records count": len(data),
            "Fields": ["sensor_uid", "timestamp", "value"]
        }

        res = au.decorate_message(aux)
        res["data"] = data
        
        response = flask.jsonify(res)            
        return response

    def get_fields_data(self):                
        res = []
        for field_name in self._field_names:
            field_data = self._database_service.get_field_sensor_data(field_name)
            res.append(field_data)        
        res = au.decorate_message(res)
        response = flask.jsonify(res)            
        return response    

    def get_water_sensor_data(self):
        res = du.generate_water_sensor_data(24)
        res = au.decorate_message(res)
        response = flask.jsonify(res)    
        return response    


    def toggle_pump(self):    
        contents = json.loads(request.data)

        if not contents or "isOn" not in contents:
            return au.bad_request()

        next_pump_status = str(contents["isOn"]).lower()    

        if next_pump_status not in ["true","false"]:
            return au.bad_request()

        shouldTurnPumpOn = next_pump_status == "true" 
        self._pumpOn = shouldTurnPumpOn
        self._lastPumpModified = datetime.now()

        
        if shouldTurnPumpOn:
            print("Turning Pump On")
            self.radio_communicator.schedule_instruction("TURN_PUMP_ON")            
        else:
            print("Turning Pump OFF")
            self.radio_communicator.schedule_instruction("TURN_PUMP_OFF")

        res = {
            "isOn": self._pumpOn,
            "lastModified": au.convert_date(self._lastPumpModified)
        }
        res = au.decorate_message(res)
        response = flask.jsonify(res)
        return response     

    def get_pump_status(self):        
        res = {}
        res["lastModified"] = au.convert_date(self._lastPumpModified)
        res["isOn"] = self._pumpOn
        res = au.decorate_message(res)
        response = flask.jsonify(res)
        return response

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
    
    def request_pump_status(self):
        print("Requesting Pump Status")
        self.radio_communicator.schedule_instruction("REQUEST_PUMP_STATUS")
    
    

if __name__ == "__main__":
    # run()
    app = Flask(__name__)
    app.run(host="0.0.0.0",port=5000)
