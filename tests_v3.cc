// FIXME
#define DO_ARMA 1
#define DO_BLITZ 1
#define GNUPLOT_ENABLE_BLITZ 1

#include <fstream>
#include <vector>

#include <boost/array.hpp>

#if DO_ARMA
#include <armadillo>
#endif
#if DO_BLITZ
#include <blitz/array.h>
#endif

#include "gnuplot-iostream.h"

#if 0
// for debugging
#include <typeinfo>
#include <cxxabi.h>

template <typename T>
std::string get_typename() {
	int status;
	char *name;
	name = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
	assert(!status);
	return std::string(name);
}
#endif

template <typename T>
void runtest(std::string header, const T &arg) {
	std::cout << "--- " << header << " -------------------------------------" << std::endl;
	std::cout << "ncols=" << ArrayTraits<T>::ncols << std::endl;
	std::cout << "depth=" << ArrayTraits<T>::depth << std::endl;
	std::cout << "tuple=" << ArrayTraits<T>::val_is_tuple << std::endl;
	//std::cout << "range_type=" << get_typename<typename ArrayTraits<T>::range_type>() << std::endl;
	send_array(std::cout, arg);
}

template <typename T>
void runtest_cols(std::string header, const T &arg) {
	std::cout << "--- " << header << " -------------------------------------" << std::endl;
	//std::cout << "ncols=" << ArrayTraits<T>::ncols << std::endl;
	//std::cout << "depth=" << ArrayTraits<T>::depth << std::endl;
	//std::cout << "range_type=" << get_typename<typename ArrayTraits<T>::range_type>() << std::endl;
	send_array_cols(std::cout, arg);
}

int main() {
	const int NX=3, NY=4;
	std::vector<double> vd;
	std::vector<int> vi;
	std::vector<std::vector<double> > vvd(NX);
	std::vector<std::vector<int> > vvi(NX);
	std::vector<std::vector<std::vector<int> > > vvvi(NX);
	int ai[NX];
	boost::array<int, NX> bi;

	for(int x=0; x<NX; x++) {
		vd.push_back(x+7.5);
		vi.push_back(x+7);
		ai[x] = x+7;
		bi[x] = x+70;
		for(int y=0; y<NY; y++) {
			vvd[x].push_back(100+x*10+y);
			vvi[x].push_back(200+x*10+y);
			std::vector<int> tup;
			tup.push_back(300+x*10+y);
			tup.push_back(400+x*10+y);
			vvvi[x].push_back(tup);
		}
	}

	runtest("vd,vi,bi", std::make_pair(vd, std::make_pair(vi, bi)));
	runtest("vvd,vvi", std::make_pair(vvd, vvi));
	runtest("ai", ai);
	// FIXME - doesn't work because array gets cast to pointer
	//runtest(std::make_pair(ai, bi));
	runtest("vvd,vvi,vvvi", std::make_pair(vvd, std::make_pair(vvi, vvvi)));

#if DO_ARMA
	arma::vec armacol(NX);
	arma::mat armamat(NX, NY);

	for(int x=0; x<NX; x++) {
		armacol(x) = x+0.123;
		for(int y=0; y<NY; y++) {
			armamat(x, y) = x*10+y+0.123;
		}
	}

	runtest("armacol", armacol);
	runtest("armamat", armamat);
#endif

#if DO_BLITZ
	blitz::Array<double, 1> blitz1d(NX);
	blitz::Array<double, 2> blitz2d(NX, NY);
	{
		blitz::firstIndex i;
		blitz::secondIndex j;
		blitz1d = i + 0.777;
		blitz2d = i*10 + j;
	}
	blitz::Array<blitz::TinyVector<double, 2>, 2> blitz2d_tup(NX, NY);
	for(int x=0; x<NX; x++) {
		for(int y=0; y<NY; y++) {
			blitz2d_tup(x, y)[0] = 100+x*10+y;
			blitz2d_tup(x, y)[1] = 200+x*10+y;
		}
	}

	runtest("blitz1d", blitz1d);
	runtest("blitz1d,vd", std::make_pair(blitz1d, vd));
	runtest("blitz2d", blitz2d);
	runtest("blitz2d_tup", blitz2d_tup);
	runtest("blitz2d,vvi", std::make_pair(blitz2d, vvi));
	runtest("blitz2d,vd", std::make_pair(blitz2d, vd));
#endif

	runtest_cols("vvvi cols", vvvi);
	runtest_cols("blitz2d cols", blitz2d);
}