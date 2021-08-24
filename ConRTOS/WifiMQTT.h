#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi101.h>

/************************* WiFI Setup *****************************/
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2           // or, tie EN to VCC

char ssid[] = "Poca_P1";      //  your network SSID (name)
char pass[] = "poca052894W";  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;             // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

/************************** MQTT *********************************/

#define AIO_SERVER      "70.37.56.209"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "admincdt"
#define AIO_KEY         "cdtlora"

/************ Global State (you don't need to change this!) ******************/

//Set up the wifi client
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
#define halt(s) { SERIAL.println(F( s )); while(1);  }

/****************************** Topics ***************************************/
Adafruit_MQTT_Publish Uplink = Adafruit_MQTT_Publish(&mqtt, "iotemido/Pruebas/Uplink/RFID/atwincRFID");
Adafruit_MQTT_Subscribe Downlink = Adafruit_MQTT_Subscribe(&mqtt, "iotemido/Pruebas/Downlink/RFID/atwincRFID");

#define LEDPIN 13



void wifisetup () 
{
  digitalWrite(LEDPIN, LOW);
  WiFi.setPins(WINC_CS, WINC_IRQ, WINC_RST, WINC_EN);
  SERIAL.println(F("Adafruit MQTT demo for WINC1500"));

  // Initialise the Client
  SERIAL.print(F("\nInit the WiFi module..."));
  // check for the presence of the breakout
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    SERIAL.println("WINC1500 not present");
    // don't continue:
    while (true);
  }
  SERIAL.println("ATWINC OK!");
  
  pinMode(LEDPIN, OUTPUT);
  mqtt.subscribe(&Downlink);
}



// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() 
{
  int8_t ret;

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) 
  {
    SERIAL.print("Attempting to connect to SSID: ");
    SERIAL.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    uint8_t timeout = 10;
    while (timeout && (WiFi.status() != WL_CONNECTED)) 
    {
      timeout--;
      delay(1000);
    }
  }
  
  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  SERIAL.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) 
  { // connect will return 0 for connected
       SERIAL.println(mqtt.connectErrorString(ret));
       SERIAL.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  SERIAL.println("MQTT Connected!");
}




static void threadWifiMQTT( void *pvParameters ) 
{
  SERIAL.println("Tarea WiFi MQTT: Iniciada");
  while(1)
  {
    digitalWrite(LEDPIN, LOW);
    MQTT_connect();
    // this is our 'wait for incoming subscription packets' busy subloop
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(5000))) 
    {
      if (subscription == &Downlink) 
      {
        SERIAL.print(F("Got: "));
        SERIAL.println((char *)Downlink.lastread);
  
        if (0 == strcmp((char *)Downlink.lastread, "OFF")) 
        {
          digitalWrite(LEDPIN, LOW);
        }
        if (0 == strcmp((char *)Downlink.lastread, "ON")) 
        {
          digitalWrite(LEDPIN, HIGH);
        }
      }
    }
    String strmensaje = "{\"Hall\":" + String(mediahall) + "}";
    char mensaje[strmensaje.length() + 1];
    strmensaje.toCharArray(mensaje, strmensaje.length() + 1);
    // Now we can publish stuff!
    SERIAL.print(F("\nSending Uplink val "));
    SERIAL.print(mensaje);
    SERIAL.print("...");
    if (! Uplink.publish(mensaje)) 
    {
      digitalWrite(LEDPIN, LOW);
      SERIAL.println(F("Failed"));
    } else 
    {
      digitalWrite(LEDPIN, HIGH);
      SERIAL.println(F("OK!"));
    }
    
    myDelayMs(300000);
  }
}
