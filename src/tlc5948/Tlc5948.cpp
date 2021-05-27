#include "Tlc5948.h"

Tlc5948::Tlc5948() {
    sidStatus = SidFlags::NONE;
    funcControlBits = Fctrls::empty_bits;
    lastExchangeType = DataKind::none;
    for (int i = 0; i < 32; i++) {
        gsDataBuf[i] = 0;
        controlDataBuf[i] = 0;
    }
}

// dot correction data, 7 bits per channel, 0 to 100%
void Tlc5948::updateDcData(Channels channelMask, uint8_t value) {
    value &= 0x7f;
    int bytesRequired = NUM_CHANNELS*7/8;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        if ((channelMask & Channels::out0) != Channels::out0) {// don't write byte to selected Channels
            channelMask >>= 1;
            continue; 
        }
        // writing 7 bits!
        int bitnum= i * 7;
        int bytenum= bitnum / 8 + 1;
        int align = bitnum % 8;
        controlDataBuf[bytesRequired-1-bytenum] = (value >> (8-align)) & 0xff;
        controlDataBuf[bytesRequired-bytenum] &=  ~(0xff << align); // 0 out bits
        controlDataBuf[bytesRequired-bytenum] |=  (value << align) & 0xff;
        channelMask >>= 1;
    }

    if (lastExchangeType == DataKind::controldata)
        lastExchangeType = DataKind::none; // invalidated error data
}

// Global brightness control data, 7 bits for all Channels
void Tlc5948::updateBcData(uint8_t value) {
    value &= 0x7f;
    int bytesRequired = NUM_CHANNELS * 7 / 8;
    controlDataBuf[bytesRequired] = value;

    if (lastExchangeType == DataKind::controldata)
        lastExchangeType = DataKind::none; // invalidated error data
}

// Function Control, 18 bits
void Tlc5948::updateFctrlData(Fctrls f) {
    unsigned long fbits = static_cast<unsigned long>(f);
    int bytesRequired = NUM_CHANNELS * 7 / 8;
    controlDataBuf[bytesRequired+1] = fbits & 0xff;
    controlDataBuf[bytesRequired+2] = (fbits >> 8) & 0xff;
    controlDataBuf[bytesRequired+3] = (fbits >> 16) & 0x03;

    if (lastExchangeType == DataKind::controldata)
        lastExchangeType = DataKind::none; // invalidated error data
}

// greyscale, pwm data, 16 bits per channel
// when blank bit of gs control reg is set (1), output is all 0's
// blank is set to 1 on startup, must write gs data before setting blank to 0
void Tlc5948::updateGsData(Channels channelMask, unsigned int data) {
    data &= 0xffff;
    for (int i = NUM_CHANNELS-1; i >= 0; i--) {
        if ((channelMask & Channels::out0) != Channels::out0) {
            channelMask >>= 1;
            continue;
        }
        gsDataBuf[i*2] = data & 0xFF;
        gsDataBuf[i*2-1] = (data >> 8) & 0xFF;
        channelMask >>= 1;
    }
    if (lastExchangeType == DataKind::gsdata)
        lastExchangeType = DataKind::none; // invalidated error data
}

// send data from either ctrl buff or gs data buff
void Tlc5948::exchangeData(DataKind type) {
    SPI.beginTransaction(SPISettings(SPI_SPEED,BIT_ORDER,SPI_MODE));
    switch (type) {
        case DataKind::gsdata:
            notifyGsData(); // send 0
            SPI.transfer(gsDataBuf,32);
            break;
        case DataKind::controldata:
            notifyControlData(); // send 1
            SPI.transfer(controlDataBuf,32); // (although 137 bits, need 1st bit to be 1)
            break;
        default:
            break;
    }
    SPI.endTransaction(); 
    lastExchangeType = type;
}

// Quick and dirty error data parsing
SidFlags Tlc5948::parseErrorData() {
    if (lastExchangeType == DataKind::none)
        return SidFlags::BADPARSE;
    SidFlags flags = SidFlags::NONE;
    uint8_t *dataBuf = lastExchangeType == DataKind::controldata ? controlDataBuf : gsDataBuf;

    for (int i = 18; i < 32; i+=2) {
        uint8_t byte = dataBuf[i];
        if (byte == 0 && i != 18) // byte 18 contains reserved bits
            continue;
        switch(i) {
            case 18:
                if ((byte >> 5) & 0x1)
                    flags |= SidFlags::ISF; // IREF is shorted
                if ((byte >> 6) & 0x1) 
                    flags |= SidFlags::PTW; // Pre-thermal warning
                if ((byte >> 7) & 0x1) 
                    flags |= SidFlags::TEF; // Thermal error flag
                break;
            case 20:
            case 22:
                flags |= SidFlags::OLD; // Output leakage detected
                break;
            case 24:
            case 26:
                flags |= SidFlags::LSD; // LED short detected
                break;
            case 28:
            case 30:
                flags |= SidFlags::LOD; // LED open detected
                break;
        }
    }
    return flags;
}



void Tlc5948::init() {
    // driver must first send dot correction data before it will turn on
    // pin assignments
    pinMode(SIN,OUTPUT); // serial data input
    pinMode(SOUT,INPUT);  // serial data output (of Tlc5948 not mcu)
    pinMode(SCLK,OUTPUT);  // serial data clock
    pinMode(LAT,OUTPUT);   // latch control
    pinMode(GSCLK,OUTPUT); // pwm clock

    updateGsData(Channels::all,0xBEAD);
    //exchangeData(DataKind::gsdata);

    // Example Channels
    Channels workingChannels = Channels::out0 | Channels::out5 | Channels::out15;

    updateDcData(Channels::all,0x00);
    updateDcData(workingChannels,0x7f); 
    updateBcData(0xff);

    Fctrls funcControls = Fctrls::blank_mode_1    |
        Fctrls::dsprpt_mode_1   |
        Fctrls::tmgrst_mode_0   |
        Fctrls::espwm_mode_0    |
        Fctrls::lodvlt_mode_12  |
        Fctrls::lsdvlt_mode_065 |
        Fctrls::lattmg_mode_17  |
        Fctrls::idmcur_mode_2ua |
        Fctrls::psmode_noclk;

    updateFctrlData(funcControls);
    //exchangeData(DataKind::controldata);

}
