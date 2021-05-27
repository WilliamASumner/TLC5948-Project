#include <SPI.h>

uint8_t inputBuf[33];
uint8_t dummyBuf[33] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 ,0x07,
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                         0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                         0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                         0x20};
volatile uint8_t pos;
volatile boolean dataReady;

ISR (SPI_STC_vect) {
    byte c = SPDR; // get byte from SPI data reg
    if (pos < 33) {
        inputBuf[pos] = c;
        pos++;
        SPDR = dummyBuf[pos];
        if (pos == 33)
            dataReady = true;
    }
}


const int interruptPin = 2;
void onLatch(void);

void setup() {
    Serial.begin(9600);
    pinMode(MISO,OUTPUT); 
    SPCR |= _BV(SPE); // enable SPI slave mode
    pos = 0;
    dataReady = false;

    pinMode(interruptPin,INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), onLatch, RISING);


    SPI.attachInterrupt(); // enable SPI interrupt
}

//response whenever a latch is sent, emulate TLC5948 behavior
void onLatch() {
    pos = 0; // reset position
    SPDR = inputBuf[pos];
}



void loop() {
    if (dataReady) {
        Serial.print("Data: \n");
        for (int i = 0; i < 33; i++) {
            Serial.print(inputBuf[i],HEX);
            if (i%8==7) Serial.print("\n");
            else Serial.print(" ");
        }
        Serial.print("\n");
        dataReady = false;
        pos = 0;
    }
}
