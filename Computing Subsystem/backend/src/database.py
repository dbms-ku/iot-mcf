from dataclasses import Field
import datetime
import json
from sqlite3 import Date
import time
import flask
from cmath import nan
import mongoengine
from numpy import NaN
from os import sys, path
sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))
from src.models.DBField import DBField
from src.models.DBDevice import DBDevice
from src.models.DBSensor import DBSensor
from src.models.DBSensor import DBSensor
from src.models.Sensor import sensors as sensors_dict, get_sensor_property_code_by_property_name
from src.models.DBSensorData import DBSensorData
import random

import src.dummy_utils as du

class Database():
    def __init__(self,host="localhost",db="test",username="pi",password="1234",port=27017):
        self.host = host
        self.db = db
        self.username = username
        self.password = password
        self.port = port

        print("Connecting to database")
        self.__init_mongoengine_(host,db,username,password,port)
        print("Connection to database successfull!")
        self._fields = self.get_complete_fields_data()
        # print(self._fields)

    def __init_mongoengine_(self, host, db, username, password, port):
        mongoengine.connect(db, host=host,username=username,password=password,port=port)


    def process_data(self, results):
        if results is not None:
            data,_ = results
            # print("$$$$$$",results)
            if data:
                # print("$$$$$$",data)
                self.insert_sensor_data(data["sensor"],data["device"],data["value"],data["timestamp"])

    def insert_sensor_data(self,sensor_property_name,device_name,value,timestamp):
        sensor = self.get_device_sensor(device_name, sensor_property_name)
        
        # print(".--------.")
        if sensor is not None:
            # print("++++++++++++++++++")
            # print(sensor)
            sensor_uid = sensor["uid"]
        else:
            sensor_uid = 0
        
        sensor_data = DBSensorData(sensor_uid=sensor_uid,value=value,timestamp=timestamp)
        
        sensor_data.save()

    def get_device_sensor(self,device_name: str,sensor_property_name: str) -> DBSensor:
        if sensor_property_name in list(sensors_dict.values()):
            sensor_property_code = list(sensors_dict.keys())[list(sensors_dict.values()).index(sensor_property_name)]
        else:
            return None
        
        fields = self._fields
        
        for field in fields:
            for device in field["devices"]:
                if device_name == device["property_code"]:
                    for sensor in device["sensors"]:
                        if sensor_property_code == sensor["property_code"]:
                            return sensor
        return None

    def get_complete_fields_data(self):
        fields_data = []

        for field in DBField.objects:
            field_data = field.to_mongo().to_dict()
            print(field_data)
            
            field_devices = []
            for device in DBDevice.objects.filter(uid__in=field.devices):
                device_data = device.to_mongo().to_dict()
                device_sensors = []
                for sensor in DBSensor.objects.filter(uid__in=device.sensors):
                    device_sensors.append(sensor.to_mongo().to_dict())
                
                device_data["sensors"] = device_sensors

                field_devices.append(device_data)

            field_data["devices"] = field_devices
            fields_data.append(field_data)

        return fields_data

    def get_sensor_data(self, field_name, property_name, from_date, to_date):
        
        property_code = get_sensor_property_code_by_property_name(property_name)

        device_uids = DBField.objects(name=field_name).first().devices
        sensor_uids = []
        [[sensor_uids.append(uid) for uid in db_device.sensors] for db_device in DBDevice.objects(uid__in=device_uids).only("sensors")]
        sensor_uids = [sensor.uid for sensor in DBSensor.objects(property_code=property_code, uid__in=sensor_uids).only("uid")]

        print(sensor_uids)
        params = {}
        
        params["timestamp"] = {
            "$gte": datetime.datetime.strptime(from_date,'%Y-%m-%d %H:%M:%S'),
            "$lte": datetime.datetime.strptime(to_date,'%Y-%m-%d %H:%M:%S')
        }

        return [
            {"sensor_uid": sensor_data.sensor_uid, "value": sensor_data.value, "timestamp": sensor_data.timestamp} for sensor_data in 
            DBSensorData.objects(sensor_uid__in=sensor_uids, __raw__=params).only("sensor_uid", "value", "timestamp")
        ]

    def get_all_fields(self):
        return DBSensor.objects.to_json()

    def get_field_sensor_data(self,name):
        res = {}
        res["data"] = DBSensor.objects().to_json()
        res["data"] = "ddfsd"
        # response = flask.jsonify(res)
        return res

        return flask.json(DBField.objects())
        return DBField.objects()
        return self._fields[name]["data"]

    def __secret_fn(self):
        print("SECRET")
    