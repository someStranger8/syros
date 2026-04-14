#!/bin/bash

# isorun.sh - Runs syros.iso in qemu

if [ -e ./syros.iso ]; then
	echo "Running syros.iso in qemu-system-x86_64"
	qemu-system-x86_64 \
    		-cdrom syros.iso \
    		-m 512M \
    		-boot d \
    		-enable-kvm \
		-serial stdio
else 
	echo "syros.iso file not found"
fi
