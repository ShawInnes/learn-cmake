#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <remoteserviceagent.h>
#include <version_config.h>

RemoteServiceAgent::RemoteServiceAgent() {
    std::cout << "RemoteServiceAgent::ctor()" << std::endl;
}

void RemoteServiceAgent::Init() {
    std::cout << "RemoteServiceAgent->Init()" << std::endl;
}

boost::thread *remoteServiceAgentThread;

void wait(int milliseconds) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds{milliseconds});
}

void remoteServiceFunc() {
    std::cout << "remoteServiceFunc start" << std::endl;

    int counter = 0;

    for (;;) {
        std::cout << "remoteServiceFunc " << ++counter << std::endl;

        try {
            // Sleep and check for interrupt.
            // To check for interrupt without sleep,
            // use boost::this_thread::interruption_point()
            // which also throws boost::thread_interrupted
            wait(500);
        }
        catch (boost::thread_interrupted &) {
            std::cout << "remoteServiceFunc cleanup (2000ms)" << std::endl;
            wait(2000);
            std::cout << "remoteServiceFunc exit" << std::endl;
            return;
        }
    }
}

void RemoteServiceAgent::Start() {
    std::cout << "RemoteServiceAgent->Start()\n";

    remoteServiceAgentThread = new boost::thread(&remoteServiceFunc);

    TestAzureIoT * iot = new TestAzureIoT();
    iot->Init();

    std::cout << "RemoteServiceAgent::Start Done" << std::endl;
}

void RemoteServiceAgent::Stop() {
    std::cout << "RemoteServiceAgent->Stop()\n";

    // signal thread to stop
    remoteServiceAgentThread->interrupt();

    // wait until thread actually exits
    remoteServiceAgentThread->join();

    std::cout << "RemoteServiceAgent::Stop Done" << std::endl;
}