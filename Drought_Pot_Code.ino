//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library for two ore more HX711 modules
// Settling time (number of samples) and data filtering can be adjusted in the config.h file

#include <HX711_ADC.h>
#include <EEPROM.h>

//pins:
const int HX711_dout_1 = 4; //mcu > HX711 no 1 dout pin
const int HX711_sck_1 = 5; //mcu > HX711 no 1 sck pin
const int HX711_dout_2 = 2; //mcu > HX711 no 2 dout pin
const int HX711_sck_2 = 3; //mcu > HX711 no 2 sck pin

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1); //HX711 1
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); //HX711 2

const int calVal_eepromAdress_1 = 0; // eeprom adress for calibration value load cell 1 (4 bytes)
const int calVal_eepromAdress_2 = 4; // eeprom adress for calibration value load cell 2 (4 bytes)
long t;
float k=0;
float u=0;
String pot;
int m =0;
float p=0;
float w=0;
float e=40;

void setup() {
  pinMode(7,OUTPUT);
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  float calibrationValue_1; // calibration value load cell 1
  float calibrationValue_2; // calibration value load cell 2

  calibrationValue_1 = -107.6; // uncomment this if you want to set this value in the sketch
  calibrationValue_2 = -107.6; // uncomment this if you want to set this value in the sketch
#if defined(ESP8266) || defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch the value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress_1, calibrationValue_1); // uncomment this if you want to fetch the value from eeprom
  //EEPROM.get(calVal_eepromAdress_2, calibrationValue_2); // uncomment this if you want to fetch the value from eeprom

  LoadCell_1.begin();
  LoadCell_2.begin();
  long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  while ((loadcell_1_rdy + loadcell_2_rdy) < 2) { //run startup, stabilization and tare, both modules simultaniously
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
  }
  if (LoadCell_1.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.1 wiring and pin designations");
  }
  if (LoadCell_2.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.2 wiring and pin designations");
  }
  LoadCell_1.setCalFactor(calibrationValue_1); // user set calibration value (float)
  LoadCell_2.setCalFactor(calibrationValue_2); // user set calibration value (float)
  Serial.println("Startup is complete");

   // receive command from serial terminal, send 't' to initiate tare operation:
  //Serial.println("Please press t for Tare");
  //while(Serial.available() == 0){
    
 //}
  
  //if (Serial.available() > 0) {
    float i;
    //char inByte = Serial.read();
    //if (inByte == 't') {
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
    //}
  //}

  //check if last tare operation is complete
  if (LoadCell_1.getTareStatus() == true) {
    Serial.println("Tare load cell 1 complete");
  }
  if (LoadCell_2.getTareStatus() == true) {
    Serial.println("Tare load cell 2 complete");
  }

  Serial.println("Keep the 1st Drought Watered pot on the scale to initiate");
  

}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();

  //get smoothed value from data set
  if ((newDataReady)) {
    if (millis() > t + serialPrintInterval) {
      float a = LoadCell_1.getData();
      float b = LoadCell_2.getData();
      k = a+b;
      if( k > 3000){
      if( k < u){
        for(m; m<1 ; m++){
          /*Serial.print("u at begini = ");
          Serial.print(u);
          Serial.print("  k at begin =  ");
          Serial.println(k); */
        //Serial.flush();
        Serial.print("please enter the pot number: , ");
        //Serial.print(" ,  ");
        
        while(Serial.available() == 0){
        }
        delay(5000);

        
        pot = Serial.readString();
        //Serial.print(pot);
        //Serial.print(pot);
        //Serial.print(" ,");
        //Serial.print(" ,");
        //Serial.print(pot);
       // Serial.print(" ,");
        //Serial.print(pot);
        Serial.print(" ,");
        Serial.print("  Initial weight is: ");
        //Serial.print(pot);
        Serial.print(" , ");
        Serial.print(u);
        Serial.print(" ,");
        }
       if(k<4245){
        /*Serial.print("u at mid = ");
          Serial.print(u);
          Serial.print("  k at mid =  ");
          Serial.println(k); */
          digitalWrite(7,HIGH);

          while(p < 4245-e) {
  
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();

  //get smoothed value from data set
  if ((newDataReady)) {
    if (millis() > t + serialPrintInterval) {
      float a = LoadCell_1.getData();
      float b = LoadCell_2.getData();
      p = a+b;
      newDataReady = 0;
      t = millis();
      
    }
  }
          }
          //delay(1200);
          digitalWrite(7,LOW);
        
      }

      else{
        p=u;
      }

      if(p > 4245-e){
        m = 0;
        Serial.print("Watering completed");
        Serial.print(" ,");
        Serial.print(p);
        Serial.print("  ,  ");
        Serial.print("remove the pot and wait......");
        /*Serial.print("testing: ");
        Serial.print("k= ");
        Serial.print(k);
        Serial.print("u= ");
        Serial.print(u);
        Serial.print("p= ");
        Serial.print(p);*/
        w=p;
        k=0;
        u=0;
        p=0;

         while(w > 100) {
  
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();

  //get smoothed value from data set
  if ((newDataReady)) {
    if (millis() > t + serialPrintInterval) {
      float a = LoadCell_1.getData();
      float b = LoadCell_2.getData();
      w = a+b;
      newDataReady = 0;
      t = millis();
      u=0;
      
    }
  }
          }
          
        u=0;
Serial.print("  ,  ");
        Serial.println("keep the next pot");
      }
      }
     // Serial.print("Total weight is: ");
      //Serial.println(k);
      u = k;
      }
      
      /*Serial.print("Load_cell 1 output val: ");
      Serial.print(a);
      Serial.print("    Load_cell 2 output val: ");
      Serial.println(b);*/
      newDataReady = 0;
      t = millis();
    
  }

 /* // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    float i;
    char inByte = Serial.read();
    if (inByte == 't') {
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
    }
  }

  //check if last tare operation is complete
  if (LoadCell_1.getTareStatus() == true) {
    Serial.println("Tare load cell 1 complete");
  }
  if (LoadCell_2.getTareStatus() == true) {
    Serial.println("Tare load cell 2 complete");
  }
  */

}

}
