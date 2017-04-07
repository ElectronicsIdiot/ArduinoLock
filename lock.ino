#define RED 9
#define GREEN 8
#define BLUE 7
#define LOCK 6

const int pin[] = {0, 0, 1, 2, 3}; // The code to unlock
const int pinLength = sizeof(pin) / sizeof(int); // Length of the code

const int buttons[] = {13, 12, 11, 10}; // Arduino pins the buttons are connected to
const int buttonsLength = sizeof(buttons) / sizeof(int); // Number of buttons connected
bool pressed[buttonsLength]; // Which buttons were pressed pressed when last checked

int index = 0; // Index of current pin digit
bool correct = true; // If input matches pin
int attempt = 0;

void setup() {
  // Set LED and unlock pins to output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(LOCK, OUTPUT);

  // Set buttons to inputs
  for (int i = 0; i < buttonsLength; ++i) {
    pinMode(buttons[i], INPUT);
  }
}

void loop() {
  digitalWrite(BLUE, !index); // Show blue LED if index is 0

  // Loop buttons
  for (int i = 0; i < buttonsLength; ++i) {
    //If the button is pressed
    if (digitalRead(buttons[i])) {
      if(pressed[i]) return; // Ignore if already pressed
      pressed[i] = true; // Set current button as pressed

      correct = correct && i == pin[index]; // If the pin doesn't match set correct to false

      if (++index == pinLength) { // Increment digit index and check if last digit
        if (correct) { // Unlock and show green LED if code is correct
          digitalWrite(GREEN, HIGH);
          digitalWrite(LOCK, HIGH);
          attempt = 0; // Reset timer
          delay(1000); // Leave LED/motor on for 1 second
        } else { // The code is incorrect
          attempt++; // Increase a timer

          for(int j = 0; j < attempt; j++) { // Take longer if is not first attempt
            for(int k = 0; k < 5; k++) { // Flash red LED
              digitalWrite(RED, HIGH);
              delay(100);
              digitalWrite(RED, LOW);
              delay(100);
            }
          }
        }

        // Turn off LED/motor
        digitalWrite(GREEN, LOW);
        digitalWrite(LOCK, LOW);
        digitalWrite(RED, LOW);

        // Reset for next attempt
        index = 0;
        correct = true;
      }
    } else { // Button is not pressed
      pressed[i] = false; // Set current button as not pressed
    }
  }

  delay(10); // Run every 10ms
}
