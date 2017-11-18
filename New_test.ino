#define left_motor 10
#define right_motor 9
#define IR1 19
#define IR2 18
#define IR3 17

int BLED = 3; //define LED
int GLED = 2;
int RLED = 4;

boolean stopConst = false;
int stopSigIntv = -1;

int Linitial; //initial value of differences read by sensor
int Rinitial;
int Lreal; //real value of differences read by sensor when the car moves
int Rreal;
int thresh = -150; //threshold value before a car starts to adjust itself

int speedOffset = 1; //speed difference between left and right wheers to go straight
int rmaxSpeed = 250;
int lmaxSpeed = rmaxSpeed*speedOffset;
float turnFactor; //left and right wheel speed difference by the factor
float turnFactor0 = 0.85;

int left, middle, right;

void setup()
{
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
    float tmp = turnFactor0+Lreal/900;
    turnFactor = tmp <= 0 ? 0 : tmp;
    analogWrite(left_motor, lmaxSpeed*turnFactor);
    analogWrite(right_motor, rmaxSpeed);
    digitalWrite(BLED, HIGH); 
    digitalWrite(GLED, LOW); 
    digitalWrite(RLED, LOW); 
}

void turnRight()
{
    float tmp = turnFactor0+Rreal/900;
    turnFactor = tmp <= 0 ? 0 : tmp;
    analogWrite(left_motor, lmaxSpeed);
    analogWrite(right_motor, rmaxSpeed*turnFactor);
    digitalWrite(RLED, HIGH); 
    digitalWrite(GLED, LOW); 
    digitalWrite(BLED, LOW); 
}

void stop()
{
    digitalWrite(left_motor, LOW);                     
    digitalWrite(right_motor, LOW); 
    digitalWrite(RLED, HIGH);
    digitalWrite(BLED, HIGH);
    digitalWrite(GLED, LOW);
}

void checkStopPoint()
{
    if(stopSigIntv == -1){ //first encounter a black cross strip
        stopSigIntv = 0;
    }
    else if (stopSigIntv == 100){
        stopSigIntv = 0;
    }
    else if(stopSigIntv >= 200){ //check if cross the white space between two black strips
        stopConst = true;
    }
}

void incStopIntv(){
    if (stopSigIntv != -1){ //a black cross strip has been encountered
        stopSigIntv += 100;
        if (stopSigIntv >= 1000){ //beeeeen encountered long before, won't count
            stopConst = -1;
        }
    }
}

// the loop function runs over and over again forever
void loop() 
{
    left = analogRead(IR1);
    middle = analogRead(IR2);
    right = analogRead(IR3);

    Lreal = left - middle;
    Rreal = right - middle;

    incStopIntv();
    if (stopConst){
        stop();
    }
    else if(Lreal < thresh && Rreal > thresh){
        turnRight();
    }
    else if (Lreal > thresh && Rreal < thresh){
        turnLeft();
    }
    else if (Lreal > thresh && Rreal > thresh){
        goStraight();
    }
    else{ //both below thresh
        stop();
        checkStopPoint();
    }

    // Serial.print(Lreal - Rreal);
    // Serial.print(' ');
    // Serial.print(Rreal);
    // Serial.println();
    Serial.print(left);
    Serial.print(' ');
    Serial.print(middle);
    Serial.print(' ');
    Serial.print(right);
    Serial.println();

    delay(100);
}