#include <iostream>
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <remoteserviceagent.h>
#include <boost/format.hpp>
#include "azure_macro_utils/macro_utils.h"
#include "azureiot/iothub.h"
#include "azureiot/iothubtransportmqtt.h"
#include "azureiot/iothub_device_client.h"
#include "azureiot/iothub_message.h"
#include "azure_prov_client/prov_device_client.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

const char certificates[] =
/* Baltimore CyberTrust Root --Used Globally--*/
        "-----BEGIN CERTIFICATE-----\r\n"
        "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\r\n"
        "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\r\n"
        "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\r\n"
        "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\r\n"
        "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\r\n"
        "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\r\n"
        "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\r\n"
        "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\r\n"
        "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\r\n"
        "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\r\n"
        "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\r\n"
        "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\r\n"
        "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\r\n"
        "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\r\n"
        "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\r\n"
        "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\r\n"
        "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\r\n"
        "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\r\n"
        "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\r\n"
        "-----END CERTIFICATE-----\r\n";

MU_DEFINE_ENUM_STRINGS_WITHOUT_INVALID(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_VALUE);
MU_DEFINE_ENUM_STRINGS_WITHOUT_INVALID(PROV_DEVICE_REG_STATUS, PROV_DEVICE_REG_STATUS_VALUES);

volatile static bool g_registration_complete = false;
std::string global_prov_uri;
std::string id_scope;
std::string registration_name;
std::string symmetric_key;
std::string connectionString;

static void connection_status_callback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void *user_context) {
    (void) user_context;
    BOOST_LOG_TRIVIAL(info) << "Connection Status: " << MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONNECTION_STATUS, result) << ", " << MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONNECTION_STATUS_REASON, reason);
}

static void registration_status_callback(PROV_DEVICE_REG_STATUS reg_status, void *user_context) {
    (void) user_context;

    BOOST_LOG_TRIVIAL(info) << "Provisioning Status: " << MU_ENUM_TO_STRING(PROV_DEVICE_REG_STATUS, reg_status);
}

static void register_device_callback(PROV_DEVICE_RESULT register_result, const char *iothub_uri, const char *device_id,
                                     void *user_context) {
    (void) user_context;
    if (register_result == PROV_DEVICE_RESULT_OK) {
        BOOST_LOG_TRIVIAL(info) << "Registration Information received from service: " << iothub_uri << ", deviceId: " << device_id;
    } else {
        BOOST_LOG_TRIVIAL(error) << "Failure registering device: " << MU_ENUM_TO_STRING(PROV_DEVICE_RESULT, register_result);
    }
    g_registration_complete = true;
}

static void send_confirm_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback) {
    (void) userContextCallback;

    BOOST_LOG_TRIVIAL(info) << boost::format("Confirmation callback received for message with result %s") % MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result);
}

std::string getEnvVar(std::string const &key) {
    char const *val = getenv(key.c_str());
    return val == NULL ? std::string() : std::string(val);
}

void TestAzureIoT::Init() {
    global_prov_uri = "global.azure-devices-provisioning.net";
    id_scope = getEnvVar("DPS_SCOPE");
    registration_name = getEnvVar("DEVICE_ID");
    symmetric_key = getEnvVar("SYMMETRIC_KEY");
    connectionString = getEnvVar("CONNECTION_STRING");

    bool error = false;
    if (id_scope == "") {
        BOOST_LOG_TRIVIAL(error) << "DPS_SCOPE is not set";
        error =true;
    }
    if (registration_name == "") {
        BOOST_LOG_TRIVIAL(error) << "DEVICE_ID is not set";
        error =true;
    }
    if (symmetric_key == "") {
        BOOST_LOG_TRIVIAL(error) << "SYMMETRIC_KEY is not set";
        error =true;
    }
    if (connectionString == "") {
        BOOST_LOG_TRIVIAL(error) << "CONNECTION_STRING is not set";
        error =true;
    }

    if(error)
        throw;

    (void) IoTHub_Init();

#ifdef PROVISION
    SECURE_DEVICE_TYPE hsm_type = SECURE_DEVICE_TYPE_SYMMETRIC_KEY;
    (void) prov_dev_security_init(hsm_type);

    // az iot dps compute-device-key --key <master key> --registration-id <registration_id>
    prov_dev_set_symmetric_key_info(registration_name, symmetric_key);

    PROV_DEVICE_TRANSPORT_PROVIDER_FUNCTION prov_transport = Prov_Device_MQTT_Protocol;
    BOOST_LOG_TRIVIAL(info) << "Provisioning API Version: " << Prov_Device_GetVersionString();

    PROV_DEVICE_RESULT prov_device_result = PROV_DEVICE_RESULT_ERROR;
    PROV_DEVICE_HANDLE prov_device_handle;

    if ((prov_device_handle = Prov_Device_Create(global_prov_uri, id_scope, prov_transport)) == NULL) {
        BOOST_LOG_TRIVIAL(error) << "failed calling Prov_Device_Create";
    } else {
        Prov_Device_SetOption(prov_device_handle, OPTION_TRUSTED_CERT, certificates);

        prov_device_result = Prov_Device_Register_Device(prov_device_handle, register_device_callback, NULL,
                                                         registration_status_callback, NULL);
        BOOST_LOG_TRIVIAL(info) << "Registering Device";
        do {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
            BOOST_LOG_TRIVIAL(info) << "waiting";
        } while (!g_registration_complete);

        BOOST_LOG_TRIVIAL(info) << "Device Registration Complete";

        Prov_Device_Destroy(prov_device_handle);
    }
    prov_dev_security_deinit();
#endif

    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol = MQTT_Protocol;
    IOTHUB_MESSAGE_HANDLE message_handle;
    IOTHUB_DEVICE_CLIENT_HANDLE device_handle;

    if ((device_handle = IoTHubDeviceClient_CreateFromConnectionString(connectionString.c_str(), protocol)) == NULL) {
        BOOST_LOG_TRIVIAL(error) << "iotHubClientHandle is NULL for Device " << registration_name;
    } else {
        (void) IoTHubDeviceClient_SetConnectionStatusCallback(device_handle, connection_status_callback, NULL);
        char telemetry_msg_buffer[80];
        sprintf(telemetry_msg_buffer, "{\"temperature\":%.3f,\"humidity\":%.3f}", 23.0f, 60.0f);

        message_handle = IoTHubMessage_CreateFromString(telemetry_msg_buffer);

        boost::uuids::random_generator gen;
        boost::uuids::uuid message_id = gen();
        boost::uuids::uuid correlation_id = gen();

        const std::string message_id_string = to_string(message_id);
        const std::string correlation_id_string = to_string(correlation_id);

        (void) IoTHubMessage_SetMessageId(message_handle, message_id_string.c_str());
        (void) IoTHubMessage_SetCorrelationId(message_handle, correlation_id_string.c_str());
        (void) IoTHubMessage_SetContentTypeSystemProperty(message_handle, "application%2fjson");
        (void) IoTHubMessage_SetContentEncodingSystemProperty(message_handle, "utf-8");

        BOOST_LOG_TRIVIAL(info) << boost::format("Sending message to IoTHub: %s") % telemetry_msg_buffer;

        IoTHubDeviceClient_SendEventAsync(device_handle, message_handle, send_confirm_callback, NULL);

        IoTHubMessage_Destroy(message_handle);
    }

    IoTHub_Deinit();
}
