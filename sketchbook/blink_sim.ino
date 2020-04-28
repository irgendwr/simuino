// ===============================================
// SIMUINO_BEGIN
// ===============================================
// BOARD_TYPE:  UNO
// SKETCH_NAME: blink.ino
// SIM_LENGTH:  100
// WIN_LAYOUT:    2
// SO_DELAY:     40
//================================================
//  Scenario
//================================================
//
//================================================
// Simuino log text customization
//================================================
// PINMODE_OUT: 13  "PIN: Led Blink"
//
// SIMUINO_END
// ===============================================
//
// ##### Aufgabe x.y #####
// Digital I/O / Blinking LED
// Board connection pin of the LED
// ANODE (+) -> "longer leg" of the LED

const int pin_led = 13;

bool ledstate = false;

void toggle_ledstate() {
  ledstate = !ledstate;
}

void setup() {
  pinMode(pin_led, OUTPUT);   // Configure direction of board connection pin of the LED (-> OUTPUT)
  digitalWrite(pin_led, LOW); // Configure initial logic level on board connection pin of the LED
}

void loop() {
  toggle_ledstate();
  digitalWrite(pin_led, ledstate);
  delay(1000);
  toggle_ledstate();
  digitalWrite(pin_led, ledstate);
  delay(500);
}
