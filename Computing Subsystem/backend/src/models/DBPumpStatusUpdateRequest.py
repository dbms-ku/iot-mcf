import mongoengine

class DBPumpStatusUpdateRequest(mongoengine.Document):
    value = mongoengine.StringField(required=True)
    processed = mongoengine.BooleanField(required=True, default=False)
    timestamp = mongoengine.DateTimeField(required=True)