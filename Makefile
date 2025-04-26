.PHONY: clean compile run runiso setupDir
compile:
	./HelperScripts.sh build
run: compile
	qemu-system-i386 -kernel sysroot/boot/myos.kernel	
iso:
	. ./HelperScripts.sh build
	mkdir -p isodir/boot/grub
	cp sysroot/boot/myos.kernel isodir/boot/myos.kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
runiso: iso
	qemu-system-i386 -cdrom myos.iso
clean:
	./HelperScripts.sh clean
	rm -rf sysroot isodir myos.iso
setupDir:
	./HelperScripts.sh headers
