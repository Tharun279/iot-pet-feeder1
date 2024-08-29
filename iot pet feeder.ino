#include <ESP8266WiFi.h>
#include <Servo.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
#define WIFI_SSID "POCO C55"
#define WIFI_PASSWORD "00000000"

// SinricPro credentials
#define APP_KEY "c2998508-ac47-4b3f-9fc3-f77c9be50d0b"
#define APP_SECRET "05a451c8-451a-4f63-81cf-46fd17d396e6-914c1a6c-0b42-4715-902b-27f232a29f80"

// Device IDs
#define DOG_FEEDER_ID "668e5ace674e208e6ff3cafa"
#define CAT_FEEDER_ID "668eef685d818a66fac5b8d7"
#define AUTOMATIC_FEEDER_ID "668ef5fd888aa7f7a244b7e6"
#define SECONDARY_APP_KEY "f6889a5c-5e54-4fb1-b3e6-3321f01cebde"
#define SECONDARY_APP_SECRET "14349591-4915-4595-92ab-73cc1898650a-961cdca3-7bf3-4a3b-a5d7-c99dcea76916"
#define DOG_FOOD_LEVEL_ID "669df2976e1af359351982f1"
#define CAT_FOOD_LEVEL_ID "669df325674e208e6ffbf965"
// Servo pin
#define SERVO_PIN D8

// Ultrasonic sensor pins
#define TRIG_PIN1 D5
#define ECHO_PIN1 D6
#define TRIG_PIN2 D7
#define ECHO_PIN2 D4

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address of your I2C LCD

// NTP client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // IST offset 19800 seconds (5 hours 30 minutes)

// Variables to track if the feeders have been triggered
bool dogMorningTriggered = false;
bool dogAfternoonTriggered = false;
bool dogEveningTriggered = false;
bool catMorningTriggered = false;
bool catAfternoonTriggered = false;
bool catEveningTriggered = false;
bool feedDog = false;
bool feedCat = false;
bool automaticFeeder = false;

void feedPet(bool isDog) {
  Serial.println(isDog ? "Feeding the dog..." : "Feeding the cat...");
  lcd.setCursor(0, 0);
  lcd.print(isDog ? "Feeding the dog" : "Feeding the cat");
  if (isDog) {
    myServo.write(180);  // Rotate to 180 degrees for dog food
  } else {
    myServo.write(0);  // Rotate to 0 degrees for cat food
  }
  delay(3000);        // Keep feeder open for 3 seconds
  myServo.write(90);  // Return to the neutral position (90 degrees)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Feeding done.");
  Serial.println("Feeding done.");
}

// Function to reset the triggers at midnight
void resetTriggers() {
  if (timeClient.getHours() == 0 && timeClient.getMinutes() == 0) {
    dogMorningTriggered = false;
    dogAfternoonTriggered = false;
    dogEveningTriggered = false;
    catMorningTriggered = false;
    catAfternoonTriggered = false;
    catEveningTriggered = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Triggers reset");
    lcd.setCursor(0, 1);
    lcd.print("at midnight");
    Serial.println("Triggers reset at midnight.");
  }
}

// Function to measure distance using ultrasonic sensor
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1;
  
  return distance;
}

// Callback function for SinricPro commands
bool onPowerState(const String &deviceId, bool &state) {
  if (state) {
    if (deviceId == DOG_FEEDER_ID) {
      feedPet(true);
    } else if (deviceId == CAT_FEEDER_ID) {
      feedPet(false);
    } else if (deviceId == AUTOMATIC_FEEDER_ID) {
      automaticFeeder = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Automatic mode on");
      Serial.println("Automatic mode activated.");
    } else if (deviceId == DOG_FOOD_LEVEL_ID) {
      long distance = measureDistance(TRIG_PIN1, ECHO_PIN1);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Dog food level:");
      lcd.setCursor(0, 1);
      lcd.print(String(distance) + " cm");
      Serial.print("Dog food level: ");
      Serial.print(distance);
      Serial.println(" cm");
    } else if (deviceId == CAT_FOOD_LEVEL_ID) {
      long distance = measureDistance(TRIG_PIN2, ECHO_PIN2);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cat food level:");
      lcd.setCursor(0, 1);
      lcd.print(String(distance) + " cm");
      Serial.print("Cat food level: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    return true;  // Request handled properly
  }
  return false;   // Request not handled
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.printf("Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");

  // Initialize servo motor
  myServo.attach(SERVO_PIN);
  myServo.write(90);   // Initialize servo in closed position

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // Initialize NTP client
  timeClient.begin();

  // Initialize SinricPro
  SinricProSwitch &dogFeeder = SinricPro[DOG_FEEDER_ID];
  dogFeeder.onPowerState(onPowerState);

  SinricProSwitch &catFeeder = SinricPro[CAT_FEEDER_ID];
  catFeeder.onPowerState(onPowerState);

  SinricProSwitch &automaticFeederDevice = SinricPro[AUTOMATIC_FEEDER_ID];
  automaticFeederDevice.onPowerState(onPowerState);

  SinricProSwitch &dogFoodLevelDevice = SinricPro[DOG_FOOD_LEVEL_ID];
  dogFoodLevelDevice.onPowerState(onPowerState);

  SinricProSwitch &catFoodLevelDevice = SinricPro[CAT_FOOD_LEVEL_ID];
  catFoodLevelDevice.onPowerState(onPowerState);

  SinricPro.onConnected([]() {
    Serial.printf("Connected to SinricPro\r\n");
  });
  SinricPro.onDisconnected([]() {
    Serial.printf("Disconnected from SinricPro\r\n");
  });

  SinricPro.begin(APP_KEY, APP_SECRET);

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready...");
}

void loop() {
  SinricPro.handle();
  timeClient.update();

  // Get the current time
  Serial.printf("Current time: %02d:%02d\r\n", timeClient.getHours(), timeClient.getMinutes());
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.setCursor(6, 0);
  lcd.printf("%02d:%02d", timeClient.getHours(), timeClient.getMinutes());

  // Measure distances from ultrasonic sensors
  long distance1 = measureDistance(TRIG_PIN1, ECHO_PIN1);
  long distance2 = measureDistance(TRIG_PIN2, ECHO_PIN2);

  // Display distances on LCD
  lcd.setCursor(0, 1);
  lcd.printf("D1:%3dcm D2:%3dcm", distance1, distance2);

  // Automatic feeding logic
  if (automaticFeeder) {
    if (timeClient.getHours() == 8 && timeClient.getMinutes() == 0 && !dogMorningTriggered) {
      feedPet(true);
      dogMorningTriggered = true;
      delay(1000);
      feedPet(false);
      catMorningTriggered = true;
    }
    if (timeClient.getHours() == 13 && timeClient.getMinutes() == 0 && !dogAfternoonTriggered) {
      feedPet(true);
      dogAfternoonTriggered = true;
      delay(1000);
      feedPet(false);
      catAfternoonTriggered = true;
    }
    if (timeClient.getHours() == 18 && timeClient.getMinutes() == 0 && !dogEveningTriggered) {
      feedPet(true);
      dogEveningTriggered = true;
      delay(1000);
      feedPet(false);
      catEveningTriggered = true;
    }
  }

  // Reset triggers at midnight
  resetTriggers();
}
