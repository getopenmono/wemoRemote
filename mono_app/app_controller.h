// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
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

class HeaderView : public View
{
public:

    mono::display::Color bgColor;
    mono::display::Color textColor;
    TextLabelView txtView;

    HeaderView(geo::Rect rect, String text);

    void repaint();
};

class AppController : public mono::IApplication {

    mbed::SPI spi;
    redpine::ModuleSPICommunication spiComm;
    network::HttpClient client;

    mono::display::Color bgColor;
    BackgroundView bg;
    HeaderView logo;
    TextLabelView wemoLabel;
    TextLabelView msgLabel;
    OnOffButtonView toggle;
    StatusIndicatorView ind;
    mono::Timer dimmer;
    mono::Timer sleeper;
    bool on;

public:

    AppController();


    void toggleState();
    void networkReady();
    void dim();
    void undim();

    void repaintIndicator();

    void monoWakeFromReset();
    void monoWillGotoSleep();
    void monoWakeFromSleep();

};

#endif /* app_controller_h */
