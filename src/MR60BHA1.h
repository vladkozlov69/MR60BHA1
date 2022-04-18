#ifndef _MR60BHA1_H_
#define _MR60BHA1_H_

#include <Arduino.h>


struct MR60BHA1_FRAME_HEADER 
{
    byte start_code_L = 0x53;
    byte start_code_H = 0x59;
    byte control_char;
    byte command_char;
    byte data_len_L;
    byte data_len_H;
} __attribute__((packed));



constexpr int MR60BHA1_MAX_DATA_SIZE = 24 + sizeof(MR60BHA1_FRAME_HEADER);

struct MR60BHA1_PAYLOAD
{
    MR60BHA1_FRAME_HEADER header;
    uint8_t payload[MR60BHA1_MAX_DATA_SIZE];

} __attribute__((packed));

class MR60BHA1
{
private:
    enum STATUS { STATUS_WAITING, STATUS_OK };
    Stream * m_Stream;
    Print * _debug;
    STATUS _status;
    uint8_t _control_char;
    uint8_t _command_char;
    uint8_t _payload[MR60BHA1_MAX_DATA_SIZE];
    uint8_t _index = 0;
    uint16_t _dataLen;
    uint8_t _checksum;
    uint8_t _calculatedChecksum;
private:
    bool doSend(unsigned char * buffer, int len);
    void dump(const unsigned char* data, const int len);
    void loop();
protected:
public:
    static const uint16_t SINGLE_RESPONSE_TIME = 1000;
    MR60BHA1(Stream * data, Print * debug);
    bool read();
    bool readUntil(uint16_t timeout = SINGLE_RESPONSE_TIME);
    MR60BHA1_PAYLOAD * getPayload();
    uint16_t getFrameLength() { return _dataLen; }
    uint8_t getByteData(int position = 0) { return _payload[sizeof(MR60BHA1_FRAME_HEADER) + position]; }
    uint16_t getWordData() { return getByteData(0) << 8 | getByteData(1); }
};


#endif