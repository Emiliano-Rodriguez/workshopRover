#include <LiquidCrystal.h> //adding LCD library 
#include <Servo.h> //adding Servo library

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo servo; // create servo object to control a servo

int trig = 2; //trig to pin 2
int echo = 26; //echo to pin 26

int leftForward = 3; 
int leftBackward = 5;//H-bridge for motors(forward/backward) set to various pins
int rightForward = 6;
int rightBackward = 9;

int const RED = 12;
int const GREEN = 10; //LED set to pins for each color
int const BLUE = 11;

const int rs = 22, en = 13, d4 = 7, d5 = 24, d6 = 8, d7 = 23;

int left,right,dist; //distance readings for supersonic sensor


void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows:
  servo.attach(4);  // Attaches the servo on pin 4 to the servo object
  servo.write(90); // Sets servo to 90 degrees
  delay(1000); // Gives the servo one second the position itself to 90 degrees
  
  pinMode(trig,OUTPUT); //setting tring as an output
  pinMode(echo,INPUT); //setting echo as an input
  
  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT); //all motors set as outputs 
  pinMode(rightForward , OUTPUT);
  pinMode(rightBackward , OUTPUT);
  
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT); //LED pins set as outputs 
  pinMode(BLUE,OUTPUT);

}

void loop(){
   dist = radar(); //distance is set to supersonic sensor reading (radar function) in CM

   //if distance reading is less than 30 CM go into if condition and execute actions
   if(dist <= 30 && dist >0) {
  
      lcd.clear(); // clears lcd screen 
      lcd.print("Stop!"); //LCD prints out "Stop!"
      setColor(255,0,0); //Red
      analogWrite(leftForward , 0); //stop motors 
      analogWrite(rightForward , 0);

      servo.write(180); // Look left
      delay(1000); //wait 1 second
      left = radar(); //take left side reading
      delay(10);
      lcd.clear(); //clear LCD
      lcd.print("Left Value = "); //LCD prints out "Left Value = (left reading,CM)"
      lcd.print(left);
      delay(1000);
  

      servo.write(10); // Look right
      delay(1000); //wait 1 second
      right = radar(); //take right side reading
      delay(10);
      lcd.clear(); //clear LCD
      lcd.print("Right Value = "); //LCD prints out "Right Value = (right reading,CM)"
      lcd.print(right);
      delay(1000);

      lcd.clear(); //clear LCD
      lcd.print("Backward"); //LCD prints out "Backward"
      setColor(0,0,255);//Blue
      servo.write(90); //look straight
      analogWrite(leftBackward , 255);
      analogWrite(rightBackward , 255); //Set motors to go backward
      delay(1000);
      analogWrite(leftBackward , 0);
      analogWrite(rightBackward , 0); //stop motors
      delay(1000);

      //if right side of room has more room, go there
      if(left < right) {
         lcd.clear();
         lcd.print("Turn Right"); //LCD prints out "Turn Right"
         setColor(255,0,190); //Purple
         analogWrite(leftForward , 255); //turns left motor faster than right one
         analogWrite(rightForward , 100);
         delay(600); 
      }
      else if(left > right) { //if left side of room has more room, go there
         lcd.clear();
         lcd.print("Turn Left"); //LCD prints out "Turn Left"
         setColor(255,0,190); //Purple
         analogWrite(leftForward , 100);
         analogWrite(rightForward , 255);
         delay(600);
      }

   // stop motors
   analogWrite(leftForward , 0);
   analogWrite(rightForward , 0);
   delay(2000);
   } //end of if condition 

  //Executes if sensor reading is greater than 30CM
   lcd.clear(); //clear LCD
   lcd.print("Forward"); //LCD prints out "Forward"
   servo.write(90); //Look straight
   setColor(0,255,0); //Green
   analogWrite(leftForward , 255);
   analogWrite(rightForward , 255); //motors set to full speed ahead
   delay(10); 
}

//setColor function sets LED to certain color values 
void setColor(int red, int green, int blue){
   analogWrite(RED,red);
   analogWrite(GREEN,green);
   analogWrite(BLUE,blue);
}

//radar uses supersonic sensor and returns the distance ahead
int radar()
{
   int distance;
   int duration;  
   digitalWrite(trig,LOW);
   delayMicroseconds(2);
   digitalWrite(trig,HIGH);
   delayMicroseconds(10);
   digitalWrite(trig,LOW);
  
   duration=pulseIn(echo,HIGH);
   distance=duration*0.034/2;
   return distance;
}

