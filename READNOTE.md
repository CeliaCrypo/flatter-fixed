# MAIN FUNC

## flatter::LatticeReduction.solve()

 problems/lattice_reduction.h, problems/lattice_reduciton/lattice_reduction.cpp

**InPUT object** : flatter::LatticeReduction

**OUTPUT** : 

### relative classes 相关的类（对象继承关系）

- class flatter::LatticerReduction : public LatticeReductionImpl::Base 

problems/lattice_reduction.h

- class Base : public Problem 

problems/lattice_reduction/base.h

- class Problem

problems/problem.h

- class ComputationContext (mainly used "cc")

computation_context.h

- class LatticeReductionParams (mainly used "p")

problems/lattice_reduction/params.h

## flatter::initialize() & flatter::finalize()

monitor.h, monitor.cpp

# 理论算法结构


**Algorithm 1**: Reduce

**Input**: Lattice basis $B$ of rank $n$ ; reduction quality $\alpha$.

**Output**: Unimodular matrix $U$ such that $BU$ is $\alpha$-lattice-reduced

