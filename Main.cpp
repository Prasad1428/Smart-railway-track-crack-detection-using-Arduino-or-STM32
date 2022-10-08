//This code is to implement Smart Railway track crack detection robot using Arduino UNO/STM32-Nucleo-F401RE board using Arduino IDE.


#include <SoftwareSerial.h>
#include <TinyGPS++.h>
int in1=5; 
int in2=6; 
int in3=13; 
int in4=12; 
int sensor=2;  
int value=0;
unsigned long start;
double lat_val, lng_val, alt_m_val;
SoftwareSerial mySerial(9, 10);
SoftwareSerial GPS_SoftSerial(3,4);
TinyGPSPlus gps;
volatile float minutes, seconds;
volatile int degree, secs, mins;


void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  GPS_SoftSerial.begin(9600);
  delay(100);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(sensor,INPUT);
}


void loop()
{ value=digitalRead(sensor);
  Serial.print(value);
  if(value==1){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH); 
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  data();
  delay(1000);
  SendMessage();
  delay(1000);
 }else{
  digitalWrite(in1,HIGH); 
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  }
  
}



 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+91xxxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Crack detected !");
  mySerial.println("Latitude:");
  mySerial.println(lat_val,6);
  mySerial.println("Longitude:");
  mySerial.println(lng_val,6);
 
  Serial.print("\nMessage Sent");
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


void data() {
  smartDelay(1000);
  
  bool loc_valid, alt_valid;
  lat_val = gps.location.lat(); 
  loc_valid = gps.location.isValid(); 
  lng_val = gps.location.lng();
  alt_m_val = gps.altitude.meters(); 
  alt_valid = gps.altitude.isValid(); 

  if (!loc_valid)
  {
    Serial.print("Latitude : ");
    Serial.println("***");
    Serial.print("Longitude : ");
    Serial.println("***");
    delay(4000);
  }
  else
  {
    Serial.println("GPS READING: ");
    DegMinSec(lat_val);
    Serial.print("Latitude in Decimal Degrees : ");
    Serial.println(lat_val, 6);

    DegMinSec(lng_val); 
    Serial.print("Longitude in Decimal Degrees : ");
    Serial.println(lng_val, 6);
    delay(4000);
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SoftSerial.available()) 
    gps.encode(GPS_SoftSerial.read());
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
