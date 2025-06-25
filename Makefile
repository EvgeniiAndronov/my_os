CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

KERNEL = kernel.bin
ISO = myos.iso

SRCDIR = src
BUILDDIR = build

OBJS = $(BUILDDIR)/boot.o $(BUILDDIR)/kernel.o

all: $(BUILDDIR)/$(KERNEL)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.s | $(BUILDDIR)
	$(AS) $< -o $@

$(BUILDDIR)/$(KERNEL): $(OBJS)
	$(LD) -T scripts/linker.ld -o $@ $^ -nostdlib -lgcc \
		-z max-page-size=0x1000 -no-pie

$(ISO): $(BUILDDIR)/$(KERNEL)
	@mkdir -p isodir/boot/grub
	cp $(BUILDDIR)/$(KERNEL) isodir/boot/
	cp grub/grub.cfg isodir/boot/grub/
	mkisofs -R -b boot/grub/grub.img -no-emul-boot \
		-boot-load-size 4 -boot-info-table -input-charset utf8 \
		-o $(ISO) isodir

run: $(BUILDDIR)/$(KERNEL)
	qemu-system-i386 -kernel $(BUILDDIR)/$(KERNEL)

run-iso: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILDDIR) isodir $(ISO)

.PHONY: all clean run run-iso