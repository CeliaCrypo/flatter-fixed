我将给你一个基于cmake的linux运行的cpp项目，而我希望你能指导我如何修改源代码，为这个工程添加一个功能（具体我会在下面告诉你要求）。我想知道我应该告诉你哪些信息、以及应该以何种形式告诉你。比如说你或许想知道这个项目的文件结构是什么样子的，以及这个项目的源代码是什么样的。

.
├── CMakeLists.txt
├── CMakeSettings.json
├── COPYING.LESSER
├── README.md
├── apps
│   ├── CMakeLists.txt
│   └── flatter.cpp
├── cmake
│   ├── FindFPLLL.cmake
│   ├── FindGMP.cmake
│   └── FindMPFR.cmake
├── include
│   └── flatter
│       ├── computation_context.h
│       ├── data
│       │   ├── lattice
│       │   │   ├── lattice.h
│       │   │   └── profile.h
│       │   ├── lattice.h
│       │   ├── matrix
│       │   │   ├── matrix.h
│       │   │   └── matrix_data.h
│       │   └── matrix.h
│       ├── flatter.h
│       ├── monitor.h
│       ├── problems
│       │   ├── fused_qr_size_reduction.h
│       │   ├── fused_qr_sizered
│       │   │   ├── base.h
│       │   │   └── params.h
│       │   ├── lattice_reduction
│       │   │   ├── base.h
│       │   │   ├── goal.h
│       │   │   ├── params.h
│       │   │   └── sublattice_split.h
│       │   ├── lattice_reduction.h
│       │   ├── matrix_multiplication
│       │   │   └── base.h
│       │   ├── matrix_multiplication.h
│       │   ├── problem.h
│       │   ├── qr_factorization
│       │   │   └── base.h
│       │   ├── qr_factorization.h
│       │   ├── relative_size_reduction
│       │   │   ├── base.h
│       │   │   └── params.h
│       │   ├── relative_size_reduction.h
│       │   ├── size_reduction
│       │   │   └── base.h
│       │   └── size_reduction.h
│       ├── problems.h
│       └── workspace_buffer.h
├── scripts
│   ├── rsahighbits.sage
│   └── visualize_profile.py
├── src
│   ├── CMakeLists.txt
│   ├── computation_context.cpp
│   ├── data
│   │   ├── CMakeLists.txt
│   │   ├── lattice
│   │   │   ├── CMakeLists.txt
│   │   │   └── lattice.cpp
│   │   └── matrix
│   │       ├── CMakeLists.txt
│   │       ├── matrix.cpp
│   │       ├── matrix_data.cpp
│   │       ├── matrix_data_double.cpp
│   │       ├── matrix_data_int64.cpp
│   │       ├── matrix_data_mpfr.cpp
│   │       ├── matrix_data_mpz.cpp
│   │       └── workspace_buffer.cpp
│   ├── flatter.cpp
│   ├── math
│   │   ├── CMakeLists.txt
│   │   ├── matrix_tools.cpp
│   │   ├── matrix_tools.h
│   │   ├── mpfr_blas.cpp
│   │   ├── mpfr_blas.h
│   │   ├── mpfr_lapack.cpp
│   │   └── mpfr_lapack.h
│   ├── monitor.cpp
│   ├── problems
│   │   ├── CMakeLists.txt
│   │   ├── fused_qr_sizered
│   │   │   ├── CMakeLists.txt
│   │   │   ├── base.cpp
│   │   │   ├── columnwise.cpp
│   │   │   ├── columnwise.h
│   │   │   ├── columnwise_double.cpp
│   │   │   ├── columnwise_double.h
│   │   │   ├── fused_qr_size_reduction.cpp
│   │   │   ├── iterated.cpp
│   │   │   ├── iterated.h
│   │   │   ├── lazy_refine.cpp
│   │   │   ├── lazy_refine.h
│   │   │   ├── params.cpp
│   │   │   ├── seysen_refine.cpp
│   │   │   └── seysen_refine.h
│   │   ├── lattice_reduction
│   │   │   ├── CMakeLists.txt
│   │   │   ├── base.cpp
│   │   │   ├── fplll_impl.cpp
│   │   │   ├── fplll_impl.h
│   │   │   ├── goal.cpp
│   │   │   ├── heuristic_1.cpp
│   │   │   ├── heuristic_1.h
│   │   │   ├── heuristic_2.cpp
│   │   │   ├── heuristic_2.h
│   │   │   ├── heuristic_3.cpp
│   │   │   ├── heuristic_3.h
│   │   │   ├── irregular.cpp
│   │   │   ├── irregular.h
│   │   │   ├── lagrange.cpp
│   │   │   ├── lagrange.h
│   │   │   ├── latred_relative_sr.cpp
│   │   │   ├── latred_relative_sr.h
│   │   │   ├── lattice_reduction.cpp
│   │   │   ├── params.cpp
│   │   │   ├── proved_1.cpp
│   │   │   ├── proved_1.h
│   │   │   ├── proved_2.cpp
│   │   │   ├── proved_2.h
│   │   │   ├── proved_3.cpp
│   │   │   ├── proved_3.h
│   │   │   ├── recursive_generic.cpp
│   │   │   ├── recursive_generic.h
│   │   │   ├── schoenhage.cpp
│   │   │   ├── schoenhage.h
│   │   │   ├── sublattice_split.cpp
│   │   │   ├── sublattice_split_2.cpp
│   │   │   ├── sublattice_split_2.h
│   │   │   ├── sublattice_split_3.cpp
│   │   │   ├── sublattice_split_3.h
│   │   │   ├── threaded_3.cpp
│   │   │   └── threaded_3.h
│   │   ├── matrix_multiplication
│   │   │   ├── CMakeLists.txt
│   │   │   ├── aliased.cpp
│   │   │   ├── aliased.h
│   │   │   ├── base.cpp
│   │   │   ├── elementary_RRZ.cpp
│   │   │   ├── elementary_RRZ.h
│   │   │   ├── elementary_RRl.cpp
│   │   │   ├── elementary_RRl.h
│   │   │   ├── elementary_ZZl.cpp
│   │   │   ├── elementary_ZZl.h
│   │   │   ├── elementary_mpfr.cpp
│   │   │   ├── elementary_mpfr.h
│   │   │   ├── elementary_mpz.cpp
│   │   │   ├── elementary_mpz.h
│   │   │   ├── elementary_native.cpp
│   │   │   ├── elementary_native.h
│   │   │   ├── matrix_multiplication.cpp
│   │   │   ├── strassen.cpp
│   │   │   ├── strassen.h
│   │   │   ├── threaded.cpp
│   │   │   ├── threaded.h
│   │   │   ├── tri_matmul.cpp
│   │   │   └── tri_matmul.h
│   │   ├── problem.cpp
│   │   ├── qr_factorization
│   │   │   ├── CMakeLists.txt
│   │   │   ├── base.cpp
│   │   │   ├── blocked.cpp
│   │   │   ├── blocked.h
│   │   │   ├── eigen_impl.cpp
│   │   │   ├── eigen_impl.h
│   │   │   ├── householder_mpfr.cpp
│   │   │   ├── householder_mpfr.h
│   │   │   ├── qr_factorization.cpp
│   │   │   ├── threaded.cpp
│   │   │   └── threaded.h
│   │   ├── relative_size_reduction
│   │   │   ├── CMakeLists.txt
│   │   │   ├── base.cpp
│   │   │   ├── generic.cpp
│   │   │   ├── generic.h
│   │   │   ├── orthogonal.cpp
│   │   │   ├── orthogonal.h
│   │   │   ├── orthogonal_double.cpp
│   │   │   ├── orthogonal_double.h
│   │   │   ├── params.cpp
│   │   │   ├── relative_size_reduction.cpp
│   │   │   ├── triangular.cpp
│   │   │   └── triangular.h
│   │   └── size_reduction
│   │       ├── CMakeLists.txt
│   │       ├── base.cpp
│   │       ├── blocked.cpp
│   │       ├── blocked.h
│   │       ├── elementary_ZZ.cpp
│   │       ├── elementary_ZZ.h
│   │       ├── elementary_ll.cpp
│   │       ├── elementary_ll.h
│   │       └── size_reduction.cpp
│   └── profile.cpp
这是这个项目的文件结构

touch readme
我希望将原本的输出结果格，修改为输出运算过程中每一个格，并且注明每个格具体对应了哪个子算法。附件中是原本的算法的说明，对应了文件结构里的README.md

touch 美密会的论文
对了，或许你可以看看这个项目对应的论文。代码几乎是按照论文实现的。

主豹！如果种子界面输完种子按确认，不启程或不进战斗，退出后下一局种子号一定是之前的种子！包括每日挑战

4GA1FJTB2BNAH4

4H9RUW00ZRXPL

389G5AE6C86VH