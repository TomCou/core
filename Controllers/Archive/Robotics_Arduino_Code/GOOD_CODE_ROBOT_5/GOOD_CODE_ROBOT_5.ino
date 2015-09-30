/*
 * Robotics Motor Controller
*/

const int myBase = 121;

//Debug Status
const boolean debug = false;

//Runtime Variables
const int auxRate = 170;
const int Volts26 = 254;
const int Volts17 = 170;
const int Volts14 = 135;
const int Volts11 = 100;
const int Volts8 = 75;


boolean swapLR = false;
boolean swapFR = false;
int inByte = 0;
int old = 0;
int otherRobot = 0;
int noInput = 0;
int noD1 = 0;
int noD2 = 0;
int noA1 = 0;
int noA2 = 0;
int noBrake = 0;


// Drive Motor Pins
const int D1FwPin =  2;
const int D1EnPin =  3; 
const int D1RvPin =  4;

const int D2FwPin =  5;
const int D2EnPin =  6; 
const int D2RvPin =  7;

//AUX Motor Pins
const int A1FwPin =  8;
const int A1EnPin =  9;
const int A1RvPin =  10;

const int A2FwPin =  12;
const int A2EnPin = 11;
const int A2RvPin =  13;

const int D1UpHigh = myBase;
const int D1UpMed = myBase + 1;
const int D1UpLow = myBase + 2;
const int D1DnLow = myBase + 3;
const int D1DnMed = myBase + 4;
const int D1DnHigh = myBase + 5;
const int D2UpHigh = myBase + 6;
const int D2UpMed = myBase + 7;
const int D2UpLow = myBase + 8;
const int D2DnLow = myBase + 9;
const int D2DnMed = myBase + 10;
const int D2DnHigh = myBase + 11;
const int L1 = myBase + 12;
const int L2 = myBase + 13;
const int R1 = myBase + 14;
const int R2 = myBase + 15;
const int Start = myBase + 16;
const int Select = myBase + 17;
const int DpadUp = myBase + 18;
const int DpadRight = myBase + 19;
const int DpadDown = myBase + 20;
const int DpadLeft = myBase + 21;
const int Triangle = myBase + 22;
const int Circle = myBase + 23;
const int X = myBase + 24;
const int Square = myBase + 25;


void setup()
{

  pinMode(D1EnPin, OUTPUT);
  pinMode(D2EnPin, OUTPUT);
  pinMode(D1FwPin, OUTPUT);
  pinMode(D1RvPin, OUTPUT);
  pinMode(D2FwPin, OUTPUT);
  pinMode(D2RvPin, OUTPUT);
  pinMode(A1EnPin, OUTPUT);
  pinMode(A2EnPin, OUTPUT);
  pinMode(A1FwPin, OUTPUT);
  pinMode(A1RvPin, OUTPUT);
  pinMode(A2FwPin, OUTPUT);
  pinMode(A2RvPin, OUTPUT);

  digitalWrite(D1EnPin, LOW);
  digitalWrite(D2EnPin, LOW);
  digitalWrite(D1FwPin, LOW);
  digitalWrite(D1RvPin, LOW);
  digitalWrite(D2FwPin, LOW);
  digitalWrite(D2RvPin, LOW);
  digitalWrite(A1EnPin, LOW);
  digitalWrite(A2EnPin, LOW);
  digitalWrite(A1FwPin, LOW);
  digitalWrite(A1RvPin, LOW);
  digitalWrite(A2FwPin, LOW);
  digitalWrite(A2RvPin, LOW);
  
  //Set serial to 57600 Baud
  Serial.begin(57600);
}


