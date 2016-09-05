// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "app_controller.h"

using namespace mono::geo;

namespace
{
    static const int dimBrightness = 50;
}

HeaderView::HeaderView(geo::Rect rct, String text) :
    View(rct),
    txtView(rct, text)
{
    bgColor = display::BlackColor;
    textColor = display::EmeraldColor;

    uint32_t yOff = rct.Y() + (rct.Height()/2 - txtView.TextPixelHeight()/2 + 8);
    txtView.setPosition(geo::Point(rct.X(),yOff));
    txtView.setAlignment(TextLabelView::ALIGN_CENTER);
    txtView.setBackground(bgColor);
    txtView.setText(textColor);
}

void HeaderView::repaint()
{
    painter.setBackgroundColor(bgColor);
    painter.drawFillRect(viewRect,true);
    txtView.repaint();
}

AppController::AppController() :
    spi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK, RP_SPI_CS),
    spiComm(spi, RP_SPI_CS, RP_nRESET, RP_INTERRUPT),
    logo(Rect(0,0,176,30), "WeMo"),
    wemoLabel(Rect(10,130,70,25),"Switch"),
    msgLabel(Rect(0,220-40,176,40), ""),
    toggle(Rect(85,120,80,35)),
	ind(Rect(20+176-40-20, 137, 10, 10)),
    dimmer(60*1000,true),
    sleeper(10*1000,true)
{
    bgColor = display::CloudsColor;
    bg.setBackgroundColor(bgColor);

    msgLabel.setAlignment(TextLabelView::ALIGN_CENTER);
    msgLabel.setTextColor(display::WetAsphaltColor);
    msgLabel.setBackgroundColor(bgColor);

    wemoLabel.setBackground(bgColor);
    wemoLabel.setTextColor(display::WetAsphaltColor);

    toggle.setBorder(display::WetAsphaltColor);
    toggle.setBackground(bgColor);
    toggle.setClickCallback<AppController>(this, &AppController::toggleState);

	ind.setOffStateColor(mono::display::WetAsphaltColor);

    dimmer.setCallback<AppController>(this,&AppController::dim);
    sleeper.setCallback(IApplicationContext::EnterSleepMode);
    dimmer.Start();
}

void AppController::toggleState()
{
    undim();
    on = toggle.isActive();
    client = network::HttpClient(String::Format("http://" SERVER_HOST "/%s",on ? "on" : "off"));
	ind.setState(on);
}

void AppController::networkReady()
{
    toggle.show();
	ind.setState(false);
    msgLabel.setText("");
    wemoLabel.show();
}

void AppController::monoWakeFromReset()
{
    bg.show();
    logo.show();
    msgLabel.show();

    msgLabel.setText("Init wifi...");

    redpine::Module::Instance()->initialize(&spiComm);
    redpine::Module::setupWifiOnly(WIFI_SSID, WIFI_PASSPHRASE);
    redpine::Module::Instance()->setNetworkReadyCallback<AppController>(this, &AppController::networkReady);
}

void AppController::dim ()
{
    dimmer.Stop();
    IDisplayController * display = IApplicationContext::Instance->DisplayController;
    for (int i = display->Brightness(); i >= dimBrightness; --i)
    {
        display->setBrightness(i);
        wait_ms(2);
    }
    sleeper.Start();
}

void AppController::undim ()
{
    sleeper.Stop();
    IDisplayController * display = IApplicationContext::Instance->DisplayController;
    display->setBrightness(255);
    dimmer.Start();
}

void AppController::monoWillGotoSleep()
{
}

void AppController::monoWakeFromSleep()
{
    mono::IApplicationContext::SoftwareResetToApplication();
}
