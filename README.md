# Lightdome.
## 360° Lux Measurement with nodeMCU

- Measures the lux from four sides (N,E,S,W) with photo-resistors.  
- Publish the values via MQTT
- Set `factor`, `offset` and `interval` per MQTT at runtime

## Hardware

- 1 x nodeMCU
- 1 x Analog-Digital-Converter per I2C: ADS1115
- 4 x Photo-Resistors
- 4 x 10K Ohm Resistors

![build process](../master/docs/image_01.png?raw=true "build process")

## Wiring:

- nodeMCU - ADS1115
```
 - D1 -> SCL  
 - D2 -> SDA  
 - 3V -> VIN
 - G  -> G
```

- Photorestistors
```
 - Pin 1 -> 3V (nodeMCU)
 - Pin 2 -> A0 (ADS1115)
         -> 10k Resistor -> G (nodeMCU)
```
## Setup
- duplicate the `config.h.default` as `config.h`
- change the file at your own


## Use
The device reconnects automatically to the given wifi accesspoint. Then it reconnects also to the mqtt broker.

### Topics
The values will be pushed on:  
`SKWNCR_MQTT_TOPIC/SENSOR-NUMBER-WITH-LEADING-ZERO`

### Behavior
- the analog read interval equals the `SKWNCR_TICK_RATE` in milliseconds
- a value will be pushed, when it has been changed
- a value will be pushed again, if the last push is older then `SKWNCR_VALUE_AGE` in milliseconds
- the values can be shifted with a scale factor (`SKWNCR_MQTT_TOPIC_SET_FACTOR`), a float multiplier
- the values can be shifted with an offset, a float
- change `factor`, `offset` and `interval` at runtime per mqtt

### Set `factor`, `offset` and `interval` at runtime

Publish a single number or float an the topic:  
`SKWNCR_MQTT_TOPIC/SENSOR-NUMBER-WITH-LEADING-ZERO/set/ACTION`

**ACTION** can be:

```
factor
offset
interval
```

---
*have fun*

---  

#### Hint
> To have the 5th sensor for the zenith, use the only one analog input directly on the nodeMCU.

## TODO
- store and read `factor`, `offset` and `interval` from eprom.

## Why?

The reason for this tiny project is my own problem to know the light situation outside.
In the evening hours the light comes flat from one side.
The other sides of the house can be in a situation, that a turn on the light is necessary - but it isn't triggered.
The lux dependency for our smart home lightning works crazy with only one lux sensor outside.
And i have absolut keinen Bock drauf, to place lux sensors in any room.

## How i use it?

- The device is located on the roof. No Shadow can reach the sensors.  
- In the attic is the power and a low power DC wire is going up.  
- The wifi access point is located directly in the attic.  
- A mqtt broker (mosquitto) manages the messages. 
- My own "transponder" node.js app writes all values from a given root topic in a influx database. 
- The transponder create a `average` topic with the average of all four sensors.
- Grafana shows me the data. 