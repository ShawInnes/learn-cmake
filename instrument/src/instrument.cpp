//
// Created by Shaw Innes on 4/7/21.
//

#include "instrument.h"
#include "remoteserviceagent.h"
#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

boost::thread *instrumentThread;
RemoteServiceAgent *rsa;

void instrumentFunc() {
    BOOST_LOG_TRIVIAL(debug) << "instrumentFunc start";

    int counter = 0;

    for (;;) {
        BOOST_LOG_TRIVIAL(debug) << "instrumentFunc " << ++counter;

        try {
            // Sleep and check for interrupt.
            // To check for interrupt without sleep,
            // use boost::this_thread::interruption_point()
            // which also throws boost::thread_interrupted
            boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
        }
        catch (boost::thread_interrupted &) {
            BOOST_LOG_TRIVIAL(debug) << "instrumentFunc cleanup";
            BOOST_LOG_TRIVIAL(debug) << "instrumentFunc exit";
            return;
        }
    }
}

void Instrument::Start() {
    BOOST_LOG_TRIVIAL(info) << "Instrument->Start()";

//    instrumentThread = new boost::thread(&instrumentFunc);

    rsa = new RemoteServiceAgent();
    rsa->Init();
    rsa->Start();

    BOOST_LOG_TRIVIAL(info) << "Instrument::Start Done";
}

void Instrument::Stop() {
    BOOST_LOG_TRIVIAL(info) << "Instrument->Stop()";

    rsa->Stop();

    // signal thread to stop
    instrumentThread->interrupt();

    // wait until thread actually exits
    instrumentThread->join();

    BOOST_LOG_TRIVIAL(info) << "Instrument->Stop Done";
}
