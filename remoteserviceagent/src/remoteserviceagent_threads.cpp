#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

namespace RemoteServicesAgent {
    boost::thread *remoteServiceAgentThread;

    void wait(int milliseconds) {
        boost::this_thread::sleep_for(boost::chrono::milliseconds{milliseconds});
    }

    void workerFunc() {
        std::cout << "workerFunc start" << '\n';
        for (int i = 0; i < 5; ++i) {
            wait(500);
            std::cout << boost::this_thread::get_id() << ' ' << i << '\n';
        }
        std::cout << "workerFunc finish" << '\n';
    }

    void TestThreads() {
        std::cout << "Calling TestThreads\n";

        remoteServiceAgentThread = new boost::thread(&workerFunc);

        // join forces execution on the thread
        // remoteServiceAgentThread->join();

        std::cout << "main:wait(10000)" << '\n';
        wait(10000);

        std::cout << "main:done" << '\n';
    }
}
