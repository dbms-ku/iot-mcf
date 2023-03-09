from datetime import datetime
import random as random
import src.api_utils as au

def generate_dummy_humidity_data():
    time = random.randint(1,12)
    value = random.randint(1,100)
    return {
        "time":time,
        "value":value,
        "type":"humidity"
    }

def generate_dummy_data(quantity=5):
    return [generate_dummy_humidity_data() for _ in range(quantity)]

def generate_water_sensor_data(amount):
    return [{"value":random.randint(0,1000),"time":au.convert_date(datetime.now())} for _ in range(amount)]    

def generate_field_data(id):
    res = {}
    res["id"] = id
    res["name"] = f"Field-{id}"    
    res["airTemperature"] = random.randint(0,45)
    res["airHumidity"] = random.randint(0,100)
    res["condition"] = "sunny"
    res["battery"] = random.randint(0,100)
    res["waterLevel"] = random.randint(0,100)
    res["soilTemperature"] = random.randint(0,45)
    res["soilHumidity"] = random.randint(0,100)
    res["connected"] = True if random.randint(0,1) > 0 else False
    return res