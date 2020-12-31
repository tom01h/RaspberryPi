#include <M5EPD.h>
#include <WiFi.h>
#include "ImageResource.h"

M5EPD_Canvas canvas(&M5.EPD);

void setup()
{
    M5.begin();
    M5.EPD.SetRotation(0);
    M5.EPD.Clear(true);
    WiFi.begin("WIFI-SSID", "WIFI-PASSWORD");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    canvas.createCanvas(480, 270);
    canvas.setTextSize(3);
    canvas.drawPngUrl("http://192.168.0.100/munin/localdomain/localhost.localdomain/co2-day.png");
    canvas.pushCanvas(0,0,UPDATE_MODE_GC16);
    
    canvas.setTextSize(3);
    canvas.drawPngUrl("http://192.168.0.100/munin/localdomain/localhost.localdomain/hum-day.png");
    canvas.pushCanvas(480,0,UPDATE_MODE_GC16);
    
    canvas.setTextSize(3);
    canvas.drawPngUrl("http://192.168.0.100/munin/localdomain/localhost.localdomain/pres-day.png");
    canvas.pushCanvas(0,270,UPDATE_MODE_GC16);
    
    canvas.setTextSize(3);
    canvas.drawPngUrl("http://192.168.0.100/munin/localdomain/localhost.localdomain/temp-day.png");
    canvas.pushCanvas(480,270,UPDATE_MODE_GC16);
    canvas.deleteCanvas();

    canvas.createCanvas(60 + 32, 32);
    canvas.setTextDatum(CR_DATUM);
    canvas.setTextSize(2);
    canvas.pushImage(60, 0, 32, 32, ImageResource_status_bar_battery_32x32);

    uint32_t vol = M5.getBatteryVoltage();
    if(vol < 3300)
    {
        vol = 3300;
    }
    else if(vol > 4350)
    {
        vol = 4350;
    }
    float battery = (float)(vol - 3300) / (float)(4350 - 3300);
    if(battery <= 0.01)
    {
        battery = 0.01;
    }
    if(battery > 1)
    {
        battery = 1;
    }
    uint8_t px = battery * 25;
    char buf[20];
    sprintf(buf, "%d%%", (int)(battery * 100));
    canvas.drawString(buf, 60 - 5, 16);
    canvas.fillRect(60 + 3, 10, px, 13, 15);
    canvas.pushCanvas(860,0,UPDATE_MODE_GC16);
    canvas.deleteCanvas();
    
    delay(500);
    M5.shutdown(5*60);
}

void loop()
{

}
