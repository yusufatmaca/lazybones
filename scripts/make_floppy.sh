#!/bin/bash

sudo /sbin/losetup /dev/loop55 floppy.img
sudo mount /dev/loop55 /mnt
sudo cp src/kernel /mnt/kernel
sudo umount /dev/loop55
sudo /sbin/losetup -d /dev/loop55
