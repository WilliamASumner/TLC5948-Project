#include "Tlc5948.h"
Tlc5948     tlc; // PWM LED driver (using Hardware SPI)

inline void printChannels(Channels c) {
    Serial.println(static_cast<int>(c),HEX);
}

inline void printSidFlags(SidFlags s) {
    if ((s & SidFlags::TEF) != SidFlags::NONE)
        Serial.print(" TEF ");
    if ((s & SidFlags::PTW) != SidFlags::NONE)
        Serial.print(" PTW ");
    if ((s & SidFlags::ISF) != SidFlags::NONE)
        Serial.print(" ISF ");
    if ((s & SidFlags::OLD) != SidFlags::NONE)
        Serial.print(" OLD ");
    if ((s & SidFlags::LSD) != SidFlags::NONE)
        Serial.print(" LSD ");
    if ((s & SidFlags::LOD) != SidFlags::NONE)
        Serial.print(" LOD ");
    Serial.println();
}

void setup() {
    TCCR1B = TCCR1B & B11111000 | B00000001; // increase PWM frequency to 31Khz, 490Hz may be too slow

    Serial.begin(9600);
    delay(3000); // LEDs are somehow one before this completes

    SPI.begin();// TLC5948 Interface
    tlc.begin(); // sets up pins, default GS/DC/BC data and Func Ctrl bits

    tlc.updateGsData(Channels::all,0xFFFF); // all channels 100%

    Serial.print("Greyscale data:\t");
    tlc.printGsDataBuf();

    tlc.exchangeData(DataKind::gsdata);
    delay(1000);

    tlc.updateDcData(Channels::all,0x7F); // all channels high
    Fctrls f = tlc.getFctrlBits()  & ~(Fctrls::blank_mask); // clear blank and timing reset bits
    tlc.updateFctrlData(f);

    Serial.print("Control data:\t");
    tlc.printCtrlDataBuf();

    tlc.exchangeData(DataKind::controldata);
    tlc.startGsclk();

    Serial.println("Getting SID data...");
    Channels old = Channels::none, lsd = Channels::none, lod = Channels::none;
    SidFlags flags = tlc.getSidData(old,lsd,lod,true); // get Sid data

    Serial.print("TLC5948: Received Error flags: ");
    printSidFlags(flags);
    Serial.print("lod channels: ");
    printChannels(lod);
    Serial.print("lsd channels: ");
    printChannels(lsd);
    Serial.print("old channels: ");
    printChannels(old);

}

void loop() {
}
