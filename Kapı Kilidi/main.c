#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "F4 11 4B 18";
byte lock = 0;

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

// LED'lerin pinlerini tan�mlay�n
const int greenLedPin = 6; // Ye�il LED'in pin numaras�
const int redLedPin = 5;   // K�rm�z� LED'in pin numaras�

void setup() {
  Serial.begin(9600);

  // LED'lerin pinlerini ��k�� olarak ayarlay�n
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  // Servo motoru ba�lat�n ve ilk konumunu ayarlay�n
  servo.attach(3);
  servo.write(70);

  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
      String ID = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
        ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
        ID.concat(String(rfid.uid.uidByte[i], HEX));
        delay(300);
      }
      ID.toUpperCase();

      // Kart ID'sini do�rulay�n ve LED'leri uygun �ekilde kontrol edin
      if (ID.substring(1) == UID) {
        digitalWrite(greenLedPin, HIGH); // Do�ru kart okundu�unda ye�il LED'i yak
        delay(1000); // 1 saniye bekleyin
        digitalWrite(greenLedPin, LOW); // Ye�il LED'i s�nd�r
        if (lock == 0) {
          servo.write(160); // Servo motoru di�er a��ya d�nd�r
          lock = 1;
        } else {
          servo.write(70); // Servo motoru belirli bir a��ya d�nd�r
          lock = 0;
        }
      } else {
        // Yanl�� kart okundu�unda k�rm�z� LED'i yan�p s�nd�r
        digitalWrite(redLedPin, HIGH);
        delay(1000);
        digitalWrite(redLedPin, LOW);
      }
    }
  }
}
