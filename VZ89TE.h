#ifndef __VZ89TE_H__
#define __VZ89TE_H__

#include "Arduino.h"
#include <Wire.h>

#define VZ89TE_ADDRESS 0x70
const uint8_t VZ89TE_CMD_GETSTATUS[] = {0xC,0,0,0,0,0xF3};
const uint8_t VZ89TE_CMD_GETREVISION[] = {0xD,0,0,0,0,0xF2};
const uint8_t VZ89TE_CMD_GETR0[] = {0x10,0,0,0,0,0xEF};
#define VZ89TE_DATE_CODE 0x0D
#define VZ89TE_CMD_LEN 6
#define VZ89TE_RESPONSE_LEN 7

#define VZ89TE_ERR_NO_RESPONSE 0
#define VZ89TE_ERR_CRC -1
#define VZ89TE_OK 1

#define VZ89DELAY_CMD_RESPONSE_MS 100

class VZ89TE
{
public:

    VZ89TE(void);

    bool begin(TwoWire &wirePort = Wire); //By default use Wire port

    bool sendCmdGetStatus(void);
    bool sendCmdGetRevision(void);
    bool sendCmdGetR0(void);

    int retrieveStatus(void);
    int retrieveRevision(void);
    int retrieveR0(void);
    
    uint8_t getAddress(void);
    uint8_t getStatus(void);
    float getCO2(void);
    float getVOC(void);
    float getRes(void);
    float getR0(void);
    uint8_t getYear(void);
    uint8_t getMonth(void);
    uint8_t getDay(void);
    uint8_t getRev(void);
    
private:    

    TwoWire *_i2cPort;

    float _co2 = 0.0;
    float _voc = 0.0;
    float _res = 0.0;
    float _r0 = 0.0;
    
    uint8_t _year = 0;
    uint8_t _month = 0;
    uint8_t _day = 0;
    uint8_t _rev = 0;

    uint8_t _status = 0;
    uint8_t _crc = 0;
    uint8_t _data[VZ89TE_RESPONSE_LEN];

    bool _sendCmd(const uint8_t * data, int len);
    int _readResponse(void);
    void _calc_crc(uint8_t *d, int len);

};

#endif
