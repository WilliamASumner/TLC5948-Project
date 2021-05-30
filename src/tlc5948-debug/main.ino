#include "Tlc5948.h"
Tlc5948     tlc; // PWM LED driver (using Hardware SPI)

inline void printChannels(Channels c) {
    Serial.println(static_cast<unsigned int>(c),HEX);
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
    Serial.begin(9600);
    delay(3000); // LEDs are somehow on before this completes

    SPI.begin();// TLC5948 Interface
    tlc.begin(); // sets up pins, default GS/DC/BC data and Func Ctrl bits

    tlc.updateGsData(Channels::all,0xFFFF); // all channels 100%
    tlc.exchangeData(DataKind::gsdata);

    tlc.updateDcData(Channels::all,0x7F); // all channels DC high (maximum current defined by IREF ~ 20ma)
    Fctrls f = tlc.getFctrlBits()  & ~(Fctrls::blank_mask); // clear blank bit
    tlc.updateFctrlData(f);

    Serial.print("Control data:\t");
    tlc.printCtrlDataBuf();

    tlc.exchangeData(DataKind::controldata);
    Serial.println("Exchanged Control data");
    tlc.startGsclk();
    Serial.println("started GSCLK");

    Serial.println("Getting SID data...");
    Channels old = Channels::none, lsd = Channels::none, lod = Channels::none;
    SidFlags flags = tlc.getSidData(old,lsd,lod,true);

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
