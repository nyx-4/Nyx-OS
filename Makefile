.PHONY: clean compile run runiso setupDir
compile:
	./HelperScripts.sh build
run: iso
	qemu-system-i386 -cdrom NyxOS.iso -vga std -m 128M -display gtk,show-menubar=off
debug: iso
	qemu-system-i386 -cdrom NyxOS.iso -vga std -m 128M -display gtk,show-menubar=off -s -S
iso: compile
	@mkdir -p isodir/boot/
	cp sysroot/boot/NyxOS.kernel isodir/boot/NyxOS.kernel
	rm -rf isoir/boot/grub/
	cp -rf grub/ isodir/boot/grub/
	grub-mkrescue -o NyxOS.iso isodir
clean:
	./HelperScripts.sh clean
	rm -rf sysroot isodir NyxOS.iso
setupDir:
	./HelperScripts.sh headers
