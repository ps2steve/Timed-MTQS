#!/bin/sh
#
#Restart NFS 
sudo /etc/init.d/nfs-kernel-server restart
#Start QEMU
sudo ./arm-softmmu/qemu-system-arm -cpu arm11mpcore -m 256 -M realview-eb-mpcore -kernel zImage_new -smp 4 -net nic -net tap -append "root=/dev/nfs rw nfsroot=192.168.114.1:/home/cyhsu/rootfs/NFS ip=192.168.114.2::192.168.114.1:255.255.255.0" # -D ~/qemu_debug.log -d in_asm # -d in_asm,op,out_asm 2> /dev/null #-gdb tcp::1234 -S
