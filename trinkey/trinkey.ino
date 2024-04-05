/*

Trinkey SHT45 implementation with: json output and faster output.
This makes it usable for a quick reading from a local metric collector like zabbix.

Original source by Limor Fried for Adafruit Industries, 2024.
https://learn.adafruit.com/adafruit-sht4x-trinkey/arduino-sht4x-demo

*/
#include <Adafruit_SHT4x.h>
#include <Adafruit_FreeTouch.h>
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_FreeTouch touch = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
String serial_number;

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(10);     // will pause until serial console opens
  }
  
  Serial.println("# OSSO trinkey");
  if (! sht4.begin()) {
    Serial.println("# Couldn't find SHT4x");
    while (1) delay(1);
  }
  Serial.println("# Found SHT4x sensor.");

    if (! touch.begin())  {
    Serial.println("# Failed to begin QTouch on pin 1");
  }

  sht4.setPrecision(SHT4X_HIGH_PRECISION);  
  sht4.setHeater(SHT4X_NO_HEATER);
  Serial.println("# Temperature in *C, Relative Humidity %, touch");

  serial_number =  sht4.readSerial();
}


void loop() {
  sensors_event_t humidity, temp;

  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("{\"serial_number\": ");
  Serial.print(serial_number);
  Serial.print(", \"temperature\": ");
  Serial.print(temp.temperature);
  Serial.print(", \"humidity\": ");
  Serial.print(humidity.relative_humidity);
  Serial.print(", \"touch\": ");
  Serial.print(touch.measure());
  Serial.println("}");


  // 0.1 second between readings
  delay(100);  
}