void loop()                     
{
  
  //Read input command 0-254, -1 for no input avaliable
  inByte = Serial.read();
  
  if((otherRobot + noInput) < 22500){      
    if(inByte != -1){
      if((inByte > (myBase -1)) && (inByte < (myBase + 30))){
        
        //valid input for this robot recieved, resetting counters since last command recieved
        otherRobot = 0;
        noInput = 0;
        
        //save the old value for comparison
        old = inByte;
        
        //If debug is enabled, sent the recieved byte back to source
        if(debug){
          Serial.write(inByte);
        }
        
        //check to see how long it's been since a command for D1 has been recieved, reset the counter to zero if command for D1 is recieved
        if((inByte < D1UpHigh) || (inByte > D1DnHigh)){
          noD1 = noD1 + 1;
          if(noD1 > 10){
            noD1 = 0;
            coastD1();
          }
        }
        else{
          noD1 = 0;
        }
        
        //check to see how long it's been since a command for D2 has been recieved, reset the counter to zero if command for D2 is recieved
        if((inByte < D2UpHigh) || (inByte > D2DnHigh)){
          noD2 = noD2 + 1;
          if(noD2 > 10){
            noD2 = 0;
            coastD2();
          }
        }
        else{
          noD2 = 0;
        }
        
        //check to see how long it's been since a command for A1 has been recieved, reset the counter to zero if command for A1 is recieved
        if((inByte < L1) || (inByte > L2)){
          noA1 = noA1 + 1;
          if(noA1 > 10){
            noA1 = 0;
            coastA1();
          }
        }
        else{
          noA1 = 0;
        }
        
        //check to see how long it's been since a command for A2 has been recieved, reset the counter to zero if command for A2 is recieved
        if((inByte < R1) || (inByte > R2)){
          noA2 = noA2 + 1;
          if(noA2 > 10){
            noA2 = 0;
            coastA2();
          }
        }
        else{
          noA2 = 0;
        }

        //Decode the command and act on it  
        switch (inByte){   
         
          case D1UpHigh:
            if(!swapLR){
              D1Forward(Volts14);}
            else{
              D2Reverse(Volts14);}
            delay(10);
            break;
            
          case D1UpMed:
            if(!swapLR){
              D1Forward(Volts11);}
            else{
              D2Reverse(Volts11);}
            delay(10);  
            break;
      
          case D1UpLow:
            if(!swapLR){
              D1Forward(Volts8);}
            else{
              D2Reverse(Volts8);}
            delay(10);
            break;
             
          case D1DnLow:
            if(!swapLR){
              D1Reverse(Volts8);}
            else{
              D2Forward(Volts8);}
            delay(10);     
            break;
      
          case D1DnMed:
            if(!swapLR){
              D1Reverse(Volts11);}
            else{
              D2Forward(Volts11);}
            delay(10);
            break;
           
          case D1DnHigh:
            if(!swapLR){
              D1Reverse(Volts14);}
            else{
              D2Forward(Volts14);}
            delay(10);
            break;
          
          case D2UpHigh:
            if(!swapLR){
              D2Reverse(Volts14);}
            else{
              D1Forward(Volts14);}
            delay(10);
            break;
            
          case D2UpMed:
            if(!swapLR){
              D2Reverse(Volts11);}
            else{
              D1Forward(Volts11);}
            delay(10); 
            break;
      
          case D2UpLow:
            if(!swapLR){
              D2Reverse(Volts8);}
            else{
              D1Forward(Volts8);}
            delay(10); 
            break;
             
          case D2DnLow:
            if(!swapLR){
              D2Forward(Volts8);}
            else{
              D1Reverse(Volts8);}
            delay(10);
            break;
             
          case D2DnMed:
            if(!swapLR){
              D2Forward(Volts11);}
            else{
              D1Reverse(Volts11);}
            delay(10);             
            break;
          
          case D2DnHigh:
            if(!swapLR){ 
              D2Forward(Volts14);}
            else{
              D1Reverse(Volts14);}
            delay(10);       
            break;
            
          //L1 
          case L1:
            A1Forward();
            delay(10);
            break;
            
            
          //L2
          case L2:
            A1Reverse();
            delay(10);
            break;
           
          //R1
          case R1:
            A2Forward();
            delay(10);
            break;
          
          //R2
          case R2:
            A2Reverse();
            delay(10);     
            break;
          
          //Start
          case Start:
            if(old != Start){
              swapLR = !swapLR;
              delay(10);
            }
            break;
           
          //Select
          case Select:
            if(old != Select){
              swapFR = !swapFR;
              delay(10);
            }
            break;
          
          //D-pad Up
          case DpadUp:
            delay(10);
            break;
          
          //D-pad Right  
          case DpadRight:
            delay(10); 
            break;
          
          //D-pad Down
          case DpadDown:
            delay(10); 
            break;
          
          //D-pad Left
          case DpadLeft:
            delay(10);
            break;
          
          //Triangle
          case Triangle:
            delay(10);             
            break;
          
          //Circle  
          case Circle:
            delay(10);             
            break;
          
          //X  
          case X:
            breakDrive();
            delay(10);
            break;
          
          //Square
          case Square:
            delay(10);
            break;
          
          case 27:
            delay(10);
            break;
            
          case 28:
            delay(10);
            break;
          
          case 29:
            delay(10);
            break;
          
          case 30:
            delay(10);
            break;
        }  
      }
      else{
        otherRobot = otherRobot + 1;
      }
    }
    else{
      noInput = noInput + 1;
    }
  }
  else{
    noInput = 0;
    otherRobot = 0;
    coastAll();
    delay(10);
  }
}
      
