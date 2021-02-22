#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int pin;
int pin2;
int pin3;
int pin4;

char auth[] = "HMqxx_bjkFacOB5FokzwWJOVS0a0vGdm";
char ssid[] = "KaewKanya"; 
char pass[] = "05051214";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  pin= digitalRead(D0);
  Serial.print(pin);
  
  pin2= digitalRead(D1);
  Serial.println(pin2);

  pin3= digitalRead(D2);
  Serial.println(pin3);

  pin4= digitalRead(D3);
  Serial.println(pin4);
  delay (1000);

  
}
