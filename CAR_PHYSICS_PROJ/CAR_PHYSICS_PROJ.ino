
const int PIN_RED = 6;
const int PIN_GREEN = 5;
const int PIN_BLUE = 3;

const int PIN_RED_2 = 9;
const int PIN_GREEN_2 = 10;
const int PIN_BLUE_2 = 11;

const int PIN_IN1 = 7;
const int PIN_IN2 = 8;

const int PIN_IN3 = 12;
const int PIN_IN4 = 13;

const int ORANGE_R = 0; // Max Red
const int ORANGE_G = 225;  // *** FIX: Reduced Green from 100 to 30 for truer orange/less yellow ***
const int ORANGE_B = 255;   // No Blue

bool isBlinkingOrange = false;
bool ledIsOn = false;

bool isBlinkingOrange_2 = false;
bool ledIsOn_2 = false;

unsigned long lastBlinkTime = 0;
const long blinkInterval = 500; 

// Variable to store the incoming serial command
char command = ' ';

// Function to set the RGB LED color
void setColor(int R, int G, int B) {
  // Use analogWrite (PWM) to control the brightness of each color
  analogWrite(PIN_RED, R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE, B);
}

void setColor_2(int R, int G, int B) {
  // Use analogWrite (PWM) to control the brightness of each color
  analogWrite(PIN_RED_2, R);
  analogWrite(PIN_GREEN_2, G);
  analogWrite(PIN_BLUE_2, B);
}

void moveMotor(char direction) {
    if (direction == 'W') {
        // Spin Forward: IN1 HIGH, IN2 LOW
        digitalWrite(PIN_IN1, HIGH);
        digitalWrite(PIN_IN2, LOW);

    } else if (direction == 'S') {
        // Spin Backward: IN1 LOW, IN2 HIGH
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, HIGH);

    } else if (direction == 'X') {
        // Stop Motor: Both LOW
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, LOW);
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, LOW);   

    } else if (direction == 'L'){
        digitalWrite(PIN_IN3, HIGH);
        digitalWrite(PIN_IN4, LOW);      

    } else if (direction == 'L'){
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, HIGH);         
    }
}

void setup() {
  Serial.begin(9600);
  Serial.println("RGB LED Control Ready. Type 'N' to blink orange, 'F' to stop.");

  // Set the LED pins as outputs
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);

  pinMode(PIN_RED_2, OUTPUT);
  pinMode(PIN_BLUE_2, OUTPUT);
  pinMode(PIN_GREEN_2, OUTPUT);

  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  // Start with the LED off
  setColor(0, 0, 0);
  moveMotor('X'); // Stop the motor initially
}

void loop() {
  // 1. Handle Serial Input (Non-blocking check)
  if (Serial.available()) {
    command = Serial.read(); 

    if (command == 'N') {
      // Start blinking orange
      isBlinkingOrange = true;
      // Reset the state to ensure the blink starts immediately on command
      ledIsOn = false; 
      lastBlinkTime = 0; 
    } 
    else if (command == 'F') {
      // Stop blinking and turn the LED off
      isBlinkingOrange = false;
      isBlinkingOrange_2 = false;
      setColor(0, 0, 0); // Turn LED off
    }    
    else if (command == 'W' || command == 'S' || command == 'X' || command == 'R' || command == 'L') {
      moveMotor(command);
    }
    else if (command == 'G'){
      isBlinkingOrange_2 = true;
      ledIsOn_2 = false;
      lastBlinkTime = 0;
    }
  }

  // 2. Handle Non-Blocking Blinking Logic
  if (isBlinkingOrange) {
    unsigned long currentTime = millis();

    // Check if the blink interval time has passed
    if (currentTime - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentTime; // Save the current time for the next blink

      if (ledIsOn) {
        // If the LED is currently ON, turn it OFF
        setColor(0, 0, 0);
        ledIsOn = false;
      } else {
        // If the LED is currently OFF, turn it ORANGE
        setColor(ORANGE_R, ORANGE_G, ORANGE_B);
        ledIsOn = true;
      }
    }
  }

  if (isBlinkingOrange_2) {
    unsigned long currentTime = millis();

    // Check if the blink interval time has passed
    if (currentTime - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentTime; // Save the current time for the next blink

      if (ledIsOn_2) {
        // If the LED is currently ON, turn it OFF
        setColor_2(0, 0, 0);
        ledIsOn_2 = false;
      } else {
        // If the LED is currently OFF, turn it ORANGE
        setColor_2(ORANGE_R, ORANGE_G, ORANGE_B);
        ledIsOn_2 = true;
      }
    }
  }

}
