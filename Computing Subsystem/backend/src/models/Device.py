from datetime import datetime

class Device:
    def __init__(self, name):
        self.name = name
        self.field = f"Field {name}"
        self.sensors = []
        self.timestamp = "2022-01-01 00:00:00"

    def __str__(self):
        return f'Device {self.name}: {self.showSensors()}'

    def showSensors(self):
        temp_str = ""

        for sensor in self.sensors:
            temp_str += str(sensor)

        return temp_str

    def fetchFieldName(device_name):
        return f"Field {device_name}"
        
    def addSensorData(self, sensor_data):
        found = []
        for ind in range(len(self.sensors)):
            this_sensor = self.sensors[ind]
            if this_sensor["id"] == sensor_data.id:
                found.append(ind)
        for f in found:
            self.sensors.pop(f)
        self.sensors.append(sensor_data.__dict__)
        self.timestamp = sensor_data.timestamp
        # print(f'Device {self.id}: {self.showSensors()}')