#include <OneWire.h>            
#include <DallasTemperature.h>  
#include <TimerOne.h>          

#define ONE_WIRE_BUS 8  // DS18B20 data pin connected to Arduino pin 8
#define relay 4    // Relay control pin
#define led 11    // LED (used as heater indicator)
#define buzzer 10   // Buzzer pin

// Defining system states
typedef enum {
    IDLE_STATE,                 // Heater OFF (temperature very low)
    HEATING_STATE,              // Heater ON (in heating range)
    STABILIZING_STATE,          // Close to target, heater OFF to stabilize
    TARGET_REACHED_STATE,       // Target temperature reached
    OVER_HEAT_STATE,            // Overheating detected
    MAX_ELEMENT
} state;

void idle_state();
void heating_state();
void stabilizing_state();
void target_reached_state();
void overheat_state();

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Function pointer table for calling state functions
void (*state_table[MAX_ELEMENT])(void) = {
    idle_state,
    heating_state,
    stabilizing_state,
    target_reached_state,
    overheat_state
};

volatile bool Flag = false; // Flag that is set by the timer interrupt

void timerISR() // This function runs every 500ms (Timer interrupt) 
{
  Flag = true;  // Set flag when timer triggers
}

void setup() {
  Serial.begin(9600);      // Start Serial communication for debugging
  sensors.begin();         // Initialize temperature sensor
  pinMode(relay, OUTPUT);  // Relay pin as output
  pinMode(led, OUTPUT);    // LED pin as output
  pinMode(buzzer, OUTPUT); // Buzzer pin as output
  digitalWrite(led, HIGH); // Turn LED ON (indicates system is powered)

  // Set up Timer1 to trigger every 500ms
  Timer1.initialize(500000);   // 500,000 microseconds = 500ms
  Timer1.attachInterrupt(timerISR);  // Attaching ISR function
}

// Function to decide which state based on temperature
state getStateFromTemp(float tempC) {
  if (tempC < 30)                  // Very low temperature
    return IDLE_STATE;
  else if (tempC < 90)             // Heating range
    return HEATING_STATE;
  else if (tempC < 98)             // Almost reached target
    return STABILIZING_STATE;
  else if (tempC < 100)            // Target temperature reached
    return TARGET_REACHED_STATE;
  else                             // Overheating
    return OVER_HEAT_STATE;
}

void loop() {
  // Only run this block when timer flag is set
  if (Flag) {
    Flag = false;  // Clear the flag

    // Read temperature from DS18B20
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);  
    Serial.println(tempC);        // Print temperature to Serial Monitor

    // Decide the state based on current temperature
    state current_state = getStateFromTemp(tempC);

    // Call the corresponding function for the state
    state_table[current_state]();
  }
}

void idle_state() {
  digitalWrite(relay, LOW);       // Heater OFF
  Serial.println("Idle state");
}

void heating_state() {
  digitalWrite(relay, HIGH);      // Heater ON
  Serial.println("Heating state");
}

void stabilizing_state() {
  digitalWrite(relay, LOW);       // Heater OFF (to prevent overshoot)
  Serial.println("Stabilizing state");
}

void target_reached_state() {
  digitalWrite(relay, LOW);       // Heater OFF
  Serial.println("Target reached state");
  tone(buzzer, 500);              // Beep to indicate target reached
  delay(200);
  noTone(buzzer);                 // Stop beep
}

void overheat_state() {
  digitalWrite(relay, LOW);       // Heater OFF (safety)
  Serial.println("Overheat State");
}

