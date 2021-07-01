#ifndef REMOTESERVICEAGENT_LIBRARY_H
#define REMOTESERVICEAGENT_LIBRARY_H

const char *getLibInterfaceName();

const char *getLibInterfaceVersion();

class RemoteServiceAgent {
private:

public:
    RemoteServiceAgent();

    /**
     * Initialises the Remote Service Agent
     */
    void Init();

    /**
     * Starts the Remote Service Agent
     */
    void Start();

    /**
     * Sends a notification to the Remote Service Agent indicating that the calling application is still alive
     */
    void SendHeartBeat();

    /**
     * Sends an explicit message to the Remote Service Agent
     */
    void SendEvent();

    /**
     * Stops the Remote Service Agent
     */
    void Stop();
};

#endif //REMOTESERVICEAGENT_LIBRARY_H

