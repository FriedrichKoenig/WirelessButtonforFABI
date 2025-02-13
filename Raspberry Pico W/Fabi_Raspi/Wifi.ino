void setup_Wifi() {
  Serial.println("Access Point Web Server");
  digitalWrite(LED_BUILTIN, HIGH);
  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Create access point
  Serial.print("Creating access point named: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_OFF); // Reset WiFi mode
  WiFi.disconnect(true); // Clear previous WiFi settings

  delay(100);
  WiFi.mode(WIFI_AP);  // Set mode to Access Point
  status = WiFi.beginAP(ssid, pass);
  WiFi.setHostname(ssid);
  delay(5000);  // Wait for the AP to start
  server.begin();  // Start the web server
  printWiFiStatus();  // Print the connection status
}

void loop_Wifi() {
  WiFiClient client = server.available();  // Check for incoming client
  if (client) {
    Serial.println("New client connected");
    String currentLine = "";
    String request = "";
    bool isRequestLine = true;
    // wait for data to be available
    unsigned long timeout = millis();
//    while (client.available() == 0) {
//      if (millis() - timeout > 5000) {
//        Serial.println(">>> Client Timeout !");
//        client.stop();
//        delay(60000);
//        return;
//      }
//    }
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // Accumulate the request line
        if (c != '\r' && c != '\n') {
          currentLine += c;
        }

        if (c == '\n') {
          if (isRequestLine) {
            request = currentLine;
            Serial.print("Request: ");
            Serial.println(request);
            isRequestLine = false;
          }

          if (currentLine.length() == 0) {
            // Send HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html><body><h1>Configure Bluetooth Buttons</h1>");
            client.println("<form action=\"/save\" method=\"GET\">");
            client.println("<label for=\"maxButtons\">Max Buttons: </label>");
            client.print("<input type=\"number\" id=\"maxButtons\" name=\"maxButtons\" value=\"");
            client.print(maxButtons);
            client.println("\" min=\"1\" max=\"20\"/><br/><br/>");

            for (int i = 0; i < maxButtons; i++) {
              client.print("<label for=\"button");
              client.print(i);
              client.print("\">Button ");
              client.print(i + 1);
              client.print(" Action: </label>");
              client.print("<input type=\"text\" id=\"button");
              client.print(i);
              client.print("\" name=\"button");
              client.print(i);
              client.print("\" value=\"");
              client.print(buttonActions[i]);
              client.println("\"/><br/><br/>");
            }

            client.println("<input type=\"submit\" value=\"Save Configuration\"/></form>");
            client.println("<form action=\"/startBLE\" method=\"GET\">");
            client.println("<input type=\"submit\" value=\"Start FABI\"/></form>");
            client.println("</body></html>");
            client.println();
            break;
          }
          currentLine = "";
        }
      }
    }

    //client.stop();  // Close the connection
    Serial.println("Client disconnected");

    // Process request
    if (request.startsWith("GET /save?")) {
      Serial.println("Saving configuration");
      parseConfig(request);
    } else if (request.startsWith("GET /startBLE")) {
      Serial.println("Starting FABI (BLE)");
      startFABI();
    }
  }
}


void parseConfig(String line) {
  // Extract the maximum number of buttons
  int idx = line.indexOf("maxButtons=");
  if (idx != -1) {
    int endIdx = line.indexOf('&', idx);
    if (endIdx == -1) endIdx = line.length();
    maxButtons = line.substring(idx + 11, endIdx).toInt();
  }

  // Ensure maxButtons is within a valid range
  maxButtons = constrain(maxButtons, 1, 20); // Limit maxButtons

  // Extract actions for each button
  for (int i = 0; i < maxButtons; i++) {
    String buttonParam = "button" + String(i) + "=";
    idx = line.indexOf(buttonParam);
    if (idx != -1) {
      int endIdx = line.indexOf('&', idx);
      if (endIdx == -1) endIdx = line.length();
      String action = line.substring(idx + buttonParam.length(), endIdx);
      action.replace("+", " "); // Replace '+' with space
      action.trim(); // Remove leading/trailing whitespace

      // Remove anything starting with "HTTP"
      int httpIdx = action.indexOf("HTTP");
      if (httpIdx != -1) {
        action = action.substring(0, httpIdx); // Truncate before "HTTP"
      }
      action.trim();
      // Store the cleaned action
      buttonActions[i] = action; // Save the action for the button
      Serial.println("Button " + String(i + 1) + " Action: " + buttonActions[i]);
    }
  }

  // Reset the buttonActions array if necessary
  for (int i = maxButtons; i < 20; i++) {
    buttonActions[i] = ""; // Clear any unused actions
  }
}

void startFABI() {
  // Close Wifi Connection so BLE can start
  WiFi.end();
  Serial.println("WiFi off, BLE started");
  setup_BLE();
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
