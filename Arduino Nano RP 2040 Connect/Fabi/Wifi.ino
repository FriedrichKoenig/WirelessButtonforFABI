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
    status = WiFi.beginAP(ssid, pass);
    if (status != WL_AP_LISTENING) {
        Serial.println("Creating access point failed");
        while (true);
    }

    delay(10000);  // Wait for connection

    // Start the web server on port 80
    server.begin();

    // You're connected now, so print out the status
    printWiFiStatus();
}

void loop_Wifi() {
    WiFiClient client = server.available();   // listen for incoming clients
    if (client) {                             // if you get a client
        Serial.println("New client connected");
        String currentLine = "";                // Stores current line of HTTP request
        String request = "";                    // Stores full HTTP request line
        bool isRequestLine = true;              // Flag for first request line

        while (client.connected()) {            // loop while the client is connected
            if (client.available()) {             // if there’s bytes to read from the client
                char c = client.read();             // read a byte
                Serial.write(c);                    // print it out to the serial monitor

                // Accumulate the current line
                if (c != '\r' && c != '\n') {
                    currentLine += c;                 // Accumulate the HTTP request line
                }

                // If the line is complete (newline), process it
                if (c == '\n') {
                    // If this is the request line (first line), store it
                    if (isRequestLine) {
                        request = currentLine;
                        Serial.print("Received Request: ");
                        Serial.println(request);
                        isRequestLine = false;          // Ensure this is only done once
                    }

                    // If the line is blank, this means the headers are done
                    if (currentLine.length() == 0) {
                        // Send the HTML response (this part happens for every request)
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        // Generate the HTML form response
                        client.print("<html><body><h1>Configure Bluetooth Buttons</h1>");
                        client.print("<form action=\"/save\" method=\"GET\">");
                        client.print("<label for=\"maxButtons\">Max Buttons: </label>");
                        client.print("<input type=\"number\" id=\"maxButtons\" name=\"maxButtons\" value=\"" + String(maxButtons) + "\" min=\"1\" max=\"20\"/><br/><br/>");

                        // Generate input fields for each button action
                        for (int i = 0; i < maxButtons; i++) {
                            client.print("<label for=\"button" + String(i) + "\">Button " + String(i + 1) + " Action: </label>");
                            client.print("<input type=\"text\" id=\"button" + String(i) + "\" name=\"button" + String(i) + "\" value=\"" + buttonActions[i] + "\"/><br/><br/>");
                        }

                        client.print("<input type=\"submit\" value=\"Save Configuration\"/></form>");
                        client.print("<form action=\"/startBLE\" method=\"GET\">");
                        client.print("<input type=\"submit\" value=\"Start FABI\"/></form>");
                        client.println("</body></html>");

                        client.println();  // End the response
                        break;             // Exit the client loop after sending the response
                    }

                    // Clear the current line for the next line
                    currentLine = "";
                }
            }
        }

        // Now that the response is sent, handle the request logic
        client.stop();  // Close the connection after serving the page
        Serial.println("Client disconnected");

        // Handle the GET request (check after the client is disconnected)
        if (request.startsWith("GET /save?")) {
            Serial.println("Processing save request");
            parseConfig(request);  // Process the configuration form data
            Keyboard.key_code_raw(KEY_F5);
        } else if (request.startsWith("GET /startBLE")) {
            Serial.println("Processing Start FABI request");
            startFABI();                         // Start BLE and disable WiFi
            wifiActive = false;                  // Mark WiFi as inactive
            setup_BLE();                         // Setup BLE
        }

        // Reset the request variable after processing
        request = ""; // Clear request to prevent old data from affecting future requests
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
