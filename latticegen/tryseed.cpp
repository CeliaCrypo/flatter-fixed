#include<iostream>
#include<chrono>
#include<random>

int TimeRandomInt(){
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

    // 创建随机数生成器
    std::mt19937 generator(static_cast<unsigned int>(seed));

    // 定义随机数的范围
    std::uniform_int_distribution<int> distribution(-100, 100); // 生成1到100之间的随机整数

    // 生成并输出随机整数
    int random_number = distribution(generator);

    return random_number;
}

int main() {
    std::cout << TimeRandomInt() << std::endl;
    return 0;
}