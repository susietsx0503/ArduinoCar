#define left_motor 10
#define right_motor 9
#define IR1 19
#define IR2 18
#define IR3 17

int BLED = 3;  //define LED
int GLED = 2;
int RLED = 4;

int Linitial;  //initial value of differences read by sensor
int Rinitial;
int Lreal;  //real value of differences read by sensor when the car moves
int Rreal;
int thresh = -200; //threshold value before a car starts to adjust itself
int threshStop = 100;
int absLR;

//int speedOffset = -2; //speed difference between left and right wheers to go straight
int rmaxSpeed = 255;
int lmaxSpeed = 255;
double turnFactor = 0.3;

long aveIR1;  //average value of three IRs when initializing
long aveIR2;
long aveIR3;
int sample = 150;

bool initialState = true;

long left, middle, right;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);

}






void goStraight()
{
  analogWrite(left_motor, lmaxSpeed);   // offset 22
  analogWrite(right_motor, rmaxSpeed);
  digitalWrite(GLED, HIGH);
  digitalWrite(RLED, LOW);
  digitalWrite(BLED, LOW);
}

void turnLeft()
{
  analogWrite(left_motor, lmaxSpeed * turnFactor);
  analogWrite(right_motor, rmaxSpeed);
  digitalWrite(BLED, HIGH);
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, LOW);
  //delay(100);
}

void turnRight()
{
  analogWrite(left_motor, lmaxSpeed);
  analogWrite(right_motor, rmaxSpeed * turnFactor);
  digitalWrite(RLED, HIGH);
  digitalWrite(GLED, LOW);
  digitalWrite(BLED, LOW);
  //delay(100);
}

void stopCar()
{
  digitalWrite(left_motor, LOW);
  digitalWrite(right_motor, LOW);
  digitalWrite(RLED, HIGH);
  digitalWrite(BLED, HIGH);
  digitalWrite(GLED, LOW);



}

// the loop function runs over and over again forever
void loop() {

  left = analogRead(IR1);
  middle = analogRead(IR2);
  right = analogRead(IR3);

  Lreal = left - middle;
  Rreal = right - middle;

  absLR = abs(Lreal - Rreal);



  if (initialState) {
    if (absLR >= 100) {
      if (Lreal < thresh && Rreal > thresh) {
        turnRight();
      }
      else {
        turnLeft();
      }
    }

    else {

      if (left > 800 && right > 800)
      {
        goStraight();
      }

      else if (left < 400 && right < 400)
      {
        stopCar();
        initialState = false;

      }
    }
  }
  else
  {
    stopCar();
  }


  Serial.print(left);
  Serial.print(' ');
  Serial.print(middle);
  Serial.print(' ');
  Serial.print(right);
  Serial.print(' ');

  Serial.println();
  //delay(100);



}
