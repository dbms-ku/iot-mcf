from ast import alias
import mongoengine

alias_core = "core"
db = "test"
data = dict(
    username="pi",
    password="1234",
    host="raspberrypi",
    port=27017, 
    alias="core"
)

def global_init():
    mongoengine.connect(db, **data)
    # mongoengine.register_connection(alias="core", name=db, **data)