/*
  Simple POST client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a POST request
  and a request body

 

  created 14 Feb 2016
  by Tom Igoe
  
  this example is in the public domain
 */

// all the restful beauty
#include <ArduinoHttpClient.h>
#include <WiFi101.h>
#include "arduino_secrets.h"

// we are using a LCD screen
#include <LiquidCrystal.h>


/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char serverAddress[] = "128.122.6.151";  // server address
int port = 8080;

WiFiClient wifi;
//HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
String response;
int statusCode = 0;


/////// LCD settings ///////
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


/////// button & potentiometer settings + logic ///////
const int buttonPin = 6;
int onSwitch = 0;
const int potPinStyle = 3;
const int potPinTempo = 4;
const int potPinColor = 5;
const int potPinFeet = 2;

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonPushCounterCurrent = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


void setup() {
  Serial.begin(9600);

    // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, dancers!");

  //connect to wifi
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  /////// READ BUTTONS & POTENTIOMETERS ///////
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      if (onSwitch == 0) {
        onSwitch = 1;
      } else {
        onSwitch = 0;
      }
      } 
    delay(50);
    }
    lastButtonState = buttonState; // save the current state as the last state


    // potentiometer reading, mapping and display on LCD
    int readingStyle  = analogRead(potPinStyle);
    int readingTempo  = analogRead(potPinTempo);
    int readingColor  = analogRead(potPinColor);
    int readingFeet  = analogRead(potPinFeet);
    int styleVal = 0;
    int tempoVal = 0;
    int colorVal = 0;
    int feetVal = 0;

    styleVal = map((- readingStyle + 1023), 0, 1023, 1, 5);
    tempoVal = map((- readingTempo + 1023), 0, 1023, 30, 281);
    colorVal = map((- readingColor + 1023), 0, 1023, 0, 360);
    feetVal = map((- readingFeet + 1023), 0, 1023, 3, 29);
    
    //write to LCD screen
    lcd.setCursor(0, 0);
    lcd.print("  Your Choices " );
    lcd.setCursor(0, 1);
    lcd.print(styleVal);
    // arrange it for display position, vary between 3 or 2 digits
    if (tempoVal < 281) {
      if (tempoVal < 100) {
        lcd.setCursor(3, 1);
        lcd.print(" ");
        lcd.print(tempoVal);
      }
      else {
        lcd.setCursor(3, 1);
        lcd.print(tempoVal);
      }
    };
    if (colorVal < 360) {
      if (colorVal < 100) {
        lcd.setCursor(8, 1);
        lcd.print(" ");
        lcd.print(colorVal);       
      } else {
        lcd.setCursor(8, 1);
        lcd.print(colorVal);
      }
    };
    if (feetVal < 29) {
      if (feetVal < 10) {
        lcd.setCursor(13, 1);
        lcd.print(" ");
        lcd.print(feetVal);
      } else {
        lcd.setCursor(13, 1);
        lcd.print(feetVal);
      }
    }
  
  /////// RESTFUL BEAUTY ///////
  Serial.println("making POST request");
  String contentType = "application/json";
  String postDataConnect = "{\"connect\":\"" + String("true") + "\"}";
  String postDataPeople = "{\"numPeople\":\"" + String(feetVal) + "\"}";
  String postDataColor = "{\"floorHue\":\"" + String(colorVal) + "\"}";
  String postDataDance = "{\"danceNum\":\"" + String(styleVal) + "\"}";
  String postDataTempo = "{\"tempoVal\":\"" + String(tempoVal) + "\"}";
  
  String postConnect = String(postDataConnect);
  String postPeople = String(postDataPeople);
  String postColor = String(postDataColor);
  String postDance = String(postDataDance);
  String postTempo = String(postDataTempo);
  
  Serial.println(postConnect);
  Serial.println(postPeople);
  Serial.println(postColor);
  Serial.println(postDance);
  Serial.println(postTempo);
  
  HttpClient clientC = HttpClient(wifi, serverAddress, port);
  clientC.post("/connect", contentType, postConnect);
  delay(200);
  HttpClient clientP = HttpClient(wifi, serverAddress, port);
  clientP.post("/people", contentType, postPeople);
  delay(200);
  HttpClient clientH = HttpClient(wifi, serverAddress, port);
  clientH.post("/color", contentType, postColor);
  delay(200);
   HttpClient clientD = HttpClient(wifi, serverAddress, port);
  clientD.post("/dance", contentType, postDance);
  delay(200);
   HttpClient clientT = HttpClient(wifi, serverAddress, port);
  clientT.post("/tempo", contentType, postTempo);
  delay(200);

}
