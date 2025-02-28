#include <chrono>  // 引入时间处理库，用于计算运行时间
#include <flatter/data/matrix.h>  // 引入矩阵数据结构
#include <flatter/data/lattice.h>  // 引入格基数据结构
#include <flatter/problems.h>  // 引入与问题相关的定义
#include <flatter/computation_context.h>  // 引入计算上下文
#include <iostream>  // 引入输入输出流库
#include <cassert>  // 引入断言库，用于调试
#include <fstream>  // 引入文件流库
#include <vector>  // 引入向量库
#include <sstream>  // 引入字符串流库
#include <string.h>  // 引入字符串处理库

#include <flatter/monitor.h>  // 引入监控相关的头文件
#include <flatter/flatter.h>  // 引入 Flatter 算法的主头文件

// 显示帮助信息的函数
void print_help() {
  std::cout << "Usage: flatter [-h] [-v] [-alpha ALPHA | -rhf RHF | -delta DELTA] [-logcond LOGCOND] [INFILE [OUTFILE]]" << std::endl;
  std::cout << "\tINFILE -\tinput lattice (FPLLL format). Defaults to STDIN" << std::endl;
  std::cout << "\tOUTFILE -\toutput lattice (FPLLL format). Defaults to STDOUT" << std::endl;
  std::cout << "\t-h -\thelp message." << std::endl;
  std::cout << "\t-v -\tverbose output." << std::endl;
  std::cout << "\t-q -\tdo not output lattice." << std::endl;
  std::cout << "\t-p -\toutput profiles." << std::endl;
  std::cout << "\tReduction quality - up to one of the following. Default to RHF 1.0219" << std::endl;
  std::cout << "\t\t-alpha ALPHA -\tReduce to given parameter alpha" << std::endl;
  std::cout << "\t\t-rhf RHF -\tReduce analogous to given root hermite factor" << std::endl;
  std::cout << "\t\t-delta DELTA -\tReduce analogous to LLL with particular delta (approximate)" << std::endl;
  std::cout << "\t-logcond LOGCOND -\tBound on condition number." << std::endl;
}

std::shared_ptr<std::istream> inp_lat;  // 输入流指针
std::shared_ptr<std::ostream> out_lat;  // 输出流指针
bool show_help = false;  // 是否显示帮助
bool verbose = false;  // 是否启用详细输出
bool quiet = false;  // 是否安静模式（不输出）
bool red_quality_set = false;  // 是否设置了约简质量
bool logcond_set = false;  // 是否设置了条件数
bool show_profile = false;  // 是否显示配置文件
double alpha = 0.0;  // 约简质量参数
double logcond = 0.0;  // 条件数对数参数

