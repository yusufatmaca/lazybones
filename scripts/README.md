# Disk Image & Floppy Operations
This directory contains scripts for preparing and updating the bootable floppy image used by the OS.

= = =

## Using Disk Image
```bash
./make_floppy.sh
```

### What it does:
  - Attaches `floppy.img` to a loop device
  - Mounts it
  - Copies kernel into image
  - Unmounts and detaches

### Flow
```sh
floppy.img → loop device → mount → copy → unmount
```

= = =

### Debug Tips

Check mounts:
```sh
mount | grep loop
```

Check loop devices:
```sh
losetup -a
```

= = =

### Improvements (TODO)
  - [ ] Add argument support (--image, --device)
  - [ ] Auto-detect free loop device
  - [ ] Safer mount directory handling
