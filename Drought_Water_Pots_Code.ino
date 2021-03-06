#include <HX711_ADC.h>
#include <EEPROM.h>

//pins:
const int HX711_dout_1 = 4; 
const int HX711_sck_1 = 5; 
const int HX711_dout_2 = 2; 
const int HX711_sck_2 = 3; 

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

  calibrationValue_1 = -107.6; 
  calibrationValue_2 = -107.6;

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

   
  
    float i;
    
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
  

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
        Serial.print("please enter the pot number: , ");
        
        while(Serial.available() == 0){
        }
        delay(5000);

        
        pot = Serial.readString();
        Serial.print(" ,");
        Serial.print("  Initial weight is: ");
        Serial.print(" , ");
        Serial.print(u);
        Serial.print(" ,");
        }
       if(k<4245){
        
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
     
      u = k;
      }
      
      
      newDataReady = 0;
      t = millis();
    
  }

 

}

}
