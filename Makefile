# To cross-compile NyxOS: make HOST=i386-elf
# By default the build system will assume it's running on NyxOS.

include build-aux/common.mk

ifndef SYSROOT
  SYSROOT:=$(shell pwd)/sysroot
endif

QEMU?=qemu-system-$(HOSTARCH)
# QEMU_ENABLE_KVM?=-enable-kvm
QEMU_OPTIONS?=-vga std -m 128M -display gtk,show-menubar=off

MODULES=\
	libc   \
	kernel \

HEADERS_LIBS=\
	libc   \
	kernel \

.PHONY: all
all: sysroot

.PHONY: sysroot
sysroot: sysroot-headers
	export SYSROOT="$(SYSROOT)" && export DESTDIR="$(SYSROOT)" && \
	(for D in $(MODULES); do $(MAKE) -C $$D install || exit $$?; done)

.PHONY: sysroot-headers
sysroot-headers:
	export SYSROOT="$(SYSROOT)" && export DESTDIR="$(SYSROOT)" && \
	(for D in $(HEADERS_LIBS); do $(MAKE) -C $$D install-headers || exit $$?; done)


.PHONY: distclean
distclean: clean
	rm -rf "$(SYSROOT)"
	rm -rf isodir
	rm -f NyxOS.iso

.PHONY: clean
clean:
	export SYSROOT="$(SYSROOT)" && export DESTDIR="$(SYSROOT)" && \
	(for D in $(MODULES); do $(MAKE) -C $$D clean || exit $$?; done)


.PHONY: qemu
qemu: NyxOS.iso
	$(QEMU) $(QEMU_ENABLE_KVM) $(QEMU_OPTIONS) -cdrom NyxOS.iso

.PHONY: iso
iso: NyxOS.iso

NyxOS.iso: isodir/boot/grub/ isodir/boot/NyxOS
	grub-mkrescue -o NyxOS.iso isodir

isodir/boot/grub/:
	mkdir -p isodir/boot/
	cp -r grub/ $@

isodir/boot/NyxOS: sysroot
	mkdir -p isodir/boot
	cp "$(SYSROOT)$(BOOTDIR)/NyxOS" $@


# compile:
# 	./HelperScripts.sh build
# run: iso
# 	qemu-system-i386 -cdrom NyxOS.iso -vga std -m 128M -display gtk,show-menubar=off
# debug: iso
# 	qemu-system-i386 -cdrom NyxOS.iso -vga std -m 128M -display gtk,show-menubar=off -s -S
# iso: compile
# 	@mkdir -p isodir/boot/
# 	cp sysroot/boot/NyxOS.kernel isodir/boot/NyxOS.kernel
# 	rm -rf isoir/boot/grub/
# 	cp -rf grub/ isodir/boot/grub/
# 	grub-mkrescue -o NyxOS.iso isodir
# clean:
# 	./HelperScripts.sh clean
# 	rm -rf sysroot isodir NyxOS.iso
# setupDir:
# 	./HelperScripts.sh headers
