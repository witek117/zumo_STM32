#include <boost/throw_exception.hpp>
#include <cstdio>

void boost::throw_exception(std::exception const & e) {
    std::printf("boost exception %s\n\r", e.what());
    std::terminate();
    while(1) {}
}