VHD="../write_gpt/test.vhd"
DRIVE=$1

qemu-img convert -f vpc -O raw $VHD test.raw
sudo dd if=test.raw of=$DRIVE bs=4M status=progress
sync
sudo eject $DRIVE
rm test.raw