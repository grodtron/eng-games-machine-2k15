const int SEL_STOP    = A4;
const int DECREMENT   = A3;
const int INCREMENT   = A2;
const int START_ENTER = A1;

// Change a pin from a high-impedance input to an LOW-level output
void toggle_output(int pin, int duration){
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  delay(duration);
  pinMode(pin, INPUT);
}

// delay and blink the LED at the same time.
//
// delays for ~exactly duration, blinking with period 200ms
void blinky_delay(int duration){
  pinMode(13, OUTPUT);
  
  for(int i = 0; i < (duration/200); ++i){
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }
  
  delay(duration % 200);
}

void setup(){
    
  blinky_delay(2000);
  
  toggle_output(INCREMENT, 100);
  delay(100);
  toggle_output(INCREMENT, 100);
  delay(100);
  
  toggle_output(START_ENTER, 2000);
  delay(100);
  
  toggle_output(START_ENTER, 100);
}

void loop(){
  
}
