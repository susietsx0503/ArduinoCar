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
bool isstop = false;

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
    lspeed-=1;
    rspeed-=1;
}

void loop(){
    if(!isstop){
        slowdown();
    }
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
        int lfirst = larr[0];
        int rfirst = rarr[0];
        int j = 0;
        for(j; j < 20;j++){
            if (abs(larr[j]-lfirst) > 10 || abs(rarr[j]-rfirst)>10){
                break;
            }
        }
        if(j == 20){
            isstop = true;
        }
    }
    delay(10);

}