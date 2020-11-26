// Arduino driver for VZ89TE air quality sensor
// DM1CR, Nov 26, 2020
//
#include "VZ89TE.h"

VZ89TE::VZ89TE(void) {
}

bool VZ89TE::begin(TwoWire &wirePort) {
    _i2cPort = &wirePort;
    return true;
}

bool VZ89TE::_sendCmd(const uint8_t * data, int len) {
    _i2cPort->beginTransmission(VZ89TE_ADDRESS);
    for (int cnt = 0; cnt < len; cnt++) {
            _i2cPort->write(data[cnt]);
    }
    if(_i2cPort->endTransmission() != 0)
        return false;
    return true;
}

bool VZ89TE::sendCmdGetRevision(void) {
    return _sendCmd(VZ89TE_CMD_GETREVISION, VZ89TE_CMD_LEN);
}

bool VZ89TE::sendCmdGetStatus(void) {
    return _sendCmd(VZ89TE_CMD_GETSTATUS, VZ89TE_CMD_LEN);
}

bool VZ89TE::sendCmdGetR0(void) {
    return _sendCmd(VZ89TE_CMD_GETR0, VZ89TE_CMD_LEN);
}

void VZ89TE::_calc_crc(uint8_t *d, int len) {
    _crc = 0;
    uint16_t sum = 0;
    for (int i=0; i<len; i++) {
        sum = _crc + d[i];
        _crc = sum & 0xff;
        _crc += sum/0x100;
    }
    _crc = 0xff-_crc;
}

int VZ89TE::_readResponse(void) {
    _i2cPort->requestFrom(VZ89TE_ADDRESS, VZ89TE_RESPONSE_LEN);
    if (_i2cPort->available()) {
        for (int i=0; i<VZ89TE_RESPONSE_LEN; i++) {
            _data[i] = _i2cPort->read();
        }
        _calc_crc(_data, VZ89TE_RESPONSE_LEN-1);
        if (_crc == _data[6]) {
            return VZ89TE_RESPONSE_LEN;
        }
        else {
            return VZ89TE_ERR_CRC;
        }
    }
    else {
        return VZ89TE_ERR_NO_RESPONSE;
    }
}

int VZ89TE::retrieveStatus(void) {
    sendCmdGetStatus();
    delay(VZ89DELAY_CMD_RESPONSE_MS);
    int ans = _readResponse();
    if( ans > 0) {
        _voc = (_data[0] - 13) * (1000.0/229); // ppb: 0 .. 1000
        _co2 = (_data[1] - 13) * (1600.0 / 229) + 400; // ppm: 400 .. 2000
        _res = 10.0 * (_data[4] + 256.0 * _data[3]) + 65536.0 * _data[2];
        _status = _data[5];
        return VZ89TE_OK;
    }
    else {
        return ans;
    }
}

int VZ89TE::retrieveRevision(void) {
    sendCmdGetRevision();
    delay(VZ89DELAY_CMD_RESPONSE_MS);
    int ans = _readResponse();
    if( ans > 0) {
        _year = _data[0];
        _month = _data[1];
        _day = _data[2];
        _rev = _data[3];
        return VZ89TE_OK;
    }
    else {
        return ans;
    }
}

int VZ89TE::retrieveR0(void) {
    sendCmdGetR0();
    delay(VZ89DELAY_CMD_RESPONSE_MS);
    int resp = _readResponse();
    if( resp > 0) {
        _r0 = _data[0] + 256 * _data[1];
        return VZ89TE_OK;
    }
    else {
        return resp;
    }
}

float VZ89TE::getCO2(void) {
    return _co2;
}

float VZ89TE::getVOC(void) {
    return _voc;
}

float VZ89TE::getRes(void) {
    return _res;
}

float VZ89TE::getR0(void) {
    return _r0;
}

uint8_t VZ89TE::getYear(void) {
    return _year;
}

uint8_t VZ89TE::getMonth(void) {
    return _month;
}

uint8_t VZ89TE::getDay(void) {
    return _day;
}

uint8_t VZ89TE::getRev(void) {
    return _rev;
}

uint8_t VZ89TE::getStatus(void) {
    return _status;
}
