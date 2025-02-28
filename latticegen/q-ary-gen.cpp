// 包含必要的库
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <random>
#include <ctime>
#include <iomanip>
#include <fstream>

// 函数用于生成一个随机整数
int TimeRandomInt(){
   auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
   auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

   // 创建一个梅森 Twister 随机数生成器
   std::mt19937 generator(static_cast<unsigned int>(seed));

   // 定义随机整数的范围
   std::uniform_int_distribution<int> distribution(-100, 100); // 生成 -100 到 100 包含

   // 生成并输出一个随机整数
   int random_number = distribution(generator);

   return random_number;
}

// 函数用于打印当前时间的 YYYYMMDD 格式
std::string TimePrint(){
   // 获取当前时间
   time_t now = time(0);

   // 将时间转换为本地时间结构
   tm* localTime = localtime(&now);

   // 使用 strftime 将日期格式化为 YYYYMMDD-Time(24h-ver)
   char buffer[19]; // 存储格式化日期字符串的缓冲区
   strftime(buffer, sizeof(buffer), "%Y%m%d-%H%M%S", localTime);

   // 输出格式化的日期
   return buffer;
}

int main(){
   // 声明用于用户输入的变量
   std::string mod;
   int dim, k, modul;
   std::string entermod;

   // 初始化命令数量的变量
   int num_commands;

   // 提示用户输入并验证它
   std::cout << "请输入基类型: ";
   std::cin >> mod;
   std::cout << "请输入 dim 值: ";
   std::cin >> dim;
   std::cout << "请输入 k 值: ";
   std::cin >> k;
   std::cout << "模掉多少: ";
   std::cin >> modul;
   std::cout << "模数的输入模式, b or p or q: ";
   std::cin >> entermod;

   // 默认值，如果用户输入无效
   mod = "q";

   // 使用简短的条件和名称的变量名
   std::string condition_summary = " " + mod + " " + std::to_string(dim) + " " + std::to_string(k) + " " + std::to_string(modul) + " " + entermod + " ";
   std::string name_summary = TimePrint() + "-" + std::to_string(dim) + "-" + std::to_string(k) + "-" + std::to_string(modul) + "-";

   // 初始化总时间成本的变量
   double time = 0;

   // 检查用户输入是否无效并要求纠正如果必要
   if (std::cin.fail() || dim <= 0 || k <= 0 || modul <= 0){
       std::cerr << "错误: 无效输入数值" << std::endl;
       return 1;
   }

   //check entermod if ture if true input num_commands
   if (entermod == "b" || entermod == "p" || entermod == "q")
   {
        // 提示用户输入命令数量
        std::cout << "请输入迭代次数: ";
        std::cin >> num_commands;
   }
   else
   {
    std::cerr << "ERROR: 无效输入迭代次数" << std::endl;
    return 1;
   }
   

   // 遍历每个命令并执行 latticegen、FLATTER_LOG 和 FLATTER_LOG 命令
   for (int i = 0; i < num_commands; i++)
   {
       // 生成每次迭代的随机种子
       std::string seed_str = std::to_string(TimeRandomInt() + i);

       // 构造用于生成 latticegen 输出的命令
       std::string command = "latticegen -randseed " + seed_str + condition_summary + "> q-ary/original_basis/" + name_summary +std::to_string(i);

       // 执行命令并打印结果
       int result = system(command.c_str());
       
       if (result != 0){
        std::cerr << "FIRST COMMAND ERROR" << result << std::endl;
        return 1;//返回非0值表示程序失败
       }

       // 构造用于处理 latticegen 输出的第二个命令
       std::string sec_command = "FLATTER_LOG=./q-ary/log/log-" + name_summary + std::to_string(i) + " flatter -q < q-ary/original_basis/" + name_summary +std::to_string(i);

       //开始计时
       auto start = std::chrono::high_resolution_clock::now();

       // 执行第二个命令并打印结果
       int other_result = system(sec_command.c_str());

       //结束计时
       auto end = std::chrono::high_resolution_clock::now();

       // 打印第二个命令的结果
       if (other_result != 0){
           std::cerr << "SECOND COMMAND ERROR" << other_result << std::endl;
           return 1;
       }
       else{
       // 计算每个命令的执行时间
       std::chrono::duration<double> runtime = end - start;

       // 更新总时间成本
       time = time + runtime.count();
       }
   }

   // 输出总时间成本和平均时间成本, and write the info into the file name_summary+"TimeInfo"
   std::ofstream summary_file(name_summary + "TimeInfo");
   if (!summary_file.is_open()) {
       std::cerr << "CAN NOT OPEN THE FILE" << std::endl;
       return 1;
   }

   //write the INFO into the file
   summary_file << "q-ary basis [[I_(n-k) H], [0 q*I_k] ], n = " + std::to_string(dim) + ", k = " + std::to_string(k) + ". Enteries are randomly in [0," + std::to_string(modul) + entermod + "]. \n If char is 'b', then it first samples an integer q of bit-length b; if char is 'p', it does the same and updates q to the smallest (probabilistic) prime that is greater; if char is 'q', then it sets q to the provided value. It returns a 2 x 2 block matrix [[I, H], [0, q*I]], where H is (d-k) x k and uniformly random modulo q. These bases correspond to the SIS/LWE q-ary lattices (see [MR09]). Goldstein-Mayer lattices correspond to k=1 and q prime (see [GM03])." << std::endl;
   summary_file << "迭代次数：" << num_commands << std::endl;
   summary_file << "总时间成本: " << time << " 秒" << std::endl;
   summary_file << "平均时间成本: " << time / num_commands << " 秒" << std::endl;
   summary_file << "reference:\n [MR09] D. Micciancio and O. Regev. Post-Quantum Cryptography. Chapter of Lattice-based Cryptography, 147-191 (2009) \n [GM03] D. Goldstein and A. Mayer. On the equidistribution of Hecke points. Forum Mathematicum, 15:165–189 (2003)" << std::endl;

   //close the opened file
   summary_file.close();

   return 0;
}