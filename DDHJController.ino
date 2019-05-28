#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Encoder.h>

Adafruit_MCP4725 dac;

Encoder myEnc(22, 23);

float g = 5114.0; // Counts per revolution
float p;  // Encoder position
float x;  // Controll variable
int vR; // Voltage to right motor
int vL; // Voltage to left motor
int vP; // Potentiometer voltage
float rP;  // Potentiometer resistance
float E; // Error for controll
float sum; // For integral controll

void setup() {
  pinMode(A0,INPUT);
  analogReadResolution(12);
  Serial.begin(9600);

  dac.begin(0x62);
  dac.setVoltage(4096/2,false);
  
  dac.begin(0x63);
  dac.setVoltage(4096/2,false);
}

void loop() {

  //Getting Position//
  long pCounts = myEnc.read();
  p = (pCounts/g);

  //Getting Potentiometer Reading//
  vP = analogRead(A0);
  rP = (4000.0-vP)/vP;
  
  
  //Error//
  E = rP-p;
  sum = sum+E;
  Serial.println(E);
  
  //Calculating Control Voltage//
  x = 5.0*E;

  if (x > 1) {
    x = 1.0;
  }
  
  if (x < -1) {
    x = -1.0;
  }
  
  //Setting Motor Speeds//
  vR = 4000*(1/2.0+1/2.0*x);
  vL = 4000*(1/2.0-1/2.0*x);
  
  dac.begin(0x62);
  dac.setVoltage(vR,false);
  
  dac.begin(0x63);
  dac.setVoltage(vL,false);

}
