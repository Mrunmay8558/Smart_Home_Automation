#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <time.h>

const int led_pin = 5; // set the relay pin number
int relayStatus = LOW; // initialize relay status as off


ESP8266WebServer server(80);


void handleRoot() {
  String html = "<html><div style=\"text-align:center;padding:1em 0;\"> <h3><a style=\"text-decoration:none;\" href=\"https://www.zeitverschiebung.net/en/country/in\"><span style=\"color:gray;\">Current local time in</span><br />India</a></h3> <iframe src=\"https://www.zeitverschiebung.net/clock-widget-iframe-v2?language=en&size=medium&timezone=Asia%2FKolkata\" width=\"100%\" height=\"115\" frameborder=\"0\" seamless></iframe> </div>\n"
"";
  html += "<style> body{ text-align: center; width: 100%; height: 100vh; margin: 0; background-image: url(https://cdndailyexcelsior.b-cdn.net/wp-content/uploads/2020/09/smart-home-technology.jpg); ;font-family: Tahoma;font-size: 16px;}";
  html += "h1, p {margin: 1em auto;text-align: center; }"; 
  html += ".section {background-color: #1b1b32; color: #f5f6f7; opacity: 0.8;background-repeat: no-repeat;  background-attachment: fixed;  background-position: center; width: 30%; margin-left:auto; margin-right: auto; padding: 20px; max-width: 80px;  }";
  html += "form {width: 60vw; max-width: 500px; min-width: 300px;  margin: 0 auto;  padding-bottom: 2em;} ";
  html += "input,select {  margin: 10px 0 0 0;  width: 20%;  min-height: 2em;} "; 
  html += "input[type='submit'] {  display: block;  width: 15%;  margin: 1em auto;  height: 2em;  font-size: 1.1rem;  background-color: #3b3b4f;  border-color: white;  min-width: 150px;} ";
  html += "input[type='reset'] {  display: block;  width: 15%;  margin: 1em auto;  height: 2em;  font-size: 1.1rem;  background-color: #3b3b4f;  border-color: white;  min-width: 150px;}</style> ";
  html += "<head><meta charset='UTF-8'> <title> Home Automation </title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'></head>";
  
  html += "<body><fieldset class='section'><h1>Relay Control</h1>";
  html += "<form method='post' action='/'>";
  html += "<label for='relay'>Relay: </label>";
  html += "<select id='relay' name='relay'>";
  html += "<option value='on'>OFF</option>";
  html += "<option value='off'>ON</option>";
  html += "</select>";
  html += "<br>";
  html += "<input type='submit' value='Submit'>";
  html += "</form>";
  html += "<br>";
  html += "<h1>Timer Control</h1>";
  html += "<p>Set the timer in minutes and seconds:</p>";
  html += "<form action='/start' method='get'>";
  html += "<input type='number' name='minutes' placeholder='Minutes' min='0'><br>";
  html += "<input type='number' name='seconds' placeholder='Seconds' min='0' max='59'><br>";
  
  html += "<input type='submit' value='Start Timer'>";
  

 
  html += "</form> </fieldset>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleForm() {
  String relayState = server.arg("relay");

  if (relayState == "on") {
    digitalWrite(led_pin, HIGH);
    relayStatus = HIGH;
  } else {
    digitalWrite(led_pin, LOW);
    relayStatus = LOW;
  }

  server.send(200, "text/html", "Relay state set!");
}
void handleStart() { // Handle the timer start request
  String relayState = server.arg("relay");
  
  
  int minutes = server.arg("minutes").toInt();
  int seconds = server.arg("seconds").toInt();

  // Start the timer
  int duration = (minutes * 60 + seconds) * 1000;
   delay(duration);
   
  if(relayStatus ==HIGH){
  // Turn off the LED
  digitalWrite(led_pin, LOW);
  String html = "<html><body>";
  html += "<h1>Timer Ended</h1>";
  html += "<p>The timer has ended and the LED has been turned off.</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
  }
  else{
    digitalWrite(led_pin, HIGH);
    String html = "<html><body>";
  html += "<h1>Timer Ended</h1>";
  html += "<p>The timer has ended and the LED has been turned on.</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
  }
  // Send the response
  
  
}



void setup() {
  pinMode(led_pin, OUTPUT); // set the relay pin as output
  Serial.begin(9600);
  WiFi.begin("realme_3_pro", "12345678"); // replace with your WiFi SSID and password
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
 Serial.println(WiFi.localIP());
 

  server.on("/", HTTP_GET, handleRoot);
  server.on("/start", handleStart); // Set the timer start handler
  server.on("/", HTTP_POST, handleForm);
//  server.on("/timer", HTTP_POST, handleTimer);
  
  server.begin();
}

void loop() {
  
  server.handleClient();
  
}
