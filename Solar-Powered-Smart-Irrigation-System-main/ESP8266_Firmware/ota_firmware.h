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
