#include <Wire.h>
#include <MQUnifiedsensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MQ-9 sensor setup
#define Board "ESP8266"
#define Voltage_Resolution 3.3
#define ADC_Bit_Resolution 10
#define pin A0
#define Type "MQ-9"
#define RatioMQ9CleanAir 9.6
#define RLOAD 10

MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, pin, Type);

void setup() {
  Serial.begin(115200);

  // OLED initialization
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("MQ-9 Calibration...");
  display.display();
  delay(2000);

  // MQ-9 sensor setup
  MQ9.setRegressionMethod(1);  // 1 -> Exponential
  MQ9.init();
  MQ9.setRL(RLOAD);

  Serial.println("Calibrating... Please keep sensor in clean air");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Calibrating...");
  display.display();

  MQ9.calibrate(RatioMQ9CleanAir);  // Calibrate R0 in clean air

  Serial.print("Calibration complete. R0 = ");
  Serial.println(MQ9.getR0());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("R0 Calib: ");
  display.print(MQ9.getR0(), 2);
  display.display();
  delay(2000);
}

void loop() {
  MQ9.update(); // Read analog value
  float ppm = MQ9.readSensor(); // Get gas concentration

  Serial.print("Air Quality (PPM): ");
  Serial.println(ppm);

  // Show reading on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Air Quality:");

  display.setCursor(0, 20);
  display.setTextSize(2);
  display.print(ppm, 1);
  display.setTextSize(1);
  display.print(" PPM");
  display.display();

  delay(2000); // Adjust delay as needed
}
