#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/log/trivial.hpp>
#include <remoteserviceagent.h>
#include <version_config.h>

RemoteServiceAgent::RemoteServiceAgent() {
    BOOST_LOG_TRIVIAL(debug) << "RemoteServiceAgent::ctor()";
}

void RemoteServiceAgent::Init() {
    BOOST_LOG_TRIVIAL(debug) << "RemoteServiceAgent->Init()";
}

boost::thread *remoteServiceAgentThread;

void wait(int milliseconds) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds{milliseconds});
}

void remoteServiceFunc() {
    BOOST_LOG_TRIVIAL(debug) << "remoteServiceFunc start";

    int counter = 0;

    for (;;) {
        BOOST_LOG_TRIVIAL(debug) << "remoteServiceFunc " << ++counter;

        try {
            // Sleep and check for interrupt.
            // To check for interrupt without sleep,
            // use boost::this_thread::interruption_point()
            // which also throws boost::thread_interrupted
            wait(500);
        }
        catch (boost::thread_interrupted &) {
            BOOST_LOG_TRIVIAL(debug) << "remoteServiceFunc cleanup (2000ms)";
            wait(2000);
            BOOST_LOG_TRIVIAL(debug) << "remoteServiceFunc exit";
            return;
        }
    }
}

void RemoteServiceAgent::Start() {
    BOOST_LOG_TRIVIAL(debug) << "RemoteServiceAgent->Start()";
    remoteServiceAgentThread = new boost::thread(&remoteServiceFunc);

    TestAzureIoT * iot = new TestAzureIoT();
    iot->Init();

    BOOST_LOG_TRIVIAL(debug) << "RemoteServiceAgent::Start Done";
}

void RemoteServiceAgent::Stop() {
    BOOST_LOG_TRIVIAL(debug) << "RemoteServiceAgent->Stop()";

    // signal thread to stop
    remoteServiceAgentThread->interrupt();

    // wait until thread actually exits
    remoteServiceAgentThread->join();

    BOOST_LOG_TRIVIAL(debug) << "RemoteServiceAgent::Stop Done";
}