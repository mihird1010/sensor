
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHTTYPE DHT22   // DHT 22
#define DHTPIN 2


DHT dht(DHTPIN, DHTTYPE);


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int sensorValue1;
int digitalValue1;
int sensorValue2;
int digitalValue2;
float voltage2;
float Rs;
float ratio;
float sco2 = -0.4271;
int co2ppm;
float Rl=8.05;
float Ro=76.3;


void setup()   {                
  Serial.begin(9600);
  pinMode( 0, INPUT);
  pinMode( 2, INPUT);
  
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  }


void loop() {  
  
  display.clearDisplay();   // Clear the buffer.

  sensorValue1 = analogRead(0);       // read analog input pin 0
  digitalValue1 = digitalRead(0); 
  Serial.print("Figaro Value: ");
  Serial.print(sensorValue1, DEC);  // prints the value read
  Serial.print(" Gases detected?: ");
  if (digitalValue1==1){  Serial.print("Y"); }
  else {  Serial.print("N"); }
  Serial.print("\t\n");

  sensorValue2 = analogRead(0);       // read analog input pin 0
  digitalValue2 = digitalRead(0); 
  Serial.print("MQ135 Value: ");
  Serial.print(sensorValue2, DEC);  // prints the value read
  Serial.print(" Gases detected?: ");
  if (digitalValue2==1){  Serial.print("Y"); }
  else {  Serial.print("N"); }
  Serial.print("\t\n");

  

  voltage2 = 5.0*(sensorValue2/1024.0);
  Rs = (5*Rl)/voltage2 - Rl;
  ratio = Rs/Ro;
  co2ppm = pow(10,(((log(ratio/0.8))/sco2)+log(110))/2.302);

  Serial.print( "CO2: ");
  Serial.print(co2ppm);
  Serial.print("\t\n");
 
  
  float h = dht.readHumidity();  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) { // Check if any reads failed and exit early (to try again).
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t\n");
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Temp: ");
  display.setCursor(80,0);
  display.println((int)t);
  display.setTextSize(0.5);
  display.setCursor(105,0);
  display.println("o");
  display.setTextSize(2);
  display.setCursor(115,0);
  display.println("C");

  display.setTextSize(2);
  display.setCursor(0,20);
  display.println("Hum(%): ");
  display.setCursor(100,20);
  display.println((int)h);
  display.display();
  delay(2000);
  display.clearDisplay();
  
 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("F2600: ");
  display.setCursor(80,0);
  display.println((int)sensorValue1);
  display.display();
  display.setTextSize(2);
  display.setCursor(0,20);
  display.println("Gas: ");
  display.setCursor(60,20);
  if (digitalValue1==1){  display.println("Y"); }
  else {  display.println("N"); }
  display.display();
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("MQ135: ");
  display.setCursor(80,0);
  display.println((int)sensorValue2);
  display.display();
  display.setTextSize(2);
  display.setCursor(0,20);
  display.println("Gas: ");
  display.setCursor(60,20);
  if (digitalValue2==1){  display.println("Y"); }
  else {  display.println("N"); }
  display.display();
  display.clearDisplay();  
 
}


