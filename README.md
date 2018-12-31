# dual-axissolar-tracking-and-its-monitoring-using-IOT
Arduino is programmed such that the solar panel moves in the direction of sun with help of dc motors  . On the other hand, we will monitor the solar panel and its directions which can be observed in the app with help of nodemcu 
we need to write code for arduino and nodemcu.
SERIAL COMMUNICATION TAKES PLACE BETWEEN NODEMCU AND ARDUINO
for arduino
include lcd library
define the directions
setup the pins
begin lcd and set cursor
analogread reads the ldr values from the pins and based on the comparisions we get the direction of panel
here the analog pins connected to ldrs
for nodemcu
include library files
if you create a all things talk account you get a client id and key
you should have a common wifi network between your device and nodemcu
define pin numbers and id's
SET WIFI
FROM SERIAL DATA SEND THE DIRECTIONS TO THE DEVICE
