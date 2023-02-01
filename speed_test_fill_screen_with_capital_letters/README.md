# Acorn BBC BASIC speed test
Fill the entire screen with capital letters

## Results
As expected, the fastest implementation is the Hand written 6502 assembly version in BBC BASIC (`HAND_WRITTEN.txt`) that is even faster than the compiled C source code binary (`FILL_SCREEN_WITH_CHARACTERS`). `cc65` seems to generate a lot of SubRoutines/procedures and extra code. The pure BBC BASIC version seems to be slightly slower than the `cc65` generated one.

## Pure BBC BASIC version of the test
```
MODE 7
FOR I = &7C00 TO &7FE7
?I = 65 + ((I - 24) MOD 26)
NEXT I
```

### With no module operation
```
MODE 7
E = 0
FOR I = &7C00 TO &7FE7
?I = 65 + E
E = E + 1
IF E > 25 THEN E = 0
NEXT I
```

## C source code version
`fill_screen_with_characters.c`

### Compiled C source code binary
`FILL_SCREEN_WITH_CHARACTERS`

### Disassembled compiled C source code binary
`DISASSEMBLED.txt`
disassembled through the [masswerk's online disassembler](https://www.masswerk.at/6502/disassembler.html) and modified commenting out illegal opcodes.
<br/>
It doesn't work yet

### Disassembled compiled C binary in BASIC format
`DISASSEMBLED.BBC`

## Hand written 6502 assembly version in BBC BASIC
`HAND_WRITTEN.txt`

### Hand written 6502 assembly version in BBC BASIC and BBC BASIC format
`HAND_WRITTEN.BBC`

### Hand written 6502 assembly version in BBC BASIC with line numbers
`HAND_WRITTEN.line_number.txt`
Obtained using
```
nl -s' ' -v 10 HAND_WRITTEN.txt > HAND_WRITTEN.line_number.txt
```

## Floppy ADFS image
`FILL_SCREEN_WITH_CHARACTERS.adl`
