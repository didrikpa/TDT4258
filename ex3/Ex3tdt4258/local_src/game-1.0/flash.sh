ptxdist clean game
ptxdist compile game || exit 1
ptxdist targetinstall game
ptxdist image root.romfs
ptxdist test flash-rootfs