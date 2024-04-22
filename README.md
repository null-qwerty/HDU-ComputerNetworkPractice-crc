# CRC

这是计网实践课程的课程设计，实现了一个简单的计算 CRC 校验码程序，能够使用 `CRC8`, `CRC16_XMODEM` 和 `CRC16_CCITT` 计算输入数据的 CRC 校验码。

# 代码解析

## CRC 校验原理

CRC 校验是一种循环冗余校验，其原理是在发送端，计算数据的 CRC 校验码，附加在数据后面一起发送，接收端在接收到数据后，重新计算 CRC 校验码，与接收到的 CRC 校验码进行比较，如果一致，则说明数据传输正确，否则说明数据传输错误。

CRC 校验的计算方法是将数据与一个生成多项式进行模 2 除法，得到的余数就是 CRC 校验码。

## 计算 CRC 校验码 (main.cpp)

代码分为输入、计算、输出三部分。

+ 输入部分: 为保证能够处理多字节数据，先将数据读入 `std::string`，再用 `std::stoi()` 转成 `int`，保存在 `std::vector` 中。同时进行数据预处理，如当输入不满整数个字节时，在前面补 `0`。
```cpp
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
```

+ 计算部分: 通过 `class crc` 生成校验码。

```cpp
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
```

+ 输出部分: 使用 `printf()` 格式化输出，保证格式一致。

```cpp
    // 格式化输出，crc8 输出 2 位 16 进制，crc16 输出 4 位 16 进制
    if(crc_method == 0)
        printf("CRC校验结果：0x%02x\n", crc_result);
    else
        printf("CRC校验结果：0x%04x\n", crc_result);
```

## class crc

计算 crc 需要有多项式、宽度、初始值、结果异或值等参数，以及是否反转输入输出数据。针对不同的 crc 计算方法，这些参数不同。基于此，定义了一个 `crc` 类，用于计算 crc 校验码，其定义可以在 `include/crc.hpp` 中找到。

## crc 计算步骤

以 `crc::crc16()` 为例，介绍工作流程。

1. 预处理
   1. 如果输入数据需要反转，则反转输入数据。
   2. 因为输入为 `uint8_t`，需要将其转为 `uint16_t`，以便进行位运算。

```cpp
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
```

2. 计算
   1. 初始化 crc 寄存器为初始值。
   2. 每次取两个字节数据，与 crc 寄存器异或，然后进行位运算。
   3. 如果需要反转输出，最后输出前反转结果。

```cpp
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
```

**需要注意的是，输入数据和输出数据的反转是不同的，输入数据反转是将每个字节的位反转，输出数据反转是将整个 16 位数据的位反转。**

# 运行结果

```shell
./crc
输入数据：0x12345678
选择CRC校验方法(0: CRC8, 1:CRC16_XMODEM, 2:CRC16_CCITT)：
1
CRC校验结果：0xb42c
```