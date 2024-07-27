#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
String IDname;

// Function prototypes
void displayMainScreen();
int getFingerprintIDez();
void displayUserGreeting(String Name);
void displayUserNotMatch();

void setup()
{
  Serial.begin(9600);
  finger.begin(57600);
  
  if (finger.verifyPassword()) 
  {
    Serial.println("Found fingerprint sensor");
  } 
  else 
  {
    Serial.println("Did not find fingerprint sensor");
    while (1) 
    { 
      delay(1); 
    }
  }
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  //OLED display setup
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //displays main screen
  displayMainScreen();
}

void loop()
{
  displayMainScreen();
  fingerprintID = getFingerprintIDez();
  delay(30);
  if (finger.getImage() == FINGERPRINT_NOFINGER)
  {
    // No finger is present, do nothing
  }
  else
  {
    // A finger is present, check the fingerprint ID
    if(fingerprintID == 25)
    {
      IDname = "ARKAJYOTI";  
      displayUserGreeting(IDname);       
    }
    else
    {
      displayUserNotMatch(); // Display "Fingerprint not found" message
    }
  }
}

// Function definitions
void displayMainScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17,5);
  display.println("Press your thumb");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(55,20);
  display.println("!!!");  
  display.display();
  delay(500);
}

int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  return finger.fingerID; 
}

void displayUserGreeting(String Name)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(45,5);
  display.print("WELCOME");
  display.setCursor(40,20);
  display.print(Name); 
  display.display();
  digitalWrite(6, HIGH);
  for(int z=1; z<=5; z++)
  {
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(5, LOW);
    delay(100);
  }
  digitalWrite(7, LOW);
  delay(5000);
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  fingerprintID = 0; 
  delay(500);
}

void displayUserNotMatch()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17,5);
  display.println("Fingerprint");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(55,20);
  display.println("not found");  
  display.display();
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  delay(1500);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  fingerprintID = 0; 
  delay(500);
}
