// Mono Application Template V1.1
//

#ifndef app_controller_h
#define app_controller_h

#include <mono.h>
#include <wireless/redpine_module.h>
#include <wireless/module_communication.h>
#include <SPI.h>

#define WIFI_SSID "ptype"
#define WIFI_PASSPHRASE "CAI7-huzzas"
#define SERVER_HOST "10.0.41.190:3090"

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

//class OnOffButton : public ButtonView
//{
//public:
//    bool state;
//    mono::display::Color enabledColor;
//    TextLabelView offLbl;
//
//    OnOffButton(geo::Rect rct);
//
//    void setBackground(Color c);
//    void TouchEnd(mono::TouchEvent &event);
//
//    void repaint();
//};


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
    bool on;

public:

    AppController();


    void toggleState();
    void networkReady();

    void repaintIndicator();

    void monoWakeFromReset();
    void monoWillGotoSleep();
    void monoWakeFromSleep();

};

#endif /* app_controller_h */
