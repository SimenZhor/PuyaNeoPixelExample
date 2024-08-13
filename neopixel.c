// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <system.h>                               // system functions
#include <neo_spi.h>                              // NeoPixel functions
#include <gamma_lut.h>                            // Gamma correction

#define EYE_CNT 2
#define ENERGY_CNT 4
#define MAX_HUE 254

#define CLIP(val) ((val) > 254 ? 254 : (val) < 0 ? 0 : (val))
#define APPLY_GAMMA(val) (gamma_lut[(val)])
#define CLIP_AND_GAMMA(val) (APPLY_GAMMA(CLIP(val)))

uint8_t idx, new_r, new_g, new_b;
float factor;
uint8_t nst = 0;
// int16_t offset = 0;
uint8_t eye_idxs[EYE_CNT] = {0, 1};
uint8_t energy_idxs[ENERGY_CNT]  = {3, 4, 5, 6};

uint8_t next_flicker[ENERGY_CNT] = {0, 0, 0, 0};
uint8_t prev_flicker[ENERGY_CNT] = {0, 0, 0, 0};
uint8_t flicker[ENERGY_CNT]      = {0, 0, 0, 0};
// int max_value = 55;
// int min_value = 0;
// uint8_t next_flicker = (rand() % (max_value - min_value + 1) + min_value);

uint8_t eye_color[3] = {0, 0, 255};
uint8_t energy_color[3] = {226, 121, 35}; // Orange fire effect

// ===================================================================================
// Pseudo random number generator (Linear Congruential Generator)
// ===================================================================================

static uint8_t seed = 137;

void srand8(uint8_t s)
{
  seed = s;
}

uint8_t rand8(void)
{
  seed = seed * 37 + 17;
  return seed;
}

void srand(uint32_t s)
{
  seed = s;
}

uint32_t rand(void)
{
  seed = seed * 1103515245 + 12345;
  return (seed >> 16) & 0x7FFF;
}

// ===================================================================================
// Animation Functions
// ===================================================================================
uint8_t linear_intererpolation(uint8_t a, uint8_t b, float t)
{
  return (uint8_t)(a + t * (b - a));
}
void animateEyes(float brightnessFactor)
{

  new_r = APPLY_GAMMA((uint8_t)CLIP(eye_color[0] * brightnessFactor));
  new_g = APPLY_GAMMA((uint8_t)CLIP(eye_color[1] * brightnessFactor));
  new_b = APPLY_GAMMA((uint8_t)CLIP(eye_color[2] * brightnessFactor));
  for (idx = 0; idx < EYE_CNT; idx++)
  {
    NEO_writeColor(eye_idxs[idx], new_r, new_g, new_b);
  }
}
void animateEnergy(uint8_t counter)
{
  uint8_t freq_divider = 80;
  for (idx = 0; idx < ENERGY_CNT; idx++)
  {
    if (counter % freq_divider == 0)
    {
      // Slow down fire animation relative to the eye animation
      prev_flicker[idx] = next_flicker[idx]; 
      next_flicker[idx] = rand8() % 150;
    }

    // interpolate flickering
    flicker[idx] = linear_intererpolation(prev_flicker[idx], next_flicker[idx], (float)(counter % freq_divider) / (float)freq_divider);

    new_r = APPLY_GAMMA((uint8_t)CLIP(energy_color[0] - flicker[idx]));
    new_g = APPLY_GAMMA((uint8_t)CLIP(energy_color[1] - flicker[idx]));
    new_b = APPLY_GAMMA((uint8_t)CLIP(energy_color[2] - flicker[idx]));
    NEO_writeColor(energy_idxs[idx], new_r, new_g, new_b);
  }
}

// ===================================================================================
// Main Function
// ===================================================================================

int main(void) {
  // Setup
  NEO_init();
  NEO_clearAll();
  
  // Loop
  while(1) {
    // Fade in
    for (uint8_t brightness = 0; brightness < 255; brightness++)
    {
      // Eyes
      factor = (float) brightness / 255;
      animateEyes(factor);

      // Energy
      animateEnergy(brightness);
      NEO_update();
      DLY_ms(5);
    }

    // "Delay" (eyes don't change, energy is changing)
    for (int delay = 0; delay < (MAX_HUE + 1); delay++)
    {
      animateEnergy(delay);
      NEO_update();
      DLY_ms(5);
    }

    // Fade out
    for (uint8_t brightness = 0; brightness < 255; brightness++)
    {
      // Eyes
      factor = 1.0 - (float) brightness / 255;
      animateEyes(factor);

      // Energy
      animateEnergy(brightness);

      NEO_update();
      DLY_ms(5);
    }

    // "Delay" (eyes don't change, energy is changing)
    for(int delay = 0; delay < (MAX_HUE + 1); delay++){
      animateEnergy(delay);
      NEO_update();
      DLY_ms(5);
    }
  }
}
