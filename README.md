# Captain Marvel SAO Firmware

Firmware for my Captain Marvel Simple(/Shitty) Add-On (SAO). The board uses a [Puya PY32F002A MCU](https://www.lcsc.com/product-detail/Microcontrollers-MCU-MPU-SOC_PUYA-PY32F002AL15S6TU_C5292060.html) and [SK6805-EC3210R individually addressable LEDs](https://www.lcsc.com/product-detail/RGB-LEDs-Built-in-IC_OPSCO-Optoelectronics-SK6805-EC3210R_C5378735.html).

> [!WARNING]
> This repository has not been prettified (and probably never will be), so don't trust any comments in the files without reading/understanding the code.
>
> For better documentation of how to use the PY32F002 and the LEDs, check the [resources at the bottom](#resources)

Most of the code in this repo is either copied from or based on [this Neopixel on PY32 example](https://github.com/wagiminator/Development-Boards/blob/main/PY32F002AF15P_DevBoard/software/neopixel_spi/src/main.c) by [wagiminator](https://github.com/wagiminator/). I did make some slight modifications (most notably to the default CPU clock) to make it run as expected.

I also used the makefile from the same example to compile (using WSL on Windows), but I did not use the flashing tool. 

For flashing I used a mishmash of a config based on guides linked in the resources listed below

## Resources

### For the MCU
- [The cheapest flash microcontroller you can buy is actually an Arm Cortex-M0+](https://jaycarlson.net/2023/02/04/the-cheapest-flash-microcontroller-you-can-buy-is-actually-an-arm-cortex-m0/)
    - Github repo by the same author: [py32-template](https://github.com/jaydcarlson/py32-template).
        - The guide above refers to a [DFP](https://web.archive.org/web/20230205071913if_/https://www.puyasemi.com/uploadfiles/2022/11/PY-MCU资料-20221117.rar) which was not accessible through Puya's official site at the time when I wanted to download it. I did however find it on [the internet archive](https://web.archive.org/web/20230205071913if_/https://www.puyasemi.com/uploadfiles/2022/11/PY-MCU资料-20221117.rar). The download in the archive link  contains version `1.1.0` the DFP pack (`pack\MDK\Keil\Puya.PY32F0xx_DFP.1.1.0.pack`)
        - At the time of writing this is the official link to the Puya "Downloads" website where these resources can be found: https://www.puyasemi.com/en/gongjuyuruanjian.html 
- Another Github Template: [https://github.com/IOsetting/py32f0-template](py32f0-template)
- Datasheet and Technical Reference Manual (community archive since Puya's sites can't be trusted): [https://py32.org](https://py32.org/en/mcu/PY32F002Axx.html#introduction)
    - Github repo: [https://github.com/OpenPuya/website](https://github.com/OpenPuya/website)

### For Simple/Shitty Add-Ons (footprints etc)
- https://hackaday.io/project/52950-shitty-add-ons
- https://hackaday.io/project/175182-simple-add-ons-sao