/* Read Quadrature Encoder
 * Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.
 *
 * Sketch by max wolf / www.meso.net
 * v. 0.1 - very basic functions - mw 20061220
 *
 */
int val;
int n = LOW;
int encoderPin = 11; // sensor is on T1 and is counted using the 16 bit timer/counter 1 
int DOWNPin = 10;
int UPPin = 9;
const int DOWN = 1;
const int UP = 0;

boolean counting;

void setup() { 
  pinMode (encoderPin,INPUT);
  pinMode (DOWNPin,OUTPUT);
  pinMode (UPPin,OUTPUT);

  Serial.begin (9600);
  
  configureCounter();
}

void configureCounter(){
 
 TCCR1A = 0;
 TCCR1B =  (1 << WGM12) // CTC mode using T1 as the input for counting events (page 136)
           | (1 << CS12) | (1 << CS11) | (1 << CS10) // Clock on T1 rising edge (page 137)
           ;
 TCCR1C = 0;
 
 // initially disable timer coutner interrupt;
 TIMSK1 = 0;
 // TIMSK1 = (1 << OCIE1A); // interrupt on compare match;
 // OCR1AH and OCR1AL used to set n of ticks
}

ISR(TIMER1_COMPA_vect) {
  // turn off the motors
  digitalWrite(DOWNPin, LOW);
  digitalWrite(UPPin, LOW);
  counting = false;
}

void startCountingUpTo(int steps){
  counting = true;
  // count up to `steps`. NB that OCR1AH must be written *before* OCR1AL
  OCR1AH = 0xff & (steps >> 8);
  OCR1AL = 0xff &  steps;
  // enable output compare interrupt
  TIMSK1 = (1 << OCIE1A);
  sei();
}

int getCurrentMoveCount(){
  int val = TCNT1L;
  val |= TCNT1H << 8;
  return val;
}

void loop() {
  motorMove(3100, DOWN);
  while( (! motorMove(3100, UP)) && (getCurrentMoveCount() < 1000) ){
    Serial.println("Lost the bag, going back down 100");
    motorMove(100, DOWN);
  }
  //  motorMove(400, DOWN);
  while(1);
} 


// Move n steps, then stop
bool motorMove(int steps, int direction) {
  Serial.print("motorMove "); Serial.println(steps);
  
  int count = 0;
  
  if(direction == DOWN) { // move DOWN
    digitalWrite(DOWNPin, HIGH);
    digitalWrite(UPPin, LOW);
  }
  else { // move UPs
    digitalWrite(UPPin, HIGH);
    digitalWrite(DOWNPin, LOW);    
  }

  startCountingUpTo(steps);
  
  while(counting){
    if(direction == UP && getCurrentMoveCount() > 250){
      int weight = analogRead(A0);
      
      if(weight < 300){
        counting = false;
        digitalWrite(DOWNPin, LOW);
        digitalWrite(UPPin, LOW);
        Serial.println("Aborting");
        return false;
      }
      
      Serial.println(weight);
    }else{
      Serial.println("in else"); 
    }
  }

  return true;
}
