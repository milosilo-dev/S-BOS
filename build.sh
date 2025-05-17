#!/bin/sh

SOURCE="main.c"
TARGET="BOOTX64.EFI"
DISK_IMG_FOLDER="write_gpt"
DISK_IMG_PGM="./write_gpt"

QEMU="./scripts/qemu.sh"

find . -name '*.c' -or -name '*.h' -or -name '*.sh' | xargs wc -l | grep 'total'  | awk '{ SUM += $1; print $1} END { print "Total text lines",SUM }'

CC="x86_64-w64-mingw32-gcc -Wl,--subsystem,10 -e efi_main"

CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -mno-red-zone -ffreestanding -nostdlib -fno-stack-protector -mno-stack-arg-probe"

if [ ! -f "$DISK_IMG_FOLDER/$DISK_IMG_PGM" ]; then
    (cd "$DISK_IMG_FOLDER" && make)
fi

$CC $CFLAGS -o "$TARGET" "$SOURCE"

cp "$TARGET" "$DISK_IMG_FOLDER"
cd "$DISK_IMG_FOLDER"
./"$DISK_IMG_PGM" -v -ae / ../data/kiwi.bmp -ad ../data/test.txt

cd ..
$QEMU
cd "$DISK_IMG_FOLDER"

rm -f "$TARGET"
rm -f "FILE.TXT"
cd ..

rm -f "$TARGET"
