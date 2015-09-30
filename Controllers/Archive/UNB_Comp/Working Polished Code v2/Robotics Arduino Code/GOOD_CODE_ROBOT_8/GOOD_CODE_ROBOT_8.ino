/*
 * Robotics Motor Controller
*/
 
/*
 *myBase is the only value that will change between robots. Everything else is identical
*/

//Runtime Variables
boolean swapLR = false;
boolean swapFR = false;
int inByte = 0;
int old = 0;
int time = 0;
boolean star = false;

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

const int starPin = 13;


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
  pinMode(starPin, INPUT);

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
  
  //if the mag sensor detected a magnet, bump drive to 100%
  if(starPin == 1){
    star = true;
    time = time + 1;
  }
  
  if(time == 1000){
    star = false;
    time = 0;
  }
  
  //Decode the command and act on it  
  switch (inByte){   
   
    case 211:
      if(!swapLR){
        motor1Forward(225);}
      else{
        motor2Reverse(225);}
        
      delay(10);
      break;
      
    case 212:
      if(!swapLR){
        motor1Forward(180);}
      else{
        motor2Reverse(180);}
      delay(10);  
      break;

    case 213:
      if(!swapLR){
        motor1Forward(135);}
      else{
        motor2Reverse(135);}
      delay(10);
      break;
       
    case 214:
      if(!swapLR){
        motor1Reverse(135);}
      else{
        motor2Forward(135);}
      delay(10);     
      break;

    case 215:
      if(!swapLR){
        motor1Reverse(180);}
      else{
        motor2Forward(180);}
      delay(10);
      break;
     
    case 216:
      if(!swapLR){
        motor1Reverse(225);}
      else{
        motor2Forward(225);}
      delay(10);
      break;
    
    case 217:
      if(!swapLR){
        motor2Reverse(225);}
      else{
        motor1Forward(225);}
      delay(10);
      break;
      
    case 218:
      if(!swapLR){
        motor2Reverse(180);}
      else{
        motor1Forward(180);}
      delay(10); 
      break;

    case 219:
      if(!swapLR){
        motor2Reverse(135);}
      else{
        motor1Forward(135);}
      delay(10); 
      break;
       
    case 220:
      if(!swapLR){
        motor2Forward(135);}
      else{
        motor1Reverse(135);}
      delay(10);
      break;
       
    case 221:
      if(!swapLR){
        motor2Forward(180);}
      else{
        motor1Reverse(180);}
      delay(10);             
      break;
    
    case 222:
      if(inByte != old){
        if(!swapLR){ 
          motor2Forward(225);}
        else{
          motor1Reverse(225);}
        delay(10); 
      }        
      break;
      
    //L1 
    case 223:
      aux1Forward();
      delay(10);
      break;
      
      
    //L2
    case 224:
      aux1Reverse();
      delay(10);
      break;

    //R1
    case 225:
      aux2Forward();
      delay(10);
      break;
    
    //R2
    case 226:
      aux2Reverse();
      delay(10);     
      break;
    
    //Start
    case 227:
      if((inByte != old) && (inByte != 0)){
        swapLR = !swapLR;
        delay(10);
      }
      break;
     
    //Select
    case 228:
      if((inByte != old) && (inByte != 0)){
        swapFR = !swapFR;
        delay(10);
      }
      break;
    
    //D-pad Up
    case 229:
      delay(10);
      break;
    
    //D-pad Right  
    case 230:
      delay(10); 
      break;
    
    //D-pad Down
    case 231:
      delay(10); 
      break;
    
    //D-pad Left
    case 232:
      delay(10);
      break;
    
    //Triangle
    case 233:
      delay(10);             
      break;
    
    //Circle  
    case 234:
      delay(10);             
      break;
    
    //X  
    case 235:
      breakDrive();
      delay(10);
      break;
    
    //Square
    case 236:
      delay (10);
      break;
    
    case 237:
      delay (10);
      break;
      
    case 238:
      delay (10);
      break;
    
    case 239:
      delay(10);
      break;
    
    case 240:
      delay(10);
      break;
      
    default:
      coast();
  } 
}
      
void motor1Forward(int rate)
{
  if(star && (rate == 225)){
    rate = 255;
  }
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
  if(star && (rate == 225)){
    rate = 255;
  }
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
  if(star && (rate == 225)){
    rate = 255;
  }
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
  if(star && (rate == 225)){
    rate = 255;
  }
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
  digitalWrite(auxEnPin,HIGH);
  digitalWrite(auxOnefwPin,HIGH);
  digitalWrite(auxOnerwPin,LOW);
  digitalWrite(auxTwofwPin,LOW);
  digitalWrite(auxTworwPin,LOW);
}

void aux1Reverse(){
  digitalWrite(auxEnPin,HIGH);
  digitalWrite(auxOnefwPin,LOW);
  digitalWrite(auxOnerwPin,HIGH);
  digitalWrite(auxTwofwPin,LOW);
  digitalWrite(auxTworwPin,LOW);
}

void aux2Forward(){
  digitalWrite(auxEnPin,HIGH);
  digitalWrite(auxOnefwPin,LOW);
  digitalWrite(auxOnerwPin,LOW);
  digitalWrite(auxTwofwPin,HIGH);
  digitalWrite(auxTworwPin,LOW);
}

void aux2Reverse(){
  digitalWrite(auxEnPin,HIGH);
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


