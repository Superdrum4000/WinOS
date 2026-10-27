#!/bin/bash
# compile (si nécessaire) puis lance WinOS dans QEMU
make && qemu-system-i386 -drive format=raw,file=build/winos.img
