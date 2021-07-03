#include <iostream>
#include <boost/thread.hpp>
#include <remoteserviceagent.h>
#include "azureiot/iothub.h"
#include "azureiot/iothub_message.h"
#include "azure_prov_client/prov_device_client.h"
#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/prov_transport_mqtt_client.h"

volatile static bool g_registration_complete = false;
static const char *global_prov_uri = "global.azure-devices-provisioning.net";
static const char *id_scope = "[ID Scope]";

static void registration_status_callback(PROV_DEVICE_REG_STATUS reg_status, void *user_context) {
    (void) user_context;
    (void) printf("Provisioning Status: %d\r\n", reg_status);
}

static void register_device_callback(PROV_DEVICE_RESULT register_result, const char *iothub_uri, const char *device_id,
                                     void *user_context) {
    (void) user_context;
    if (register_result == PROV_DEVICE_RESULT_OK) {
        (void) printf("\r\nRegistration Information received from service: %s, deviceId: %s\r\n", iothub_uri,
                      device_id);
    } else {
        (void) printf("\r\nFailure registering device: %d\r\n", register_result);
    }
    g_registration_complete = true;
}

void TestAzureIoT::Init() {
    SECURE_DEVICE_TYPE hsm_type = SECURE_DEVICE_TYPE_X509;

    // Used to initialize IoTHub SDK subsystem
    (void) IoTHub_Init();
    (void) prov_dev_security_init(hsm_type);

    PROV_DEVICE_TRANSPORT_PROVIDER_FUNCTION prov_transport = Prov_Device_MQTT_Protocol;

    std::cout << "Provisioning API Version: " << Prov_Device_GetVersionString() << std::endl;
    PROV_DEVICE_RESULT prov_device_result = PROV_DEVICE_RESULT_ERROR;
    PROV_DEVICE_HANDLE prov_device_handle;

    if ((prov_device_handle = Prov_Device_Create(global_prov_uri, id_scope, prov_transport)) == NULL) {
        std::cout << "failed calling Prov_Device_Create" << std::endl;
    } else {
        prov_device_result = Prov_Device_Register_Device(prov_device_handle, register_device_callback, NULL,
                                                         registration_status_callback, NULL);

        (void) printf("\r\nRegistering Device\r\n\r\n");
        do {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
            std::cout << "waiting..." << std::endl;
        } while (!g_registration_complete);

        std::cout << "done!" << std::endl;

        Prov_Device_Destroy(prov_device_handle);
    }
    prov_dev_security_deinit();

    IoTHub_Deinit();

    (void) printf("Press enter key to exit:\r\n");
    (void) getchar();
}
