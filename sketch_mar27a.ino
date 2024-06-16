// Define pin numbers for LEDs
const int lane1[3] = {2,3,4};
const int lane2[3] = {5,6,7};
const int lane3[3] = {8,9,10};
const int redLED[3] = {2,5,8};
const int orangeLED[3] = {3,6,9};
const int greenLED[3] = {4,7,10};
int glow[3] = {0,0,0}; //lane light time
int input = 0; //serial input
unsigned long previousMillis = millis();
int emergency[2] = {0,0};//state ,line
int lowTraf = 0;

void setup() {
  // // Initialize the digital pins as outputs for led's
   Serial.begin(9600);
   pinMode(2, OUTPUT);
   pinMode(3, OUTPUT);
   pinMode(4, OUTPUT);
   
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(7, OUTPUT);
   
   pinMode(8, OUTPUT);
   pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);
}

void loop() {
  // taking input from user
  if(Serial.available()>16){
    if (Serial.read() == 0x7E) {
      for (int i=0; i<14; i++) {
        byte discard = Serial.read();
      }
      input = Serial.read() - '0';
      byte discard =Serial.read();
    }

    if(input>=0&&input<=2){
      lowTraf = 0;
      glow[1] = 0;
      glow[2] = 0;
      if(emergency[0]==0||input!=emergency[1]){
        emergencyGreen(input);
        emergency[0]=1;
        emergency[1] = input;
      }
      else if(emergency[1]==input){
        previousMillis = millis() - 1000;
        emergency[0] = 0;
      }
      else{
        Serial.print("invalid input");
      }
    }

    if(input==3){
      
        emergency[0]=0;
      if(lowTraf==0){
        lowTraffic();
        lowTraf = 1;
      }
        
      else{
        lowTraf = 0;
        previousMillis = millis()-1000;
      }
    }
  }

  //no input is given
  if(emergency[0]==1||lowTraf==1)
  previousMillis = millis(); 


  unsigned long currentMillis = millis();
  // normal continuous loop when no input is given 
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    glowLED(glow[0]);
  }
}

void glowLED(int lane){
  if (lane == 0){
    digitalWrite(greenLED[3], LOW);
  }
  else
    digitalWrite(greenLED[lane-1], LOW);
  
  for(int i = 0;i<3;i++){
    if (i!=lane ){
      digitalWrite(redLED[i],HIGH);
      digitalWrite(orangeLED[i],LOW);
      digitalWrite(greenLED[i],LOW);
    }
      
    else
      digitalWrite(redLED[i],LOW);
  }

  switch(glow[1]){
    case 0:
      digitalWrite(orangeLED[lane],HIGH);
      digitalWrite(greenLED[lane],LOW);
      if (glow[2]==1){
        glow[1]=1;
        glow[2]=0;
      }
      else
        glow[2] +=1;
      break;
    case 1:
      digitalWrite(greenLED[lane],HIGH);
      digitalWrite(orangeLED[lane],LOW);
      if (glow[2]==4){
        glow[1]=0;
        glow[2]=0;
        glow[0] = (glow[0]==2) ? 0 : glow[0]+1;
      }
      else
        glow[2] +=1;
      break;
  }
}


void emergencyGreen(int lane){
  for(int i = 0; i<3; i++){
    if (i != lane){
      digitalWrite(redLED[i],HIGH);
    }
    else
    digitalWrite(redLED[i],LOW);
  }
  for(int i = 0; i<3; i++){
    if (i != lane){
      digitalWrite(greenLED[i],LOW);
    }
    else
    digitalWrite(greenLED[i],HIGH);
  }
  for(int i = 0;i<3; i++){
    digitalWrite(orangeLED[i],LOW);
  }
}


void lowTraffic(){
  for(int i = 0; i<3;i++){
    digitalWrite(redLED[i],LOW);
    digitalWrite(orangeLED[i],HIGH);
    digitalWrite(greenLED[i],LOW);
  }
  glow[1] = 0;
  glow[2] = 0;
}