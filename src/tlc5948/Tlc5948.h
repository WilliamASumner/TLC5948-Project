#ifndef TLC5948_LIB_H
#define TLC5948_LIB_H
#include <SPI.h>

// pin assignments; todo replace with enum maybe
#ifdef ARDUINO_TEENSY41 // Teensy version
int const SIN = 3;   // serial data input (to Tlc5948)
int const SOUT = 4;  // serial data output (from Tlc5948) 
int const SCLK = 5;  // serial data clock
int const LAT = 6;   // latch control
int const GSCLK = 7; // pwm clock
#else // assume Arduino Nano
int const SIN = 3;   // serial data input (to Tlc5948)
int const SOUT = 4;  // serial data output (from Tlc5948) 
int const SCLK = 5;  // serial data clock
int const LAT = 6;   // latch control
int const GSCLK = 7; // pwm clock
#endif // ifdef ARDUINO_TEENSY41

// SPI settings
uint32_t const SPI_SPEED = 33000000; // 33mhz listed on data sheet
unsigned int const BIT_ORDER = MSBFIRST;
unsigned int const SPI_MODE = SPI_MODE0; // todo check this
int const NUM_CHANNELS = 16;

// led open, led short, output leakage, iref short flag, pre-thermal warning, thermal error flag
enum class SidFlags { LOD=1,LSD=2,OLD=4,ISF=8,PTW=16,TEF=32,NONE=0, BADPARSE=64 };

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
    out0 =   0x0001,
    out1 =   0x0002,
    out2 =   0x0004,
    out3 =   0x0008,
    out4 =   0x0010,
    out5 =   0x0020,
    out6 =   0x0040,
    out7 =   0x0080,
    out8 =   0x0100,
    out9 =   0x0200,
    out10 =  0x0400,
    out11 =  0x0800,
    out12 =  0x1000,
    out13 =  0x2000,
    out14 =  0x4000,
    out15 =  0x8000,
    even =   0xaaaa,
    odd =    0x5555,
    all =    0xffff,
    upper8 = 0xff00,
    lower8 = 0x00ff,
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
    BLANK_MASK =      0x00001, // turn off outputs
    DSPRPT_MASK =     0x00002, // auto display repeat
    TMGRST_MASK =     0x00004, // identical to blank
    ESPWM_MASK =      0x00008,
    LODVLT_MASK =     0x00030,
    LSDVLT_MASK =     0x000c0,
    LATTMG_MASK =     0x00300,
    IDMENA_MASK =     0x00400,
    IDMRPT_MASK =     0x00800,
    IDMCUR_MASK =     0x03000,
    OLDENA_MASK =     0x04000,
    PSMODE_MASK =     0x38000,

    psmode_none =        0x00000,
    psmode_sclk =        0x08000,
    psmode_data =        0x10000,
    psmode_noclk =       0x20000,
    oldena_mode_0 =      0x00000,
    oldena_mode_1 =      0x04000,
    idmcur_mode_2ua =    0x00000,
    idmcur_mode_10ua =   0x01000,
    idmcur_mode_20ua =   0x02000,
    idmcur_mode_1ma =    0x03000,
    idmrpt_mode_0=       0x00000,
    idmrpt_mode_1=       0x00800,
    idmena_mode_0=       0x00000,
    idmena_mode_1=       0x00400,
    lattmg_mode_17 =     0x00000,
    lattmg_mode_33 =     0x00100,
    lattmg_mode_65 =     0x00200,
    lattmg_mode_129 =    0x00300,
    lsdvlt_mode_035 =    0x00000,
    lsdvlt_mode_045 =    0x00040,
    lsdvlt_mode_055 =    0x00080,
    lsdvlt_mode_065 =    0x000c0,
    lodvlt_mode_03 =     0x00000,
    lodvlt_mode_06 =     0x00010,
    lodvlt_mode_09 =     0x00020,
    lodvlt_mode_12 =     0x00030,
    espwm_mode_0  =      0x00000,
    espwm_mode_1  =      0x00008,
    tmgrst_mode_0  =     0x00000,
    tmgrst_mode_1  =     0x00004,
    dsprpt_mode_0 =      0x00000,
    dsprpt_mode_1 =      0x00002,
    blank_mode_0 =       0x00000,
    blank_mode_1 =       0x00001,
    empty_bits   =       0x00000,
};

inline Fctrls operator|(Fctrls a, Fctrls b) {
    return static_cast<Fctrls>(static_cast<int>(a) | static_cast<int>(b));
}
inline Fctrls operator&(Fctrls a, Fctrls b) {
    return static_cast<Fctrls>(static_cast<int>(a) & static_cast<int>(b));
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
        void updateDcData(Channels channelMask, uint8_t value);
        void updateBcData(uint8_t value);
        void updateGsData(Channels channelMask, unsigned int value);
        void updateFctrlData(Fctrls f);

        void exchangeData(DataKind kind);
        SidFlags parseErrorData();
        void init(void);

        Tlc5948();

    private:
        SidFlags sidStatus;
        Fctrls funcControlBits;
        DataKind lastExchangeType;
        uint8_t gsDataBuf[32];
        uint8_t controlDataBuf[32];

};

inline void pulse_hilo(int pinNum) {
    digitalWrite(pinNum,HIGH);
    asm volatile ("nop");
    digitalWrite(pinNum,LOW);
}

inline void pulse_lohi(int pinNum) {
    digitalWrite(pinNum,LOW);
    asm volatile ("nop");
    digitalWrite(pinNum,HIGH);
}

inline void notifyGsData() {
    digitalWrite(SIN,LOW); // GS data MSB is low
    pulse_hilo(SCLK);
}

inline void notifyControlData() {
    digitalWrite(SIN,HIGH); // Control data MSB is high
    pulse_hilo(SCLK);
    digitalWrite(SIN,LOW);
}


/*
 * Explanation of TLC5948 data + signals
 */

// serial input structure (to tlc5948)
/*
   bit number
   function

bit: 255 .. 0
    

    0..255
    out0,out1,... in 16 bit increments

    control data (lower 137 bits of common register used)
bit: 136..119  118..112  111..105  104..98 97..91  90..84  (112 bits)  27..21  20..14  13..7 6..0
fnc: func ctrl  bcdata   dcdata15  dcdata14  ....                                            dcdata0

     func ctrl (18 bits)
bit: 136..134    133   132..131    130    129   128..127 126..125 124..123   122     121    120    119
fnc: psm sel     olden idm cur sel idmrpt idmen lattmg   lsdvolt  lodvolt    espwm   tmgrst dsprpt blank
  
 */

// sid data structure of 257-bit serial output (from tlc5948)
/*
bit: 256       255..248  247..240  239..232  231..224  223..216  215..208  207..200  199..192  191..184 183..176
fnc: no data   reserved  15:8 lod  reserved  7:0 lod   reserved  15:8 lsd  reserved  7:0 lsd   reserved 15:8 old

bit: 175..168  167..160  159..152  151       150       149       148...0
fnc: reserved  7:0 old   reserved  tef       ptw       isf       reserved
 */



#endif
