# WirelessButtonforFABI
This page consists of all files related to the Bachelor Thesis of Friedrich Koenig. 

## Uploading the Code to Puck.js
0. Put a Coin Battery in the Puck
1. Open https://www.espruino.com/ide/ on a WebBLE enabled computer
2. Paste the code.
3. Flash the Puck.js

## Uploading the Code to Raspberry Pi Pico W
1. Download the Folder Fabi_Raspi. Open the File Fabi_Raspi.ino using Arduino IDE. All other files should be opened as well in different tabs.
2. It is important to use the Earle Philhower Core and use the settings in the screenshot. Especially the IP/Bluetooth Stack.
   ![image](https://github.com/user-attachments/assets/9af6be12-7d01-4f8e-b1b8-90cfbb6e2445)
3. Upload the code to the Pico

## Using the Prototype
1. Connect the Pico to a PC via USB. Make sure the cable is a full fledged USB cable with data transmission lines
2. Connect to the Pico via Wi-Fi using the SSID: FABI and the Password: asterics
3. Open the website http://192.168.42.1 in a browser of your choice
4. Set the Max Buttons to 1 and enter the Button Action of your choice, for example "Hello World".
5. Click on "Save Configuration"
6. Click on "Start FABI"
7. The Pico is now attemting to connect to the Puck. Once the connection is made you can press the onboard button on the puck
8. You may open the serial monitor in the Arduino IDE to see all debug messages
