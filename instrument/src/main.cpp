#include <iostream>
#include "instrument.h"
#include "remoteserviceagent.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

int main() {
    logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info
    );
    BOOST_LOG_TRIVIAL(info) << "Name " << getLibInterfaceName();
    BOOST_LOG_TRIVIAL(info) << "Version " << getLibInterfaceVersion();

    Instrument *instrument = new Instrument();
    instrument->Start();

    // Wait for Enter
    std::cout << "Enter to exit" << std::endl;

    char ch;
    std::cin.get(ch);

    instrument->Stop();

    return 0;
}
