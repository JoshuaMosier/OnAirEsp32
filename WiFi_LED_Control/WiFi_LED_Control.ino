/* Largely based on WiFi Web Server LED Blink
 * Adapted by Joshua Mosier 2020 */
 
#include <Adafruit_NeoPixel.h>

//Define Strip Inputs
#define PIN_1 32
#define PIN_2 33

//Instantiate Strips w/ length of 16 LEDs
Adafruit_NeoPixel Strip0 = Adafruit_NeoPixel(16, PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Strip1 = Adafruit_NeoPixel(16, PIN_2, NEO_GRB + NEO_KHZ800);

//Enter WiFi Information
#include <WiFi.h>
const char* ssid     = "ssid";
const char* password = "password";

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    Strip0.begin();
    Strip1.begin();
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;
//Define Strip Colors (R,G,B,Brightness)
uint32_t red_0 = Strip0.Color(128, 0, 0, 64);
uint32_t red_1 = Strip1.Color(128, 0, 0, 64);
uint32_t off_0 = Strip1.Color(0, 0, 0, 0);
uint32_t off_1 = Strip1.Color(0, 0, 0, 0);

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";
    // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          Strip0.fill(red_0,1,16);
          Strip0.show();
          Strip1.fill(red_1,1,16);
          Strip1.show();
        }
        if (currentLine.endsWith("GET /L")) {
          Strip0.fill(off_0,1,16);
          Strip1.fill(off_1,1,16);
          Strip1.show();
          Strip0.show();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
