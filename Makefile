.PHONY: clean compile run runiso setupDir
compile:
	@./HelperScripts.sh build
run: compile
	@qemu-system-i386 -kernel sysroot/boot/NyxOS.kernel	
iso: compile
	@mkdir -p isodir/boot/grub
	@cp sysroot/boot/NyxOS.kernel isodir/boot/NyxOS.kernel
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o NyxOS.iso isodir
runiso: iso
	@qemu-system-i386 -cdrom NyxOS.iso
clean:
	@./HelperScripts.sh clean
	@rm -rf sysroot isodir NyxOS.iso
setupDir:
	@./HelperScripts.sh headers
