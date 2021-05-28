#ifndef TLC5948_LIB_H
#define TLC5948_LIB_H
#include <SPI.h>

// pin assignments; todo replace with enum maybe
#ifdef ARDUINO_TEENSY41 // Teensy version
#warning "Using Teensy Pin Definitions"
#error "Unimplemented"
int const LAT = 0;   // latch control
int const GSCLK = 0; // pwm clock
int const SSEL = 0; // slave select, HW SS
int const SIN = 0;   // serial data input (to Tlc5948)
int const SOUT = 0;  // serial data output (from Tlc5948) 
int const SCLK = 0;  // serial data clock
#else // assume Arduino Nano
#warning "Using Arduino Nano Pin Definitions"
int const LAT = 3;   // latch control, using D3
int const GSCLK = 9; // pwm clock, using D9 for PWM (~490Hz)
int const SSEL = 10; // slave select, HW SS
int const SIN = 11;   // serial data input (to Tlc5948) HW MOSI, using D11
int const SOUT = 12;  // serial data output (from Tlc5948)  HW MISO, using D12
int const SCLK = 13;  // HW SCLK, using D13
#endif // ifdef ARDUINO_TEENSY41

// SPI settings
uint32_t const SPI_SPEED = 10000; // 10Khz to start //33000000; // 33mhz listed on data sheet
const unsigned int BIT_ORDER = MSBFIRST;
const unsigned int SPI_MODE = SPI_MODE0; // todo check if this is right
const int NUM_CHANNELS = 16;
const int PWM_FREQ = 490; // default arduino pin 9 freq 

// led open, led short, output leakage, iref short flag, pre-thermal warning, thermal error flag
// SidFlags = BADPARSE TEF PTW ISF OLD LSD LOD
enum class SidFlags { NONE=0,LOD=1,LSD=2,OLD=4,ISF=8,PTW=16,TEF=32,BADPARSE=64 };

inline SidFlags operator|(SidFlags a, SidFlags b) {
    return static_cast<SidFlags>(static_cast<int>(a) | static_cast<int>(b));
}
inline SidFlags operator&(SidFlags a, SidFlags b) {
    return static_cast<SidFlags>(static_cast<int>(a) & static_cast<int>(b));
}
inline void operator&=(SidFlags& a, SidFlags b) {
    a = static_cast<SidFlags>(static_cast<int>(a) & static_cast<int>(b));
}
inline void operator|=(SidFlags& a, SidFlags b) {
    a = static_cast<SidFlags>(static_cast<int>(a) | static_cast<int>(b));
}
inline SidFlags operator~(SidFlags a) {
    return static_cast<SidFlags>(~static_cast<int>(a));
}

enum class Channels : uint16_t { // Channel masks
    none =     0x0000,
    chan_set = 0x0001,
    out0 =     0x0001,
    out1 =     0x0002,
    out2 =     0x0004,
    out3 =     0x0008,
    out4 =     0x0010,
    out5 =     0x0020,
    out6 =     0x0040,
    out7 =     0x0080,
    out8 =     0x0100,
    out9 =     0x0200,
    out10 =    0x0400,
    out11 =    0x0800,
    out12 =    0x1000,
    out13 =    0x2000,
    out14 =    0x4000,
    out15 =    0x8000,
    even =     0xaaaa,
    odd =      0x5555,
    all =      0xffff,
    upper8 =   0xff00,
    lower8 =   0x00ff,
};

inline Channels operator|(Channels a, Channels b) {
    return static_cast<Channels>(static_cast<int>(a) | static_cast<int>(b));
}
inline Channels operator&(Channels a, Channels b) {
    return static_cast<Channels>(static_cast<int>(a) & static_cast<int>(b));
}
inline void operator&=(Channels& a, Channels b) {
    a = static_cast<Channels>(static_cast<int>(a) & static_cast<int>(b));
}
inline void operator|=(Channels& a, Channels b) {
    a = static_cast<Channels>(static_cast<int>(a) | static_cast<int>(b));
}
inline Channels operator~(Channels a) {
    return static_cast<Channels>(~static_cast<int>(a));
}
inline Channels operator>>(Channels a, int b) {
    return static_cast<Channels>(static_cast<int>(a) >> b);
}
inline Channels operator<<(Channels a, int b) {
    return static_cast<Channels>(static_cast<int>(a) << b);
}
inline void operator>>=(Channels& a, int b) {
    a = static_cast<Channels>(static_cast<int>(a) >> b);
}
inline void operator<<=(Channels& a, int b) {
    a = static_cast<Channels>(static_cast<int>(a) << b);
}

enum class Fctrls : uint32_t { // function control masks and values
    blank_mask =      0x00001, // turns off outputs
    dsprpt_mask =     0x00002, // auto display repeat, DC,BC,GS data updated async
    tmgrst_mask =     0x00004, // allows LAT to control timing (new data interrupts)
    espwm_mask =      0x00008,
    lodvlt_mask =     0x00030,
    lsdvlt_mask =     0x000c0,
    lattmg_mask =     0x00300,
    idmena_mask =     0x00400,
    idmrpt_mask =     0x00800,
    idmcur_mask =     0x03000,
    oldena_mask =     0x04000,
    psmode_mask =     0x38000,

