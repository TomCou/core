/*
 * Robotics Motor Controller
*/

//Debug Status
const boolean debug = true;

//Runtime Variables
const int auxRate = 180;
boolean swapLR = false;
boolean swapFR = false;
int inByte = 0;
int old = 0;
int time = 0;

// Drive Motor Pins
const int enaPin =  3; 
const int enbPin =  6; 

const int onefwPin =  2; 
const int onerwPin =  4; 

const int twofwPin =  5; 
const int tworwPin =  7; 

//AUX Motor Pins
const int auxEnPin =  9;

const int auxOnefwPin =  8; 
const int auxOnerwPin =  10; 

const int auxTwofwPin =  11; 
const int auxTworwPin =  12; 

void setup()
{

  pinMode(enaPin, OUTPUT);
  pinMode(enbPin, OUTPUT);
  pinMode(onefwPin, OUTPUT);
  pinMode(onerwPin, OUTPUT);
  pinMode(twofwPin, OUTPUT);
  pinMode(tworwPin, OUTPUT);
  pinMode(auxEnPin, OUTPUT);
  pinMode(auxOnefwPin, OUTPUT);
  pinMode(auxOnerwPin, OUTPUT);
  pinMode(auxTwofwPin, OUTPUT);
  pinMode(auxTworwPin, OUTPUT);

  digitalWrite(enaPin, LOW);
  digitalWrite(enbPin, LOW);
  digitalWrite(onefwPin, LOW);
  digitalWrite(onerwPin, LOW);
  digitalWrite(twofwPin, LOW);
  digitalWrite(tworwPin, LOW);
  digitalWrite(auxEnPin, LOW);
  digitalWrite(auxOnefwPin, LOW);
  digitalWrite(auxOnerwPin, LOW);
  digitalWrite(auxTwofwPin, LOW);
  digitalWrite(auxTworwPin, LOW);
  
  //Set serial to 57600 Baud
  Serial.begin(57600);
}


void loop()                     
{
  //Save previous value
  old = inByte;
  
  //Read input command 0-254
  inByte = Serial.read();
  
  //If debug is enabled, sent the recieved byte back to source
  if((debug) && (inByte>120) && (inByte<151)){
    Serial.write(inByte);
  }
  
  //Decode the command and act on it  
  switch (inByte){   
   
    case 121:
      if(!swapLR){
        motor1Forward(225);}
      else{
        motor2Reverse(225);}
        
      delay(10);
      break;
      
    case 122:
      if(!swapLR){
        motor1Forward(180);}
      else{
        motor2Reverse(180);}
      delay(10);  
      break;

    case 123:
      if(!swapLR){
        motor1Forward(135);}
      else{
        motor2Reverse(135);}
      delay(10);
      break;
       
    case 124:
      if(!swapLR){
        motor1Reverse(135);}
      else{
        motor2Forward(135);}
      delay(10);     
      break;

    case 125:
      if(!swapLR){
        motor1Reverse(180);}
      else{
        motor2Forward(180);}
      delay(10);
      break;
     
    case 126:
      if(!swapLR){
        motor1Reverse(225);}
      else{
        motor2Forward(225);}
      delay(10);
      break;
    
    case 127:
      if(!swapLR){
        motor2Reverse(225);}
      else{
        motor1Forward(225);}
      delay(10);
      break;
      
    case 128:
      if(!swapLR){
        motor2Reverse(180);}
      else{
        motor1Forward(180);}
      delay(10); 
      break;

    case 129:
      if(!swapLR){
        motor2Reverse(135);}
      else{
        motor1Forward(135);}
      delay(10); 
      break;
       
    case 130:
      if(!swapLR){
        motor2Forward(135);}
      else{
        motor1Reverse(135);}
      delay(10);
      break;
       
    case 131:
      if(!swapLR){
        motor2Forward(180);}
      else{
        motor1Reverse(180);}
      delay(10);             
      break;
    
    case 132:
      if(inByte != old){
        if(!swapLR){ 
          motor2Forward(225);}
        else{
          motor1Reverse(225);}
        delay(10); 
      }        
      break;
      
    //L1 
    case 133:
      aux1Forward();
      delay(10);
      break;
      
      
    //L2
    case 134:
      aux1Reverse();
      delay(10);
      break;

    //R1
    case 135:
      aux2Forward();
      delay(10);
      break;
    
    //R2
    case 136:
      aux2Reverse();
      delay(10);     
      break;
    
    //Start
    case 137:
      if((inByte != old) && (inByte != 0)){
        swapLR = !swapLR;
        delay(10);
      }
      break;
     
    //Select
    case 138:
      if((inByte != old) && (inByte != 0)){
        swapFR = !swapFR;
        delay(10);
      }
      break;
    
    //D-pad Up
    case 139:
      delay(10);
      break;
    
    //D-pad Right  
    case 140:
      delay(10); 
      break;
    
    //D-pad Down
    case 141:
      delay(10); 
      break;
    
    //D-pad Left
    case 142:
      delay(10);
      break;
    
    //Triangle
    case 143:
      delay(10);             
      break;
    
    //Circle  
    case 144:
      delay(10);             
      break;
    
    //X  
    case 145:
      breakDrive();
      delay(10);
      break;
    
    //Square
    case 146:
      delay (10);
      break;
    
    case 147:
      delay (10);
      break;
      
    case 148:
      delay (10);
      break;
    
    case 149:
      delay(10);
      break;
    
    case 150:
      delay(10);
      break;
      
    default:
      coast();
  } 
}
      
