#pragma once

#include <iostream>
#include <vector>

enum CRC_METHOD { CRC8 = 0, CRC16_XMODEM, CRC16_CCITT };

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

class crc {
private:
    CRC_METHOD method;      ///< crc 方法
    uint8_t crc8_P = 0x07;  ///< 8 位 crc 除数
    uint8_t crc8_init = 0x00;   ///< 8 位 crc 初始值
    uint16_t crc16_P = 0x1021;  ///< 16 位 crc 除数
    uint16_t crc16_init = 0x0000;   ///< 16 位 crc 初始值

    bool reverse_input, reverse_output; ///< 输入、输出翻转

    /**
     * @brief 反转一个字节的数据
     * 
     * @param data 反转前的数据
     * @return uint8_t 反转后的数据
     */
    uint8_t reverseData(uint8_t data);

public:
    /**
     * @brief crc 构造函数
     * 
     * @param method crc 方法
     */
    crc(CRC_METHOD method);
    /**
     * @brief 计算 crc
     * 
     * @param data 输入数据
     * @return int crc
     */
    int calclateCRC(std::vector<uint8_t> data);
    /**
     * @brief 计算 8 位 crc
     * 
     * @param data 输入数据
     * @return int crc
     */
    int crc8(std::vector<uint8_t> data);
    /**
     * @brief 计算 16 位 crc
     * 
     * @param data 输入数据
     * @return int crc
     */
    int crc16(std::vector<uint8_t> data);
};