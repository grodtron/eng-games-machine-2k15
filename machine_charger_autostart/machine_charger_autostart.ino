const int SEL_STOP    = 2;
const int DECREMENT   = 5;
const int INCREMENT   = 3;
const int START_ENTER = 4      ;

// Change a pin from a high-impedance input to an LOW-level output
void toggle_output(int pin, int duration){
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  digitalWrite(13, HIGH);
  delay(duration);
  pinMode(pin, INPUT);
  digitalWrite(13, LOW);
}


void setup(){
    
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(5000);
  digitalWrite(13, LOW);
  
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
