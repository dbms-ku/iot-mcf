from datetime import datetime
from enum import unique
import mongoengine
from os import sys, path

sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

class DBField(mongoengine.Document):
    uid           = mongoengine.StringField(required=True)
    name          = mongoengine.StringField(required=True,unique=True)
    growing_stage = mongoengine.IntField(required=True)
    devices       = mongoengine.ListField()
    stages        = mongoengine.ListField()
    timestamp     = mongoengine.DateTimeField(default=datetime.now, required=True)

    meta = {
        "collection": "fields",
        # 'db_alias': 'core'
    }

    def __str__(self):
        return f"***** {self.name} {self.growing_stage} {self.devices} {self.timestamp}"