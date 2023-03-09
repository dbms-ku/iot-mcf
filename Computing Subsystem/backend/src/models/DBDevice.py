from datetime import datetime
import mongoengine
from models.DBSensor import DBSensor
import mongo_setup as mongo_setup


class DBDevice(mongoengine.Document):
    uid           = mongoengine.StringField(required=True,unique=True)
    name          = mongoengine.StringField(required=True)
    property_code = mongoengine.StringField(required=True)
    status        = mongoengine.BooleanField(default=True)
    sensors       = mongoengine.ListField(required=True)
    timestamp     = mongoengine.DateTimeField(default=datetime.now, required=True)

    meta = {
        "collection": "devices",
        # 'db_alias': 'core'
    }