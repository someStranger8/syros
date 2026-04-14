#!/bin/bash

# isogen.sh - Generates an iso image of syros

if [ ! -d "limine" ]; then
    echo "Cloning limine repository"
    git clone https://github.com/Limine-Bootloader/Limine.git --branch=v11.x-binary --depth=1 limine
else
    echo "Limine directory already exists, skipping clone"
fi

echo "Building limine utility"
make -C limine

if [ ! -f "build/syros.elf" ]; then
    echo "ERROR: build/syros.elf not found! Did you compile your kernel?"
    exit 1
fi

echo "Preparing iso_root"
rm -rf iso_root
mkdir -p iso_root/boot/limine
mkdir -p iso_root/EFI/BOOT

cp -v build/syros.elf iso_root/boot/
cp -v limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin \
      limine/limine-uefi-cd.bin iso_root/boot/limine/
cp -v limine.conf iso_root/
cp -v limine.conf iso_root/EFI/BOOT/
cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

echo "Creating bootable syros.iso image"
xorriso -as mkisofs -R -r -J \
    -b boot/limine/limine-bios-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot boot/limine/limine-uefi-cd.bin \
    -efi-boot-part --efi-boot-image --protective-msdos-label \
    iso_root -o syros.iso

echo "Installing limine stage1 and stage2"
chmod +x ./limine/limine
./limine/limine bios-install syros.iso
