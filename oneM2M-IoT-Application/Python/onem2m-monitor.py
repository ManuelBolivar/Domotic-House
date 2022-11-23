# Version 1.2
from typing import Optional, Any

import requests
import json
from flask import Flask
from flask import request
from flask import Response
import sys
import random
import configparser

# /////////////// Parameters /////////////////
config = configparser.ConfigParser()
with open('default.json', 'r') as f:
    config = json.load(f)
# CSE Params
csePoA = "http://" + config["cse"]["ip"] + ":" + str(config["cse"]["port"])
cseName = config["cse"]["name"]
cseRelease = config["cse"]["release"]
poa_in_nu = config["cse"]["poa_in_nu"]
# AE params
monitorId = config["monitor"]["id"]
monitorIP = config["monitor"]["ip"]
monitorPort = config["monitor"]["port"]
monitorPoA = "http://" + monitorIP + ":" + str(monitorPort)

sensorOffState = config["TiltSensor"]["sensorOffState"]
luminosityThreshold = config["LuminositySensor"]["luminosityThreshold"]
#pantallaThreshold = config["Pantalla_nextion"]["pantallaThreshold"]
isLedOn = config["LedActuator"]["isLedOn"]
potentiometerThreshold = config["PoteniometerSensor"]["potentiometerThreshold"]
requestNr: int = 0

sensorToMonitor = ""
actuatorToTrigger = ""
Pantalla_Nextion="Pantalla_nextion"
Presence="Presence"
global Sensor_name
global Sensor_name1
global Sensor_id
global cin

def createSUB(sensorToMonitor):
    global requestNr
    global cseRelease
    global poa_in_nu
    global Sensor

    headers = {
        'Content-Type': 'application/json;ty=23',
        'X-M2M-Origin': monitorId,
        "X-M2M-RI": "req" + str(requestNr),
    }

    if (cseRelease != "1"):
        headers.update({"X-M2M-RVI": cseRelease})

    notificationUri = [cseName + "/Monitor"]
    if (poa_in_nu):
        notificationUri = [monitorPoA]

    response = requests.post(csePoA + '/' + cseName + "/" + sensorToMonitor + '/DATA',
                             json={
                                 "m2m:sub": {
                                     "rn": "SUB_Monitor",
                                     "nu": notificationUri,
                                     "nct": 1,
                                     "enc": {
                                         "net": [3]
                                     }
                                 }
                             },
                             headers=headers
                             )
    requestNr += 1
    if response.status_code != 201:
        print("SUB Creation error : ", response.text)
    else:
        # guardar subscription
        response_dict = json.loads(response)
        Sensor = response_dict['m2m:sub']['ri']
        return Sensor
        Sensor_id= response_dict['ri']
        print("SUB Creation :", response.status_code)


def createAE():
    global requestNr
    global cseRelease
    headers = {
        'Content-Type': 'application/json;ty=2',
        'X-M2M-Origin': monitorId,
        "X-M2M-RI": "req" + str(requestNr),
    }
    ae_json = {
        "m2m:ae": {
            "rn": "Monitor",
            "api": "Norg.demo.monitor-app",
            "rr": True,
            "poa": [monitorPoA]
        }
    }
    if (cseRelease != "1"):
        headers.update({"X-M2M-RVI": cseRelease})
        ae_json['m2m:ae'].update({"srv": [cseRelease]})

    response = requests.post(csePoA + "/" + cseName,
                             json=ae_json,
                             headers=headers
                             )
    requestNr += 1
    if response.status_code != 201:
        print("AE Creation error : ", response.text)
    else:
        print("AE Creation :", response.status_code)
    sensor_Name=createSUB(Pantalla_Nextion)
    sensor_Name1=createSUB(Presence)


def createCIN(actuatorName, commandName):
    global requestNr
    global cseRelease
    headers = {
        'Content-Type': 'application/json;ty=4',
        'X-M2M-Origin': monitorId,
        "X-M2M-RI": "req" + str(requestNr),
    }

    if (cseRelease != "1"):
        headers.update({"X-M2M-RVI": cseRelease})

    response = requests.post(csePoA + "/" + cseName + "/" + actuatorName + '/COMMAND',
                             json={
                                 "m2m:cin": {
                                     "con": commandName
                                 }
                             },
                             headers=headers
                             )
    requestNr += 1
    if response.status_code != 201:
        print("CIN Creation error : ", response.text)
    else:
        print("CIN Creation :", response.status_code)


api = Flask(__name__)


@api.route('/', methods=['POST'])
def processNotification():
    global  sensorValue
    global sensorValue1

    if (Sensor_name=="Pantalla_Nextion"):
     #sensorValue = cin['m2m:cin']['con']
     sensorValue=getLatest("Pantalla_Nextion")
     print("Receieved sensor value : ", sensorValue)
     commandPantalla(sensorValue)
    else:
        print("Demo not implemented")

    if (Sensor_name=="Presence"):
     #sensorValue1 = cin['m2m:cin']['con']
     sensorVAlue1=getLatest("Presence")
     print("Receieved sensor value : ", sensorValue1)
     commandPresence(sensorValue)
    else:
        print("Demo not implemented")