// 解析命令行参数的函数
bool parse_args(int argc, char** argv) {
  if (argc < 1) {
    return false;  // 参数不足
  }

  int arg_ind = 1;  // 从第一个参数开始解析
  while (arg_ind < argc) {
    char* arg = argv[arg_ind];  // 当前参数

    // 处理不同的命令行选项
    if (strcmp(arg, "-h") == 0) {
      show_help = true;  // 设置帮助标志
      return true;
    } else if (strcmp(arg, "-v") == 0) {
      verbose = true;  // 启用详细模式
    } else if (strcmp(arg, "-q") == 0) {
      if (out_lat != nullptr) {
        std::cerr << "Cannot set output file in quiet (-q) mode" << std::endl;  // 如果在安静模式下无法设置输出文件
        return false;
      }
      quiet = true;  // 启用安静模式
    } else if (strcmp(arg, "-p") == 0) {
      show_profile = true;  // 启用显示profile !!!!!!!!! 源码内置了现实profile的地方
    } else if (strcmp(arg, "-alpha") == 0) {
      if (red_quality_set || arg_ind + 1 >= argc) {
        return false;  // 如果已经设置过约简质量或参数不足
      }
      arg_ind++;
      double alpha_param = atof(argv[arg_ind]);  // 获取 alpha 参数
      alpha = alpha_param;  // 设置 alpha
      red_quality_set = true;  // 标记已设置约简质量
    } else if (strcmp(arg, "-rhf") == 0) {
      if (red_quality_set || arg_ind + 1 >= argc) {
        return false;  // 参数不足或已设置过
      }
      arg_ind++;
      double rhf_param = atof(argv[arg_ind]);  // 获取 RHF 参数
      alpha = 2 * log2(rhf_param);  // 根据 RHF 设置 alpha
      red_quality_set = true;  // 标记已设置约简质量
    } else if (strcmp(arg, "-delta") == 0) {
      if (red_quality_set || arg_ind + 1 >= argc) {
        return false;  // 参数不足或已设置过
      }
      arg_ind++;
      double delta_param = atof(argv[arg_ind]);  // 获取 delta 参数
      alpha = pow(0.255 / delta_param, 2);  // 根据 delta 设置 alpha
      red_quality_set = true;  // 标记已设置约简质量
    } else if (strcmp(arg, "-logcond") == 0) {
      if (logcond_set || arg_ind + 1 >= argc) {
        return false;  // 参数不足或已设置过
      }
      arg_ind++;
      logcond = atof(argv[arg_ind]);  // 获取条件数
      logcond_set = true;  // 标记已设置条件数
    } else {
      // 处理输入文件和输出文件
      if (inp_lat == nullptr) {
        // 输入文件尚未设置
        if (strcmp(arg, "-") == 0) {
          inp_lat.reset(&std::cin, [](...){});
        } else {
          inp_lat.reset(new std::ifstream(arg));  // 从文件打开输入流
        }
      } else if (out_lat == nullptr) {
        // 输出文件尚未设置
        if (quiet) {
          std::cerr << "Cannot set output file in quiet (-q) mode" << std::endl;  // 在安静模式下无法设置输出文件
          return false;
        }
        if (strcmp(arg, "-") == 0) {
          out_lat.reset(&std::cout, [](...){});
        } else {
          out_lat.reset(new std::ofstream(arg));  // 从文件打开输出流
        }
      } else {
        std::cerr << "Too many input/output files specified" << std::endl;  // 输入输出文件过多
        return false;
      }
    }
    arg_ind++;
  }

  return true;  // 参数解析成功
}

