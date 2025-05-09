/*
  Heater Control System
  ----------------------
  - Reads temperature using DHT22
  - Controls heater and cooling based on temperature thresholds
  - Displays status using RGB LED
  - Alerts with buzzer during overheat or when target is reached
  - BLE functionality available (commented)
*/

#include <DHT.h>

// Pin definitions
#define HEATER_PIN 5
#define DHTPIN 15
#define DHTTYPE DHT22
#define BUZZER_PIN 19
#define RED_PIN 21
#define GREEN_PIN 22
#define BLUE_PIN 23
#define COOLING 18

// Initialize sensor
DHT dht(DHTPIN, DHTTYPE);

// Thresholds
const float TARGET_TEMP = 35;
const float MARGIN_TEMP = 1.5;
const float OVERHEAT_TEMP = 45;

// Global variables
float currentTemp = 0;
bool heaterOn = false;
bool buzzerTriggered = false;

// Heater states
enum HeaterState {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

HeaterState currentState = IDLE;

// Function declarations
void setRGB(bool r, bool g, bool b);
void readTemp();
void updateHeaterState();
void Heater();
String getStateName(HeaterState state);
void coolingToggle();

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(COOLING, OUTPUT);

  Serial.println("Heater System Initialized.");
}

void loop() {
  readTemp();
  updateHeaterState();
  coolingToggle();
  delay(1000);
}

void readTemp() {
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Error Reading Temperature!");
    delay(1000);
    return;
  }

  currentTemp = temp;

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C");
}

void updateHeaterState() {
  if (currentTemp >= OVERHEAT_TEMP) {
    currentState = OVERHEAT;
    tone(BUZZER_PIN, 1000, 500);
  } else {
    noTone(BUZZER_PIN);
  }

  switch (currentState) {
    case IDLE:
      setRGB(false, true, false);
      if (currentTemp < TARGET_TEMP - MARGIN_TEMP) currentState = HEATING;
      else if (currentTemp >= TARGET_TEMP - MARGIN_TEMP && currentTemp <= TARGET_TEMP + MARGIN_TEMP) {
      currentState = STABILIZING;
      buzzerTriggered = false; 
      }
      break;

    case HEATING:
      setRGB(true, true, false);
      if (currentTemp >= TARGET_TEMP - MARGIN_TEMP) currentState = STABILIZING;
      break;

    case STABILIZING:
      setRGB(false, true, true);
      if (currentTemp >= TARGET_TEMP - 0.5) {
        currentState = TARGET_REACHED;
        if (!buzzerTriggered) {
          tone(BUZZER_PIN, 250, 500);
          buzzerTriggered = true;
        }
      }
      break;

    case TARGET_REACHED:
      setRGB(false, false, true);
      if (currentTemp < TARGET_TEMP - MARGIN_TEMP) currentState = HEATING;
      break;

    case OVERHEAT:
      setRGB(true, false, false);
      if (currentTemp < TARGET_TEMP - 5) {
        currentState = IDLE;
        buzzerTriggered = false;
      }
      else if(currentTemp > TARGET_TEMP - MARGIN_TEMP && currentTemp < OVERHEAT_TEMP) {
      currentState = STABILIZING;
      }
      break;
  }

  Serial.print(" | State: ");
  Serial.print(getStateName(currentState));
  Heater();
}

void Heater() {
  heaterOn = (currentState == HEATING || currentState == STABILIZING);
  digitalWrite(HEATER_PIN, heaterOn ? HIGH : LOW);

  Serial.print(" | Heater: ");
  Serial.print(heaterOn ? "ON" : "OFF");

  Serial.print(" | Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println("s");
}

void setRGB(bool r, bool g, bool b) {
  digitalWrite(RED_PIN, r ? HIGH : LOW);
  digitalWrite(GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(BLUE_PIN, b ? HIGH : LOW);
}

String getStateName(HeaterState state) {
  switch (state) {
    case IDLE: return "IDLE";
    case HEATING: return "HEATING";
    case STABILIZING: return "STABILIZING";
    case TARGET_REACHED: return "TARGET_REACHED";
    case OVERHEAT: return "OVERHEAT";
    default: return "UNKNOWN";
  }
}

void coolingToggle() {
  digitalWrite(COOLING, (currentState == OVERHEAT || currentTemp > TARGET_TEMP + 3) ? HIGH : LOW);
}

/*
// BLE section (optional)
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

BLEAdvertising *advertising;
String lastPayload = "";

void setup(){
  ...

  BLEDevice::init("HeaterSystem");
  advertising = BLEDevice::getAdvertising();
}

void loop(){
  ...

  startBLEAdvertising();
}

void startBLEAdvertising() {
  String payload = "T:" + String(currentTemp) + " H:" + (heaterOn ? "On" : "Off") + " S:" + getStateName(currentState);

  if (payload != lastPayload) {
    BLEAdvertisementData advData;
    advData.setName("HeaterSystem");
    advData.setManufacturerData(payload);
    advertising->setAdvertisementData(advData);
    advertising->start();
    lastPayload = payload;
  }

  Serial.println("Advertising: " + payload);
}
*/

