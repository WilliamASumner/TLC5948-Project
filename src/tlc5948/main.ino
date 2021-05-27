#include "Tlc5948.h"
Tlc5948 interface;

int const ledPin = 13;

void setup() {
    pinMode(ledPin,OUTPUT);
    
    interface.init();
}

void loop() {
    interface.updateGsData(Channels::upper8,0xffff);
    //interface.exchangeData(DataKind::gsdata);
    digitalWrite(ledPin,HIGH);
    delay(1000); // wait a second
    interface.updateGsData(Channels::lower8,0xffff);
    //interface.exchangeData(DataKind::gsdata);
    digitalWrite(ledPin,LOW);
    delay(1000);

}

