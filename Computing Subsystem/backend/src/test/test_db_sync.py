from sshtunnel import SSHTunnelForwarder
from ssh_pymongo import MongoSession
import pymongo
from datetime import datetime



if __name__ == '__main__':
    #Time Stamp
    date = datetime.now().strftime("%Y_%m_%d-%I:%M:%S_%p")
    #database connection
    client = pymongo.MongoClient("localhost", 27017, maxPoolSize=50)

    print("RASPBERRY PI TEST DATABASE")
    print("..................................................................")
    #database name
    db = client['test']

    #collection names
    devices = db['devices']
    sensors = db['sensors']
    fields = db['fields']
    sensor_data = db['sensor_data']

    #cursor = devices.find({})
    #for document in cursor:
          #print(document)

    #Number of documents in each collectio
    device_count = devices.find().count()
    sensor_count = sensors.find().count()
    fields_count = fields.find().count()
    sensor_data_count = sensor_data.find().count()

    #formulae for sum and average of documents in the collections
    total_entries=device_count+sensor_count+fields_count+sensor_data_count
    average_entries=total_entries/4

    print("..................................................................")
    print("The number of documents in devices : ", device_count) 
    print("The number of documents in sensors : ", sensor_count) 
    print("The number of documents in fields : ", fields_count) 
    print("The number of documents in sensor data : ", sensor_data_count) 

    print("..................................................................")
    print("The Actaul Total Entries in the Test Database are: ",total_entries)
    print("The Actual Average Entries in the Test Database are: ", average_entries)
    print("..................................................................")

    print("SERVER TEST DATABASE")
    print("..................................................................")
    #connecting from the Server
    MONGO_HOST = "133.95.105.97"
    MONGO_USER = "iot"
    MONGO_PASS = "agriculture"
    MONGO_DB = "test"

    server = SSHTunnelForwarder(
        (MONGO_HOST,2200),
        ssh_username=MONGO_USER,
        ssh_password=MONGO_PASS,
        remote_bind_address=('127.0.0.1', 27017)
      )

    server.start()
    client = pymongo.MongoClient(host='127.0.0.1',
        port=server.local_bind_port)
        # user='dev',
        # password='1234')

    #database name from the server
    db = client[MONGO_DB]

    #collection names
    devices = db['devices']
    sensors = db['sensors']
    fields = db['fields']
    sensor_data = db['sensor_data']

    #cursor = devices.find({})
    #for document in cursor:
          #print(document)

    #Number of documents in each collectio
    device_count = devices.find().count()
    sensor_count = sensors.find().count()
    fields_count = fields.find().count()
    sensor_data_count = sensor_data.find().count()

    #formulae for sum and average of documents in the collections
    total_entries=device_count+sensor_count+fields_count+sensor_data_count
    average_entries=total_entries/4
    
    print("..................................................................")
    print("The number of documents in devices@server: ", device_count) 
    print("The number of documents in sensors@server : ", sensor_count) 
    print("The number of documents in fields@server : ", fields_count) 
    print("The number of documents in sensor data@server : ", sensor_data_count) 

    print("..................................................................")
    print("The Actaul Total Entries in the Test Database@server are: ",total_entries)
    print("The Actual Average Entries in the Test Database@server are: ", average_entries)
    print("..................................................................")

    #Documents Added Check
    f= open("presentDumps.txt","a")
    f.write(str(total_entries))
    f.write("\n")
    f.close()

    f= open("presentDumps.txt","r")
    content1 = f.readlines()[-1]
    print(content1,f"Lastest Entry:_{date}")
    f.close()

    f= open("presentDumps.txt","r")
    content2 = f.readlines()[-2]
    print(content2,f"Previous Entry:_{date}")
    f.close()
    
    print("..................................................................")
    if content2<content1:
      print("TEST REPORT: Documents Added to the Server Test Database Successfully")
    elif content2==content1:
      print("TEST REPORT: Documents Not Added")
    else:
      print("TEST REPORT: Errors")
    print("..................................................................")
  