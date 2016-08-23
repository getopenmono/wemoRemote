// Mono Application Template V1.1
//

#ifndef app_controller_h
#define app_controller_h

#include <mono.h>
#include <wireless/redpine_module.h>
#include <wireless/module_communication.h>
#include <SPI.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSPHRASE "PASSPHRASE"
#define SERVER_HOST "my_wemo_server"

using namespace mono;
using namespace mono::ui;

class AppController : public mono::IApplication {

    mbed::SPI spi;
    redpine::ModuleSPICommunication spiComm;
    network::HttpClient client;

    TextLabelView helloLabel;
    ButtonView toggle;
    bool on;

public:

    AppController();


    void toggleState();
    void networkReady();

    void monoWakeFromReset();
    void monoWillGotoSleep();
    void monoWakeFromSleep();

};

#endif /* app_controller_h */
