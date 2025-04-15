#!/bin/sh

SOURCE="main.c"
TARGET="BOOTX64.EFI"
DISK_IMG_FOLDER="write_gpt"
DISK_IMG_PGM="./write_gpt"

QEMU="./scripts/qemu.sh"

CC="x86_64-w64-mingw32-gcc -Wl,--subsystem,10 -e efi_main"

CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -mno-red-zone -ffreestanding -nostdlib -fno-stack-protector -mno-stack-arg-probe"

# Build disk image program if needed
if [ ! -f "$DISK_IMG_FOLDER/$DISK_IMG_PGM" ]; then
    (cd "$DISK_IMG_FOLDER" && make)
fi

# Compile the EFI application
$CC $CFLAGS -o "$TARGET" "$SOURCE"

# Copy and write the image
cp "$TARGET" "$DISK_IMG_FOLDER"
cd "$DISK_IMG_FOLDER"
./"$DISK_IMG_PGM" -v -ae / ../data/kiwi.bmp

# Run QEMU
cd ..
$QEMU
cd "$DISK_IMG_FOLDER"

rm -f "$TARGET"
rm -f "FILE.TXT"
cd ..

# Clean up
rm -f "$TARGET"
