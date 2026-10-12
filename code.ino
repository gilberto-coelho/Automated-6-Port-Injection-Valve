#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <Servo.h>

Servo myServo;

// Access Point configuration
const char* ssid = "AUTO_VALVE";
const char* password = "";

// IP Configuration
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
DNSServer dnsServer;

int servoPin = 2;
int buttonPin = 3;

int automatic = 0;
int openDegree = 0;
int closedDegree = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  myServo.attach(servoPin);

  EEPROM.begin(512); 

  // Read EEPROM values
  openDegree = EEPROM.read(0);
  closedDegree = EEPROM.read(2);

  // Access Point configuration
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  // Captive Portal
  dnsServer.start(53, "*", local_IP);

  // Webpage
  server.on("/", []() {
    String modeMessage = automatic == 0 ? "Automatic mode" : "Test mode";

    server.send(200, "text/html",
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset=\"UTF-8\">"
                "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                "<title>Automated Valve</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; margin: 20px; }"
                "table { width: 100%; border-collapse: collapse; }"
                "td { padding: 10px; }"
                "input[type='number'], input[type='submit'] { padding: 10px; font-size: 1rem; }"
                "</style>"
                "</head>"
                "<body>"
                "<h1>Automated valve</h1><br>"
                "<h4>Created by: Gilberto Coelho</h4><br>"
                "<form action=\"/setdegrees\">"
                "<table>"
                "<tr><td>OPEN degree:</td><td><input type=\"number\" name=\"opendegree\" min=\"0\" max=\"180\" value=\"" + String(openDegree) + "\"></td><td> Set to:  " + String(openDegree) + "&deg;</td></tr>"
                "<tr><td>CLOSE degree:</td><td><input type=\"number\" name=\"closeddegree\" min=\"0\" max=\"180\" value=\"" + String(closedDegree) + "\"></td><td> Set to: " + String(closedDegree) + "&deg;</td></tr>"
                "</table>"
                "<br><input type=\"submit\" value=\"SAVE degrees\"></form>"
                "<br><form action=\"/moveopen\"><input type=\"submit\" value=\"Move OPEN position\"></form>"
                "<br><form action=\"/moveclose\"><input type=\"submit\" value=\"Move CLOSE position\"></form>"
                "<br><form action=\"/automatic\"><input type=\"submit\" value=\"Return to AUTOMATIC mode\"></form>"
                "<br><p>" + modeMessage + "</p>"  // AAdd mode message
                "</body>"
                "</html>");
  });

  // Servo degrees definition
  server.on("/setdegrees", []() {
    String openDegreeString = server.arg("opendegree");
    String closedDegreeString = server.arg("closeddegree");
    openDegree = openDegreeString.toInt();
    closedDegree = closedDegreeString.toInt();

    EEPROM.write(0, openDegree);
    EEPROM.write(2, closedDegree);
    EEPROM.commit();

    // Retun to main page after saving
    server.sendHeader("Location", "/", true);
    server.send(303);  
  });

  // Move servo to "openDegree"
  server.on("/moveopen", []() {
    myServo.write(openDegree);
    automatic = 1;
    server.sendHeader("Location", "/", true);
    server.send(303);  
  });

  // Move servo to "closedDegree"
  server.on("/moveclose", []() {
    myServo.write(closedDegree);
    automatic = 1;
    server.sendHeader("Location", "/", true);
    server.send(303); 
  });

  // Change to automatic mode
  server.on("/automatic", []() {
    automatic = 0;
    server.sendHeader("Location", "/", true);
    server.send(303);  
  });

  // Sendo any route to the main page
  server.onNotFound([]() {
    server.sendHeader("Location", "/", true);
    server.send(303);
  });

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();

  if (digitalRead(buttonPin) == HIGH && automatic == 0) {
    myServo.write(closedDegree);
    delay(50);
  }
  if (digitalRead(buttonPin) == LOW && automatic == 0){
    myServo.write(openDegree);
    delay(50);
  }
}
