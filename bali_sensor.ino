
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <GSM.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define ONE_WIRE_BUS 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


MPU6050 mpu(Wire);
unsigned long timer = 0;
int TurbsensorPin=A0;
float volt;
float ntu;
float temp;
float Celcius=0;
float Fahrenheit=0;

void setup() {
  Serial.begin(115200);                               
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))   
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);                                      // Don't proceed, loop forever
  }   
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);            // Draw white text
  display.clearDisplay();                         
  Wire.begin();
  mpu.begin();
  display.println(F("Calculating gyro offset, do not move MPU6050"));
  display.display();        
  mpu.calcGyroOffsets();                          // This does the calibration
  display.setTextSize(1);     
  sensors.begin();     
}

void loop() {

  sensors.requestTemperatures(); 
  mpu.update();  
  if((millis()-timer)>10)                         // print data every 10ms
  {                                           
    volt = 0;
    for(int i=0; i<800; i++)
    {
        volt += ((float)analogRead(TurbsensorPin)/1023)*5;
    }
    volt = volt/800;
    
    if(volt < 2.5)
    {
      ntu = 3000;
    }
    else
    {
      ntu = -1120.4*square(volt)+5742.3*volt-4353.8;
    }
    Celcius=sensors.getTempCByIndex(0);
    Fahrenheit=sensors.getTempFByIndex(0);
      
      display.clearDisplay();                       // clear screen
      display.setCursor(0,0);                         
      display.print("P : ");
      display.println(mpu.getAngleX());
      display.print("R : ");
      display.println(mpu.getAngleY());
      display.print("Y : ");
      display.println(mpu.getAngleZ());
      display.print("NTU : ");
      display.println(ntu);
      display.print("Temp in C : ");
      display.println(Celcius);
      display.print("Temp in F : ");
      display.println(Fahrenheit);

      display.display();                            // display data
      timer = millis();  
  }
}
