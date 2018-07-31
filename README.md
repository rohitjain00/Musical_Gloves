### Musical_Gloves
Arduino project to make music out of gloves.It produces sounds of Do-Re-Me-Sa.
### Prerequisite
Some basic knowlege about using arduino which includes software installation and uploading code.For beginners visit https://www.arduino.cc/en/Guide/HomePage

In the Arduino IDE, navigate to Sketch > Include Library. At the top of the drop-down list, select the option to "Add .ZIP Library''.Select the "Keypad.zip" file.
### Working
  * In this project, I've used Keypad.h header file and used input from gloves as a keypad where four fingers from each hand is a row and the thumb act as a column.
  * For the file with MPU6050 I've used its "roll" value and for each time roll > 20 duration in tone function is increased by value 10ms and for roll < 20 10ms decreases.
### API
* __setMPU__
  * Calibrates the MPU sensor and sets a base for it
* __getSensorData__
  * Recieves the data from the MPU6050 return the rotation of required axis
### Connections
1. Attach any conducting strip to the fingers and thumbs on glove(make sure it is enough thick to touch to thumb's strip)
2. A wire is connected from each conducting strip.
3. The wire from the index, middle, ring, little finger on both gloves are connected as one on pin D3, D4, D5, D6 port respectively on arduino.
4. The wire from the thumb of the left hand is connected to D7 and right hand to D8 port on arduino.
5. If you are using a MPU6050 sensor with the gloves make sure to connect it as specified below 
![MPU6050 connections](http://42bots.com/wp-content/uploads/2014/03/MPU6050-Arduino-Uno-Connections.jpg "Image containing the connections made by MPU6050 on Arduino UNO board")


