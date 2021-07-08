/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/ianp/Documents/iot2021/labs/lab-14/src/lab-14.ino"
#include "MQTT.h"
#include "oled-wing-adafruit.h"
#include "blynk.h"

void setup();
void loop();
void callback(char *topic, unsigned char *payload, unsigned int length);
#line 5 "/home/ianp/Documents/iot2021/labs/lab-14/src/lab-14.ino"
SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;
MQTT client("lab.thewcl.com", 1883, callback);

const char *mqtt_chan = "lab-14";
const char *chat_mqtt_chan = "lab-14-chat";
const pin_t led = D7;

void setup()
{
  Blynk.begin("odxX7fJIzlYk9KSvqupATEtsxhdFenM-", IPAddress(167, 172, 234, 162), 9090); // in setup

  display.setup();
  display.clearDisplay();
  display.display();

  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  // init blynk
  Blynk.run();

  // init display
  display.loop();

  if (client.isConnected())
  {
    client.loop();
    if (display.pressedA())
    {
      client.publish(mqtt_chan, "display A button was pressed!");
    }
    else if (display.pressedB())
    {
      client.publish(mqtt_chan, "display B button was pressed!");
    }
    else if (display.pressedC())
    {
      client.publish(mqtt_chan, "display C button was pressed!");
    }
  }
  else // makes sure to connect to MQTT when device goes online
  {
    client.connect(System.deviceID());

    if (client.isConnected())
    {
      client.subscribe(mqtt_chan);
      client.subscribe(chat_mqtt_chan);
      //client.publish(mqtt_chan, "hello world from Ian's particle argon, id: " + System.deviceID() + "!");
    }
  }
}

void callback(char *topic, unsigned char *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;

  Serial.printlnf("bytes: %d", payload[0]);
  Serial.printlnf("chars: %d", p[0]);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.printlnf("topic: %s\n", topic);
  display.printlnf("data: %s", p);
  display.display();

  if (p[0] == 49)
  {
    display.println("got: 1");
    display.display();
    digitalWrite(led, HIGH);
  }
  else if (p[0] == 48)
  {
    display.println("got: 0");
    digitalWrite(led, LOW);
  }
}

BLYNK_WRITE(V0)
{
  const char *payload = param.asStr();
  client.publish(chat_mqtt_chan, payload);
}