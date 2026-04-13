#include "algo_plus.hpp"
// 引入输入输出库，用来打印结果
#include <iostream>

int main() {
    // 调用我们的加法函数
    int result = add(2, 3);
    // 打印结果
    std::cout << "2 + 3 = " << result << std::endl;
    return 0;
}