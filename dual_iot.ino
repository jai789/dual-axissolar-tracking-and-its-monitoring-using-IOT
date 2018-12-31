

#include <ESP8266WiFi.h>
#include <stdio.h>
#include <PubSubClient.h>
#include <ATT_IOT.h>                            //AllThingsTalk IoT library
#include <SPI.h>                                //required to have support for signed/unsigned long type..


char deviceId[] = "N7FT67TPMFnh5k5eLgtnrf7t"; // Your device id comes here
char clientId[] = "venu"; // Your client id comes here;
char clientKey[] = "f4q3zndnjrq"; // Your client key comes here;

const char* ssid     = "wifi";
const char* password = "wifipassword";

int rdata,i=0;
char a[10];
ATTDevice Device(deviceId, clientId, clientKey);            //create the object that provides the connection to the cloud to manager the device.
#define httpServer "api.AllThingsTalk.io"                  // HTTP API Server host                  
#define mqttServer httpServer                        // MQTT Server Address 


// Define PIN numbers & id's for assets

int DigitalActuator = 0;                                    // Digital Actuator is connected to pin D0 on nodemcu -> built in led
int DigitalActuatorId = 4;                                   // the pin numbers go above 9, so we use a separate id for the assets
//int left_traffic=1,right_traffic=2,AnalogActuator3=3,AnalogActuator4=9;
//int AnalogActuator5=5,AnalogActuator6=6,AnalogActuator7=7,AnalogActuator8=8;
int east=1,west=2,north=3,south=9,device=10;
//required for the device
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient ethClient;
PubSubClient pubSub(mqttServer, 1883, callback,ethClient);  

void setup()
{      
  pinMode(DigitalActuator, OUTPUT);                             // initialize the digital pin as an output.         
  Serial.begin(9600);                                           // init serial link for debugging
  
  setup_wifi();
  delay(1000);                                                  //give the Ethernet shield a second to initialize:
  while(!Device.Connect(&ethClient, httpServer))                // connect the device with the IOT platform.
    Serial.println("retrying");
 
  while(!Device.Subscribe(pubSub))                              // make certain that we can receive message from the iot platform (activate mqtt)
    Serial.println("retrying");                                                               
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
                    
void loop()
{
  String val;
  if(Serial.available())
  {
  for(i=1;rdata!='#';i++)
  {
   rxd();
      a[i]=rdata; 
  }
  rdata=0;
  //int j=(((a[2]^0x30)*100)+((a[3]^0x30)*10)+(a[4]^0x30));
  val=a[2];
  val+=a[3];
  val+=a[4];
  Serial.println(val);
 if(a[1]=='E')
    {
    Device.Send(val, east);
    }
  else if(a[1]=='W')
    {
    Device.Send(val, west);
    }
  else if(a[1]=='N')
    {
    Device.Send(val, north);
    }
  else if(a[1]=='S')
    {
    Device.Send(val, south);
    }
    else if(a[1]=='D')
    {
      Device.Send(val, device);
      }
  }
  Device.Process(); 
  delay(100);
}
void rxd()
{
while(Serial.available()==0)  ;
rdata=Serial.read();
}

// Callback function: handles messages that were sent from the iot platform to this device.
void callback(char* topic, byte* payload, unsigned int length) 
{ 
  String msgString; 
  {                                                           //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
  char message_buff[length + 1];                        //need to copy over the payload so that we can add a /0 terminator, this can then be wrapped inside a string for easy manipulation
  strncpy(message_buff, (char*)payload, length);        //copy over the data
  message_buff[length] = '\0';                  //make certain that it ends with a null     
      
  msgString = String(message_buff);
  msgString.toLowerCase();            //to make certain that our comparison later on works ok (it could be that a 'True' or 'False' was sent)
  }
  int* idOut = NULL;
  {                                                       //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
      int pinNr = Device.GetPinNr(topic, strlen(topic));
  
      Serial.print("Payload: ");                            //show some debugging
      Serial.println(msgString);
      Serial.print("topic: ");
      Serial.println(topic);

      if (pinNr == DigitalActuatorId)  
      {
        if (msgString == "false") {
            digitalWrite(DigitalActuator, HIGH);        //change the actuator status to false -> the pin appears to be inversed, so set high
            idOut = &DigitalActuatorId;                           
        }
        else if (msgString == "true") {
            digitalWrite(DigitalActuator, LOW);              //change the actuator status to true  -> the pin appears to be inversed, so set high
            idOut = &DigitalActuatorId;
        }
      }  
  }
  if(idOut != NULL)                                           //Let the iot platform know that the operation was succesful
    Device.Send(msgString, *idOut);    
}



