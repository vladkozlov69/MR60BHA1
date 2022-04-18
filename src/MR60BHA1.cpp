#include "MR60BHA1.h"

MR60BHA1::MR60BHA1(Stream * data, Print * debug)
{
    m_Stream = data;
    _debug = debug;
}

// Non-blocking function for parse response.
bool MR60BHA1::read()
{
    loop();
    return _status == STATUS_OK;
}

// Blocking function for parse response. Default timeout is 1s.
bool MR60BHA1::readUntil(uint16_t timeout)
{
    uint32_t start = millis();
    do
    {
        loop();
        if (_status == STATUS_OK) break;
    } while (millis() - start < timeout);

    return _status == STATUS_OK;
}

void MR60BHA1::loop()
{
    _status = STATUS_WAITING;
    if (m_Stream->available())
    {
        uint8_t ch = m_Stream->read();

        // if (_debug)
        // {
        //     _debug->print(_index);
        //     _debug->print("\t");
        //     _debug->println(ch, HEX);
        // }

        if (_index < sizeof(_payload))
        {
             _payload[_index] = ch;
        }

        switch (_index)
        {
            case 0:
                if (ch != 0x53)
                {
                    if(_debug)
                    {
                        _debug->println("Wrong packet header signature (0x53)");
                    }
                    _index = 0;
                    return;
                }

                break;

            case 1:
                if (ch != 0x59)
                {
                    if(_debug)
                    {
                        _debug->println("Wrong packet header signature (0x59)");
                    }
                    _index = 0;
                    return;
                }

                break;

            case 2:
                _control_char = ch;
                break;

            case 3:
                _command_char = ch;
                break;

            case 4:
                _dataLen = (ch << 8);
                break;

            case 5:
                _dataLen |= ch;
                // Unsupported sensor, different frame length, transmission error e.t.c.
                if (_dataLen < 1 || _dataLen > sizeof(_payload))
                {
                    if(_debug)
                    {
                        _debug->print("Unsupported sensor, dataLen=");
                        _debug->println(_dataLen);
                    }
                    _index = 0;
                    return;
                }
                break;

            default:
                if (_index == sizeof(MR60BHA1_FRAME_HEADER) + _dataLen) // Checksum 
                {
                    _checksum = ch;
                }
                else if (_index == sizeof(MR60BHA1_FRAME_HEADER) + _dataLen + 1) // 0x54
                {
                     if (ch != 0x54)
                    {
                        if(_debug)
                        {
                            _debug->println("Wrong packet footer signature");
                        }
                        _index = 0;
                        return;
                    }
                    break;
                }
                else if (_index == sizeof(MR60BHA1_FRAME_HEADER) + _dataLen + 2) // 0x43
                {
                    // if(_debug)
                    // {
                    //     _debug->print("<< ");
                    //     dump(_payload, _dataLen + sizeof(MR60BHA1_FRAME_HEADER) + 3);
                    // }

                    if (ch != 0x43)
                    {
                        if(_debug)
                        {
                            _debug->println("Wrong packet footer signature");
                        }
                        _index = 0;
                        return;
                    }
                    _status = STATUS_OK;
                    _index = 0;
                    return;
                }

                break;
        }

        _index++;
    }
}

MR60BHA1_PAYLOAD * MR60BHA1::getPayload()
{
    return (MR60BHA1_PAYLOAD * )&_payload;
}

void MR60BHA1::dump(const unsigned char* data, const int len)
{
    for (int i=0; i<len; i++) 
    {
        char nibble, hexchar;

        nibble = (data[i] >> 4) & 0x0f;
        hexchar = nibble <= 9 ? '0' + nibble : 'A' + (nibble-10);
        _debug->print(hexchar);

        nibble = data[i] & 0x0f;
        hexchar = nibble <= 9 ? '0' + nibble : 'A' + (nibble-10);

        _debug->print(hexchar);
        _debug->print(' ');
    }
    _debug->println("");
}

