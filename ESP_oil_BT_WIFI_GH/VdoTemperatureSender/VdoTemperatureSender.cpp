#include <Arduino.h>
#include "VdoTemperatureSender.h"

VdoTemperatureSender::VdoTemperatureSender(int analogPin, unsigned int dataTable[16]) {
    _debug = true;

    _analogPin = analogPin;

    for(int i = 0; i < 16; i++) {
      _dataTable[i] = dataTable[i];
    }

    _checkOrderTable = checkOrder();
}

/**
 * Get temperature in degrees celcius
 */
int VdoTemperatureSender::getTemperature() {
    // Read raw value 0 - 1023
    int rawValue = analogRead(_analogPin);
  
    if (_debug) {
        Serial.println(" === VDO Temperature Sender === ");
        Serial.print("Analog Pin: ");
        Serial.println(_analogPin);
        Serial.print("RAW Value: ");
        Serial.println(rawValue);
        Serial.println("");
    }

    // Temperature
    int temperatureCelcius = 0;
  
    //This searches the 2 surrounding values, and then linear interpolates between them.
    for (int i = 0; i < 15; i++)
    {
        if (_checkOrderTable > 0) {
            // Normal order, first element has less value than the second element
            if (rawValue >= _dataTable[i] && rawValue <= _dataTable[i + 1])
            {
                temperatureCelcius = (i * 10) + (10.0 * (rawValue - _dataTable[i]) / (_dataTable[i+1] - _dataTable[i]));
                break;
            }
        } else if (_checkOrderTable < 0) {
            // Inverted order, first element has more value than the second element
            if (rawValue <= _dataTable[i] && rawValue >= _dataTable[i + 1])
            {
                temperatureCelcius = (i * 10) + (10.0 * (_dataTable[i] - rawValue) / (_dataTable[i] - _dataTable[i+1]));
                break;
            }
        } else {
            Serial.println("ERROR on data table!");
            return 0;
        }
    }  
 
    //Limits the Output to 999°C, so an open circuit gets detectet!
    temperatureCelcius = constrain(temperatureCelcius, -40, 999);
    
    return temperatureCelcius;
}

/**
 * Check order of data table
 *
 * @return boolean
 */
int VdoTemperatureSender::checkOrder() {
    bool init = _dataTable[0] > _dataTable[1];

    for (int i = 1; i < 15; i++)
    {
        if (_dataTable[i] > _dataTable[i+1])
        {
            if (init) {
                continue;
            } else {
                Serial.println("Data table with wrong values (order isn't preserved)!");
                return 0; // ERROR
            }
        }
        else 
        {
            if (init) {
                Serial.println("Data table with wrong values (order isn't preserved)!");
                return 0; // ERROR
            } else {
                continue;
            }
        }
    } 

    if (init) {
        Serial.println("Table data with [inverted] order found!");
        return -1;
    } else {
        Serial.println("Table data with [normal] order found!");
        return 1;
    }
}

void VdoTemperatureSender::debug(bool debug) { 
    _debug = debug;
}