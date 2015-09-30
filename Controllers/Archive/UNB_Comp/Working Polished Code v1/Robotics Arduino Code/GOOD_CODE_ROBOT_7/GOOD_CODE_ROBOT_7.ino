/*
 * Robotics Motor Controller
 */
 
 /*
  * Controller buttons are mapped to the following keys
  *   using xpadder software.
  *
  * Motor 1 Forward = 1 = dpad up
  * Motor 1 Reverse = 2 = dpad down
  * Motor 2 Forward = 3 = triangle
  * Motor 2 Reverse = 4 = X
  */
 
 
int inByte =0;
int old = 0;

// Drive Motor Pins
const int enaPin =  3; 
const int enbPin =  6; 

const int onefwPin =  2; 
const int onerwPin =  4; 

const int twofwPin =  5; 
const int tworwPin =  7; 

// AUX Motor Pins
const int auxEnaPin =  9; 
const int auxEnbPin =  9; 

const int auxOnefwPin =  8; 
const int auxOnerwPin =  10; 

const int auxTwofwPin =  11; 
const int auxTworwPin =  12; 

const int solenoidPin =  13; 


void setup()                    // run once, when the sketch starts
{

  pinMode(enaPin, OUTPUT);
  pinMode(enbPin, OUTPUT);
  pinMode(onefwPin, OUTPUT);
  pinMode(onerwPin, OUTPUT);
  pinMode(twofwPin, OUTPUT);
  pinMode(tworwPin, OUTPUT);
  pinMode(auxEnaPin, OUTPUT);
  pinMode(auxEnbPin, OUTPUT);
  pinMode(auxOnefwPin, OUTPUT);
  pinMode(auxOnerwPin, OUTPUT);
  pinMode(auxTwofwPin, OUTPUT);
  pinMode(auxTworwPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);

  digitalWrite(enaPin, LOW);
  digitalWrite(enbPin, LOW);
  digitalWrite(onefwPin, LOW);
  digitalWrite(onerwPin, LOW);
  digitalWrite(twofwPin, LOW);
  digitalWrite(tworwPin, LOW);
  digitalWrite(auxEnaPin, LOW);
  digitalWrite(auxEnbPin, LOW);
  digitalWrite(auxOnefwPin, LOW);
  digitalWrite(auxOnerwPin, LOW);
  digitalWrite(auxTwofwPin, LOW);
  digitalWrite(auxTworwPin, LOW);
  digitalWrite(solenoidPin, LOW);
  
  
  Serial.begin(57600);           // set up Serial library at 9600 bps
  
  //Serial.println("Connected to robot!");  // Connected!!

}

/* Motor Controls:
 * Motor 1 Forward: enaPin: High 3
 *                  onefwPin: High 2
 *                  onerwPin: Low 4
 *
 * Motor 1 Reverse: enaPin: High 3
 *                  onefwPin: Low 2
 *                  onerwPin: High 4
 *
 * Motor 2 Forward: enbPin: High 6
 *                  twofwPin: High 20
 *                  tworwPin: Low 7
 * 
 * Motor 2 Reverse: enbPin: High 6
 *                  onefwPin: Low  
 *                  onerwPin: High 7
 */

