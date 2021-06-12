#include "Tlc5948.h"

Tlc5948     tlc; // PWM LED driver (using Hardware SPI)

void setup() {
    SPI.begin();// TLC5948 Interface
    tlc.begin(); // sets up pins, default GS/DC/BC data and Func Ctrl bits

    tlc.setGsData(Channels::all,0xffff); // clear all data in the chip
    tlc.exchangeData(DataKind::gsdata);

    tlc.setDcData(Channels::all,0x0f);
    tlc.setBcData(0x7f);
    Fctrls fSave = tlc.getFctrlBits();
    fSave &= ~(Fctrls::dsprpt_mask);
    fSave |= Fctrls::dsprpt_mode_1; // set autodisplay repeat
    fSave |= Fctrls::espwm_mode_1;
    tlc.setFctrlBits(fSave);
    tlc.exchangeData(DataKind::ctrldata);

    tlc.startGsclk();
}


void loop() { // Blink

    tlc.setGsData(Channels::all,0x7fff);
    tlc.exchangeData(DataKind::gsdata);
    delay(1000);
    tlc.setGsData(Channels::all,0x0);
    tlc.exchangeData(DataKind::gsdata);
    delay(1000);

}
