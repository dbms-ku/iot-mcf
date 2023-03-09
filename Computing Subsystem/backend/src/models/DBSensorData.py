import mongoengine

class DBSensorData(mongoengine.Document):
    sensor_uid = mongoengine.IntField(required=True)
    value = mongoengine.FloatField(required=True)
    timestamp = mongoengine.DateTimeField(required=True)

    meta = {
        "collection": "sensor_data",
        # 'db_alias': 'core'
    }

    def __str__(self):
        return f"SensorData Object [sensor_uid:{self.sensor_uid}, value:{self.value}, timestamp:{self.timestamp}]"