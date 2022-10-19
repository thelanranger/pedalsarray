#include <Joystick.h>

// --Input Pins--
const int startButton = 8;            //First Button Pin (Digital)
const int totalButtons = 0;           //Total Buttons
const int startPin = 18;              //First Axis Pin (Analog) A0 == 54 (Uno) A0 = 18 (Leo)
const int numAxis = 3;                //Number of Axis
// --Input Pins--

// --Create the Joystick--
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  totalButtons, 0,                    //Button Count, Hat Switch Count
  true, true, true,                  //X, Y, Z
  false, false, false,                //Rx, Ry, Rz
  false, false,                       //Rudder, Throttle
  false, false, false);                 //Accelerator, Brake, Steering
// --Create the Joystick--
int JoystickIsActive = 2;             // Enabled Global

// --Smoothing--
const int numReadings = 15;           // Number of Reading levels
int readings[numReadings][numAxis];   // the readings from the analog input
int readIndex = 0;                    // the index of the current reading
int axisIndex = 0;
int total[numReadings];               // the running total
int average[numReadings];             // the average
// --Smoothing--

// --Axis Arrays--
int sensorValue[numAxis];             // Sensor Current/Averaged Values
int sMinValue[numAxis];               // Sensor Min Values
int sMaxValue[numAxis];               // Sensor Max Values
int axisValue[numAxis];               // Values pushed to Joystick
// --Axis Arrays--

int currentButtonState = 0;           // Current Button State
int lastButtonState[totalButtons];    // Last state of the buttons
//int AxisSensorValue[4] = {0,0,0,0}; // Joystick Axis Sensors

void setup() {
  Serial.begin(9600);
  Joystick.begin();
  
  // --Initialize Button Pins--
  for (int initPinIndex = startButton; initPinIndex < (startButton+totalButtons); initPinIndex++) {
    if (initPinIndex >= 0 && initPinIndex < 14){
      pinMode(initPinIndex, INPUT_PULLUP);
    }
  }
  // --Initialize Button Pins--
   
  // --Smothing Array Init--
  for (int thatReading = 0; thatReading < numAxis; thatReading++) {
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      readings[thisReading][thatReading] = 0;
      Serial.print("Readings Zero: ");
      Serial.println(readings[thisReading][thatReading]);
  //    delay(1);
    }
  }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    total[thisReading] = 0;
    average[thisReading] = 0;
    Serial.print("Total/Average Zero: ");
    Serial.println(average[thisReading]);
  //  delay(1);
  }
  // --Smothing Array Init--
  
  // --All Axis Init--
  for (int thisReading = 0; thisReading < numAxis; thisReading++) {
    sensorValue[thisReading] = 0;
    //sMinValue[thisReading] = 0;
    sMaxValue[thisReading] = 0;
    axisValue[thisReading] = 0;
    Serial.print("Axis Zero: ");
    Serial.println(axisValue[thisReading]);
  //  delay(1);
  }
  for (int thisReading = 0; thisReading < numAxis; thisReading++) {
    sMinValue[thisReading] = 9999;
    Serial.print("Axis Zero: ");
    Serial.println(axisValue[thisReading]);
  //  delay(1);
  }
  // --All Axis Init--  

  // --Button Init--
  for (int thisReading = 0; thisReading < totalButtons; thisReading++) {
    lastButtonState[thisReading] = 0;
  }
  // --Button Init--
  
}

