#include <SoftwareSerial.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

SoftwareSerial RFID(2, 3); // RX and TX

int data1 = 0;
int ok = -1;
int LED = 13;
int y = 0;

int tag1[14] = {2,48,49,48,70,65,66,49,70,52,50,70,56,3}; //mk
int tag2[14] = {2,50,49,48,48,50,52,53,69,56,68,68,54,3}; //pl
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip

void setup()
{
  RFID.begin(9600);     // start serial to RFID reader
  Serial.begin(57600);  // start serial to PC
  pinMode(LED, OUTPUT); // for status LED
  // initialize LCD with number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD
  lcd.print("Hello AIS World!");
  lcd.setCursor(0,0);
  delay(1000);
  lcd.print("Place your IDtag");
  Serial.println("Ready to read your RFID tags:");
}

boolean comparetag(int aa[14], int bb[14])
{
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < 14 ; cc++)
  {
    if (aa[cc] == bb[cc])
    {
      fg++;
    }
  }
  if (fg == 14)
  {
    ff = true;
  }
  return ff;
}

void checkmytags() // compares each tag against the tag just read
{
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (comparetag(newtag, tag1) == true) { ok = 1; }
  if (comparetag(newtag, tag2) == true) { ok = 2; }
}

void readTags()
{
  ok = -1;

  if (RFID.available() > 0) 
  {
    // read tag numbers
    delay(100); // needed to allow time for the data to come in from the serial buffer.

    for (int z = 0 ; z < 14 ; z++) // read the rest of the tag
    {
      data1 = RFID.read();
      newtag[z] = data1;
    }
    while(RFID.read() != -1) { }   // prevent double readings

    // do the tags match up?
    checkmytags();
  }

  // now do something based on tag type
  if (ok > 0) // if we had a match
  {
    Serial.print("User: ");
    Serial.print(ok);
    Serial.println(" - Accepted");
    digitalWrite(LED, HIGH);
    lcd.clear();
    if (ok == 1) {
      lcd.setCursor(0, 0);
      lcd.print("    Master of   "); // hard coded is not perfect, but string causes errors in display
      lcd.setCursor(0, 1);
      lcd.print(" the Makerspace ");
    }
    if (ok == 2) {
      lcd.setCursor(0, 0);
      lcd.print("   Le  Nguyen   ");
      lcd.setCursor(0, 1);
      lcd.print("  Xuan Phuong   ");
    }
    delay(2000); 
    digitalWrite(LED, LOW);
    lcd.setCursor(0,0);
    lcd.print("Place your IDtag");
    while(RFID.read() != -1) { }   // prevent double readings 
    ok = -1;
  }
  else if (ok == 0) // if we didn't have a match
  {
    Serial.print("ID read: ");
    // clear lcd
    lcd.clear();
    lcd.setCursor(2, 0);
    for (int z = 1 ; z < 14 ; z++) // print card id
    {
      Serial.print(newtag[z]);
      lcd.print(newtag[z]);
      if(z == 6) {
        lcd.setCursor(2, 1);
      }
    }
    Serial.println(" - Rejected");    
    for (int z = 0 ; z < 50 ; z++) // blink for 5 seconds
    {
         digitalWrite(LED, HIGH);
         delay(50);
         digitalWrite(LED, LOW);
         delay(50);
    }           
    lcd.setCursor(0,0);
    lcd.print("Place your IDtag");
    while(RFID.read() != -1) { }   // prevent double readings
    // RFID.flush(); // stops multiple reads - flush has a different intention and does not work
    ok = -1;
  }
}

void loop()
{
  readTags();
  lcd.setCursor(y, 1);
  lcd.print(" *");
  y++;
  delay(100);
  if( y > 16 ) y = 0 ;
}
