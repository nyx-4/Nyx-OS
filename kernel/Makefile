# To cross-compile NyxOS: make HOST=i386-elf SYSROOT=../sysroot
# By default the build system will assume it's running on NyxOS.

include ../build-aux/common.mk

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)

-include $(ARCHDIR)/arch.mk

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
	$(KERNEL_ARCH_OBJS) \
	kernel/kernel.o \

OBJS=\
	$(ARCHDIR)/crti.o \
	$(ARCHDIR)/crtbegin.o \
	$(KERNEL_OBJS) \
	$(ARCHDIR)/crtend.o \
	$(ARCHDIR)/crtn.o \

LINK_LIST=\
	$(LDFLAGS) \
	$(ARCHDIR)/crti.o \
	$(ARCHDIR)/crtbegin.o \
	$(KERNEL_OBJS) \
	$(LIBS) \
	$(ARCHDIR)/crtend.o \
	$(ARCHDIR)/crtn.o \


.SUFFIXES: .o .c .S .asm

.PHONY: all
all: NyxOS

NyxOS: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot2 NyxOS

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(AS) -felf32 $< -o $@

.asm.o:
	$(AS) -felf32 $< -o $@


.PHONY: install
install: install-headers install-kernel

.PHONY: install-headers
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

.PHONY: install-kernel
install-kernel: NyxOS
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp NyxOS $(DESTDIR)$(BOOTDIR)


.PHONY: clean
clean:
	rm -f NyxOS
	find . -name "*.o" -delete
	find . -name "*.d" -delete

.PHONY: distclean
distclean: clean

-include $(OBJS:.o=.d)
