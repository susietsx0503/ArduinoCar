#define left_motor 10
#define right_motor 9
#define BLED 3
#define GLED 2
#define RLED 4
#define lsensor 16
#define rsensor 15
#define rmaxSpeed 255
#define lmaxSpeed 255

int lspeed = lmaxSpeed;
int rspeed = rmaxSpeed;

int larr[20];
int rarr[20];
int i = 0;
int lspeed, rspeed;
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
    if(!lstop)
        lspeed-=1;
    if(!rstop)
        rspeed-=1;
}

void check (int* arr, int LorR){
    int first = arr[0];
    int j = 0;
    for(j; j < 20;j++){
        if (abs(larr[j]-lfirst) > 10 || abs(rarr[j]-rfirst)>10){
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
    if (i<20){
        larr[i] = lspeed;
        rarr[i] = rspeed;
        i++;
    }
    else{  
        for (int j = 0; j < 19; i++){
            larr[j] = larr[j+1];
            rarr[J] = rarr[j+1];
        }
        larr[20] = lspeed;
        rarr[20] = rspeed;
    }
}

void flashled(){
    if(lstop){
        int tmp = 5;
        while(tmp!=0){
            digitalWrite(BLED, HIGH);
            delay(100);
            digitalWrite(BLED, LOW);
            delay(100);
            tmp--;
        }
    }
    if(rstop){
        int tmp = 5;
        while(tmp!=0){
            digitalWrite(RLED, HIGH);
            delay(100);
            digitalWrite(RLED, LOW);
            delay(100);
            tmp--;
        }
    }
}

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
    flashled();
    delay(10);
}