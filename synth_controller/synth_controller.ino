#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>

#define TOUCH_THRESHOLD 15
#define SPI_DEBUG       0

#define SCREEN_BAUD   9600
#define SCREEN_RXPIN  16
#define SCREEN_TXPIN  17

#define LED_PIN    5
#define LED_COUNT  33

#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_SS   15

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI FSPI
#endif

struct daisy_data {
  uint8_t sound_on;
  uint8_t left_hand;
  uint8_t right_hand;
  uint8_t placeholder;
};

long firstPixelHue = 0;

uint8_t screen_clr[] = {0x0C};
uint8_t screen_data[] = {0x1B, 0x46, 0x42, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x54, 0x4F, 0x55, 0x43, 0x48, 0x20, 0x4D, 0x45, 0x0D, 0x0A};

SPIClass *hspi = NULL;
static const int spiClk = 1000000;

uint8_t effect_value = 0;
uint8_t effect_value2 = 0;
int read_int = 0;
int rainbow_int = 0;
int screen_int = 0;
bool touch_is_touching = false;
bool touch2_is_touching = false;
int touch_value = 0;
int touch_value2 = 0;
bool screen_state = false;
static uint8_t buffer[4];
int modulate_speed = 0;
int modulate_speed2 = 0;
int touch_tracker = 200;
int touched_recently = 1;

struct daisy_data daisy {0, 0, 0, 0};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  
  Serial1.begin(SCREEN_BAUD, SERIAL_8N1, SCREEN_RXPIN, SCREEN_TXPIN);

  strip.begin();
  strip.show();
  strip.setBrightness(50);
  
  hspi = new SPIClass(HSPI);
  hspi->begin();
  pinMode(hspi->pinSS(), OUTPUT);
}

void loop() {
  // Read touch sensor periodically
  if (!read_int) {
    touch_value = touchRead(T0);
    touch_is_touching = touch_value < TOUCH_THRESHOLD;

    touch_value2 = touchRead(T7);
    touch2_is_touching = touch_value2 < TOUCH_THRESHOLD;

    Serial.printf("Touch: %d %d\n", touch_value, touch_value2);
    
    read_int = 50;
  }

  if (!modulate_speed){
    daisy.left_hand = modulate_effect_value(daisy.left_hand, touch_is_touching);
    modulate_speed = 4;
  }

  if (!modulate_speed2){
    daisy.right_hand = modulate_effect_value(daisy.right_hand, touch2_is_touching);
    modulate_speed2 = 4;
  }

  if (touch_is_touching || touch2_is_touching) {
    touched_recently = 1;
    touch_tracker = 1000;  // Reset the timer
  }

  if (touch_tracker == 0) {
    touched_recently = 0;
  }
  
  if (!rainbow_int) {
    if (touch_is_touching) {
      set_hue(daisy.left_hand);
    } else {
      rainbow_step();
    }

    rainbow_int = 5;
  }

  if (!screen_int) {
    if (!screen_state) {
      Serial1.write(screen_data, 19);
      screen_state = 1;
    } else {
      Serial1.write(screen_clr, 1);
      screen_state = 0;
    }

    screen_int = 100;
  }

  daisy.sound_on = touched_recently;
  spiCommand(hspi, (uint8_t*)&daisy, sizeof(daisy));
  
  read_int--;
  modulate_speed--;
  modulate_speed2--;
  touch_tracker--; 
  rainbow_int--;
  screen_int--;

  delay(10);
}

uint8_t modulate_effect_value(uint8_t previous_value, bool direction) {
  if (direction && previous_value < 255) {
    return previous_value + 1;
  }
  
  if (!direction && previous_value > 0) {
    return previous_value - 1;
  }

  return previous_value;
}

void modulate_effect2_value(bool is_touching) {
  if (is_touching && effect_value2 < 127) {
    effect_value2 +=1;
  } else if (!is_touching && effect_value2 >= 1) {
    effect_value2 -=1;
  }
}

void spiCommand(SPIClass *spi, const uint8_t *data, size_t length) {
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(spi->pinSS(), LOW);

#if SPI_DEBUG
  Serial.print("Sent: ");
#endif

  for (size_t i = 0; i < length; i++) {
    spi->transfer(data[i]);

#if SPI_DEBUG
    Serial.print(data[i]);
    Serial.print(" ");
#endif
  }

#if SPI_DEBUG
  Serial.println("");
#endif

  digitalWrite(spi->pinSS(), HIGH);
  spi->endTransaction();
}

void rainbow_step() {
  if (firstPixelHue > 5 * 65536) {
    firstPixelHue = 0;
  }

  firstPixelHue += 256;

  strip.rainbow(firstPixelHue);
  strip.show();
}

void set_hue(uint8_t input) {
  uint32_t c = input * 256;

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.ColorHSV(c));
  }

  strip.show();
}
