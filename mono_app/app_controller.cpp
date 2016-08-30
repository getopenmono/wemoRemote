
#include "app_controller.h"

using namespace mono::geo;

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

//OnOffButton::OnOffButton(geo::Rect rct) : ButtonView(rct, "on"), offLbl(rct, "off")
//{
//    state = false;
//    enabledColor = display::BelizeHoleColor;
//    borderColor = display::AsbestosColor;
//
//    int separation = 4;
//    uint32_t subWidth = (rct.Width()-separation)/2;
//    uint32_t yOff = rct.Y() + (rct.Height()/2 - textLabel.TextPixelHeight()/2);
//    uint32_t height = 18;//rct.Height() - (rct.Height()/2 - textLabel.TextPixelHeight()/2);
//
//    textLabel.setRect(geo::Rect(rct.X(), yOff, subWidth, height));
//    offLbl.setRect(geo::Rect(rct.X()+separation+subWidth, yOff, subWidth, height));
//    offLbl.setAlignment(TextLabelView::ALIGN_CENTER);
//}
//
//void OnOffButton::setBackground(Color c)
//{
//    ButtonView::setBackground(c);
//    offLbl.setBackground(c);
//}
//
//void OnOffButton::TouchEnd(mono::TouchEvent &event)
//{
//    bool shouldRepaint = isPressedDown;
//
//    isPressedDown = false;
//
//    // touch is stil within button area,
//    if (viewRect.contains(event.Position))
//    {
//        state = !state;
//        clickHandler.call();
//    }
//
//    if (shouldRepaint)
//        scheduleRepaint();
//}
//
//void OnOffButton::repaint()
//{
//    geo::Rect onBorder = ViewRect();
//    geo::Rect offBorder = ViewRect();
//
//    uint32_t subWidth = (viewRect.Width()-4) / 2;
//    onBorder.setWidth(subWidth);
//    offBorder.setWidth(subWidth);
//    offBorder.setX( viewRect.X()+4+subWidth );
//
//    if (!isPressedDown && state)
//    {
//        display::Color faded = background.alphaBlend(192, display::EmeraldColor);
//        textLabel.setBackground( faded );
//        painter.setBackgroundColor(faded);
//        painter.drawFillRect(onBorder, true);
//    }
//    else if (!isPressedDown)
//    {
//        textLabel.setBackground(background);
//        painter.setBackgroundColor(background);
//        painter.drawFillRect(onBorder, true);
//    }
//
//
////    if (state && !isPressedDown)
////    {
//////
////        textLabel.setTextColor(faded);
////    }
////    else
//        textLabel.setTextColor( isPressedDown ? borderColorPressed : borderColor );
//
//    offLbl.setTextColor( isPressedDown ? borderColorPressed : borderColor );
//
//
//
//    textLabel.repaint();
//    offLbl.repaint();
//
//    painter.setForegroundColor( isPressedDown ? borderColorPressed : borderColor );
//    painter.drawRect(onBorder);
//    painter.setForegroundColor( isPressedDown ? borderColorPressed : borderColor );
//    painter.drawRect(offBorder);
//
//    geo::Rect fatOffBorder(offBorder.X()+1, offBorder.Y()+1, offBorder.Width()-2, offBorder.Height()-2);
//    painter.drawRect(fatOffBorder, (state || isPressedDown));
//
//    geo::Rect fatOnBorder(onBorder.X()+1, onBorder.Y()+1, onBorder.Width()-2, onBorder.Height()-2);
//    painter.drawRect(fatOnBorder, (!state || isPressedDown));
//
////    geo::Point off1(offBorder.UpperLeft()), off2(offBorder.UpperRight());
////    off1.appendY(1); off1.appendX(1); off2.appendY(1);
////    painter.drawLine(off1, off2, (state || isPressedDown));
////
////    geo::Point on1(onBorder.UpperLeft()), on2(onBorder.UpperRight());
////    on1.appendY(1); on1.appendX(1); on2.appendY(1);
////    painter.drawLine(on1, on2, (!state || isPressedDown));
//}


AppController::AppController() :
    spi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK, RP_SPI_CS),
    spiComm(spi, RP_SPI_CS, RP_nRESET, RP_INTERRUPT),
    logo(Rect(0,0,176,30), "WeMo"),
    wemoLabel(Rect(10,130,70,25),"Switch"),
    msgLabel(Rect(0,220-40,176,40), ""),
    toggle(Rect(85,120,80,35)),
	ind(Rect(20+176-40-20, 137, 10, 10))
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
}

void AppController::toggleState()
{
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
	//ind.show();
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

void AppController::monoWillGotoSleep()
{
}

void AppController::monoWakeFromSleep()
{
    mono::IApplicationContext::SoftwareResetToApplication();
}
