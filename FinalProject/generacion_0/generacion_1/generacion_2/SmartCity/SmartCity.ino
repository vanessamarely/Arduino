#include "utils/config.h"
#include "utils/const.cpp"

#include "components/actuators.cpp"
#include "components/testActuators.cpp"

#include "controllers/trafficLights.cpp"
#include "controllers/testTrafficLights.cpp"

#include "controllers/traffic.cpp"
#include "controllers/testTraffic.cpp"

#include <Wire.h>               //Library required for I2C comms (LCD)
#include <LiquidCrystal_I2C.h>  //Library for LCD display via I2C
#include <math.h>               //Mathematics library for pow function (CO2 computation)

//Library definitions
LiquidCrystal_I2C lcd(0x27, 16, 4);  //Set the LCD address to 0x27 for a 16 chars and 4 line display

void displayMessage(String message, int delay_time) {
  Serial.println("Display shows: " + String(message));
  lcd.setCursor(0, 0);
  lcd.print(message);
  delay(delay_time);
  lcd.clear();
}

/*
void test() {

  //Test Display
  Serial.println("Start test");
  displayMessage("Start test", 2000);

  //Test Actuators
  Serial.println("Test TrafficLight 1");
  displayMessage("Test TrafficLight 1", 2000);
  testChangeTrafficLightStateByLightColor(PIN_LIGHTS_1);

  Serial.println("Test TrafficLight 2");
  displayMessage("Test TrafficLight 2", 2000);
  testChangeTrafficLightStateByLightColor(PIN_LIGHTS_2);

  //Test Controllers
  Serial.println("Test Control TrafficLights");
  displayMessage("Test Control TrafficLights", 2000);
  testControlTrafficLights(PIN_LIGHTS_1, PIN_LIGHTS_2);

  Serial.println("Test Control Traffic");
  displayMessage("Test Control Traffic", 2000);
  testControlTraffic(PIN_LIGHTS_1, PIN_LIGHTS_2);

  Serial.println("End test");
  displayMessage("End test", 2000);
}
*/

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Wait for serial port to open
  while (!Serial) {
    delay(10);
  }

  //Pin config
  pinMode(P1, INPUT);                       //Traffic light 1 button as Input
  pinMode(P2, INPUT);                       //Traffic light 2 button as Input
  pinMode(CNY1, INPUT);                     //Infrared sensor 1 in traffic light 1 as Input
  pinMode(CNY2, INPUT);                     //Infrared sensor 2 in traffic light 1 as Input
  pinMode(CNY3, INPUT);                     //Infrared sensor 3 in traffic light 1 as Input
  pinMode(CNY4, INPUT);                     //Infrared sensor 4 in traffic light 2 as Input
  pinMode(CNY5, INPUT);                     //Infrared sensor 5 in traffic light 2 as Input
  pinMode(CNY6, INPUT);                     //Infrared sensor 6 in traffic light 2 as Input
  pinMode(TRAFFIC_LIGHT_1_RED, OUTPUT);     // Red traffic light 1 as Output
  pinMode(TRAFFIC_LIGHT_1_YELLOW, OUTPUT);  // Yellow traffic light 1 as Output
  pinMode(TRAFFIC_LIGHT_1_GREEN, OUTPUT);   // Green traffic light 1 as Output
  pinMode(TRAFFIC_LIGHT_2_RED, OUTPUT);     // Red traffic light 2 as Output
  pinMode(TRAFFIC_LIGHT_2_YELLOW, OUTPUT);  // Yellow traffic light 2 as Output
  pinMode(TRAFFIC_LIGHT_2_GREEN, OUTPUT);   // Green traffic light 2 as Output

  //Output cleaning
  digitalWrite(TRAFFIC_LIGHT_1_RED, LOW);     //Turn Off Red traffic light 1
  digitalWrite(TRAFFIC_LIGHT_1_YELLOW, LOW);  //Turn Off Yellow traffic light 1
  digitalWrite(TRAFFIC_LIGHT_1_GREEN, LOW);   //Turn Off Green traffic light 1
  digitalWrite(TRAFFIC_LIGHT_2_RED, LOW);     //Turn Off Red traffic light 2
  digitalWrite(TRAFFIC_LIGHT_2_YELLOW, LOW);  //Turn Off Yellow traffic light 2
  digitalWrite(TRAFFIC_LIGHT_2_GREEN, LOW);   //Turn Off Green traffic light 2

  //Communications
  lcd.init();       //Start communications with LCD display
  lcd.backlight();  //Turn on LCD backlight

  // Run tests
  test();
}

void loop() {

  if (Serial.available() > 0) {
    // Read incoming message
    String traffic_state = Serial.readStringUntil("\r");
    // Process the message
    String message = "Traffic Control: " + traffic_state;
    Serial.println(message);
    displayMessage(message, 2000);
    controlTraffic(traffic_state.toInt(), PIN_LIGHTS_1, PIN_LIGHTS_2);
  }

}
