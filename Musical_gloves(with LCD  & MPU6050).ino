#include <LiquidCrystal.h>
#include <Keypad.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

int duration = 100;

const byte ROWS = 4; //four rows (four fingers)
const byte COLS = 2; //four columns (two hands)
int d = 0;
MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer


// orientation/motion vars
Quaternion q;
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

const int rs = 1, en = 0, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int speakerPin = 13; // speaker connected to digital pin 12

char keys[ROWS][COLS] = {
  {'f', 'j'}, //left hand 1st finger and right hand 1st finger
  {'d', 'k'}, //left hand 2nd finger and right hand 2nd finger
  {'s', 'l'}, //left hand 3rd finger and right hand 3rd finger
  {'a', ';'} //left hand 4th finger and right hand 4th finger
};
byte rowPins[ROWS] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Duration :");
  lcd.setCursor(0, 1);
  lcd.print("Frequency: ");
  Serial.begin (9600);


  setMPU();
  pinMode(speakerPin, OUTPUT); // sets the speakerPin to be an output
}

int totalSensorDataGathered = 0;
int freqPlus = 0;

void loop() {

  int pitch = abs(ypr[1]);
  int roll = abs(ypr[2]);

  if ((pitch >= 7 && pitch < 30) && ((126 * pitch) - 782) < 3000) {
    duration = (126 * pitch) - 782;
  }

  if (roll >= 20 && ((45 * roll) - 900) < 4000) {
    freqPlus = (45 * roll) - 900;
  }



  if (pitch < 7) {
    duration = 100;
  } if (roll < 20) {
    freqPlus = 0;
  }

  char key = keypad.getKey();
  getSensorData();

  totalSensorDataGathered++;

    lcd.setCursor(11, 0);
    lcd.print(duration, DEC);
    lcd.setCursor(11, 1);
    lcd.print(freqPlus, DEC);
    totalSensorDataGathered = 0;
  

  if (key != NO_KEY) {
    Serial.println(key);

    if (key == 'a') {
      tone(speakerPin, 2093 + freqPlus, duration); //tone(speakerPin,frequency,duration)
    }
    if (key == 's') {
      tone(speakerPin, 2349 + freqPlus, duration);
    }
    if (key == 'd') {
      tone(speakerPin, 2637 + freqPlus, duration);
    }
    if (key == 'f') {
      tone(speakerPin, 2793 + freqPlus, duration);
    }
    if (key == 'j') {
      tone(speakerPin, 3136 + freqPlus, duration);
    }
    if (key == 'k') {
      tone(speakerPin, 3520 + freqPlus, duration);
    }
    if (key == 'l') {
      tone(speakerPin, 3951 + freqPlus, duration);
    }
    if (key == ';') {
      tone(speakerPin, 4186 + freqPlus, duration);
    }

    //alternative code--just uncomment this and beep function below it
    /*if (key=='a'){ beep(speakerPin,2093,100);}
      if (key=='s'){ beep(speakerPin,2349,100);}
      if (key=='d'){ beep(speakerPin,2637,100);}
      if (key=='f'){ beep(speakerPin,2793,100);}
      if (key=='j'){ beep(speakerPin,3136,100);}
      if (key=='k'){ beep(speakerPin,3520,100);}
      if (key=='l'){ beep(speakerPin,3951,100);}
      if (key==';'){ beep(speakerPin,4186,100);}
      if (key==’i’){ beep(speakerPin,2093,100);}
      if (key==’j’){ beep(speakerPin,2349,100);}
      if (key==’k’){ beep(speakerPin,2637,100);}
      if (key==’l’){ beep(speakerPin,2793,100);}
      if (key==’m’){ beep(speakerPin,3136,100);}
      if (key==’n’){ beep(speakerPin,3520,100);}
      if (key==’o’){ beep(speakerPin,3951,100);}
      if (key==’p’){ beep(speakerPin,4186,100);}*/
  }
}
void setMPU() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void getSensorData() {

  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    // other program behavior stuff here
    // if you are really paranoid you can frequently test in between other
    // stuff to see if mpuInterrupt is true, and if so, "break;" from the
    // while() loop to immediately process the MPU data
  }

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    ypr[0] = ypr[0] * 180 / M_PI;
    ypr[1] = ypr[1] * 180 / M_PI;
    ypr[2] = ypr[2] * 180 / M_PI;
  }
}