void motor1Forward(int rate)
{
  analogWrite(enaPin,rate);
  if(!swapFR){
    digitalWrite(onefwPin,HIGH);
    digitalWrite(onerwPin,LOW);
  }
  else{
    digitalWrite(onefwPin,LOW);
    digitalWrite(onerwPin,HIGH);
  }
}

void motor1Reverse(int rate)
{
  analogWrite(enaPin,rate);
  if(!swapFR){
    digitalWrite(onefwPin,LOW);
    digitalWrite(onerwPin,HIGH);
  }
  else{
    digitalWrite(onefwPin,HIGH);
    digitalWrite(onerwPin,LOW);
  }
}

void motor2Forward(int rate)
{
  analogWrite(enbPin,rate);
  if(!swapFR){
    digitalWrite(twofwPin,HIGH);
    digitalWrite(tworwPin,LOW);
  }
  else{
    digitalWrite(twofwPin,LOW);
    digitalWrite(tworwPin,HIGH);
  }
}

void motor2Reverse(int rate)
{
  analogWrite(enbPin,rate);
  if(!swapFR){
    digitalWrite(twofwPin,LOW);
    digitalWrite(tworwPin,HIGH);
  }
  else{
    digitalWrite(twofwPin,HIGH);
    digitalWrite(tworwPin,LOW);
  }
}

void aux1Forward(){
  analogWrite(auxEnPin,auxRate);
  digitalWrite(auxOnefwPin,HIGH);
  digitalWrite(auxOnerwPin,LOW);
  digitalWrite(auxTwofwPin,LOW);
  digitalWrite(auxTworwPin,LOW);
}

void aux1Reverse(){
  analogWrite(auxEnPin,auxRate);
  digitalWrite(auxOnefwPin,LOW);
  digitalWrite(auxOnerwPin,HIGH);
  digitalWrite(auxTwofwPin,LOW);
  digitalWrite(auxTworwPin,LOW);
}

void aux2Forward(){
  analogWrite(auxEnPin,auxRate);
  digitalWrite(auxOnefwPin,LOW);
  digitalWrite(auxOnerwPin,LOW);
  digitalWrite(auxTwofwPin,HIGH);
  digitalWrite(auxTworwPin,LOW);
}

void aux2Reverse(){
  analogWrite(auxEnPin,auxRate);
  digitalWrite(auxOnefwPin,LOW);
  digitalWrite(auxOnerwPin,LOW);
  digitalWrite(auxTwofwPin,LOW);
  digitalWrite(auxTworwPin,HIGH);
}

//Break drive Motors
void breakDrive(){
  digitalWrite(enaPin, HIGH);
  digitalWrite(onefwPin, HIGH);
  digitalWrite(onerwPin, HIGH);
  digitalWrite(enbPin, HIGH);
  digitalWrite(twofwPin, HIGH);
  digitalWrite(tworwPin, HIGH);
}

//Coast all motors
void coast(){
  digitalWrite(enaPin,LOW);
  digitalWrite(enbPin,LOW);
  digitalWrite(onefwPin,LOW);
  digitalWrite(onerwPin,LOW);
  digitalWrite(twofwPin,LOW);
  digitalWrite(tworwPin,LOW);
  digitalWrite(auxEnPin,LOW);
  digitalWrite(auxOnefwPin,LOW);
  digitalWrite(auxOnerwPin,LOW);
  digitalWrite(auxTwofwPin,LOW);
  digitalWrite(auxTworwPin,LOW);
}


