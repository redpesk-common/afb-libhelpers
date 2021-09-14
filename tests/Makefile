.SUFFIXES:

test++.so: test++.c++ ../afb-data-utils.c ../afb-data-utils.cpp ../afb-data-utils.h ../afb-data-utils.hpp ../afb-data-utils
	g++ -o $@ -shared -DAFB_BINDING_VERSION=4 -fPIC test++.c++ ../afb-data-utils.c ../afb-data-utils.cpp -fpermissive
