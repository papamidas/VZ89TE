// CO2-Messgeraet mit VZ89TE UNO, DM1CR, 26.11.20 

#include <MultiFuncShield.h> // https://www.cohesivecomputing.co.uk/hackatronics/arduino-multi-function-shield/
#include "VZ89TE.h"
VZ89TE vz;

const int CO2ALARMVALUE = 1000;
int co2 = 0;
int last_co2 = 0;
int mute = 0;

void setup() {
  // put your setup code here, to run once:
  MFS.initialize();
  MFS.write("CO2");
  MFS.beep(5, 5, 3, 3, 50);
  delay(2000);  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("MICS-VZ-89TE CO2-Sensor");
  Wire.begin();
  vz.begin();
  int ans = vz.retrieveRevision();
  switch (ans) {
    case VZ89TE_OK: Serial.print("Year: ");
                    Serial.print(vz.getYear());
                    Serial.print(" Month: ");
                    Serial.print(vz.getMonth());
                    Serial.print(" Day: ");
                    Serial.print(vz.getDay());
                    Serial.print(" Rev: ");
                    Serial.println(vz.getRev());
                    break;
    case VZ89TE_ERR_NO_RESPONSE : Serial.println("No Response. Endless loop entered");
                                  while(1);
    case VZ89TE_ERR_CRC : Serial.println("CRC error. Endless loop entered");
                                  while(1);
  }
}
 
void loop()
{
  byte btn;

  vz.retrieveStatus();
  Serial.print("VZ89TE CO2: ");
  Serial.print(co2 = vz.getCO2());
  MFS.write(co2);
  Serial.print("ppm, VOC: ");
  Serial.print(vz.getVOC());
  Serial.print("ppb, Res: ");
  Serial.print(vz.getRes());
  Serial.println(" Ohms");

  if (co2 < CO2ALARMVALUE) {
    mute = 1;
  }
  if (last_co2 < CO2ALARMVALUE && co2 >= CO2ALARMVALUE) {
    mute = 0;
  }
  btn = MFS.getButton();
  if (btn == BUTTON_1_PRESSED || btn == BUTTON_2_PRESSED || btn == BUTTON_3_PRESSED) {
    mute = 1;
  }
  if (mute == 0) {
    MFS.beep(20, 0, 1);
  }
  delay(5000);
  last_co2 = co2;
}