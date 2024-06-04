#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>  // to handle this particular modem driver
#include "RF24.h"      // the library which helps us to control the radio modem

#define RF24_PAYLOAD_SIZE 32
#define AAAD_ARO 1
#define AAAD_MODULE 1

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10, which are CE & CSN pins  */
RF24 radio(9, 10);
const uint8_t rf24_channel[] = { 1, 26, 51, 76, 101 };                                                            // Radio channels set depending on satellite number
const uint64_t addresses[] = { 0x4141414430LL, 0x4141414431LL, 0x4141414432LL, 0x4141414433LL, 0x4141414434LL };  //with radioNumber set to zero, the tx pipe will be 'AAAD0', which is basically HEX'4141414430', which is remote DESTINATION address for our transmitted data. The rx pipe code is the local receive address, which is what the remote device needs to set for the remote devices 'tx' pipe code.
uint8_t txData[RF24_PAYLOAD_SIZE];
uint8_t rxData[RF24_PAYLOAD_SIZE];

const int stepPin = 6;                                     // PUL+ pin
const int dirPin = 8;                                      // DIR+ pin
const int stepsPerRevolution = 5;                          // Afhankelijk van je motor (meestal 200 voor een 1.8° motor)
const int rpm = 60;                                        // Aantal omwentelingen per minuut
const int stepDelay = 60000 / (stepsPerRevolution * rpm);  // Bereken de vertraging per stap

// Timing configuration
unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long currentMillis;
unsigned long sampleTime = 5000;  // milliseconds of on-time

// int to hex converter
void printHex2(unsigned v) {
  Serial.print("0123456789ABCDEF"[v >> 4]);
  Serial.print("0123456789ABCDEF"[v & 0xF]);
}

void setup() {
  Serial.begin(9600);
  Serial.println("nRF24 Application ARO" + String(AAAD_ARO) + ", Module" + String(AAAD_MODULE) + " Started!\n");

  // Activate Radio
  radio.begin();                  // Ativate the modem
  radio.setPALevel(RF24_PA_MIN);  // Set the PA Level low to prevent power supply related issues
                                  // since this is a getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setDataRate(RF24_1MBPS);  // choosing 1 Mega bit per second radio frequency data rate
                                  // radio frequency data rate choices are:  //RF24_250KBPS    //RF24_2MBPS  //RF24_1MBPS
  radio.setChannel(rf24_channel[AAAD_ARO]);
  radio.setPayloadSize(RF24_PAYLOAD_SIZE);
  radio.openWritingPipe(addresses[AAAD_MODULE]);
  radio.openReadingPipe(1, addresses[AAAD_MODULE]);

  // Start the radio listening for data
  radio.startListening();

  // Zet de pinnen als uitgang
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  // check to see if it's time to change the state of the LED
  currentMillis = millis();

  if (currentMillis - previousMillis >= sampleTime) {


    unsigned long timeStamp = millis() / 1000;
    uint8_t cursor = 0;



    // test sensorisch meten
    float AcsValue = 0.0, Samples = 0.0, AvgAcs = 0.0, AcsValueF = 0.0;

    for (int x = 0; x < 150; x++) {  // Verzamel 150 samples
      AcsValue = analogRead(A0);     // Lees de huidige sensorwaarde
      Samples += AcsValue;           // Voeg samples samen
      delay(3);                      // Laat ADC tot rust komen voor de volgende sample (3 ms)
    }

    AvgAcs = Samples / 150.0;  // Bereken het gemiddelde van de samples

    // Converteer de gemiddelde ADC waarde naar spanning (0-5V)
    float Voltage = AvgAcs * (5.0 / 1024.0);
    Voltage *= 10;
    Serial.println(Voltage);


    // Bereken de stroom in Ampère
    // 2.5 is de offset (bij 0A output is 2.5V)
    // 0.066V (66mV) is de stijging in uitgangsspanning bij 1A stroom
    AcsValueF = (Voltage - 2.5) / 0.066;
    AcsValueF *= 10;
    Serial.println(AcsValueF);
    // Serial.print("Sensor Voltage: ");
    // Serial.print(Voltage);
    // Serial.print(" V, Current: ");
    // Serial.print(AcsValueF);
    // Serial.println(" A"); // Druk de gemeten stroom af op de seriële monitor

    delay(100);  // Wacht 100ms voor de volgende meting

    float f = 123.45;        // Je hebt een float waarde
    uint8_t u = (uint8_t)f;  // Je zet de float om naar uint8_t
    // Serial.println(u);  // Print de waarden




    // GEEN kommagetallen toegestaan!
    uint16_t zonnepanelenStroom = (uint16_t)AcsValueF;  // mA - milli ampère
    uint8_t zonnepanelenSpanning = (uint8_t)Voltage;    // V * 10 - volt vermenigvuldigd door 10
    uint8_t zonnepanelenVoortgang = 76;                 // percentage van 0 t/m 100 van hoe ver de zonnepanelen zijn uitgeklapt

    uint16_t accuStroom = 1234;  // mA - milli ampère
    uint8_t accuSpanning = 93;   // V * 10 - volt vermenigvuldigd door 10



    txData[cursor++] = zonnepanelenStroom >> 8;
    txData[cursor++] = zonnepanelenStroom;

    txData[cursor++] = zonnepanelenSpanning;

    txData[cursor++] = zonnepanelenVoortgang;


    txData[cursor++] = accuStroom >> 8;
    txData[cursor++] = accuStroom;

    txData[cursor++] = accuSpanning;



    // vult de overige plekjes van de 32 bytes met 0
    while (cursor < RF24_PAYLOAD_SIZE) {
      txData[cursor++] = 0;
    }

    /****************** Transmit Mode ***************************/

    // Print transmit data in Hex format
    Serial.print("txData: ");
    for (size_t i = 0; i < cursor; ++i) {
      if (i != 0) Serial.print(" ");
      printHex2(txData[i]);
    }
    Serial.println();

    radio.stopListening();  // First, stop listening so we can talk.
    // Serial.println(F("Now Sending"));

    // Transmit data to radio
    radio.write(&txData, sizeof(txData));

    radio.startListening();  // Now, continue listening
    // Serial.println(F("Now Listing"));

    previousMillis = currentMillis;
  }

  /****************** Receive Mode ***************************/

  if (radio.available()) {  //'available' means whether valid bytes have been received and are waiting to be read from the receive buffer
    // Receive data from radio
    while (radio.available()) {             // While there is data ready
      radio.read(&rxData, sizeof(rxData));  // Get the payload
    }
    // Print received data in Hex format
    Serial.print("rxData: ");
    for (size_t i = 0; i < RF24_PAYLOAD_SIZE; ++i) {
      if (i != 0) Serial.print(" ");
      printHex2(rxData[i]);
    }
    Serial.println();

    // in of uitklappen
    if (bitRead(rxData[0], 0)) {
      Serial.println("uitklappen");
      // Draai de motor 12 seconden vooruit
      digitalWrite(dirPin, HIGH);  // Stel de draairichting in
      runMotor(18000);             // Draai de motor 12 seconden
    } else if (bitRead(rxData[0], 1)) {
      Serial.println("inklappen");
      // Draai de motor 12 seconden achteruit
      digitalWrite(dirPin, LOW);  // Verander de draairichting
      runMotor(18000);            // Draai de motor 12 seconden
    }
  }

}  // Loop

void runMotor(unsigned long duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    // Stuur een puls naar de step pin
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);
  }
}