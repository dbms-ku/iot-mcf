from datetime import datetime
import mongoengine


class DBPumpStatus(mongoengine.Document):
    uid                  = mongoengine.StringField(required=True,unique=True)
    arduino_status       = mongoengine.BooleanField(required=True)
    manual_switch_status = mongoengine.BooleanField(required=True)
    output_status        = mongoengine.BooleanField(required=True)
    timestamp            = mongoengine.DateTimeField(default=datetime.now, required=True)

    meta = {
        "collection": "pump_statuses",
        # 'db_alias': 'core'
    }