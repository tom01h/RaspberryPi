#include <M5EPD.h>
#include <WiFi.h>

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
    
    canvas.createCanvas(480, 270);
    canvas.setTextSize(3);
    canvas.drawPngUrl("http://192.168.0.100/munin/localdomain/localhost.localdomain/hum-day.png");
    canvas.pushCanvas(480,0,UPDATE_MODE_GC16);
    
    canvas.createCanvas(480, 270);
    canvas.setTextSize(3);
    canvas.drawPngUrl("http://192.168.0.100/munin/localdomain/localhost.localdomain/pres-day.png");
    canvas.pushCanvas(0,270,UPDATE_MODE_GC16);
    
    canvas.createCanvas(480, 270);
    canvas.setTextSize(3);
    canvas.drawPngUrl("http://192.168.0.100/munin/localdomain/localhost.localdomain/temp-day.png");
    canvas.pushCanvas(480,270,UPDATE_MODE_GC16);

    delay(500);
    M5.shutdown(5*60);
}

void loop()
{

}
