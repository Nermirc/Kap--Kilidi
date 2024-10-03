#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "F4 11 4B 18";
byte lock = 0;

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

// LED'lerin pinlerini tanýmlayýn
const int greenLedPin = 6; // Yeþil LED'in pin numarasý
const int redLedPin = 5;   // Kýrmýzý LED'in pin numarasý

void setup() {
  Serial.begin(9600);

  // LED'lerin pinlerini çýkýþ olarak ayarlayýn
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  // Servo motoru baþlatýn ve ilk konumunu ayarlayýn
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

      // Kart ID'sini doðrulayýn ve LED'leri uygun þekilde kontrol edin
      if (ID.substring(1) == UID) {
        digitalWrite(greenLedPin, HIGH); // Doðru kart okunduðunda yeþil LED'i yak
        delay(1000); // 1 saniye bekleyin
        digitalWrite(greenLedPin, LOW); // Yeþil LED'i söndür
        if (lock == 0) {
          servo.write(160); // Servo motoru diðer açýya döndür
          lock = 1;
        } else {
          servo.write(70); // Servo motoru belirli bir açýya döndür
          lock = 0;
        }
      } else {
        // Yanlýþ kart okunduðunda kýrmýzý LED'i yanýp söndür
        digitalWrite(redLedPin, HIGH);
        delay(1000);
        digitalWrite(redLedPin, LOW);
      }
    }
  }
}
