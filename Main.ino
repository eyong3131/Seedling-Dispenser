#include <Stepper.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

/******* Pin Declaration *****/

/*********Pin Declaration**************/

const int trigPin1 = 5;  // Trigger pin of ultrasonic sensor 1
const int echoPin1 = 6;  // Echo pin of ultrasonic sensor 1

const int trigPin2 = 8;  // Trigger pin of ultrasonic sensor 2
const int echoPin2 = 9;  // Echo pin of ultrasonic sensor 2

//Servo
const int servo1Pin = 3;
const int servo2Pin = 4;

/****************END*******************/

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

/****************END*******************/


/****************defines variables*******************/
long duration1, distance1;
long duration2, distance2;

// Servo configuration
const int servo1OpenAngle = 360; // Adjust this value to open the container
const int servo1ClosedAngle = -360; // Adjust this value to close the container
const int servo2DispenseAngle = 360; // Adjust this value to dispense the seedlings
const int servoDispenserClose = -360; // roll back to original position

// Define the number of steps per revolution for the stepper motor
const int stepsPerRevolution = 2048;

// Define the motor interface pins
const int motorPin1 = 10;
const int motorPin2 = 11;
const int motorPin3 = 12;
const int motorPin4 = 13;


// Object
Stepper myStepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);
Servo servo1;
Servo servo2;

/****************END*******************/


void setup() {
/*****Module Initial*******/

  //Initialize Trigger
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  // Set the speed of the stepper motor (in RPM)
  myStepper.setSpeed(10); // Adjust this value as needed

  /*****************************************************/
  Serial.begin(9600); // Starts the serial communication

  // Initializze Servo
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.init();        // Initialize the LCD
  lcd.backlight();   // Turn on the backlight

}
void loop() {
  // Measure distance using ultrasonic sensor 1
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // Measure distance using ultrasonic sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;



  // Display distances in the serial monitor
  Serial.print("Distance from sensor 1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print("Distance from sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  delay(500);  // Wait for 1 second before taking the next measurement
  if(distance2 >= 15){
    if(distance1 < 10){
          lcd.clear();
          lcd.print("Processing...");
        
          // Opening the container
          servo1.write(servo1OpenAngle);
          delay(500); // Delay for 1 second

          // Dispensing
          lcd.clear();
          lcd.print("Dispensing...");
          // Rotate the motor continuously in one direction
          myStepper.step(stepsPerRevolution);
          delay(1000); // Delay for 1 second
          
          //servo2.write(servo2DispenseAngle);
          //servo2.write(servoDispenserClose);


          // Close the container
          servo1.write(servo1ClosedAngle);
          delay(500); // Delay for 1 second


          lcd.clear();
          lcd.print("Done");
    }else{
          lcd.clear();
          lcd.print("AVAILABLE");
          delay(500);
        }
  }else{
    lcd.clear();
    lcd.print("Unavailable!!!");
    delay(5000); // 3Hrs delay
  }
  /*
  */
}