def commandPantalla(sensorValue):

    if (sensorValue == "door_on"):
        print("door_on")
        createCIN(actuatorToTrigger, "[door_on]")

    elif (sensorValue == "door_off"):
        print("door_off")
        createCIN(actuatorToTrigger, "[door_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "room_on"):
        print("room_on")
        createCIN(actuatorToTrigger, "[room_on]")

    elif (sensorValue == "room_off"):
        print("room_off")
        createCIN(actuatorToTrigger, "[room_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "fireplace_on"):
        print("fireplace_on")
        createCIN(actuatorToTrigger, "[fireplace_on]")

    elif (sensorValue == "fireplace_off"):
        print("fireplace_off")
        createCIN(actuatorToTrigger, "[fireplace_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "kitchen_on"):
        print("kitchen_on")
        createCIN(actuatorToTrigger, "[kitchen_on]")

    elif (sensorValue == "kitchen_off"):
        print("kitchen_off")
        createCIN(actuatorToTrigger, "[kitchen_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "worktop_on"):
        print("worktop_on")
        createCIN(actuatorToTrigger, "[worktop_on]")

    elif (sensorValue == "worktop_off"):
        print("worktop_off")
        createCIN(actuatorToTrigger, "[worktop_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "right_h_on"):
        print("right_h_on")
        createCIN(actuatorToTrigger, "[right_h_on]")

    elif (sensorValue == "right_h_off"):
        print("right_h_off")
        createCIN(actuatorToTrigger, "[right_h_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "left_h_on"):
        print("left_h_on")
        createCIN(actuatorToTrigger, "[left_h_on]")

    elif (sensorValue == "left_h_off"):
        print("left_h_off")
        createCIN(actuatorToTrigger, "[left_h_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "bathroom_on"):
        print("bathroom_on")
        createCIN(actuatorToTrigger, "[bathroom_on]")

    elif (sensorValue == "bathroom_off"):
        print("bathroom_off")
        createCIN(actuatorToTrigger, "[bathroom_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "bedroom_on"):
        print("bedroom_on")
        createCIN(actuatorToTrigger, "[bedroom_on]")

    elif (sensorValue == "bedroom_off"):
        print("bedroom_off")
        createCIN(actuatorToTrigger, "[bedroom_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "frontdoor_on"):
        print("frontdoor_on")
        createCIN(actuatorToTrigger, "[frontdoor_on]")

    elif (sensorValue == "frontdoor_off"):
        print("frontdoor_off")
        createCIN(actuatorToTrigger, "[frontdoor_off]")
    else:
        print("Nothing to do")

    if (sensorValue == "source_on"):
        print("source_on")
        createCIN(actuatorToTrigger, "[source_on]")
    elif (sensorValue == "source_off"):
        print("source_off")
        createCIN(actuatorToTrigger, "[source_off]")
    else:
        print("Nothing to do")


if (sensorValue == "pool_on"):
    print("source_on")
    createCIN(actuatorToTrigger, "[pool_on]")
elif (sensorValue == "pool_off"):
    print("pool_off")
    createCIN(actuatorToTrigger, "[pool_off]")
else:
    print("Nothing to do")



def commandPresence(sensorValue):
    global isLedOn
    if (sensorValue > luminosityThreshold) and (isLedOn == True):
        print("High luminosity => Switch OFF the led")
        createCIN(actuatorToTrigger, "[switchOff]")
        isLedOn = False
    elif (sensorValue < luminosityThreshold) and (isLedOn == False):
        print("Low luminosity => Switch ON the led")
        createCIN(actuatorToTrigger, "[switchOn]")
        isLedOn = True
    else:
        print("Nothing to do")

def getLatest(Sensor_name):
    global requestNr
    global cseRelease
    requestNr = random.randint(0, 1000)
    print("Sending request >>> ")
    headers = {
        'X-M2M-Origin': monitorId,
        "X-M2M-RI": "req" + str(requestNr),
        'Accept': 'application/json'
    }
    if (cseRelease != "1"):
        headers.update({"X-M2M-RVI": cseRelease})

    response = requests.get(csePoA + "/" + cseName + "/" + Sensor_name + "Sensor/DATA/la",
                            headers=headers)

    requestNr += 1
    print("<<< Response received ! ")

    if response.status_code != 200:
        print("Error = ", response.text)
    else:
        cin = json.loads(response.content)
        print("Effective content of CIN = ", cin['m2m:cin']['con'])
        return cin['m2m:cin']['con']
    sys.exit()

createAE()
getLatest("Pantalla_Nextion")
getLatest("Presence")
api.run(host=monitorIP, port=monitorPort)