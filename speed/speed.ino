#define left_motor 10
#define right_motor 9
#define BLED 3
#define GLED 2
#define RLED 4
#define lsensor 16
#define rsensor 15
#define rmaxSpeed 255
#define lmaxSpeed 255

int lspeed_wheel = lmaxSpeed;
int rspeed_wheel = rmaxSpeed;

int lspeed, rspeed;
int larr[20];
int rarr[20];
int i = 0;
bool lstop = false;
bool rstop = false;

void setup() {
    Serial.begin(9600);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    pinMode(lsensor, INPUT);
    pinMode(rsensor, INPUT);
}

void slowdown(){
    analogWrite(left_motor, lspeed_wheel);
    analogWrite(right_motor, rspeed_wheel);
    if(!lstop)
        lspeed_wheel-=1;
    if(!rstop)
        rspeed_wheel-=1;
    Serial.print(lspeed_wheel);
    Serial.print(' ');
    Serial.println(rspeed_wheel);
    Serial.print(' ');
}

void check (int* arr, int LorR){
    int first = arr[0];
    int j = 0;
    for(j; j < 20;j++){
        if (abs(arr[j]-first) > 10){
            break;
        }
    }
    if(j == 20){
        if(LorR == 0){
            lstop = true;
        }
        else{
            rstop = true;
        }
    }
}

void recordNewData(){
    lspeed = analogRead(lsensor);
    rspeed = analogRead(rsensor);
    Serial.print(lspeed);
    Serial.print(' ');
    Serial.print(rspeed);
    Serial.print(' ');
    if (i<20){
        larr[i] = lspeed;
        rarr[i] = rspeed;
        i++;
    }
    else{
        for (int j = 0; j < 19; j++){
            larr[j] = larr[j+1];
            rarr[j] = rarr[j+1];
        }
        larr[19] = lspeed;
        rarr[19] = rspeed;
    }
}

/*
void flashled(){
    int tmp = 5;
    while(tmp!=0){
      
        digitalWrite(BLED, HIGH);
        delay(100);
        digitalWrite(BLED, LOW);
        delay(100);
        tmp--;
    }
}*/

void loop(){
    slowdown();
    if (!lstop || !rstop){
        recordNewData();
    }
    if(!lstop){
        check(larr, 0);
    }
    if(!rstop){
        check(rarr, 1);
    }
    Serial.print(' ');
    Serial.print(lstop);
    Serial.print(' ');
    Serial.print(rstop);
    Serial.println();
    //flashled();
    delay(50);
}
