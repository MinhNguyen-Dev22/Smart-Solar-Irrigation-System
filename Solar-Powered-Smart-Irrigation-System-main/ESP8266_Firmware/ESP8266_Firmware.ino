#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6-j88RT8W"
#define BLYNK_TEMPLATE_NAME "doan11"
#define BLYNK_AUTH_TOKEN "AIOqh_uXxUlY3bVBJzoL93QI4eKKDkjW"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <DHT.h>
#include <SimpleTimer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPtimeESP.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Setup///////////////////////////////////////////////////////////
#define APSSID "Zzz"
#define APPSK  "0000000a" 
#define startWaterTime 8 //Time water start [integer]
#define endWaterTime 16 //Time water end [integer]
#define timeBTW 2 // After timeBTW hour(s) [integer], water again
#define waterLength 20 //(second) [integer]
#define timeSleep 10 //(second) [integer]
#define DHTPIN D5
#define Relay D7 // Pump
const String Ver = "1.0.1";
const String dateup = "11/05/2025";

float lastTemp = 0.0;
float lastHum = 0.0;

ESP8266WiFiMulti WiFiMulti;
WidgetTerminal terminal(V3);
SimpleTimer timer;
strDateTime dateTime;

#define DHTTYPE DHT11   // DHT 11
bool processing = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

#include "RTC_setting.h"
#include "ota_firmware.h"
#include "Blynkprocess.h"

void setup() {
  Serial.begin(115200);  // Chỉ dùng 1 lần, ưu tiên 115200

  Wire.begin(D2, D1); // Chân SDA = D2, SCL = D1
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ESP8266 san sang!");
  lcd.setCursor(0, 1);
  lcd.print("Chao phuong098");
  delay(2000);
  lcd.clear();

  WiFi.mode(WIFI_STA);
  WiFi.begin(APSSID, APPSK);
  WiFiMulti.addAP(APSSID, APPSK);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, APSSID, APPSK);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
  
  dht.begin();


  terminal.clear();
  terminal.print("Esp8266 IoT ");
  terminal.println(Ver);
  terminal.print("phuong098\n");
  terminal.print("Connected.");
  terminal.flush();

  timer.setInterval(5000L, sendUptime); // check Wifi connection sau 5s
}


void sendUptime()
{   
  if(processing) return;
  
  if (WiFi.status() != WL_CONNECTED)
  {
    ESP.deepSleep(timeSleep * (long)1e6);
  }
    
  UpdateDHT();
  if(getTimeWatering()) AcessWater();
}

void loop() {
  Blynk.run();
  timer.run();  
}
DHT dht(DHTPIN, DHTTYPE);

void UpdateDHT()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (!(isnan(h) || isnan(t) || isnan(f))) 
  {
    float hic = dht.computeHeatIndex(t, h, false);

    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, hic);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nhiet do: ");
    lcd.print(t, 1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Do am: ");
    lcd.print(h, 1);
    lcd.print("%");
  }
}

void DHTCall()
{
  processing = 1;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    terminal.println("Failed to read from DHT sensor!");
  }
  else
  {
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    terminal.print(F("Humidity: "));
    terminal.println(h);
    terminal.print(F("%  Temperature: "));
    terminal.print(t);
    terminal.print(F("°C "));
    terminal.print(f);
    terminal.print(F("°F  Heat index: "));
    terminal.print(hic);
    terminal.print(F("°C "));
    terminal.print(hif);
    terminal.println(F("°F"));
    terminal.flush();
  }
  delay(1000);
  processing = 0;
}

