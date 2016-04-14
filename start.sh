#!/bin/sh
#
#Restart NFS 
sudo /etc/init.d/nfs-kernel-server restart
#Start QEMU
#sudo ./qemu-2.5.0/arm-softmmu/qemu-system-arm -cpu arm9 -M versatilepb -kernel zImage_new -net nic -net tap -append "root=/dev/nfs rw nfsroot=192.168.114.1:/home/cyhsu/rootfs/NFS ip=192.168.114.2::192.168.114.1:255.255.255.0" #-d in_asm #-gdb tcp::1234 -S

sudo ./qemu-2.5.0/arm-softmmu/qemu-system-arm -cpu arm11mpcore -m 256 -M realview-eb-mpcore -kernel zImage_arm11mpcore -smp 4 -net nic -net tap -append "root=/dev/nfs rw nfsroot=192.168.114.1:/home/cyhsu/rootfs/NFS ip=192.168.114.2::192.168.114.1:255.255.255.0" #-d in_asm #-gdb tcp::1234 -S
