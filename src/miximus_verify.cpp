#include <cstdio>
#include <cstring>
#include <istream>
#include <fstream>

#include <libff/algebra/curves/alt_bn128/alt_bn128_pp.hpp>

#include "import.cpp"
#include "export.cpp"

using namespace std;

typedef libff::alt_bn128_pp ppT;

struct noop {
	void operator()(...) const {}
};

int main( int argc, char **argv )
{
	if( argc < 3 )
	{
		::fprintf(stderr, "Usage: %s <vk.json> <proof.json>\n", argv[0]);
		return 1;
	}

	ppT::init_public_params();

	// XXX: if argv[1] and argv[2] are both "-" do we read combined input from stdin?

	// Read input file (or stdin) into vk_stream;
	stringstream vk_stream;
	if( 0 == ::strcmp(argv[1], "-") ) {
		vk_stream << cin.rdbuf();
	}
	else {
		ifstream vk_input(argv[1]);
		if( ! vk_input ) {
			::fprintf(stderr, "Error: cannot open %s\n", argv[1]);
			return 2;
		}
		vk_stream << vk_input.rdbuf();
		vk_input.close();
	}

	auto vk = vk_from_json<ppT>(vk_stream);

	// Load proof from JSON
	stringstream proof_stream;
	ifstream proof_input(argv[2]);
	if( ! proof_input ) {
		::fprintf(stderr, "Error: cannot open %s\n", argv[2]);
		return 3;
	}
	proof_stream << proof_input.rdbuf();
	proof_input.close();

	auto proof = proof_from_json<ppT>(proof_stream);

	/*
	vk2json<ppT>(vk, argv[2]);
	*/

	return 0;
}