void loop() {
    // --Calibrate during 20s--- 
    while (millis() < 20000) {   
      Calibrate();
    }
    // --Calibrate during 20s---
    while (millis() > 20001 && millis() < 20100) {
      // --Initialize Joystick Library--  <--Init after Calibrate();
      Joystick.setXAxisRange(-127, 127);
      Joystick.setYAxisRange(-127, 127);
      Joystick.setAcceleratorRange(sMinValue[0], sMaxValue[0]);
      Serial.print("Acceleration Min: ");
      Serial.print(sMinValue[0]);
      Serial.print(" | ");
      Serial.print("Acceleration Max: ");
      Serial.println(sMaxValue[0]);
      Joystick.setBrakeRange(sMinValue[1], sMaxValue[1]);
      Serial.print("Brake Min: ");
      Serial.print(sMinValue[1]);
      Serial.print(" | ");
      Serial.print("Brake Max: ");
      Serial.println(sMaxValue[1]);
      // Joystick.setSteeringRange(0, 300);
      delay(99);
    }
    // --Initialize Joystick Library--  <--Init after Calibrate();
      
    // --Read pin values for buttons--
    for (int Bindex = 0; Bindex < totalButtons; Bindex++){
      currentButtonState = !digitalRead(Bindex + startButton); // pins 8-12 so startPin == 8 + index 0-4
      if (currentButtonState != lastButtonState[Bindex])
      {
         Joystick.setButton(Bindex, currentButtonState);
      }
      lastButtonState[Bindex] = currentButtonState;
      //delay(10);
    }
    // --Read pin values for buttons--

    // Axis Manipulation
    AxisRead();
    for (int axisIndex2 = 0; axisIndex2 < numAxis; axisIndex2++){  
      axisValue[axisIndex2] = sensorValue[axisIndex2];  //map(sensorValue[axisIndex], 0, 1023, sMinValue[axisIndex], sMaxValue[axisIndex]);
      switch (axisIndex2){
        case 0:
        Joystick.setAccelerator(axisValue[axisIndex2]);
        Serial.print("Accelerator Axis Value: ");
        Serial.println(axisValue[axisIndex2]);
        break;
        case 1:
        Joystick.setBrake(axisValue[axisIndex2]);
        Serial.print("Brake Axis Value: ");
        Serial.println(axisValue[axisIndex2]);
        break;
        case 2:
        break;
        case 3:
        break;
      }
      Serial.print("Value ");
      Serial.print(axisIndex2);
      Serial.print(": ");
      Serial.println(axisValue[axisIndex2]);
    }
    // Axis Manipulation
    
    delay(1); //delay in between reads for stability
}

void Calibrate() {
  for (axisIndex = 0; axisIndex < numAxis; axisIndex++){
        sensorValue[axisIndex] = analogRead(startPin + axisIndex);
        Serial.print("Calibrating: "); 
        Serial.println(sensorValue[axisIndex]);
        // record the maximum sensor value
        if (sensorValue[axisIndex] > sMaxValue[axisIndex]) {
           sMaxValue[axisIndex] = sensorValue[axisIndex];
           Serial.print("Max: ");
           Serial.println(sMaxValue[axisIndex]);
        }
        // record the minimum sensor value
        if (sensorValue[axisIndex] < sMinValue[axisIndex]) {
          sMinValue[axisIndex] = sensorValue[axisIndex];
          Serial.print("Min: ");
          Serial.println(sMinValue[axisIndex]);
        }
        delay(1);
  }
}

void AxisRead() {
  // --Once Per Axis--
  for (int axisIndex = 0; axisIndex < numAxis; axisIndex++){
     int arrayPin = startPin + axisIndex;
     total[axisIndex] = total[axisIndex] - readings[readIndex][axisIndex];          // subtract the last reading
     readings[readIndex][axisIndex] = analogRead(arrayPin);                         // read from the sensor
     total[axisIndex] = total[axisIndex] + readings[readIndex][axisIndex];          // add the reading to the total 
  
     // calculate the average:
     average[axisIndex] = total[axisIndex] / numReadings;
     // --Write current average to sensorValue Matrix--
     sensorValue[axisIndex] = average[axisIndex];
     // send it to the computer as ASCII digits
     //Serial.println("Feed: ");
     //Serial.println(average[axisIndex]);
  }  
  // --End AxisIndex
  
  readIndex = readIndex + 1;                       // advance to the next position in the array
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  delay(1);        // delay in between reads for stability
}
