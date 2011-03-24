/**
 * @file src/cxx/sp/test/fftfctfftpack_blitz.cc
 * @author <a href="mailto:Laurent.El-Shafey@idiap.ch">Laurent El Shafey</a>
 *
 * @brief Compare FFT and FCT based on FFTPACK with naive DFT DCT 
 * implementations.
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE sp-FCT_FFT-fftpack-blitz Tests
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "sp/FFT.h"
#include "sp/FCT.h"
// Random number
#include <cstdlib>

struct T {
  double eps;
  T(): eps(1e-3) { }
  ~T() {}
};


void test_fct1D( const blitz::Array<double,1> t, double eps) 
{
  // process using FCT
  blitz::Array<double,1> t_fct;
  Torch::sp::fct(t, t_fct);
  BOOST_REQUIRE_EQUAL(t_fct.extent(0), t.extent(0));

  // TODO: get DCT answer and compare with FCT

  // process using inverse FCT
  blitz::Array<double,1> t_fct_ifct;
  Torch::sp::ifct(t_fct, t_fct_ifct);
  BOOST_REQUIRE_EQUAL(t_fct_ifct.extent(0), t.extent(0));

  // Compare to original
  for(int i=0; i < t.extent(0); ++i)
    BOOST_CHECK_SMALL( fabs(t_fct_ifct(i)-t(i)), eps);
}


void test_fct2D( const blitz::Array<double,2> t, double eps) 
{
  // process using FCT
  blitz::Array<double,2> t_fct;
  Torch::sp::fct(t, t_fct);
  BOOST_REQUIRE_EQUAL(t_fct.extent(0), t.extent(0));
  BOOST_REQUIRE_EQUAL(t_fct.extent(1), t.extent(1));

  // TODO: get DCT answer and compare with FCT

  // process using inverse FCT
  blitz::Array<double,2> t_fct_ifct;
  Torch::sp::ifct(t_fct, t_fct_ifct);
  BOOST_REQUIRE_EQUAL(t_fct_ifct.extent(0), t.extent(0));
  BOOST_REQUIRE_EQUAL(t_fct_ifct.extent(1), t.extent(1));

  // Compare to original
  for(int i=0; i < t.extent(0); ++i)
    for(int j=0; j < t.extent(1); ++j)
      BOOST_CHECK_SMALL( fabs(t_fct_ifct(i,j)-t(i,j)), eps);
}

void test_fft1D( const blitz::Array<std::complex<double>,1> t, double eps) 
{
  // process using FFT
  blitz::Array<std::complex<double>,1> t_fft;
  Torch::sp::fft(t, t_fft);
  BOOST_REQUIRE_EQUAL(t_fft.extent(0), t.extent(0));

  // TODO: get DFT answer and compare with FFT

  // process using inverse FFT
  blitz::Array<std::complex<double>,1> t_fft_ifft;
  Torch::sp::ifft(t_fft, t_fft_ifft);
  BOOST_REQUIRE_EQUAL(t_fft_ifft.extent(0), t.extent(0));

  // Compare to original
  for(int i=0; i < t.extent(0); ++i)
    BOOST_CHECK_SMALL( abs(t_fft_ifft(i)-t(i)), eps);
}


void test_fft2D( const blitz::Array<std::complex<double>,2> t, double eps) 
{
  // process using FFT
  blitz::Array<std::complex<double>,2> t_fft;
  Torch::sp::fft(t, t_fft);
  BOOST_REQUIRE_EQUAL(t_fft.extent(0), t.extent(0));
  BOOST_REQUIRE_EQUAL(t_fft.extent(1), t.extent(1));

  // TODO: get DFT answer and compare with FFT

  // process using inverse FFT
  blitz::Array<std::complex<double>,2> t_fft_ifft;
  Torch::sp::ifft(t_fft, t_fft_ifft);
  BOOST_REQUIRE_EQUAL(t_fft_ifft.extent(0), t.extent(0));
  BOOST_REQUIRE_EQUAL(t_fft_ifft.extent(1), t.extent(1));

  // Compare to original
  for(int i=0; i < t.extent(0); ++i)
    for(int j=0; j < t.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t_fft_ifft(i,j)-t(i,j)), eps);
}

void test_fftshift( const blitz::Array<std::complex<double>,1> t, double eps) 
{
  // process using fftshift
  blitz::Array<std::complex<double>,1> t_fft;
  Torch::sp::fftshift(t, t_fft);
  BOOST_REQUIRE_EQUAL(t_fft.extent(0), t.extent(0));

  // process using ifftshift
  blitz::Array<std::complex<double>,1> t_fft_ifft;
  Torch::sp::ifftshift(t_fft, t_fft_ifft);
  BOOST_REQUIRE_EQUAL(t_fft_ifft.extent(0), t.extent(0));

  // Compare to original
  for(int i=0; i < t.extent(0); ++i)
    BOOST_CHECK_SMALL( abs(t_fft_ifft(i)-t(i)), eps);
}

void test_fftshift( const blitz::Array<std::complex<double>,2> t, double eps) 
{
  // process using fftshift
  blitz::Array<std::complex<double>,2> t_fft;
  Torch::sp::fftshift(t, t_fft);
  BOOST_REQUIRE_EQUAL(t_fft.extent(0), t.extent(0));
  BOOST_REQUIRE_EQUAL(t_fft.extent(1), t.extent(1));

  // process using ifftshift
  blitz::Array<std::complex<double>,2> t_fft_ifft;
  Torch::sp::ifftshift(t_fft, t_fft_ifft);
  BOOST_REQUIRE_EQUAL(t_fft_ifft.extent(0), t.extent(0));
  BOOST_REQUIRE_EQUAL(t_fft_ifft.extent(1), t.extent(1));

  // Compare to original
  for(int i=0; i < t.extent(0); ++i)
    for(int j=0; j < t.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t_fft_ifft(i,j)-t(i,j)), eps);
}


BOOST_FIXTURE_TEST_SUITE( test_setup, T )

/*************** FCT Tests *****************/
BOOST_AUTO_TEST_CASE( test_fct1D_1to64_set )
{
  // size of the data
  for(int N=1; N <65; ++N) {
    // set up simple 1D array
    blitz::Array<double,1> t(N);
    for(int i=0; i<N; ++i)
      t(i) = 1.0+i;

    // call the test function
    test_fct1D( t, eps);
  }
}

