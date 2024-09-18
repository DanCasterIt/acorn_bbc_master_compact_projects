# Expansion ROM cartridge for the Acorn BBC Master Compact 8-bit computer
This is an expansion ROM cartridge that plugs on the edge connector of the Acorn BBC Master Compact.
It is designed to be as compatct as possible and to fit a ZIF socket as well if needed.

## Quick start
* Ensure to program an *PROM like in the section [How to program the ROMs for this PCB design](#How-to-program-the-ROMs-for-this-PCB-design) in this document
* Ensure that the jumper *PL11* is set to the *north* position in the master compact mother board (factory configuration)
* Issue the command **ROMS* on the Master Compact and ensure that the programmed *ROM image*s are present in ROM space banks 0 and 1.
```
> *ROMS
ROM F UTILS 10
ROM E BASIC 40
ROM D ACORN ADFS 10
ROM C ?
ROM B ?
ROM A ?
ROM 9 ?
ROM 8 ?
RAM 7 ?
RAM 6 ?
RAM 5 ?
RAM 4 ?
ROM 3 ?
ROM 2 ?
ROM 1 ? <------------ HERE
ROM 0 ? <------------ HERE
```

The switch *SW1* on the cartridge helps selecting the upper or lower 32kbyte of memory of the external ROM if a 27512 EPROM IC is fitted.

If using a 27256 or lower capacity IC, ensure to set the *SW1* switch to the *ROM 0&1* position (0v).

**Commute the switch *SW1* only when the computer is powered off** to prevent MOS crashes, but if you really don't want to do a powercycle, ensure to flip the switch only when the `*ROMS` command isn't issued or finisced processing and when no language ROM or ROM software is used by the PC in that moment.

*Note*: when I say *ROM image* I refer to what's actually programmed in any 16Kbyte, 16Kbyte aligned, portion of the EPROM that contains runnable code like the Acorn DFS *ROM image*. A 27512 EPROM for instance can contain up to 4 16Kbyte *ROM image*s, but the Master Compact migh only see one or two of these images at the same time in the ROM space banks depending on wich socket the EPROM was installed or on the posizion of *SW1* on my external cartridge.

## How I ended up with this schematic
The EPROM named *IC38* can only be accessed if putting the jumper *PL11* in the *south* position in the master compact mother board. That means that we must choose if to use an exteral ROM through the expansion port OR if to use a ROM on the *IC38* socket. Those two choises are mutual exclusive and can't be used at the same time. By default the jumper *PL11* is put on the *north* position.

### *PL11* jumper behaviour explanation
What the *PL11* jumper does is to pass the signal *AT13* aka *nOE* (not output enable)... to the *gate* PIN of the multiplexer *IC12*. A pull up 10k resistor on that PIN ensures that when the *PL11* is put to the *north* position, the gate PIN goes to *high* and puts all the *Y* outputs PINs of the multiplexer on *high* disabling the *IC38* ROM. The *nOE* PIN on the ROM *IC38* will disable the output PINs of the *IC38* ROM when set *high*.

The schematic of my cartridge extension circuit is a copy of the *IC38* + *IC12* design on the Master Compact mother board on an external cartridge circuit.

The EPROM slot *IC38* can accept only two kind of ROM ICs

| ROM size | EPROM IC |
| -------- | -------- |
| 16Kbyte | 27128 |
| 32Kbyte | 27256 |

all the other EPROM slots on the Master Compact except the MOS EPROM named *IC49* can only accept 16k byte/27128 ROMs.

My expansion cartridge ROM will accept 27512 ROM ICs, but the Master Compact will only see two *ROM image*s at a time. With the switch *SW1* on the expansion cartridge is possible to select if to access the upper or lower *ROM image*s on the external cartridge. The images will be displayed as ROM space banks 0 and 1

## How to program the ROMs for this PCB design
The `*ROMS` command will list the ROMs `0` and `1` starting from the one at the bottom partition of the memory (`0x0` or `0x8000`) listed as `ROM 0`, through the one at the top partition of the ROM (`0x4000` or `0xC000`) listed as `ROM 1`. \
More specifically:
```
0x00000000-0x00003FFF corresponds to ROM 0 with SW @ low  ("ROM 0&1")
0x00004000-0x00007FFF corresponds to ROM 1 with SW @ low  ("ROM 0&1")
0x00008000-0x0000BFFF corresponds to ROM 0 with SW @ high ("ROM 2&3")
0x0000C000-0x0000FFFF corresponds to ROM 1 with SW @ high ("ROM 2&3")
```

### GitHub tool to merge multple ROM images into a single file
The following GitHub project can be used to merge 4 ROM images into a single one that can be programmed into a single 27128 EPROM IC \
[github.com - ROM_images_merger](https://github.com/DanCasterIt/ROM_images_merger) \
A Windows x86 (32-bit) and Apple Silicon executable file is pushed into the repository so that the program can be runt without compiling it.

Usage example:
```
$ ./ROM_image_generator.macM1 BASICED135 BASICEX EDIT110 ViewJ300lo
Input the memory size in bytes. (0 for 65536): 0
Input the number of equally sized partitions. (0 for 4): 0
Input if to fill blank partitions with ones (1) or zeros (0): 0
Found 4 files. Choose where to place them.
partition 0 @ 0x00000000-0x00003FFF filled with file: BASICED135
partition 1 @ 0x00004000-0x00007FFF filled with file: BASICEX
partition 2 @ 0x00008000-0x0000BFFF filled with file: EDIT110
partition 3 @ 0x0000C000-0x0000FFFF filled with file: ViewJ300lo
Apply changes? (y or n): y
Writing "ROM_image.bin"...
```
Then if using a TL866II+ programmer with the `minipro` command line tool and a W27C512 EEPROM, the immage can be flashed with the command
```bash
minipro -s -p "W27C512@DIP28" -w ROM_image.bin
```
The `-m` option can be used in place of `-w` to verify that the write process completed correctly.

After programming and installing the *PROM on the cartridge, the result we will be:
```
BASICED135 to ROM 0 with SW @ low  ("ROM 0&1")
BASICEX    to ROM 1 with SW @ low  ("ROM 0&1")
EDIT110    to ROM 0 with SW @ high ("ROM 2&3")
ViewJ300lo to ROM 1 with SW @ high ("ROM 2&3")
```

When SW is at low ("ROM 0&1"):
```
> *ROMS
ROM F UTILS 10
ROM E BASIC 40
ROM D ACORN ADFS 10
ROM C ?
ROM B ?
ROM A ?
ROM 9 ?
ROM 8 ?
RAM 7 ?
RAM 6 ?
RAM 5 ?
RAM 4 ?
ROM 3 ?
ROM 2 ?
ROM 1 BASIC Extensions 01
ROM 0 The BASIC Editor 35
```

When SW is at high ("ROM 2&3"):
```
> *ROMS
ROM F UTILS 10
ROM E BASIC 40
ROM D ACORN ADFS 10
ROM C ?
ROM B ?
ROM A ?
ROM 9 ?
ROM 8 ?
RAM 7 ?
RAM 6 ?
RAM 5 ?
RAM 4 ?
ROM 3 ?
ROM 2 ?
ROM 1 View 04
ROM 0 Edit 01
```

## ROM ICs
| ROM IC socket | EPROM | space bank | size | description |
| ------------- | ----- | ---------- | ---- | ----------- |
| IC49 | 27512 or 27C010 | D to F or 9 to F | 64Kbyte or 128Kbyte | system ROM quadruple or octuple image ROM |
| IC29 | 27128 | 8 | 16Kbyte | single image ROM |
| IC38 | 27128 or 27256 | 1 or 0 to 1 | 16Kbyte or 32Kbyte | edge connector or internal double image ROM |
| IC23 | 27128 | 2 | 16Kbyte | single image ROM |
| IC17 | 27128 | 3 | 16Kbyte | single image ROM |

## Useful links
* [BBC Master Compact SERVICE MANUAL](https://www.progettosnaps.net/manuals/pdf/bbcmc.pdf) for the computer schematic and Jumper settings
* [Mapped Master Compact Motherboard](http://www.8bs.com/incompmapped.htm) Master Compact motherboard picture with IC descriptions
* [Paged ROM](https://beebwiki.mdfs.net/Paged_ROM#Master_Compact) Paged ROM explanation with socket to space bank table and ROMFS header description
* [PC128S con una EPROM da 128k ?](https://olivettipc128s.altervista.org/pc128s-con-una-eprom-da-128k/) how to put a 27C010 EPROM into the *IC49* socket
* [BBC ROMS](http://www.flaxcottage.com/BBC_ROMS/Default.asp), [MDFS::System.ROMs](https://mdfs.net/System/ROMs/) a collection of ROM software for the Acorn BBC computers
* [The BBC and Master Computer Public Domain Library Circuit Diagrams](http://8bs.com/othrdnld/manuals/circuitdiagrams.shtml) high resolution circuit schematics
* [BBC Master 128 Cartridge Interface](http://8bs.com/submit/acornappspdf/035.pdf) The BBC Master 128 Cartridge Interface has the same signals as the Master Compact edge connector with no user port PINs
* [76K 2048x1600 EPROM cartridge](https://mdfs.net/Info/Comp/BBC/Circuits/Master/eprom.gif) Master 128 Cartridge circuit that can be easily adapter to work on the external edge connector of the Master Compact
* [Replacing the Master System ROM](https://mdfs.net/Info/Comp/BBC/SROMs/MegaROM.htm) How to build a DIP-28 to DIP-32 adapter to fit a 128Kbyte EPROM

## Images
![](https://oshwlab.com/attachments/2023/9/uSzn1Kl0ADko5lGjmrp0Ovglh6o7KOOxuqHIODAs.jpeg)
![](https://oshwlab.com/attachments/2023/9/QBBOIjEcOmMrtnVRj3M7hkFt7PF5859bgB2Lv9QO.jpeg)
![](https://oshwlab.com/attachments/2023/9/3welaU0h2qezhIpmBgbcYkxrSg3ZXdBagnEBRueO.jpeg)
![](https://oshwlab.com/attachments/2023/9/1kqi924HNgHCPkfmsuyZtFZdK9A3oNhu5Q5kwt2E.jpeg)
