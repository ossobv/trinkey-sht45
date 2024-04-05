#include <Adafruit_SHT4x.h>
#include <Adafruit_FreeTouch.h>
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_FreeTouch touch = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
char sID[7];

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
}


void printChipId() {
  volatile uint32_t val1, val2, val3, val4;
  volatile uint32_t *ptr1 = (volatile uint32_t *)0x0080A00C;
  val1 = *ptr1;
  volatile uint32_t *ptr = (volatile uint32_t *)0x0080A040;
  val2 = *ptr;
  ptr++;
  val3 = *ptr;
  ptr++;
  val4 = *ptr;

  char buf[33];
  sprintf(buf, "%8x%8x%8x%8x", val1, val2, val3, val4);
  Serial.print('\"');
  Serial.print(buf);
  Serial.print('\"');

}

void loop() {
  sensors_event_t humidity, temp;

  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("{\"serial_number\": ");
  printChipId();
  Serial.print(", \"temperature\": ");
  Serial.print(temp.temperature);
  Serial.print(", \"humidity\": ");
  Serial.print(humidity.relative_humidity);
  Serial.print(", \"touch\": ");
  Serial.print(touch.measure());
  Serial.println("}");


  // 1 second between readings
  delay(100);  
}