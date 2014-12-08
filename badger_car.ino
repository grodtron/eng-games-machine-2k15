const int lowerSensePin = A0;
const int upperSensePin = A1;
const int frontSensePin = A2;

const int lowerLedPin   = A3;
const int upperLedPin   = A4;
const int frontLedPin   = A5;

const int LEFT_FWD = 5;
const int LEFT_BAK = 3;

const int RIGHT_FWD = 6;
const int RIGHT_BAK = 4;

#define MOVE_FWD(SIDE, amount) do {  \
  analogWrite(SIDE ## _FWD, amount); \
  digitalWrite(SIDE ## _BAK, LOW);   \
} while(0)  

boolean lower, upper, front;
void (*currentState)();

void waitingForLargeRadiusTurn();
void doingLargeRadiusTurn();
void doingSpeedBump();
void doingSmallRadiusTurn();
void waitingForSmallRadiusTurn();
void error(int code);

void setup(){
  pinMode(LEFT_FWD, OUTPUT);
  pinMode(LEFT_BAK, OUTPUT);
  pinMode(RIGHT_FWD, OUTPUT);
  pinMode(RIGHT_BAK, OUTPUT);

  pinMode(lowerSensePin, INPUT);
  pinMode(upperSensePin, INPUT);
  pinMode(frontSensePin, INPUT);

  pinMode(lowerLedPin, OUTPUT);
  pinMode(upperLedPin, OUTPUT);
  pinMode(frontLedPin, OUTPUT);

  MOVE_FWD(LEFT, 0);
  MOVE_FWD(RIGHT, 0);
  
  currentState = waitingForLargeRadiusTurn;
}

void loop(){
  
  lower = digitalRead(lowerSensePin);
  upper = digitalRead(upperSensePin);
  front = digitalRead(frontSensePin);
  
  digitalWrite(lowerLedPin, lower);
  digitalWrite(upperLedPin, upper);
  digitalWrite(frontLedPin, front);

  if(front){
    // always stop everything if we hit the front bumper
    MOVE_FWD(LEFT, 0);
    MOVE_FWD(RIGHT, 0);
  }else{
    if(currentState){
      currentState();
    }
  }
}

void waitingForLargeRadiusTurn(){
  if(upper){
    error(0);
    return;
  }
  static boolean haveTouched = false;
  static int awayCount = 0;
  if(lower){
    awayCount = 0;
    haveTouched = true;
    MOVE_FWD(RIGHT, 80);
    MOVE_FWD(LEFT, 128);
  }else{
    // Don't switch states until we've touched the side atleast once
    if(awayCount < 200 || !haveTouched){
      // keep track of how long it has been since we touched the side
      ++awayCount;
      MOVE_FWD(RIGHT, 128);
      MOVE_FWD(LEFT, 80);
    }else{
      currentState = doingLargeRadiusTurn;
      // reset so that the next time we do this function it will be correct
      haveTouched = false;
      return;
    }
  }
  delay(1);
}

void doingLargeRadiusTurn(){
  if(upper){
    error(1);
    // currentState = doingSpeedBump;
    return;
  }
  
  if(lower){
    MOVE_FWD(RIGHT, 200);
    MOVE_FWD(LEFT, 255);
  }else{
    // turning ratio is ~ 5:3
    // but also I think it's not exactly linear...
    // definitely not quite linear...
    MOVE_FWD(RIGHT, 250);
    MOVE_FWD(LEFT, 100);
  }
  delay(1);
}

void error(int code){
  MOVE_FWD(RIGHT, 0);
  MOVE_FWD(LEFT,  0);

  while(1){
    for(int i = 0; i < 4; ++i){
      digitalWrite(lowerLedPin, HIGH);
      digitalWrite(upperLedPin, HIGH);
      digitalWrite(frontLedPin, HIGH);
      delay(125);
      digitalWrite(lowerLedPin, LOW);
      digitalWrite(upperLedPin, LOW);
      digitalWrite(frontLedPin, LOW);
      delay(125);
    }
    digitalWrite(lowerLedPin, (code >> 0) & 1);
    digitalWrite(upperLedPin, (code >> 1) & 1);
    digitalWrite(frontLedPin, (code >> 2) & 1);
    delay(1000);
  }
}
