# ESP32_OilTemp
An ESP32 OilTemp Display for Cars

<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_142209.jpg" height="500px" />


Note: Amazon Links are affiliate links. Thanks for the support.

## Functionality

Receiving Oil Temp from an additional VDO Temp Sensor on the ESP32
ESP32 shows Oil Temp on the mini display in the dashboard
Cycling through different display options via physical switch next to the display. The PASM switch is usually used for adapting the dampers. Not needed in my car any more. Just put a microswitch inside the existing housing. 
  Default: Show oil temp under 80°C and over 120°C, disable display between these values.
  Option1: Show always the current oil temp.
  Option2: Show Cayman logo.
  Option3: Disable screen.
ESP32 provides Oil Temp via Bluetooth Serial to a mobile phone.
OTA update for the ESP32, software can be adapted without physical access to the ESP. Important because it now sits behind the radio...

## Components Used

ESP32
The actual main component, reveives the temp signal and controls the display.
https://amzn.to/32IrhwX

OLED Display Module 0.91 Inch I2C SSD1306
The display module. Also bigger I2C displays would easily possible. In my car the small one was the best fitment.
e.g.: https://amzn.to/2VDNDyi

Oil Temp Sensor
There are differen sizes available from VDO. Choose the correct one for you car. Especially mind the threads and lenght of the sensor. My car could only be fitted with the "short" version.

VDO TEMPERATURFÜHLER 150° OEL M14 x 1,5 kurz. The short version: old VDO Nr. 323-801-012-002D, new number = A2C1755440001.

Long version with M14x1,5: https://amzn.to/32G6v0V

The sensor is connected only with one wire to the ESP. Worry was that the ground is not good enough... But first tests show that the received/calculated values are good enough.

DC/DC Converter
To convert the 12V DC from the car to 5V that the ESP32 needs.
e.g. https://amzn.to/3ajEYov

Resistors
2x 220Ohm. One for the switch, one for the sensor.

Switch
To cycle through different display modes. I put it directly into the dashboard. "Inside" an existing but unused switch
e.g. https://amzn.to/2TeMIT3

Cabling
Cables for the display and the switch.
e.g. https://amzn.to/2TxkW30

## Wiring Diagram
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/oilsensor.png" height="300px" />.

## Code
Can be found in the ESP_oil_BT_WIFI_GH folder.

## Future Improvements
Change from Bluetooth Serial to BLE.

Small Android app that shows current oil temp and records the history. Would be interesting to see the oiltemp/time line for cold start or e.g. mountain pass driving. :)

Make the opening in the dashboard nicer. 
It's not as bad as it seems on the photos.

Additional display of oiltemp in graphical fashion, some sort of gauge.

## Interesting and helpful links

I was mostly working by this project, mainly for the actual temp calculation:
https://github.com/speeddragon/Arduino-CarSensorDashboard

Quite similar but advanced project
https://www.instructables.com/id/Wifi-Enabled-OLED-ESP32-Car-Gauges/

Gauge icon:
https://thenounproject.com/icon/490005/

Nice Car logos/images:
https://thenounproject.com/search/?q=cayman&i=248360

To create the images in "arduino compativble format":
http://javl.github.io/image2cpp/

## More "Work in Progress" photos
Yes, it's not a nice looking piece of work. But it does what it should do... ;)

<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_142209.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_140325.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_142253.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_142302.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_133825.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_133836.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200301_133128.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200229_160923.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200229_113049.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200223_084447.jpg" height="300px" />
<img src="https://github.com/moe01324/ESP32_OilTemp/blob/master/pics/IMG_20200222_075008.jpg" height="300px" />
