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
float Lreal; //real value of differences read by sensor when the car moves
float Rreal;
int thresh = -200; //threshold value before a car starts to adjust itself

int speedOffset = 1; //speed difference between left and right wheers to go straight
int rmaxSpeed = 250;
int lmaxSpeed = rmaxSpeed*speedOffset;
float turnFactor; //left and right wheel speed difference by the factor
float turnFactor0 = 0.8;

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
    float fractionTmp = Rreal/1500;
    float tmp = fractionTmp < 0 ? turnFactor0+fractionTmp : turnFactor0;
    turnFactor = tmp <= 0 ? 0 : tmp;
    Serial.print("turnLeft: ");
    Serial.print(tmp);
    Serial.print(' ');
    Serial.println(turnFactor);
    analogWrite(left_motor, lmaxSpeed*turnFactor);
    analogWrite(right_motor, rmaxSpeed);
    digitalWrite(BLED, HIGH); 
    digitalWrite(GLED, LOW); 
    digitalWrite(RLED, LOW); 
}

void turnRight()
{
    float fractionTmp = Lreal/1500;
    float tmp = fractionTmp < 0 ? turnFactor0+fractionTmp : turnFactor0;
    turnFactor = tmp <= 0 ? 0 : tmp;
    Serial.print("turnRight: ");
    Serial.print(tmp);
    Serial.print(' ');
    Serial.println(turnFactor);
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

void print3LEDs(){
    Serial.print(left);
    Serial.print(' ');
    Serial.print(middle);
    Serial.print(' ');
    Serial.print(right);
    Serial.println();
}

void printDiff(){
    Serial.print(Lreal);
    Serial.print(' ');
    Serial.print(Rreal);
    Serial.println();
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

    print3LEDs();
    //printDiff();

    delay(70);
}