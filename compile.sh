THIS_DIR="$(dirname "${BASH_SOURCE[0]}")"
IN_FILE=$1
OUT_FILE=$2
CL65_PATH=$(realpath $THIS_DIR/cc65/bin)
LIBRARY=$THIS_DIR/cc65/libsrc/bbc/oslib/os.s

$CL65_PATH/cl65 --version
printf "Copiling %s into %s\n" $IN_FILE $OUT_FILE
$CL65_PATH/cl65 -t bbc $IN_FILE -o $OUT_FILE $LIBRARY

