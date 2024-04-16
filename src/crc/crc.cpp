#include "crc/crc.hpp"

crc::crc(CRC_METHOD method)
    : method(method)
{
    if (method == CRC_METHOD::CRC8) {
        reverse_input = false;
        reverse_output = false;
    } else if (method == CRC_METHOD::CRC16_XMODEM) {
        reverse_input = false;
        reverse_output = false;
    } else if (method == CRC_METHOD::CRC16_CCITT) {
        reverse_input = true;
        reverse_output = true;
    }
}

int crc::calclateCRC(std::vector<uint8_t> data)
{
    if (method == CRC_METHOD::CRC8) {
        return crc8(data);
    } else if (method == CRC_METHOD::CRC16_XMODEM) {
        return crc16(data);
    } else if (method == CRC_METHOD::CRC16_CCITT) {
        return crc16(data);
    } else
        return 0;
}

int crc::crc8(std::vector<uint8_t> data)
{
    uint8_t crc = crc8_init;
    for (auto byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ crc8_P;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

int crc::crc16(std::vector<uint8_t> data)
{
    // 反转输入
    if (reverse_input) {
        for (auto &byte : data) {
            byte = reverseData(byte);
        }
    }
    // 将数据以两个字节划分，不足在前面补0
    std::vector<uint16_t> data16;
    if (data.size() % 2 != 0) {
        data16.push_back(0 << 8 | data[0]);
        for (int i = 1; i < data.size(); i += 2) {
            data16.push_back(data[i] << 8 | data[i + 1]);
        }
    } else {
        for (int i = 0; i < data.size(); i += 2) {
            data16.push_back(data[i] << 8 | data[i + 1]);
        }
    }

    uint16_t crc = crc16_init;
    for (auto byte : data16) {
        crc ^= byte;
        for (int i = 0; i < 16; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ crc16_P;
            } else {
                crc = crc << 1;
            }
        }
    }
    // 输出反转
    if (reverse_output) {
        crc = (crc & 0x0001) << 15 | (crc & 0x0002) << 13 |
              (crc & 0x0004) << 11 | (crc & 0x0008) << 9 | (crc & 0x0010) << 7 |
              (crc & 0x0020) << 5 | (crc & 0x0040) << 3 | (crc & 0x0080) << 1 |
              (crc & 0x0100) >> 1 | (crc & 0x0200) >> 3 | (crc & 0x0400) >> 5 |
              (crc & 0x0800) >> 7 | (crc & 0x1000) >> 9 | (crc & 0x2000) >> 11 |
              (crc & 0x4000) >> 13 | (crc & 0x8000) >> 15;
    }
    return crc;
}

uint8_t crc::reverseData(uint8_t data)
{
    return (data & 0x01) << 7 | (data & 0x02) << 5 | (data & 0x04) << 3 |
           (data & 0x08) << 1 | (data & 0x10) >> 1 | (data & 0x20) >> 3 |
           (data & 0x40) >> 5 | (data & 0x80) >> 7;
}