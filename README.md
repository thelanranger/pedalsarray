##ArduioJoystickLibrary-master.zip required for compile. 

Sample code for use of Analog pedals via Arduino Leonardo. 
Key Variables:
startButton:  pin number of the first button used in the joystick/gamepad array.
totalButtons: total number of buttons on the joystick/gamepad.
startPin: First pin used for the first analog axis on the joystick/gamepad (pot 1 pin).
numAxis: Total number of analog axis (pots) that the joystick/gamepad will have. 

Create joystick def: 
IMPORTANT: Adjust True/False respective to numAxis to match what pot you would like each axis to represent!

Smoothing:
numReadings: Adjusts the 'fuzz factor' of reading the pot to keep readings from jumping around. Default 15.

Calibration: 
Default time 20s startup
During startup, move all pots to extreme limits. This will capture the absolute min and max of the pot and record it for use during that power on cycle. To recalibrate, simply powercycle/reset the device and move pots to their extremes during the first 20s. 