BOOST_AUTO_TEST_CASE( test_fct1D_range1to2048_random )
{
  // This tests the 1D FCT using 10 random vectors
  // The size of each vector is randomly chosen between 3 and 2048
  for(int loop=0; loop < 10; ++loop) {
    // size of the data
    int N = (rand() % 2048 + 1);//random.randint(1,2048)

    // set up simple 1D random array
    blitz::Array<double,1> t(N);
    for(int i=0; i<N; ++i)
      t(i) = (rand()/(double)RAND_MAX)*10.;

    // call the test function
    test_fct1D( t, eps);
  }
}

BOOST_AUTO_TEST_CASE( test_fct2D_1x1to8x8_set )
{
  // size of the data
  for(int M=1; M < 9; ++M)
    for(int N=1; N < 9; ++N) {
      // set up simple 1D array
      blitz::Array<double,2> t(M,N);
      for(int i=0; i<M; ++i)
        for(int j=0; j<N; ++j)
          t(i,j) = 1.0+i+j;

      // call the test function
      test_fct2D( t, eps);
    }
}

BOOST_AUTO_TEST_CASE( test_fct2D_range1x1to64x64_random )
{
  // This tests the 1D FCT using 10 random vectors
  // The size of each vector is randomly chosen between 3 and 2048
  for(int loop=0; loop < 10; ++loop) {
    // size of the data
    int M = (rand() % 64 + 1);
    int N = (rand() % 64 + 1);

    // set up simple 1D random array
    blitz::Array<double,2> t(M,N);
    for( int i=0; i < M; ++i)
      for( int j=0; j < N; ++j)
        t(i,j) = (rand()/(double)RAND_MAX)*10.;

    // call the test function
    test_fct2D( t, eps);
  }
}


/*************** FFT Tests *****************/
BOOST_AUTO_TEST_CASE( test_fft1D_1to64_set )
{
  // size of the data
  for(int N=1; N <65 ; ++N) {
    // set up simple 1D tensor
    blitz::Array<std::complex<double>,1> t(N);
    for(int i=0; i<N; ++i)
      t(i) = std::complex<double>(1.0+i,0);

    // call the test function
    test_fft1D( t, eps);
  }
}

BOOST_AUTO_TEST_CASE( test_fft1D_range1to2048_random )
{
  // This tests the 1D FFT using 10 random vectors
  // The size of each vector is randomly chosen between 3 and 2048
  for(int loop=0; loop < 10; ++loop) {
    // size of the data
    int N = (rand() % 2048 + 1);//random.randint(1,2048)

    // set up simple 1D random tensor 
    blitz::Array<std::complex<double>,1> t(N);
    for(int i=0; i<N; ++i)
      t(i) = (rand()/(double)RAND_MAX)*10.;

    // call the test function
    test_fft1D( t, eps);
  }
}

BOOST_AUTO_TEST_CASE( test_fft2D_1x1to8x8_set )
{
  // size of the data
  for(int M=1; M < 9; ++M)
    for(int N=1; N < 9; ++N) {
      // set up simple 1D tensor
      blitz::Array<std::complex<double>,2> t(M,N);
      for(int i=0; i<M; ++i)
        for(int j=0; j<N; ++j)
          t(i,j) = std::complex<double>(1.0+i+j,0);

      // call the test function
      test_fft2D( t, eps);
    }
}

