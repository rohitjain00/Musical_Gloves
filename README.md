# Musical_Gloves
Arduino project to make music out of gloves.It produces sounds of Do-Re-Me-Sa.
# prerequisite
In the Arduino IDE, navigate to Sketch > Include Library. At the top of the drop-down list, select the option to "Add .ZIP Library''.Select the "Keypad.zip" file.
# Working
In this project, I've used Keypad.h header file and used input from gloves as a keypad where four fingers from each hand is a row and the thumb act as a column.
# Connections
1. Attach any conducting strip to the fingers and thumbs on glove(make sure it is enough thick to touch to thumb's strip)
2. A wire is connected from each conducting strip.
3. The wire from the index, middle, ring, little finger on both gloves are connected as one on pin D2, D3, D4, D5 port respectively on arduino.
4. The wire from the thumb of the left hand is connected to D6 and right hand to D7 port on arduino.