void loop()                       // run over and over again
{
  inByte = Serial.read();
  
  switch (inByte){   
      
      
 // Movement Controls   
    case 120:    
      //Serial.println("Robot 1: Forward");
      digitalWrite(enaPin,HIGH);
      digitalWrite(enbPin,HIGH);
      digitalWrite(onefwPin,HIGH);
      digitalWrite(onerwPin,LOW);
      digitalWrite(twofwPin,HIGH);
      digitalWrite(tworwPin,LOW);
      delay(10); break;
      
    case 121:
       //Serial.println("Robot 1: Reverse");
      digitalWrite(enaPin,HIGH);
      digitalWrite(enbPin,HIGH);
      digitalWrite(onefwPin,LOW);
      digitalWrite(onerwPin,HIGH);
      digitalWrite(twofwPin,LOW);
      digitalWrite(tworwPin,HIGH);
      delay(10);  break;
      
    case 122:
      //Serial.println("Robot 1: Spin Counterclockwise");
      digitalWrite(enaPin,HIGH);
      digitalWrite(enbPin,HIGH);
      digitalWrite(onefwPin,HIGH);
      digitalWrite(onerwPin,LOW);
      digitalWrite(twofwPin,LOW);
      digitalWrite(tworwPin,HIGH);
       delay(10);              break;
       
    case 123:
      //Serial.println("Robot 1: Spin Clockwise");
      digitalWrite(enaPin,HIGH);
      digitalWrite(enbPin,HIGH);
      digitalWrite(onefwPin,LOW);
      digitalWrite(onerwPin,HIGH);
      digitalWrite(twofwPin,HIGH);
      digitalWrite(tworwPin,LOW);
      delay(10);       break;




// AUX Controls
    case 124:
      //Serial.println("Robot 1: AUX1 Clockwise");
      digitalWrite(auxEnaPin,HIGH);
      digitalWrite(auxEnbPin,LOW);
      digitalWrite(auxOnefwPin,HIGH);
      digitalWrite(auxOnerwPin,LOW);
      digitalWrite(auxTwofwPin,LOW);
      digitalWrite(auxTworwPin,LOW);
       delay(10);             break;
     
    case 125:
      //Serial.println("Robot 1: AUX1 Counterclockwise");
      digitalWrite(auxEnaPin,HIGH);
      digitalWrite(auxEnbPin,LOW);
      digitalWrite(auxOnefwPin,LOW);
      digitalWrite(auxOnerwPin,HIGH);
      digitalWrite(auxTwofwPin,LOW);
      digitalWrite(auxTworwPin,LOW);
        delay(10);             break;
    
    case 126:
      //Serial.println("Robot 1: AUX2 Clockwise");
      digitalWrite(auxEnaPin,LOW);
      digitalWrite(auxEnbPin,HIGH);
      digitalWrite(auxOnefwPin,LOW);
      digitalWrite(auxOnerwPin,LOW);
      digitalWrite(auxTwofwPin,HIGH);
      digitalWrite(auxTworwPin,LOW);
      delay(10);       break;
      
    case 127:
      //Serial.println("Robot 1: AUX2 Counterclockwise");
      digitalWrite(auxEnaPin,LOW);
      digitalWrite(auxEnbPin,HIGH);
      digitalWrite(auxOnefwPin,LOW);
      digitalWrite(auxOnerwPin,LOW);
      digitalWrite(auxTwofwPin,LOW);
      digitalWrite(auxTworwPin,HIGH);
      delay(10);       break;
      
        //Advanced Movement Controls
      case 128:
      //Serial.println("Command Received");
      digitalWrite(enaPin,HIGH);
      digitalWrite(enbPin,LOW);
      digitalWrite(onefwPin,HIGH);
      digitalWrite(onerwPin,LOW);
      digitalWrite(twofwPin,LOW);
      digitalWrite(tworwPin,LOW);
       delay(10);             break;
       
      case 129:
       //Serial.println("Command Received");
      digitalWrite(enaPin,HIGH);
      digitalWrite(enbPin,LOW);
      digitalWrite(onefwPin,LOW);
      digitalWrite(onerwPin,HIGH);
      digitalWrite(twofwPin,LOW);
      digitalWrite(tworwPin,LOW);
       delay(10);             break;
       
      case 130:
       //Serial.println("Command Received");
      digitalWrite(enaPin,LOW);
      digitalWrite(enbPin,HIGH);
      digitalWrite(onefwPin,LOW);
      digitalWrite(onerwPin,LOW);
      digitalWrite(twofwPin,HIGH);
      digitalWrite(tworwPin,LOW);
       delay(10);             break;
       
      case 131:
        //Serial.println("Command Received");
      digitalWrite(enaPin,LOW);
      digitalWrite(enbPin,HIGH);
      digitalWrite(onefwPin,LOW);
      digitalWrite(onerwPin,LOW);
      digitalWrite(twofwPin,LOW);
      digitalWrite(tworwPin,HIGH);
       delay(10);             break;
       
       case 132:
        //Serial.println("Command Received");
       digitalWrite(solenoidPin, HIGH);
       delay(250);             
       digitalWrite(solenoidPin, LOW);
       break;
      
      //  // Coast all motors
    default:
      ////Serial.println("Robot 1: Coasting");
      digitalWrite(enaPin,LOW);
      digitalWrite(enbPin,LOW);
      digitalWrite(onefwPin,LOW);
      digitalWrite(onerwPin,LOW);
      digitalWrite(twofwPin,LOW);
      digitalWrite(tworwPin,LOW);
      digitalWrite(auxEnaPin,LOW);
      digitalWrite(auxEnbPin,LOW);
      digitalWrite(auxOnefwPin,LOW);
      digitalWrite(auxOnerwPin,LOW);
      digitalWrite(auxTwofwPin,LOW);
      digitalWrite(auxTworwPin,LOW);
      digitalWrite(solenoidPin, LOW);
  }
  
   
// Debug Output - Echo commands received   
//   if (inByte != -1){
//      old = inByte;
//      Serial.println(inByte);
//   }
    

 }