    psmode_none =       0x00000,
    psmode_sclk =       0x08000,
    psmode_data =       0x10000,
    psmode_noclk =      0x20000,
    oldena_mode_0 =     0x00000,
    oldena_mode_1 =     0x04000,
    idmcur_mode_2ua =   0x00000,
    idmcur_mode_10ua =  0x01000,
    idmcur_mode_20ua =  0x02000,
    idmcur_mode_1ma =   0x03000,
    idmrpt_mode_0=      0x00000,
    idmrpt_mode_1=      0x00800,
    idmena_mode_0=      0x00000,
    idmena_mode_1=      0x00400,
    lattmg_mode_17 =    0x00000,
    lattmg_mode_33 =    0x00100,
    lattmg_mode_65 =    0x00200,
    lattmg_mode_129 =   0x00300,
    lsdvlt_mode_035 =   0x00000,
    lsdvlt_mode_045 =   0x00040,
    lsdvlt_mode_055 =   0x00080,
    lsdvlt_mode_065 =   0x000c0,
    lodvlt_mode_03v =   0x00000,
    lodvlt_mode_06v =   0x00010,
    lodvlt_mode_09v =   0x00020,
    lodvlt_mode_12v =   0x00030,
    espwm_mode_0  =     0x00000,
    espwm_mode_1  =     0x00008,
    tmgrst_mode_0  =    0x00000,
    tmgrst_mode_1  =    0x00004,
    dsprpt_mode_0 =     0x00000,
    dsprpt_mode_1 =     0x00002,
    blank_mode_0 =      0x00000,
    blank_mode_1 =      0x00001,

    empty_bits   =      0x00000,
    full_bits    =      0xfffff,
};

inline Fctrls operator|(Fctrls a, Fctrls b) {
    return static_cast<Fctrls>(static_cast<int>(a) | static_cast<int>(b));
}
inline Fctrls operator&(Fctrls a, Fctrls b) {
    return static_cast<Fctrls>(static_cast<int>(a) & static_cast<int>(b));
}
inline void operator|=(Fctrls& a, Fctrls b) {
    a = static_cast<Fctrls>(static_cast<int>(a) | static_cast<int>(b));
}
inline void operator&=(Fctrls& a, Fctrls b) {
    a = static_cast<Fctrls>(static_cast<int>(a) & static_cast<int>(b));
}
inline Fctrls operator~(Fctrls a) {
    return static_cast<Fctrls>(~static_cast<int>(a));
}

enum class DataKind { gsdata, controldata, none };

class Tlc5948 {
    public:
        void updateDcData(Channels,uint8_t);
        void updateBcData(uint8_t);
        void updateGsData(Channels,uint16_t);
        uint8_t pushGsData(uint16_t);
        void updateFctrlData(Fctrls);

        void exchangeData(DataKind);
        SidFlags getSidData(Channels&,Channels&,Channels&,bool = false);
        void startGsclk();
        void stopGsclk();
        void pulseLatch();
        Fctrls getFctrlBits();
        void printGsDataBuf();
        void printSpiBuf();
        void printCtrlDataBuf();
        void begin(void);

        Tlc5948();

    private:
        SidFlags sidStatus;
        Fctrls funcControlBits;
        uint8_t gsDataBuf[32];
        uint8_t ctrlDataBuf[32];
        uint8_t spiBuf[32];

};

inline void pulse_high(int pinNum) { // ___----___
    digitalWrite(pinNum,HIGH);
    asm volatile ("nop"); // NOP*4 -> 1/16Mhz * 4 = 62.5ns * 4 = 250ns
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    digitalWrite(pinNum,LOW);
}

inline void pulse_low(int pinNum) { // ---____---
    digitalWrite(pinNum,LOW);
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    digitalWrite(pinNum,HIGH);
}

inline void notifyGsData() {
    digitalWrite(SIN,LOW); // GS data MSB is low
    pulse_high(SCLK); // __-__
}

inline void notifyControlData() {
    digitalWrite(SIN,HIGH); // Control data MSB is high
    pulse_high(SCLK); // __-__
    digitalWrite(SIN,LOW);
}

inline void Tlc5948::startGsclk() {
    analogWrite(GSCLK,127); // PWM 490Hz at 50% -> square wave
}

inline void Tlc5948::stopGsclk() {
    analogWrite(GSCLK,0);
}

inline void Tlc5948::pulseLatch() {
    pulse_high(LAT);
}

inline void Tlc5948::printGsDataBuf() {
    for (int i = 0; i < 32; i++) {
        Serial.print("0x");
        Serial.print(gsDataBuf[i],HEX);
        Serial.print(" ");
    }
    Serial.println();
}

inline void Tlc5948::printSpiBuf() {
    for (int i = 0; i < 32; i++) {
        Serial.print("0x");
        Serial.print(spiBuf[i],HEX);
        Serial.print(" ");
    }
    Serial.println();
}

inline void Tlc5948::printCtrlDataBuf() {
    for (int i = 0; i < 32; i++) {
        Serial.print("0x");
        Serial.print(ctrlDataBuf[i],HEX);
        Serial.print(" ");
    }
    Serial.println();
}

inline Fctrls Tlc5948::getFctrlBits() {
    return funcControlBits;
}
#endif
