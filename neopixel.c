// ===================================================================================
// Project:   Example for PY32F0xx
// Version:   v1.0
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// Shows a colorful animation on a string of 8 NeoPixels. The number of pixels can be
// changed in neo_spi.h. Connect pin PA7 (MOSI) to DIN of the pixels string.
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-arm-none-eabi) and Python3 with PySerial is
//   installed. If necessary, a driver for the USB-to-serial converter used must
//   be installed.
// - Connect the USB-to-serial converter to the MCU board.
// - Run 'make flash'.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <system.h>                               // system functions
#include <neo_spi.h>                              // NeoPixel functions

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Setup
  uint8_t pixel, hue_val;
  uint8_t nst = 0;
  NEO_init();
  
  // Loop
  while(1) {
    hue_val = nst;
    for(pixel=0; pixel<NEO_COUNT; pixel++) {
      NEO_writeHue(pixel, hue_val, 0);
      hue_val += 192 / NEO_COUNT;
      if(hue_val >= 192) hue_val -= 192;
    }
    NEO_update();
    if(!nst--) nst = 191;
    DLY_ms(10);
  }
}
