    ptxdist clean driver-gamepad
    ptxdist compile driver-gamepad || exit 1
    ptxdist targetinstall driver-gamepad
    ptxdist targetinstall kernel
    ptxdist image root.romfs
    ptxdist test flash-rootfs