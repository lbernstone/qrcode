#include <Wire.h>
#include <U8g2lib.h> // https://github.com/olikraus/u8g2
#include <qrcode.h> // https://github.com/ricmoo/qrcode/

#define OLED_PINS 10,12,11 // RST,CLK,DATA
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, OLED_PINS);
QRCode qrcode;
const char *google = "https://www.google.com";

void show_qrcode(const char *str_qrcode, uint8_t qr_scale)
  // qr_scale is number of pixels per module
{
  // I would assume there is some calculation for this, but an array is easy
  uint16_t version_size[] = {0, 0, 17, 33, 54, 79, 107, 135, 155, 193};
  uint16_t max_w = u8g2.getDisplayWidth();
  uint16_t max_h = u8g2.getDisplayHeight();
  uint16_t max_pixels = (max_w > max_h) ? max_h : max_w;
  uint8_t qr_version = ((max_pixels / qr_scale) - 17) / 4; 
  uint16_t w = (qr_version * 3 + 17) * qr_scale;
  uint8_t qrcodeData[qrcode_getBufferSize(qr_version)];
  qrcode_initText(&qrcode, qrcodeData, qr_version, ECC_LOW, str_qrcode);

  u8g2.firstPage();
  do {
    if ((strlen(str_qrcode) > version_size[qr_version]) || strlen(str_qrcode) > 231) {
      u8g2.setFont(u8g2_font_5x7_mf);
      u8g2.drawStr(0, 10, "QR string is too long");
      u8g2.drawStr(0, 20, str_qrcode);
    } else {
      u8g2.drawBox(0, 0, max_pixels, max_pixels); // frame the whole thing
      uint8_t x0 = (max_pixels - w) / qr_scale / 2; // offset inside frame
      uint8_t y0 = (max_pixels - w) / qr_scale / 2;
  
      // loop through the 1 module bitmap
      for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
          u8g2.setColorIndex(!qrcode_getModule(&qrcode, x, y));
          u8g2.drawBox(x0 + x * qr_scale, y0 + y * qr_scale, qr_scale, qr_scale);
        }
      }
    }
  } while (u8g2.nextPage());
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setContrast(20);
  show_qrcode(google, 2);
}

void loop() {
  delay(-1);
}
