# A collection of small Acorn BBC Master Compact related projects
This is a small collection of Acorn BBC Master Compact projects mainly in C language I made to train myself.

## List of all the contained submodules
```
$ git submodule--helper list
160000 60a151e8c43dc850d12057a73770f49b79883116 0       cc65
```

Important [discussion](https://stardot.org.uk/forums/viewtopic.php?t=12083) where I found this submodule repository and some of the information in this `README.md` on how to compile and run the sources.

[Built Windows snapshot](https://www.dropbox.com/s/chnobakkkatddq9/cc65-bbc-win32.zip?dl=0)
<br/>
Credits 8bit-Dude.

## How to build cc65
```
git submodule update --init -- cc65
cd cc65
make
```

## How to compile an Acorn BBC Master Compact source example
Assuming that the executable is called DUMP and the source dump.c:
```
$ cd small_C_projects
$ ../compile.sh dump.c DUMP
cl65 V2.15
Copiling dump.c into DUMP
```

## How to run the executable on the Master Compact example
Assuming that the executable is called DUMP:
1) Put the executable inside an .adl or .ssd floppy image using something like [Beeb Image](http://www.cowsarenotpurple.co.uk/beeb-image.html) or MkImg from [mdfs's DiskTools](https://mdfs.net/Apps/DiskTools/)
2) Set the 'Load' and 'Execution' addres of the executable to '00000E00'
3) Load the diskette into the machine or the image into the emulator and run
```
> MODE 7
> *MOUNT n
> *RUN DUMP
```
where `n` can be 0 to 3 depending which floppy drive we use, or after step 1 load the diskette into the machine or the image into the emulator and run
```
> MODE 7
> *MOUNT n
> *LOAD DUMP E00
> CALL &E00
```
A `!BOOT` file can be also created to automatically run what listed above with `SHIFT + BREAK`.
See [here](https://github.com/damieng/BBCMicroTools/blob/master/tips-and-tricks.md) how to make `!BOOT`.
<br/>
In short:
```
> *OPT 4,3
> *BUILD !BOOT
MODE 7
*RUN DUMP
```
Press ESCAPE

## Useful floppy drive usage stuff
### Load DFS ROM image from the Master Compact Welcome disk to read DFS formatted floppies
```
> *INSERT 7
> *SRLOAD $.Library.DFSimage 8000 7Q
> CH."CONVERT"
```

### Mount and manage files on flopy drive n on ADFS/DFS:
```
> *ADFS (or *DFS)
> *MOUNT n (or *DRIVE n on DFS)
> *.
> SAVE "file_name"
> *.
> *DELETE file_name
```
Where `n` can be 0 to 3 depending on how many floppy drives we have in our system. A maximum of four floppy drives can be installed as the Shugart interface standard specifies.
<br/><br/>
When mounting floppy drive `n` and running `*CAT $`, `*CAT`, `*. $` or `*.` with a non specified drive number, they will list what's on the currently mounted drive. If no drives are mounted, the same commands will automatically mount drive 0 on ADFS, so unless using drive 1, 2 or 3, the `*MOUNT n` command can be avoided if the intention is to mount and list drive 0.
<br/><br/>
If using drives `n > 0`, the drive number must be specified and so the commands `*CAT :n.$`, `*CAT :n`, `*. :n.$` or `*. :n` will list and mount drives `n > 0` as well. These commands can be also used to specifically list and mount floppy drive `n` even when drive `n1 != n`  is currently mounted.
<br/><br/>
The `*MOUNT n` command is basically only used to specify which drive should be accessed when not specified in the `*CAT :n.$`, `*CAT :n`, `*. :n.$`,  `*. :n` or ADFS commands in general, otherwise the last mounted drive, even through a non `*MOUNT n` command, will be used.

### Unspecified floppy drive directory listing
`*CAT $`, `*CAT`, `*. $` or `*.`

### Specific floppy drive n directory listing
`*CAT :n.$`, `*CAT :n`, `*. :n.$` or `*. :n`

### Pathnames representation in Acorn BBC format
Assuming a pathname `A:\dir1\dir2\file` in DOS format, in the Acorn BBC format it will be represented as `$.dir1.dir2.file` if drive 0 is mounted or `:n.$.dir1.dir2.file` with `n = 0`. A and B floppy drives in DOS can be seen as drive 0 and 1 in the Acorn BBC. The third and fourth floppy drives (drive number 2 and 3 in the Acorn BBC) where supposed to be external drives in the original IBM PC. That's why the internal floppy drive connector cable of the PC architecture has a twist and allows only two drives to be connected.
<br/><br/>
To navigate through the directories the `*DIR directory_name` command can be used.
