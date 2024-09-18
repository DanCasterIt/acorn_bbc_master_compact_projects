#  Acorn BBC Master Compact 128K system ROM
The Acorn BBC Master Compact  MOS ROM *IC49* is by default a 64Kbyte ROM (27512 ROM IC), but if putting the jumper *PL12* on the *south* position a 128Kbyte IC can be put there.

This PCB project is a DIP-28 adaptor to place two 27512 EEPROMs instead of a single 27C010 into the system socket.

## Quick start
* Ensure to remove any ROM IC from the socket IC38 or the "L" shaped version of the PCB design won't fit on the Compact's PCB
  * The "L" shaped version of the PCB design is the one shown in the pictures
* Ensure to program the *PROMs like in the section [How to program the system and non-system ROMs for this PCB design](#How-to-program-the-system-and-non-system-ROMs-for-this-PCB-design) in this document
* Ensure that the jumper *PL12* is set to the *south* position in the master compact mother board (non-factory configuration)
* Issue the command **ROMS* on the Master Compact and ensure that the programmed *ROM image*s are present in ROM space banks 9 to C.
```
> *ROMS
ROM F UTILS 10
ROM E BASIC 40
ROM D ACORN ADFS 10
ROM C ? <------------ HERE
ROM B ? <------------ HERE
ROM A ? <------------ HERE
ROM 9 ? <------------ HERE
ROM 8 ?
RAM 7 ?
RAM 6 ?
RAM 5 ?
RAM 4 ?
ROM 3 ?
ROM 2 ?
ROM 1 ?
ROM 0 ?
```

Notes:
- The "L" shaped version of the PCB design will electrically allow to put a ROM on IC38, but due to space restrictions I had to put the two 27512 ROMs over that socket, so it's phisically impossible to put a ROM there unless putting a tall PIN header or multiple stacked sockets on the IC49 one, but the Compact's lid won't close anymore
  - A possible solution that would allow the Compact's lid to close would be to use a DIP28 extension cable, but the PCB would need to be stick somewhere otherwise it would float within the Compact case
  - The best solution would be to use an external ROM cartridge like my [Expansion ROM cartridge for the Acorn BBC Master Compact 8-bit computer](https://oshwlab.com/black__man/acorn-bbc-master-compact-cartridge_copy) to access the 32K of IC38 externally from the cartridge
    - It is better to have an external cartridge because putting jumper PL11 to south (non factory configuration) is somehow limiting because every external cartridge that has a software ROM on it won't work anymore on the Compact unless putting PL11 back to north
- The system/MOS ROM the Compact comes with can't be directly used on this PCB design unless reprogrammed with the correct memory layout
- The "L" shaped version of the PCB design requires a very short PIN header otherwise the Compact's lid won't close
  - The right PIN header height would need to be exactly the one of the sockets soldered on the Compact's PCB
- Other versions of the PCB design different from the "L" shaped one haven't been manufactured and/or tested and won't probably allow the Compact's lid to close
- The PCB design seems to work with the Acorn BBC Master too, but the memory layout must follow the one suited for the specific MOS version that is used by the Master. Refer to [Replacing the Master System ROM](https://mdfs.net/Info/Comp/BBC/SROMs/MegaROM.htm) for more information
  - All the versions of the PCB design would need a tall PIN header, stacked ROM sockets or DIP28 extension cord to fit in the Master because the econet connector will be below the system ROM
- The Compact's lid has been tested to close with socketed W27C512 EEPROMS. If using different taller *ROMS the lid might not close

## The system ROM socket *IC49*
Unfortunately the 128Kbyte/1Mbyte IC (27C010) is a DIP-32 IC, but the socket on the Master Compact is a DIP-28 one. I couldn't find any 1Mbyte parallel EEPROM IC that fits on a DIP-28 socket, so I guess that at Acorn they probably had in mind to use a mask ROM.

It is possible to build a DIP-28 to DIP-32 adapter to fit a 27C010 EPROM in the *IC49* system ROM socket like in the following picture: \
![Adaptor to enable 32-PIN EPROM to be used in BBC Master](https://mdfs.net/Info/Comp/BBC/SROMs/28to32.gif)

Only 7 ROM space banks (9 to F) are assigned to the *IC49* ROM despite the 27512 EPROM can contain up to 4 16Kbyte ROM images because the MOS ROM isn't showed as bank 10. Only bank switchable ROMs are shown by the **ROM* command.
```
> *ROMS
ROM F UTILS 10 <----------- IC49 socket
ROM E BASIC 40 <----------- IC49 socket
ROM D ACORN ADFS 10 <------ IC49 socket
ROM C ? <------------------ IC49 socket with PL12 on south
ROM B ? <------------------ IC49 socket with PL12 on south
ROM A ? <------------------ IC49 socket with PL12 on south
ROM 9 ? <------------------ IC49 socket with PL12 on south
ROM 8 ?
RAM 7 ?
RAM 6 ?
RAM 5 ?
RAM 4 ?
ROM 3 ?
ROM 2 ?
ROM 1 ?
ROM 0 ?
```

### *PL12* jumper behaviour explanation
The MOS ROM is always selected (*CS* PIN set *low*) and has the *OE* PIN set to *low* if *PL12* is on the *north* position and a 27512 ROM IC is present. Otherwise what should have been a 27C010 IC will not have an *OE* PIN and in place of it there will be the *A16* PIN with *PL12* set on the *south* position.

## How to program the system and non-system ROMs for this PCB design
The following instructions are ment for the Acorn MOS version 5.10, the one used in the Acorn BBC Master Compact. If using differens MOS versions, use the appropriate memory layout. \
The system ROM must contain the `MOS` ROM in its top partition `0x00000000-0x00003FFF` and the non-system ROM must contain `DFS/ADFS`, `BASIC` and `UTILS` ROMs in this exact order in its bottom three paritions respectively in `0x00004000-0x00007FFF`, `0x00008000-0x0000BFFF` and `0x0000C000-0x0000FFFF`.

More specifically: \
For the system ROM
```
0x00000000-0x00003FFF corresponds to no ROM slot and must contain MOS
0x00004000-0x00007FFF corresponds to ROM 9
0x00008000-0x0000BFFF corresponds to ROM A
0x0000C000-0x0000FFFF corresponds to ROM B
```
For the non-system ROM
```
0x00000000-0x00003FFF corresponds to ROM C
0x00004000-0x00007FFF corresponds to ROM D and must contain DFS/ADFS
0x00008000-0x0000BFFF corresponds to ROM E and must contain BASIC
0x0000C000-0x0000FFFF corresponds to ROM F and must contain UTILS
```

The `*ROMS` command will not list the `MOS` ROM and will list the remaining ones starting from the one at the bottom of the memory from the non-system ROM listed as `ROM F`, through the one at the top of the system ROM listed as `ROM 9`. \
This memory layout is basically the same that we would have had if we were using a `27C010` EPROM because the two `27512` are "emulating" a single `27C010`. \
The system will not boot at all if at least the `MOS` and `UTILS` ROMs aren't palced at the right places.

### GitHub tool to merge multple ROM images into a single file
The following GitHub project can be used to merge 4 ROM images into a single one that can be programmed into a single 27128 EPROM IC \
[github.com - ROM_images_merger](https://github.com/DanCasterIt/ROM_images_merger) \
A Windows x86 (32-bit) and Apple Silicon executable file is pushed into the repository so that the program can be runt without compiling it.

Usage example to generate the ROM images for the system and non-system ROMs ready to be programmed:
```
$ ./ROM_image_generator Internet_pc128s_rom/OS51.ROM KERMROM basiced EDITROM
Input the memory size in bytes. (0 for 65536): 0
Input the number of equally sized partitions. (0 for 4): 0
Input if to fill blank partitions with ones (1) or zeros (0): 0
Found 4 files. Choose where to place them.
partition 0 @ 0x00000000-0x00003FFF filled with file: Internet_pc128s_rom/OS51.ROM
partition 1 @ 0x00004000-0x00007FFF filled with file: KERMROM
partition 2 @ 0x00008000-0x0000BFFF filled with file: basiced
partition 3 @ 0x0000C000-0x0000FFFF filled with file: EDITROM
Apply changes? (y or n): y
Writing "ROM_image.bin"...
$ mv ROM_image.bin ROM_imageA.bin
$ ./ROM_image_generator DFSimage Internet_pc128s_rom/ADFS210.ROM Internet_pc128s_rom/BASIC48.ROM Internet_pc128s_rom/UTILS.ROM
Input the memory size in bytes. (0 for 65536): 0
Input the number of equally sized partitions. (0 for 4): 0
Input if to fill blank partitions with ones (1) or zeros (0): 0
Found 4 files. Choose where to place them.
partition 0 @ 0x00000000-0x00003FFF filled with file: DFSimage
partition 1 @ 0x00004000-0x00007FFF filled with file: Internet_pc128s_rom/ADFS210.ROM
partition 2 @ 0x00008000-0x0000BFFF filled with file: Internet_pc128s_rom/BASIC48.ROM
partition 3 @ 0x0000C000-0x0000FFFF filled with file: Internet_pc128s_rom/UTILS.ROM
Apply changes? (y or n): y
Writing "ROM_image.bin"...
$ mv ROM_image.bin ROM_imageB.bin
```
where `ROM_imageA.bin` goes to the system ROM socket and `ROM_imageB.bin` goes into the non-system ROM one. \
Then if using a TL866II+ programmer with the `minipro` command line tool and a W27C512 EEPROM, the immage can be flashed with the commands
```bash
$ minipro -s -p "W27C512@DIP28" -w ROM_imageA.bin
```
then swap the EEPROM...
```bash
$ minipro -s -p "W27C512@DIP28" -w ROM_imageB.bin
```
The `-m` option can be used in place of `-w` to verify that the write process completed correctly.

After programming and installing the *EPROMs on the adaptor, the `*ROMS` command will display the following on the PC
```
> *ROMS
ROM F UTILS 10
ROM E BASIC 40
ROM D ACORN ADFS 10
ROM C DFS 92
ROM B Edit 01
ROM A The BASIC Editor FF
ROM 9 KERMIT 01
ROM 8 ?
RAM 7 ?
RAM 6 ?
RAM 5 ?
RAM 4 ?
ROM 3 ?
ROM 2 ?
ROM 1 ?
ROM 0 ?
```

## ROM ICs
| ROM IC socket | EPROM | space bank | size | description |
| --- | --- | --- | --- | --- |
| IC49 | 27512 or 27C010 | D to F or 9 to F | 64Kbyte or 128Kbyte | system ROM quadruple or octuple image ROM |
| IC29 | 27128 | 8 | 16Kbyte | single image ROM |
| IC38 | 27128 or 27256 | 1 or 0 to 1 | 16Kbyte or 32Kbyte | edge connector or internal double image ROM |
| IC23 | 27128 | 2 | 16Kbyte | single image ROM |
| IC17 | 27128 | 3 | 16Kbyte | single image ROM |

## Useful links
- [BBC Master Compact SERVICE MANUAL](https://www.progettosnaps.net/manuals/pdf/bbcmc.pdf) for the computer schematic and Jumper settings
- [Mapped Master Compact Motherboard](http://www.8bs.com/incompmapped.htm) Master Compact motherboard picture with IC descriptions
- [Paged ROM](https://beebwiki.mdfs.net/Paged_ROM#Master_Compact) Paged ROM explanation with socket to space bank table and ROMFS header description
- [PC128S con una EPROM da 128k ?](https://olivettipc128s.altervista.org/pc128s-con-una-eprom-da-128k/) how to put a 27C010 EPROM into the *IC49* socket
- [BBC ROMS](http://www.flaxcottage.com/BBC_ROMS/Default.asp), [MDFS::System.ROMs](https://mdfs.net/System/ROMs/) a collection of ROM software for the Acorn BBC computers
- [The BBC and Master Computer Public Domain Library Circuit Diagrams](http://8bs.com/othrdnld/manuals/circuitdiagrams.shtml) high resolution circuit schematics
- [BBC Master 128 Cartridge Interface](http://8bs.com/submit/acornappspdf/035.pdf) The BBC Master 128 Cartridge Interface has the same signals as the Master Compact edge connector with no user port PINs
- [76K 2048x1600 EPROM cartridge](https://mdfs.net/Info/Comp/BBC/Circuits/Master/eprom.gif) Master 128 Cartridge circuit that can be easily adapter to work on the external edge connector of the Master Compact
- [Replacing the Master System ROM](https://mdfs.net/Info/Comp/BBC/SROMs/MegaROM.htm) How to build a DIP-28 to DIP-32 adapter to fit a 128Kbyte EPROM

## Images
![](https://oshwlab.com/attachments/2024/2/RT3SXSz7FH44PsN7wk818WD9kdR8vctoimQlYAM0.jpeg)
![](https://oshwlab.com/attachments/2024/2/UXlbVmvfDC9bN9r877mnYwfXGMX6P4kVMfV0sfjR.jpeg)
