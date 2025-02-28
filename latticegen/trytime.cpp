#include <iostream>
#include <ctime>

int main() {
    // 获取当前时间
    time_t now = time(0);

    // 将时间转换为本地时间结构体
    tm* localTime = localtime(&now);

    // 使用 strftime 函数将日期格式化为 YYYYMMDD 格式
    char buffer[11]; // 用于存储格式化的日期字符串
    strftime(buffer, sizeof(buffer), "%Y%m%d", localTime);

    // 输出格式化后的日期
    std::cout << buffer << std::endl;
    return 0;
}