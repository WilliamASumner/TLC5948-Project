#include <MD_AD9833.h>
#include "Tlc5948.h"
#if DEBUG_TIMING
int numTimes = 0;
unsigned long totalTime = 0;
#endif

int const analogPin = A2;
int const ledPin = 6; // pin d6 -> default freq is 980 Hz, timer1->millis()/micros()

#define FSYNC  10
//MD_AD9833  AD(FSYNC);  // Function Generator (using Hardware SPI)
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
#if DEBUG_TIMING
#warning "using debug timing"
    Serial.begin(9600);
    TCCR0B = TCCR0B & B11111000 | B00000001; // Adjusting PWM frequency of Timer1 to 61Khz - Not sure how this works? 
#endif
    
    Serial.begin(9600);

    pinMode(ledPin,OUTPUT);
    pinMode(analogPin,INPUT);
    delay(3000);

    SPI.begin();// TLC5948 Interface
    tlc.begin(); // sets up pins, default GS/DC/BC data and Func Ctrl bits

    tlc.updateGsData(Channels::all,0xFFFF); // all channels 100%

    Serial.print("Greyscale data:\t");
    tlc.printGsDataBuf();

    tlc.exchangeData(DataKind::gsdata);
    delay(1000);

    tlc.updateDcData(Channels::all,0x7F); // all channels high
    Fctrls f = tlc.getFctrlBits()  & ~(Fctrls::blank_mask) & ~(Fctrls::tmgrst_mask); // clear blank and timing reset bits
    f |= Fctrls::tmgrst_mode_1; // enable timing reset (LAT rise means: zero GS counter and force off outputs)
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


inline void funcToRun() {
    //tlc.updateGsData(Channels::all,0x8000); // 0x8000 -> AA54
    //tlc.updateGsData(Channels::all,0x00ff); // 0x00ff -> 0000 (all warnings except TEF)
    //.tlc.updateGsData(Channels::all,0x0100); // 0x0100 -> 5522 (all warnings except TEF)
    //tlc.updateGsData(Channels::all,0x0001); // 0x0001 -> 2a55 (all warnings except TEF)
    tlc.updateGsData(Channels::all,0x0000); // 0x0000 -> XXXX (all warnings except TEF)
    Serial.print("gs: \t");
    tlc.printGsDataBuf();

    Channels old = Channels::none, lsd = Channels::none, lod = Channels::none;
    //digitalWrite(SSEL, LOW); // SSEL used for SPI with a slave select
    tlc.exchangeData(DataKind::gsdata);
    //digitalWrite(SSEL, HIGH);
    // Pin 9 is PWM so ~490Hz -> 0.03469s or 34.69ms wait time required before retrieving Sid

    SidFlags flags = tlc.getSidData(old,lsd,lod,true);
    Serial.print("spi:\t");
    tlc.printSpiBuf();

    if (flags != SidFlags::BADPARSE) {
        Serial.print("TLC5948: Received Error flags: ");
        printSidFlags(flags);
        Serial.print("lod channels: ");
        printChannels(lod);
        Serial.print("lsd channels: ");
        printChannels(lsd);
        Serial.print("old channels: ");
        printChannels(old);
    }
    
    delay(1000);

}


void loop() {
#if DEBUG_TIMING
    unsigned long timeStart = micros();
#endif

    //funcToRun();

#if DEBUG_TIMING
    unsigned long runTime = micros() - timeStart;
    totalTime += runTime;

    numTimes++;
    if (numTimes >= 10000) {
        Serial.print("Average time: ");
        Serial.print(totalTime/10000.0);
        Serial.print("us\n");
        numTimes  =  0;
        totalTime = 0;
    }
#endif
}
