// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <system.h>                               // system functions
#include <neo_spi.h>                              // NeoPixel functions
#include <gamma_lut.h>                            // Gamma correction

#define EYE_CNT 2
#define ENERGY_CNT 6
#define MAX_HUE 191

#define CLIP(val) ((val) > 254 ? 254 : (val) < 0 ? 0 : (val))
#define APPLY_GAMMA(val) (gamma_lut[(val)])
#define CLIP_AND_GAMMA(val) (APPLY_GAMMA(CLIP(val)))

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Setup
  uint8_t idx, new_r, new_g, new_b, hue_val;
  float factor;
  uint8_t nst = 0;
  // int16_t offset = 0;
  uint8_t eye_idxs[EYE_CNT] = {0, 1};
  uint8_t energy_idxs[ENERGY_CNT] = {2, 3, 4, 5, 6, 7};

  uint8_t eye_color[3] = {0, 0, 255}; 
  uint8_t energy_color[3] = {252, 144, 3}; 
  NEO_init();
  
  // Loop
  while(1) {
    // Fade in
    for (uint8_t brightness = 0; brightness < 255; brightness++)
    {
      // Eyes
      factor = (float) brightness / 255;

      new_r = APPLY_GAMMA((uint8_t)CLIP(eye_color[0] * factor));
      new_g = APPLY_GAMMA((uint8_t)CLIP(eye_color[1] * factor));
      new_b = APPLY_GAMMA((uint8_t)CLIP(eye_color[2] * factor));
      for (idx = 0; idx < EYE_CNT; idx++)
      {
        NEO_writeColor(eye_idxs[idx], new_r, new_g, new_b);
      }

      // Energy
      hue_val = nst;
      for (idx = 0; idx < ENERGY_CNT; idx++)
      {
        NEO_writeHue(energy_idxs[idx], hue_val, 0);
        hue_val += (MAX_HUE + 1) / ENERGY_CNT;
        if (hue_val >= (MAX_HUE + 1))
          hue_val -= (MAX_HUE + 1);
      }
      NEO_update();
      if (!nst--) nst = MAX_HUE;
      DLY_ms(5);
    }

    // "Delay" (eyes don't change, energy is changing)
    hue_val = nst;
    for (idx = 0; idx < ENERGY_CNT; idx++)
    {
      NEO_writeHue(energy_idxs[idx], hue_val, 0);
      hue_val += (MAX_HUE + 1) / ENERGY_CNT;
      if (hue_val >= (MAX_HUE + 1))
        hue_val -= (MAX_HUE + 1);
    }

    NEO_update();
    if (!nst--)
      nst = MAX_HUE;
    DLY_ms(5);

    // Fade out
    for (uint8_t brightness = 0; brightness < 255; brightness++)
    {
      // Eyes
      factor = 1.0 - (float) brightness / 255;

      new_r = APPLY_GAMMA((uint8_t)CLIP(eye_color[0] * factor));
      new_g = APPLY_GAMMA((uint8_t)CLIP(eye_color[1] * factor));
      new_b = APPLY_GAMMA((uint8_t)CLIP(eye_color[2] * factor));
      for (idx = 0; idx < EYE_CNT; idx++)
      {
        NEO_writeColor(eye_idxs[idx], new_r, new_g, new_b);
      }

      // Energy
      hue_val = nst;
      for (idx = 0; idx < ENERGY_CNT; idx++)
      {
        NEO_writeHue(energy_idxs[idx], hue_val, 0);
        hue_val += (MAX_HUE + 1) / ENERGY_CNT;
        if (hue_val >= (MAX_HUE + 1))
          hue_val -= (MAX_HUE + 1);
      }

      NEO_update();
      if (!nst--) nst = MAX_HUE;
      DLY_ms(5);
    }

    // "Delay" (eyes don't change, energy is changing)
    for(int delay = 0; delay < (MAX_HUE + 1); delay++){
      hue_val = nst;
      for (idx = 0; idx < ENERGY_CNT; idx++)
      {
        NEO_writeHue(energy_idxs[idx], hue_val, 0);
        hue_val += (MAX_HUE + 1) / ENERGY_CNT;
        if (hue_val >= (MAX_HUE + 1))
          hue_val -= (MAX_HUE + 1);
      }
      NEO_update();
      if (!nst--)
        nst = MAX_HUE;
      DLY_ms(5);
    }
  }
}
