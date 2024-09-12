# Captain Marvel SAO Firmware

Firmware for my Captain Marvel Simple(/Shitty) Add-On (SAO). The board uses a [Puya PY32F002A MCU](https://www.lcsc.com/product-detail/Microcontrollers-MCU-MPU-SOC_PUYA-PY32F002AL15S6TU_C5292060.html) and [SK6805-EC3210R individually addressable LEDs](https://www.lcsc.com/product-detail/RGB-LEDs-Built-in-IC_OPSCO-Optoelectronics-SK6805-EC3210R_C5378735.html).

> [!WARNING]
> This repository has not been prettified (and probably never will be), so don't trust any comments in the files without reading/understanding the code.
>
> For better documentation of how to use the PY32F002 and the LEDs, check the [resources at the bottom](#resources)

Most of the code in this repo is either copied from or based on [this Neopixel on PY32 example](https://github.com/wagiminator/Development-Boards/blob/main/PY32F002AF15P_DevBoard/software/neopixel_spi/src/main.c) by [wagiminator](https://github.com/wagiminator/). I did make some slight modifications (most notably to the default CPU clock) to make it run as expected.

I also used the makefile from the same example to compile (using WSL on Windows), but I did not use the flashing tool. 

For flashing I first used a mishmash of a config based on guides linked in the resources listed below and the Arm Keil µVision IDE. I then set up a J-Link debugging environment based on the instructions in [py32-template](https://github.com/jaydcarlson/py32-template). I did however find those instructions a bit hard to follow, so I've expanded on them in the [section right below](#debugging-support-in-vscode-pyocd-and-j-link-support) and written out every step I did (some are probably unnecessary, like setting up PyOCD when I end up using J-Link instead).

## Debugging support in VScode (PyOCD and J-Link support):
Written out version of [py32-template](https://github.com/jaydcarlson/py32-template) with more explicit steps

### Installing and configuring PyOCD:
1. pip install pyocd
2. Print cache directory with: `pack-manager.exe print-cache-dir`
3. make that directory with `mkdir <path\to\cache-directory>`
4. Copy `Puya.PY32F0xx_DFP.1.1.0.pack` and rename to `Puya.PY32F0xx_DFP.1.1.0.zip` (it's just a ZIP with a .pack extension). 
5. Unzip `Puya.PY32F0xx_DFP.1.1.0.zip`
6. Copy the pdesc file to cmsis-pack-manager's cache directory (the folder created in step 2) 
7. Rename the pdsc file to: `Puya.PY32F0xx_DFP.1.1.0.pdsc` (with the version numbers present)
8. Copy the original DFP pack file to `<path\to\cache-directory>\Puya\PY32F0xx_DFP\1.1.0.pack` creating directories and renaming the pack file accordingly.
9. Add the pack to cmsis-pack-manager with: `pack-manager add-packs <path\to\cache-directory>\Puya.PY32F0xx_DFP.1.1.0.pdsc`
10. Find the name of the pack you want to install in PyOCD with: `pyocd pack find PY32`
11. Install the pack, for example: `pyocd pack install PY32F002Ax5`

### Install J-Link support:
Using the [J-Link Device Support Kit Segger Wiki page](https://wiki.segger.com/J-Link_Device_Support_Kit) as a reference. 
1. Note: These instructions assume J-Link version >= V7.62
1. Create a `JLinkDevices\Puya\PY32` directory structure in your home directory's Segger application data folder (on Windows, it's in C:\Users\<USER>\AppData\Roaming\SEGGER\). 
2. Copy all the FLM files from the unzipped `Puya.PY32F0xx_DFP.1.1.0\Flash` to this directory
3. Add a Devices.XML file that points J-Link to these files. Here's an example for the PY32F002Ax5:

<Database>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F002Ax5" WorkRAMAddr="0x20000000" WorkRAMSize="3072" Core="JLINK_CORE_CORTEX_M0" />
    <FlashBankInfo Name="Internal code flash" BaseAddr="0x08000000" AlwaysPresent="1" >
      <LoaderInfo Name="Default" MaxSize="0x5000" Loader="PY32F0xx_20.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" />
    </FlashBankInfo>
  </Device>
</Database>

4. The WorkRAMAddr, WorkRAMSize, BaseAddr, MaxSize and Loader should be manually updated. The values can be found from memory definitions in the corresponding header file in `Device\Include`. For example: `Puya.PY32F0xx_DFP.1.1.0\Device\Include\py32f002ax5.h`.
	- `WorkRAMAddr` = SRAM_BASE
	- `WorkRamSize` = (SRAM_END - SRAM_BASE) + 1    = (0x20000BFF - 0x20000000) + 1 = 3072
	- `BaseAddr`    = FLASH_BASE
	- `MaxSize`     = (FLASH_END - FLASH_BASE) + 1  = (0x08004FFF - 0x08000000) + 1 = 0x5000
	- In this case, MaxSize = 0x5000 = 20480, so the `Loader` should be the FLM file that ends in `_20` indicating 20k flash. This info can also be found in the datasheet if the rounding is ambiguous for some reason.

### Install VSCode debugger

1. Install [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) from the VSCode extension marketplace
2. Make sure `serverpath` in [launch.json](./.vscode/launch.json) is correct for your installation of J-Link
3. Make sure WSL paths to your workspace folder are correct in [tasks.json](./.vscode/tasks.json). 
  - Paths to the C: drive in WSL start with `/mnt/c/` instead of `C:/`, so I didn't use the VSCode `${workspaceFolder}` variable when calling `make` in WSL. 

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

