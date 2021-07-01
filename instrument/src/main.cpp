#include <iostream>
#include "remoteserviceagent.h"
#include <boost/thread.hpp>

boost::thread *instrumentThread;
RemoteServiceAgent *rsa;

void instrumentFunc() {
    std::cout << "instrumentFunc start" << std::endl;

    int counter = 0;

    for (;;) {
        std::cout << "instrumentFunc " << ++counter << std::endl;

        try {
            // Sleep and check for interrupt.
            // To check for interrupt without sleep,
            // use boost::this_thread::interruption_point()
            // which also throws boost::thread_interrupted
            boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
        }
        catch (boost::thread_interrupted &) {
            std::cout << "instrumentFunc cleanup" << std::endl;
            std::cout << "instrumentFunc exit" << std::endl;
            return;
        }
    }
}

void Start() {
    std::cout << "Instrument->Start()\n";

    instrumentThread = new boost::thread(&instrumentFunc);

    rsa = new RemoteServiceAgent();
    rsa->Init();
    rsa->Start();

    std::cout << "Instrument::Start Done" << std::endl;
}

void Stop() {
    std::cout << "Instrument->Stop()\n";

    rsa->Stop();

    // signal thread to stop
    instrumentThread->interrupt();

    // wait until thread actually exits
    instrumentThread->join();

    std::cout << "Instrument->Stop Done" << std::endl;
}

int main() {
    printf("Name %s\n", getLibInterfaceName());
    printf("Version %s\n", getLibInterfaceVersion());

    Start();

    // Wait for Enter
    char ch;
    std::cin.get(ch);

    Stop();

    return 0;
}
