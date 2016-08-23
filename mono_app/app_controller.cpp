
#include "app_controller.h"

using namespace mono::geo;

AppController::AppController() :
    spi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK, RP_SPI_CS),
    spiComm(spi, RP_SPI_CS, RP_nRESET, RP_INTERRUPT),
    helloLabel(Rect(0,80,176,20), ""),
    toggle(Rect(20,120,176-40,35),"On")
{
    helloLabel.setAlignment(TextLabelView::ALIGN_CENTER);
    helloLabel.setTextColor(display::TurquoiseColor);
    toggle.setClickCallback<AppController>(this, &AppController::toggleState);
}

void AppController::toggleState()
{
    on = on ? 0 : 1;

    client = network::HttpClient(String::Format("http://" SERVER_HOST "/%s",on ? "on" : "off"));

    toggle.setText(on ? "Off": "On");
}

void AppController::networkReady()
{
    toggle.show();
    helloLabel.setText("WeMo");
}

void AppController::monoWakeFromReset()
{
    helloLabel.show();

    helloLabel.setText("Init wifi...");

    redpine::Module::Instance()->initialize(&spiComm);
    redpine::Module::setupWifiOnly(WIFI_SSID, WIFI_PASSPHRASE);
    redpine::Module::Instance()->setNetworkReadyCallback<AppController>(this, &AppController::networkReady);
}

void AppController::monoWillGotoSleep()
{
}

void AppController::monoWakeFromSleep()
{
    mono::IApplicationContext::SoftwareResetToApplication();
}