void D1Forward(int rate)
{
  analogWrite(D1EnPin,rate);
  if(!swapFR){
    digitalWrite(D1FwPin,HIGH);
    digitalWrite(D1RvPin,LOW);
  }
  else{
    digitalWrite(D1FwPin,LOW);
    digitalWrite(D1RvPin,HIGH);
  }
}

void D1Reverse(int rate)
{
  analogWrite(D1EnPin,rate);
  if(!swapFR){
    digitalWrite(D1FwPin,LOW);
    digitalWrite(D1RvPin,HIGH);
  }
  else{
    digitalWrite(D1FwPin,HIGH);
    digitalWrite(D1RvPin,LOW);
  }
}

void D2Forward(int rate)
{
  analogWrite(D2EnPin,rate);
  if(!swapFR){
    digitalWrite(D2FwPin,HIGH);
    digitalWrite(D2RvPin,LOW);
  }
  else{
    digitalWrite(D2FwPin,LOW);
    digitalWrite(D2RvPin,HIGH);
  }
}

void D2Reverse(int rate)
{
  analogWrite(D2EnPin,rate);
  if(!swapFR){
    digitalWrite(D2FwPin,LOW);
    digitalWrite(D2RvPin,HIGH);
  }
  else{
    digitalWrite(D2FwPin,HIGH);
    digitalWrite(D2RvPin,LOW);
  }
}

void A1Forward(){
  analogWrite(A1EnPin,Volts11);
  digitalWrite(A1FwPin,HIGH);
  digitalWrite(A1RvPin,LOW);
}

void A1Reverse(){
  analogWrite(A1EnPin,Volts11);
  digitalWrite(A1FwPin,LOW);
  digitalWrite(A1RvPin,HIGH);
}

void A2Forward(){
  analogWrite(A2EnPin,Volts11);
  digitalWrite(A2FwPin,HIGH);
  digitalWrite(A2RvPin,LOW);
}

void A2Reverse(){
  analogWrite(A2EnPin,Volts11);
  digitalWrite(A2FwPin,LOW);
  digitalWrite(A2RvPin,HIGH);
}

//Break drive Motors
void breakDrive(){
  digitalWrite(D1EnPin, HIGH);
  digitalWrite(D1FwPin, HIGH);
  digitalWrite(D1RvPin, HIGH);
  digitalWrite(D2EnPin, HIGH);
  digitalWrite(D2FwPin, HIGH);
  digitalWrite(D2RvPin, HIGH);
}

//Coast all motors
void coastAll(){
  coastD1();
  coastD2();
  coastA1();
  coastA2();
}

//Coast Drive Motor 1
void coastD1(){
  digitalWrite(D1EnPin, LOW);
  digitalWrite(D1FwPin, LOW);
  digitalWrite(D1RvPin, LOW);
}

//Coast Drive Motor 2 
void coastD2(){
  digitalWrite(D2EnPin, LOW);
  digitalWrite(D2FwPin, LOW);
  digitalWrite(D2RvPin, LOW);
}

//Coast Aux Motor 1 
void coastA1(){
  digitalWrite(A1EnPin, LOW);
  digitalWrite(A1FwPin, LOW);
  digitalWrite(A1RvPin, LOW);
}

//Coast Aux Motor 2 
void coastA2(){
  digitalWrite(A2EnPin, LOW);
  digitalWrite(A2FwPin, LOW);
  digitalWrite(A2RvPin, LOW);
}
