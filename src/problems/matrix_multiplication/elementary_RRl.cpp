#include "elementary_RRl.h"

#include <cassert>

namespace flatter {
namespace MatrixMultiplicationImpl {

const std::string ElementaryRRl::impl_name() {return "ElementaryRRl";}

ElementaryRRl::ElementaryRRl(const Matrix& C, const Matrix& A, const Matrix& B,
                       bool accumulate_c,
                       const ComputationContext& cc) :
    Base(C, A, B, accumulate_c, cc)
{
    _is_configured = false;
    configure(C, A, B, accumulate_c, cc);
}

ElementaryRRl::~ElementaryRRl() {
    unconfigure();
}

void ElementaryRRl::unconfigure() {
    assert(_is_configured);

    delete wsb;
    _is_configured = false;
}

void ElementaryRRl::configure(const Matrix& C, const Matrix& A, const Matrix& B,
                           bool accumulate_c,
                           const ComputationContext& cc) {
    assert(C.type() == ElementType::MPFR);
    assert(A.type() == ElementType::MPFR);
    assert(B.type() == ElementType::INT64);
    assert(!C.is_transposed());
    assert(!cc.is_threaded());
    
    if (_is_configured) {
        //unconfigure();
        Base::configure(C, A, B, accumulate_c, cc);
        wsb->set_precision(prec);
        return;
    }

    if (!C.is_transposed()) {
        Base::configure(C, A, B, accumulate_c, cc);
    } else {
        Base::configure(C.transpose(), B.transpose(), A.transpose(), accumulate_c, cc);
    }

    dC = this->C.data<mpfr_t>();
    dA = this->A.data<mpfr_t>();
    dB = this->B.data<int64_t>();

    rnd = mpfr_get_default_rounding_mode();
    unsigned int sz_needed = 2;
    wsb = new WorkspaceBuffer<mpfr_t>(sz_needed, prec);
    _is_configured = true;
}

void ElementaryRRl::solve() {
    log_start();

    gemm();

    log_end();
}

void ElementaryRRl::gemm() {
    unsigned int lda = dA.stride();
    unsigned int ldb = dB.stride();

    if (!dA.is_transposed() && !dB.is_transposed()) {
        gemm_xx(lda, 1, ldb, 1);
    } else if (!dA.is_transposed() && dB.is_transposed()) {
        gemm_xx(lda, 1, 1, ldb);
    } else if (dA.is_transposed() && !dB.is_transposed()) {
        gemm_xx(1, lda, ldb, 1);  
    } else {
        gemm_xx(1, lda, 1, ldb);
    }
}

void ElementaryRRl::gemm_xx(unsigned int adr, unsigned int adc, unsigned int bdr, unsigned int bdc) {
    mpfr_t* work = wsb->walloc(2);
    mpfr_t& prod = work[0];
    mpfr_t& sum = work[1];

    mpfr_t* C = this->dC.get_data();
    mpfr_t* A = this->dA.get_data();
    int64_t* B = this->dB.get_data();
    unsigned int ldc = this->dC.stride();

    // C = alpha * A^T . B + beta * C
    for (unsigned int i = 0; i < m; i += 1) {
        for (unsigned int j = 0; j < n; j += 1) {
            mpfr_set_zero(sum, 0);
            for (unsigned int l = 0; l < k; l += 1) {
                mpfr_mul_si(prod, A[i*adr + l*adc], B[l*bdr + j*bdc], rnd);
                mpfr_add(sum, sum, prod, rnd);
            }
            if (_accumulate_C) {
                mpfr_add(C[i*ldc + j], C[i*ldc + j], sum, rnd);
            } else {
                mpfr_set(C[i*ldc + j], sum, rnd);
            }
        }
    }

    wsb->wfree(work, 2);
}

}
}