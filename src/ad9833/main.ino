#include <MD_AD9833.h>

#define DATA  11
#define CLK   13
#define FSYNC  10
int const analogPin = A2;
int const ledPin = A0;

MD_AD9833	AD(FSYNC);  // Hardware SPI

void setup() {
    pinMode(ledPin,OUTPUT);
	AD.begin();
    AD.setFrequency(MD_AD9833::CHAN_0,20000);
    AD.setMode(MD_AD9833::MODE_SINE);
}

void loop() {

    uint16_t adOutput = analogRead(analogPin);

    analogWrite(ledPin,adOutput);
    //delay(1000); // wait a second
    //digitalWrite(ledPin,LOW);
    //delay(1000);

}
