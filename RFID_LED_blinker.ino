#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10   // Define the SDA pin
#define RST_PIN 9    // Define the RST (Reset) pin
#define LED_PIN_1 8   // Define the pin for the LED green
#define LED_PIN_2 7   // Define the pin for the LED RED

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Define the expected UID of the allowed card
byte allowedCardUID[] = { 0x43, 0x4F, 0xCB, 0xDC};

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  Serial.println("Starting RFID Reading Setup...");

  SPI.begin();         // Initiate SPI bus
  Serial.println("SPI Bus Initialized...");

  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("RFID Module Initialized...");

  pinMode(LED_PIN_1, OUTPUT);  // Set LED pin as an output
  pinMode(LED_PIN_2, OUTPUT);
  Serial.println("Scan a RFID tag");
}

void loop() {
  // Look for new cards
  /*if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("New Card Detected...");                                                 
    Serial.print("Card UID: ");                                                             // for printing the card UID that is currently scanned
    printCardUID();
  }*/


  //Below code for checking if the Card is the "RIGHT" Card

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Check if the detected card matches the allowed card UID
    if (compareCardUID(allowedCardUID)) {
      Serial.println("Allowed Card Detected. Turning on LED...");
      digitalWrite(LED_PIN_1, HIGH);  // Turn on the LED
      digitalWrite(LED_PIN_2, LOW);
      delay(1000);
      digitalWrite(LED_PIN_1, LOW);
    } 
    else {
      Serial.println("Unauthorized Card Detected. Turning off LED...");
      digitalWrite(LED_PIN_1, LOW);   // Turn off the LED
      digitalWrite(LED_PIN_2,HIGH);
      delay(1000);
      digitalWrite(LED_PIN_2, LOW);
    }

    delay(1000);  // Delay for 1 second to avoid reading the same card multiple times
  }
}

void printCardUID() {
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
}

bool compareCardUID(byte* expectedUID) {
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != expectedUID[i]) {
      return false;  // UID does not match                         //To check if the show UID is same as allowed UID
    }
  }
  return true;  // UID matches
}
