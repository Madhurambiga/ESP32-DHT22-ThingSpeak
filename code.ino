#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define HUMIDITY_LED 5
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Airtel_Airtel-Pumo";
const char* password = "Mustang5.0";

unsigned long channelID = 3398190;
const char* writeAPIKey = "VI65F793YB1YR2T2";

WiFiClient client;

void setup()
{
  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  ThingSpeak.begin(client);
  pinMode(HUMIDITY_LED, OUTPUT);
digitalWrite(HUMIDITY_LED, LOW);
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if(isnan(temperature) || isnan(humidity))
  {
    Serial.println("DHT Read Failed");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if(status == 200)
  {
    Serial.println("Data Uploaded Successfully");
  }
  else
  {
    Serial.print("Upload Failed. Error: ");
    Serial.println(status);
  }

  delay(20000);   // ThingSpeak free version minimum 15 sec
  if(humidity < 40)
{
    digitalWrite(HUMIDITY_LED, LOW);
   
}
else if(humidity >= 40)
{
    digitalWrite(HUMIDITY_LED, HIGH);
    
}
else
{
    digitalWrite(HUMIDITY_LED, HIGH);
    delay(200);
    digitalWrite(HUMIDITY_LED, LOW);
    delay(200);
}
}