int main(int argc, char** argv) {
  if (!parse_args(argc, argv)) {  // 解析参数
    print_help();  // 如果解析失败，显示帮助信息
    return -1;
  }

  if (show_help) {  // 如果请求帮助
    print_help();  // 显示帮助信息
    return 0;
  }

  if (inp_lat == nullptr) {  // 如果没有设置输入流
    inp_lat.reset(&std::cin, [](...){});
  }
  if (out_lat == nullptr) {  // 如果没有设置输出流
    out_lat.reset(&std::cout, [](...){});
  }
  if (!red_quality_set) {  // 如果没有设置约简质量
    alpha = 0.06250805094100162;  // 默认值对应 RHF 1.0219
  }
  /*
  上面是检测环境和输入参数
  下面是分不同输出模式进行输出
  */
  flatter::Lattice L;  // 创建格基对象
  (*inp_lat) >> L;  // 从输入流读取格基

  if (verbose) {  // 如果启用详细模式
    std::cerr << "Input lattice of rank " << L.rank() << " and dimension " << L.dimension() << std::endl;  // 输出格基的秩和维数
    unsigned int max_sz = 0;  // 记录最大位数
    flatter::MatrixData<mpz_t> dB = L.basis().data<mpz_t>();  // 获取格基数据
    for (unsigned int i = 0; i < dB.nrows(); i++) {  // 遍历行
      for (unsigned int j = 0; j < dB.ncols(); j++) {  // 遍历列
        unsigned int entry_sz = mpz_sizeinbase(dB(i,j), 2);  // 获取每个条目的位数
        max_sz = std::max(max_sz, entry_sz);  // 更新最大位数
      }
    }
    std::cerr << "Largest entry is " << max_sz << " bits in length." << std::endl;  // 输出最大位数
    if (L.basis().is_upper_triangular()) {  // 如果格基是上三角形
      flatter::Profile prof_in(L.rank());  // 创建输入配置文件
      double logdet = 0;  // 初始化行列式的对数
      for (unsigned int i = 0; i < L.rank(); i++) {  // 遍历秩
        signed long exp;  // 指数
        double v = mpz_get_d_2exp(&exp, dB(i,i));  // 获取对角线元素的值和指数
        prof_in[i] = log2(fabs(v)) + exp;  // 计算配置文件
        logdet += prof_in[i];  // 累加行列式的对数
      }
      std::cerr << "Lattice determinant is 2^(" << logdet << ")" << std::endl;  // 输出行列式
      if (show_profile) {  // 如果请求显示配置文件
        std::cerr << "Input profile:" << std::endl;  // 输出输入配置文件
        for (unsigned int i = 0; i < L.rank(); i++) {
          std::cerr << prof_in[i];
          if (i < L.rank() - 1) {
            std::cerr << " ";
          } else {
            std::cerr << std::endl;
          }
        }
      }
    } else {
      std::cerr << "Skipped determining input profile, as input is not lower-triangular." << std::endl;  // 如果输入不是下三角形，跳过计算配置文件
    }
    std::cerr << "Target reduction quality alpha = " << alpha << ", rhf = " << (pow(2, alpha / 2)) << std::endl;  // 输出目标约简质量
  }

  flatter::initialize();  // 初始化 Flatter 算法

  flatter::Matrix U(  // 创建矩阵 U
    flatter::ElementType::MPZ,
    L.rank(), L.rank()
  );
  flatter::ComputationContext cc;  // 创建计算上下文

  auto goal = flatter::LatticeReductionGoal::from_slope(L.rank(), alpha);  // 设定约简目标
  flatter::LatticeReductionParams params(L, U, 1.02);  // 创建约简参数
  params.goal = goal;  // 设置约简目标
  if (logcond_set) {  // 如果设置了条件数
    params.log_cond = logcond;  // 设置条件数
  }

  flatter::LatticeReduction latred(params, cc);  // 创建格基约简对象

  auto start = std::chrono::high_resolution_clock::now();  // 记录开始时间

  if (omp_get_active_level() == 0) {  // 如果没有活动的 OpenMP 级别
      #pragma omp parallel num_threads(cc.nthreads())  // 并行执行
      {
          #pragma omp single
          {
            latred.solve();  // 解决格基约简问题
          }
      }
  } else {
      #pragma omp taskgroup
      {
        latred.solve();  // 解决格基约简问题
      }
  }
 
  auto elapsed = std::chrono::high_resolution_clock::now() - start;  // 计算耗时
  long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  // 转换为微秒

  if (verbose) {
    std::cerr << "Reduction took " << (microseconds / 1000) << " milliseconds." << std::endl;  // 输出耗时
  }
  if (show_profile) {  // 如果请求显示配置文件
    std::cerr << "Output profile:" << std::endl;  // 输出输出配置文件
    for (unsigned int i = 0; i < L.rank(); i++) {
      std::cerr << L.profile[i];
      if (i < L.rank() - 1) {
        std::cerr << " ";
      } else {
        std::cerr << std::endl;
      }
    }
  }
  if (verbose) {
    double logdet = 0;  // 初始化行列式的对数
    unsigned int n = L.rank();  // 获取秩
    for (unsigned int i = 0; i < n; i++) {
      logdet += L.profile[i];  // 计算行列式的对数
    }
    // 计算实际的 RHF
    // |v_0| = RHF^n * det^(1/n)
    // log |v_0| = n * log(RHF) + logdet / n
    // log(RHF) = (log(|v_0|) - logdet / n) / n
    double log_rhf = (L.profile[0] - logdet / n) / n;  // 根据配置计算 log_rhf
    double rhf = pow(2, log_rhf);  // 计算 RHF

    // 计算实际的 alpha
    double drop = L.profile.get_drop();  // 获取降落值
    double alpha = drop / n;  // 计算 alpha
    std::cerr << "Achieved reduction quality alpha = " << alpha << ", rhf = " << rhf << std::endl;  // 输出实际的约简质量
  }

  if (!quiet) {
    (*out_lat) << L;  // 如果不是安静模式，输出结果
  }

  flatter::finalize();  // 结束 Flatter 算法

  return 0;  // 返回成功
}