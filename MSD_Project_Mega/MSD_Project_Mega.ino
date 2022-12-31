
#include<Servo.h>
Servo myservo;
int pos = 0;
int val;
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial SIM900(22, 23);
byte tx = 1;
TinyGPS gps;  //Creates a new instance of the TinyGPS object

const int SPEAKER = 6;
const int LED_RED = 7;
const int LED_YELLOW = 10;
int Relay = 8;
int tempC_1 = 0; //set initial tempC 0° for all LM35
int smkC_1 = 0; //set initial tempC 0º for all MQ 2
const int SensorPin1 = A0; //fire input sensor pin
const int SensorPin2 = A1;

int pir1 = 30;
int pir2 = 33;
int pir3 = 36;

int pirval1;
int pirval2;
int pirval3;



String textForSMS;
void setup()
{

  pinMode(tx, OUTPUT);
  pinMode(Relay, OUTPUT);
  myservo.attach(13);
  pinMode(SPEAKER, OUTPUT);
  lcd.begin(14, 2);
  delay(100);
  pinMode(SensorPin1, INPUT);
  pinMode(SensorPin2, INPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(pir3, INPUT);


  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT); //Set control pins to be outputs
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);//set both motors off for start-up

  myservo.write(pos);
  SIM900.begin(9600);
  Serial.begin(9600); //Start the serial connection with the computer
}
void loop()
{
  int tempC_1 = analogRead(SensorPin1);
  int SmkC_1 = analogRead(SensorPin2);
  tempC_1 = analogRead(SensorPin1); //read the value from the LM35 sensor
  tempC_1 = (5.0 * tempC_1 * 100.0) / 1024.0; //convert the analog data to temperature
  smkC_1 = analogRead(SensorPin2); //read the value from the MQ 2 sensor
  smkC_1 = (5.0 * smkC_1 * 100.0) / 1024.0; //convert the analog data to temperature
  pirval1 = digitalRead(pir1);
  pirval2 = digitalRead(pir2);
  pirval3 = digitalRead(pir3);
  delay(1);

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c))
        newData = true;
    }
  }

  if (tempC_1 >= 50 || smkC_1 >= 50)
  {
    digitalWrite(Relay, HIGH);

    for (pos = 0; pos < 180; pos += 1)
    {
      myservo.write(pos);
    }


    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  THERE IS FIRE ");
    lcd.setCursor(0, 1);
    lcd.print(" NOT SAFE HERE ");
    delay(100);
    lcd.clear();
    lcd.print("Sending SMS...");


    tone(SPEAKER, 1047, 500);
    delay(200);
    tone(SPEAKER, 1109, 300);
    delay(200);
    tone(SPEAKER, 1175, 100);

    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    SIM900.print("AT+CMGF=1\r");
    delay(100);
    SIM900.print("AT+CMGS=\"+8801713376517\"\r");
    SIM900.print("FIRE ALERT!\r");

    SIM900.print("AT+CMGS=\"+8801713376517\"\r");
    SIM900.print("FIRE OCCURED!\r");

    SIM900.print("AT+CMGS=\"+8801713376517\"\r");
    SIM900.print("FIRE OCCURED! in\r");

    SIM900.print("Latitude = ");
    SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    SIM900.print(" Longitude = ");
    SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26

    SIM900.println();
  }



  else if (pirval1 == 1 || pirval2 == 1 || pirval3 == 1)
  {
    
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  THERE IS POSSIBLE HOME INTRUDER ");
    lcd.setCursor(0, 1);
    lcd.print(" BE AWARE ");
    delay(1);
    lcd.clear();
    lcd.print("Sending SMS...");


    tone(SPEAKER, 1047, 500);
    delay(200);
    tone(SPEAKER, 1109, 300);
    delay(200);
    tone(SPEAKER, 1175, 100);

    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    SIM900.print("AT+CMGF=1\r");
    delay(100);
    SIM900.print("AT+CMGS=\"+8801716676517\"\r");
    SIM900.print("POSSIBLE THEFT!\r");

    SIM900.print("AT+CMGS=\"+8801716676517\"\r");
    SIM900.print("POSSIBLE THEFT AT!\r");

    SIM900.print("Latitude = ");
    SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    SIM900.print(" Longitude = ");
    SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
    SIM900.println();
  }
  else
  {
    digitalWrite(Relay, LOW);
    delay(50);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    for (pos = 180; pos >= 0; pos -= 5)
    {
      myservo.write(pos);
    }


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NO FIRE OR INTRUDER");
    lcd.setCursor(0, 1);
    lcd.print("   ALL SAFE   ");
  }



}