void AcessWater()
{
    dateTime = NTPch.getNTPtime(7.0, 0);
  
    if(dateTime.valid)
    {
      NTPch.printDateTime(dateTime);
      byte actualHour = dateTime.hour;
      byte actualMinute = dateTime.minute;
      byte actualSecond = dateTime.second;

      terminal.print("Thoi gian: ");
      terminal.print(actualHour);
      terminal.print(":");
      terminal.print(actualMinute);
      terminal.print(":");
      terminal.println(actualSecond);
    }

    terminal.println("Bat dau...");
    DHTCall();
    processing = 1;
    digitalWrite(Relay, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dang tuoi nuoc");
    lcd.setCursor(0, 1);
    lcd.print("TG con lai: ");
    for(int i = 20; i >= 0; i--)
    {
      terminal.print("TG con lai: ");
      terminal.print(i);
      terminal.print(" s\n");
      terminal.flush();
      lcd.setCursor(13, 1);
      lcd.print("   ");
      lcd.setCursor(13, 1);
      lcd.print(i);
      delay(1000);
    }
    digitalWrite(Relay, LOW);
    terminal.println("Done.");
    terminal.flush();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tuoi nuoc xong");
    delay(2000);
    lcd.clear();
    processing = 0;
}

BLYNK_WRITE(V3)
{
  if (String("update") == param.asStr()) 
  {
    if ((WiFiMulti.run() == WL_CONNECTED))
      Ota();
    else
      terminal.println("No internet connection.");
  } 
  else if (String("restart") == param.asStr() || String("reboot") == param.asStr()) 
  {
    terminal.print("Restarting");
    for(byte i = 1; i <= 3; i++)
    {
      delay(1000);
      terminal.print('.');
    }
    terminal.flush();
    ESP.restart();
  }
else if (String("Tuoi nuoc") == param.asStr())
  {
    AcessWater();
  }
  else if (String("Xoa") == param.asStr())
  {
    terminal.clear();
  }
  else if (String("temperature") == param.asStr() || String("Nhiet do") == param.asStr())
  {
    DHTCall();
  }
  else if (String("help") == param.asStr())
  {
    terminal.println("Command option:");
    terminal.println("Phien ban");
    terminal.println("update");
    terminal.println("restart");
    terminal.println("Tuoi nuoc");
    terminal.println("Xoa");
    terminal.println("Nhiet do");
    terminal.print("help\n");
  }
  else if (String("Phien ban") == param.asStr())
  {
    terminal.print("Esp8266 IoT ");
    terminal.println(Ver);
    terminal.print("Ngay tao: ");
    terminal.println(dateup);
    terminal.println("Chip: NodeMCU.");
    terminal.print("phuong098\n");
    terminal.flush();
  }
  else
  {
    terminal.println("Sai cau lenh!");
  }

  terminal.flush();
}
// Only use for non-commercial purposes
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

extern LiquidCrystal_I2C lcd;
NTPtime NTPch("ch.pool.ntp.org");   // Server NTP

extern DHT dht;
extern float lastTemp;
extern float lastHum;

bool getTimeWatering()
{
  dateTime = NTPch.getNTPtime(7.0, 0); // Timezone

  // check dateTime.valid before getting time
  if(dateTime.valid)
  {
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;      // Hour
    byte actualMinute = dateTime.minute;  // Minute
    byte actualSecond = dateTime.second;  // Second

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      lastTemp = temp;
      lastHum = hum;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(lastTemp, 1);
    lcd.print((char)223); // degree symbol
    lcd.print("C H:");
    lcd.print(lastHum, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Thoi gian ");
    if (actualHour < 10) lcd.print('0');
    lcd.print(actualHour);
    lcd.print(":");
    if (actualMinute < 10) lcd.print('0');
    lcd.print(actualMinute);

    if((actualHour >= startWaterTime && actualHour <= endWaterTime) && ((actualHour % timeBTW == 0) && (actualMinute == 0) && (actualSecond <= 25)))
      return true;
  }// Acess water garden between [startWaterTime..endWaterTime] hour.
  return false;
}
// Only use for non-commercial purposes
/** Warning: Test this library carefully before uploading to OTA website if you change anything in here**/
////////////////Firmware link///////////////
const char *website = "http://waterautomatic.000webhostapp.com/firmware.bin";///////////////////////////////////

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
  terminal.println("CALLBACK:  HTTP update process started");
  terminal.flush();
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}

bool NewVerFound() // check new firmware is available or not
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, "http://waterautomatic.000webhostapp.com/firmware_ver"); //HTTP URL for hosted server(local server)
  int httpCode = http.GET();
  if(httpCode > 0) {
      if(httpCode == HTTP_CODE_OK) {
           //HTTP_CODE_OK means code == 200
             String payload = http.getString();// gives us the message received by the GET Request
             http.end();
             if(payload == Ver)
             {
              terminal.print("New update firmware is not found.\n");
              terminal.print("Up to date. ");
              terminal.println(Ver);
              terminal.flush();
              return false;
             }
             return true;
         }
     }
  http.end();
  terminal.println("Erorr: Can't get update information. Try again\n");
  terminal.flush();
  return false;
}

void Ota()
{
  if(!NewVerFound()) return;
  else terminal.print("New update found.\n");
  processing = 1;
  
  for (uint8_t t = 4; t > 0; t--) {
    terminal.print("[SETUP] WAIT ... ");
    terminal.println(t);
    terminal.flush();
    delay(1000);
  }
  
  terminal.println("Do not turn off the device during the update process.");
  terminal.flush();
  delay(500);
  WiFiClient client;

  // The line below is optional. It can be used to blink the LED on the board during flashing
  // The LED will be on during download of one buffer of data from the network. The LED will
  // be off during writing that buffer to flash
  // On a good connection the LED should flash regularly. On a bad connection the LED will be
  // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
  // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
  ESPhttpUpdate.followRedirects(true);

  // Add optional callback notifiers
  ESPhttpUpdate.onStart(update_started);
  ESPhttpUpdate.onEnd(update_finished);
  ESPhttpUpdate.onProgress(update_progress);
  ESPhttpUpdate.onError(update_error);
  
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, website);

  switch (ret) {
case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      terminal.print("HTTP_UPDATE_FAILED Error ");
      terminal.print(ESPhttpUpdate.getLastError());
      terminal.print(" : ");
      terminal.println(ESPhttpUpdate.getLastErrorString().c_str());
      terminal.flush();
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      terminal.println("HTTP_UPDATE_NO_UPDATES");
      terminal.flush();
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      terminal.println("HTTP_UPDATE_OK");
      terminal.flush();
      break;
  }
}