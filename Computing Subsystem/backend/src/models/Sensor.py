import enum

sensors = {
    "00": "undefined_sensor",
    "80": "water_level",
    "70": "temperature",
    "20": "humidity",
    "40": "rain",
    "51": "battery_voltage",
    "52": "solar_voltage",
    "31": "soil_temperature",
    "32": "soil_moisture",
    "33": "soil_ec",
    "34": "soil_ph",
    "35": "soil_nitrogen",
    "36": "soil_phosphorus",
    "37": "soil_potassium",
}

sensor_params = {
    # (m_value, c_value)
    "00": (1, 0),
    "80": (100, 0),
    "70": (10, 200),
    "20": (10, 0),
    "40": (1, 0),
    "51": (100, 0),
    "52": (100, 0),
    "31": (10, 200),
    "32": (10, 0),
    "33": (1, 0),
    "34": (100, 0),
    "35": (1, 0),
    "36": (1, 0),
    "37": (1, 0),
}

class Sensor:
    def __init__(self, name, value, timestamp) -> None:
        key = get_key(sensors, name)
        if key:
            self.id = get_key(sensors, name)
        else: 
            self.id = "u"
        self.name = name
        self.value = value
        self.timestamp = timestamp
        # print(self)

    def __str__ (self):
        return f"{self.name}-{self.id} at {self.timestamp}: (val={self.value}) "

def get_sensor_property_code_by_property_name(property_name: str) -> str:
    for key, value in sensors.items():
        if property_name == value:
            return key
    return "00"

def get_key(my_dict, val):
    for key, value in my_dict.items():
        if val == value:
            return key
 
    return "-1"

def set_enum_values(locals, temp_sensors):
    # Note that we can't use `locals.update(sensors)`
    # because it's `locals.__setitem__(k, v)` that
    # creates the enum value, and `update` doesn't
    # call `__setitem__`.
    for k in temp_sensors:
        locals[k] = temp_sensors[k]


class SensorsEnum(enum.Enum):
    set_enum_values(locals(), sensors)

    # @property
    # def double_value(self):
    #     return self.value * 2