BOOST_AUTO_TEST_CASE( test_fft2D_range1x1to64x64_random )
{
  // This tests the 2D FCT using 10 random vectors
  // The size of each vector is randomly chosen between 3 and 2048
  for(int loop=0; loop < 10; ++loop) {
    // size of the data
    int M = (rand() % 64 + 1);
    int N = (rand() % 64 + 1);

    // set up simple 1D random tensor 
    blitz::Array<std::complex<double>,2> t(M,N);
    for( int i=0; i < M; ++i)
      for( int j=0; j < N; ++j)
        t(i,j) = std::complex<double>((rand()/(double)RAND_MAX)*10.,0);

    // call the test function
    test_fft2D( t, eps);
  }
}

BOOST_AUTO_TEST_CASE( test_fftshift1D_simple )
{
  // set up simple 1D random tensor 
  blitz::Array<std::complex<double>,1> t4(4), t4_s_ref(4), t5(5), t5_s_ref(5);
  t4 = 0, 1, 2, 3;
  t4_s_ref = 2, 3, 0, 1;
  t5 = 0, 1, 2, 3, 4;
  t5_s_ref = 3, 4, 0, 1, 2;

  // 1/ Process t4
  blitz::Array<std::complex<double>,1> t4_s;
  Torch::sp::fftshift(t4, t4_s);
  // Compare to reference
  for(int i=0; i < t4.extent(0); ++i)
    for(int j=0; j < t4.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t4_s(i,j)-t4_s_ref(i,j)), eps);
  
  blitz::Array<std::complex<double>,1> t4_si;
  Torch::sp::ifftshift(t4_s, t4_si);
  // Compare to original
  for(int i=0; i < t4.extent(0); ++i)
    for(int j=0; j < t4.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t4_si(i,j)-t4(i,j)), eps);

  // 2/ Process t5
  blitz::Array<std::complex<double>,1> t5_s;
  Torch::sp::fftshift(t5, t5_s);
  // Compare to reference
  for(int i=0; i < t5.extent(0); ++i)
    for(int j=0; j < t5.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t5_s(i,j)-t5_s_ref(i,j)), eps);
  
  blitz::Array<std::complex<double>,1> t5_si;
  Torch::sp::ifftshift(t5_s, t5_si);
  // Compare to original
  for(int i=0; i < t5.extent(0); ++i)
    for(int j=0; j < t5.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t5_si(i,j)-t5(i,j)), eps);
}

BOOST_AUTO_TEST_CASE( test_fftshift2D_simple )
{
  // set up simple 1D random tensor 
  blitz::Array<std::complex<double>,2> t(3,4), t_s_ref(3,4);
  t = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11;
  t_s_ref = 10, 11, 8, 9, 2, 3, 0, 1, 6, 7, 4, 5;

  blitz::Array<std::complex<double>,2> t_s;
  Torch::sp::fftshift(t, t_s);
  // Compare to reference
  for(int i=0; i < t.extent(0); ++i)
    for(int j=0; j < t.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t_s(i,j)-t_s_ref(i,j)), eps);
  
  blitz::Array<std::complex<double>,2> t_si;
  Torch::sp::ifftshift(t_s, t_si);
  // Compare to original
  for(int i=0; i < t.extent(0); ++i)
    for(int j=0; j < t.extent(1); ++j)
      BOOST_CHECK_SMALL( abs(t_si(i,j)-t(i,j)), eps);
}

BOOST_AUTO_TEST_CASE( test_fftshift1D_random )
{
  // This tests the 1D fftshift using 10 random vectors
  // The size of each vector is randomly chosen between 3 and 2048
  for(int loop=0; loop < 10; ++loop) {
    // size of the data
    int M = (rand() % 64 + 1);

    // set up simple 1D random tensor 
    blitz::Array<std::complex<double>,1> t(M);
    for( int i=0; i < M; ++i)
      t(i) = std::complex<double>((rand()/(double)RAND_MAX)*10.,0);

    // call the test function
    test_fftshift( t, eps);
  }
}

BOOST_AUTO_TEST_CASE( test_fftshift2D_random )
{
  // This tests the 2D fftshift using 10 random vectors
  // The size of each vector is randomly chosen between 3 and 2048
  for(int loop=0; loop < 10; ++loop) {
    // size of the data
    int M = (rand() % 64 + 1);
    int N = (rand() % 64 + 1);

    // set up simple 1D random tensor 
    blitz::Array<std::complex<double>,2> t(M,N);
    for( int i=0; i < M; ++i)
      for( int j=0; j < N; ++j)
        t(i,j) = std::complex<double>((rand()/(double)RAND_MAX)*10.,0);

    // call the test function
    test_fftshift( t, eps);
  }
}

BOOST_AUTO_TEST_SUITE_END()

