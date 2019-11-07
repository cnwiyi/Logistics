#define LED_BUILTIN 13

void led_setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("LED (builtin) intialized as an output");
}

void led_blink(int num, int sec) {
  for (int i=0; i<num; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(sec); 
    digitalWrite(LED_BUILTIN, LOW); 
    delay(sec); 
  }
}

void led_blink_f(int num) {
   led_blink(num, 100);
}

void led_blink_n(int num) {
  led_blink(num, 500);
}

void led_blink_s(int num) {
  led_blink(num, 1000);
}

void led_turnOn () {
  digitalWrite(LED_BUILTIN, HIGH);
}

void led_turnOff () {
  digitalWrite(LED_BUILTIN, LOW);
}
