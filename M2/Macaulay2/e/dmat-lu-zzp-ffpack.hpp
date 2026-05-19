/**
 * @file dmat-lu-zzp-ffpack.hpp
 * @brief Free-function interface (`namespace ffpackInterface`) routing `DMatZZpFFPACK` LU work into FFLAS-FFPACK.
 *
 * Declares the `ffpackInterface` namespace --- `rank`,
 * `determinant`, `rankProfile`, `solveLinear`, `inverse`, and
 * `nullSpace` over `DMatZZpFFPACK`. Each entry point delegates to
 * FFLAS-FFPACK, which represents Z/p entries as `double`s and
 * reinterprets the matrix as a `double` matrix so multiplications
 * dispatch into BLAS; the result is reduced modulo `p` afterwards.
 * For primes small enough that intermediate products fit in the
 * 53-bit mantissa (roughly `p <= 2^25` at typical matrix sizes)
 * this is by far the fastest dense Z/p path in the engine.
 *
 * The functions live in this file as bare declarations because the
 * `DMatLinAlg<M2::ARingZZpFFPACK>` specialisation in `dmat-lu.hpp`
 * forwards through them while their definitions sit in `dmat.cpp`,
 * keeping the FFLAS-FFPACK include surface contained.
 *
 * @see dmat-lu.hpp
 * @see dmat-lu-zzp-flint.hpp
 * @see aring-zzp-ffpack.hpp
 */

//////////////////////
// ZZpFFPACK /////////
//////////////////////
namespace ffpackInterface {
// Functions for DMatZZpFFPACK, in dmat.cpp
size_t rank(const DMatZZpFFPACK& A);

void determinant(const DMatZZpFFPACK& A, ZZpFFPACK::ElementType& result_det);

M2_arrayintOrNull rankProfile(const DMatZZpFFPACK& A, bool row_profile);

void rankProfile(const DMatZZpFFPACK& A,
                 bool row_profile,
                 std::vector<size_t>& result_profile);

bool solveLinear(const DMatZZpFFPACK& A,
                 const DMatZZpFFPACK& B,
                 DMatZZpFFPACK& X);

bool inverse(const DMatZZpFFPACK& A, DMatZZpFFPACK& result_inv);

size_t nullSpace(const DMatZZpFFPACK& A, DMatZZpFFPACK& result_nullspace);

};  // namespace ffpackInterface

template <>
class DMatLinAlg<M2::ARingZZpFFPACK>
{
 public:
  typedef M2::ARingZZpFFPACK RingType;
  typedef typename RingType::ElementType ElementType;
  typedef DMat<RingType> Mat;

 public:
  DMatLinAlg(const Mat& A) : mLU(A) {}
  size_t rank() { return ffpackInterface::rank(mLU); }
  void determinant(ElementType& result)
  {
    ffpackInterface::determinant(mLU, result);
  }

  void columnRankProfile(std::vector<size_t>& profile)
  {
    ffpackInterface::rankProfile(mLU, false, profile);
  }

  void matrixPLU(std::vector<size_t>& P, Mat& L, Mat& U)
  {
    //    std::cout << "calling matrixPLU ffpack\n";
    
    DMatLUinPlace<RingType> C(mLU);

    const Mat& LU = C.LUinPlace();

    LUUtil<RingType>::setUpperLower(LU, L, U);
    P = C.permutation();
  }

  bool solve(const Mat& B, Mat& X)
  {
    return ffpackInterface::solveLinear(mLU, B, X);
  }

  bool solveInvertible(const Mat& B, Mat& X)
  {
    return ffpackInterface::solveLinear(mLU, B, X);
  }

  bool inverse(Mat& X) { return ffpackInterface::inverse(mLU, X); }
  size_t kernel(Mat& X) { return ffpackInterface::nullSpace(mLU, X); }
 private:
  const Mat& mLU;  // reference to the original matrix
};

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
