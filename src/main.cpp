#include "crc/crc.hpp"

int main(int argc, char* argv[], char** env)
{
    std::string input;
    int index = 0;
    std::cout << "输入数据：";
    std::cin >> input;

    // 如果输入数据以0x开头，跳过0x
    if(input.substr(0, 2) == "0x" || input.substr(0, 2) == "0X")
    {
        index = 2;
    }

    std::vector<uint8_t> data;
    // 如果输入数据长度不是整数个字节，第一个字节补0
    if((input.size()-index)%2 != 0)
    {
        data.push_back(std::stoi(input.substr(index, 1), nullptr, 16));
        index += 1;
    }
    for (int i = index; i < input.size(); i += 2) {
        data.push_back(std::stoi(input.substr(i, 2), nullptr, 16));
    }

    std::string method;
    std::cout << "选择CRC校验方法(0: CRC8, 1:CRC16_XMODEM, 2:CRC16_CCITT)："
              << std::endl;
    std::cin >> method;
    int crc_method = std::stoi(method);
    if(crc_method < 0 || crc_method > 2)
    {
        std::cout << "输入错误" << std::endl;
        return -1;
    }

    crc crc((CRC_METHOD)crc_method);

    int crc_result = crc.calclateCRC(data);
    // 格式化输出，crc8 输出 2 位 16 进制，crc16 输出 4 位 16 进制
    if(crc_method == 0)
        printf("CRC校验结果：0x%02x\n", crc_result);
    else
        printf("CRC校验结果：0x%04x\n", crc_result);

    return 0;
}