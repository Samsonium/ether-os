#!/bin/sh -e

# Install packages for toolchain
apk --update add build-base
apk add gmp-dev mpfr-dev mpc1-dev texinfo
apk add make cmake
apk add grub-bios xorriso
apk add gdb nasm git

# Install QEMU
apk --update add qemu-system-x86_64 --repository https://dl-cdn.alpinelinux.org/alpine/v3.20/main/
rm -rf /var/cache/apk/*

# Variables
target='x86_64-elf'
binutils='binutils-2.40'
gcc='gcc-14.1.0'

# Build binutils
cd /opt
wget https://ftp.gnu.org/gnu/binutils/${binutils}.tar.gz
tar xf ${binutils}.tar.gz
mkdir binutils-build && cd binutils-build
../${binutils}/configure \
    --target=${target} \
    --disable-nls \
    --disable-werror \
    --with-sysroot
make -j 6
make install

# Build gcc
cd /opt
wget https://ftp.gnu.org/gnu/gcc/${gcc}/${gcc}.tar.gz
tar xf ${gcc}.tar.gz
mkdir gcc-build && cd gcc-build
../${gcc}/configure \
    --target=${target} \
    --disable-nls \
    --enable-languages=c,c++ \
    --without-headers
make -j 6 all-gcc all-target-libgcc
make install-gcc install-target-libgcc

##
cd /
rm -rf /opt
