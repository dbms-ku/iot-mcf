from enum import unique
from os import sys, path
sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))
from .Sensor import SensorsEnum, sensors
import mongoengine
from datetime import datetime

sensor_choices = tuple([sensor for sensor in sensors])

class DBSensor(mongoengine.Document):
    uid            = mongoengine.StringField(required=True,unique=True)
    name          = mongoengine.StringField(required=True,unique=True)
    property_code = mongoengine.StringField(choices=sensor_choices, required=True)
    min_val       = mongoengine.FloatField(required=True)
    max_val       = mongoengine.FloatField(required=True)
    timestamp     = mongoengine.DateTimeField(default=datetime.now)

    meta = {
        "collection": "sensors",
        # 'db_alias': 'core'
    }

    def __str__(self):
        return f"Sensor {self.name}, pcode: {self.property_code}, {self.timestamp}"