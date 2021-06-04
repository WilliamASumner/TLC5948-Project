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
    delay(50); // LEDs are somehow on before this completes

    SPI.begin();// TLC5948 Interface
    tlc.begin(); // sets up pins, default GS/DC/BC data and Func Ctrl bits

    //tlc.setGsData(Channels::all,0xFFFF); // all channels 100%
    //tlc.exchangeData(DataKind::gsdata);
    tlc.setGsData(Channels::all,0x0); // clear all data in the chip
    tlc.exchangeData(DataKind::gsdata);
    tlc.setGsData(Channels::all,0x0fff); // clear all data in the chip
    tlc.exchangeData(DataKind::gsdata);

    tlc.setDcData(Channels::all,0x7f);
    tlc.setBcData(0x7f);
    Fctrls fSave = tlc.getFctrlBits();
    tlc.setFctrlBits(Fctrls::empty_bits | Fctrls::blank_mode_1);
    tlc.exchangeData(DataKind::ctrldata);
    tlc.setFctrlBits(fSave);
    tlc.exchangeData(DataKind::ctrldata);


    /*
    //Debug
    uint8_t bytes[33] = {0};
    tlc.readDeviceContents(bytes,33); // shifting in all 0's
    Serial.print("Device Contents after exchange: ");
    printBuf(bytes,33);

    tlc.readDeviceContents(bytes,33); // shifting in all 0's
    Serial.print("Device Contents after shifting in 0's: ");
    printBuf(bytes,33);

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
    */
}

void loop() {
    tlc.exchangeData(DataKind::gsdata);
}
