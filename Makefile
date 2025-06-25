CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

KERNEL = kernel.bin
ISO = myos.iso

SRCDIR = src
BUILDDIR = build

OBJS = $(BUILDDIR)/boot.o \
       $(BUILDDIR)/kernel.o \
       $(BUILDDIR)/gdt.o \
       $(BUILDDIR)/gdt_asm.o \
       $(BUILDDIR)/idt.o \
       $(BUILDDIR)/idt_asm.o \
       $(BUILDDIR)/isr.o \
       $(BUILDDIR)/screen.o \
       $(BUILDDIR)/ports.o \
       $(BUILDDIR)/pic.o \
       $(BUILDDIR)/irq.o \
       $(BUILDDIR)/irq_asm.o \
       $(BUILDDIR)/timer.o

all: $(ISO)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Явное правило для boot.s
$(BUILDDIR)/boot.o: $(SRCDIR)/boot.s | $(BUILDDIR)
	$(AS) $< -o $@

# Правило для .c файлов
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для .S файлов (ассемблер с препроцессором)
$(BUILDDIR)/%.o: $(SRCDIR)/%.S | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для .s файлов (чистый ассемблер)
$(BUILDDIR)/%.o: $(SRCDIR)/%.s | $(BUILDDIR)
	$(AS) $< -o $@

$(BUILDDIR)/$(KERNEL): $(OBJS)
	$(LD) -T scripts/linker.ld -o $@ $^ -nostdlib -lgcc \
		-z max-page-size=0x1000 -no-pie

$(BUILDDIR)/idt.o: src/idt.c src/idt.h src/irq.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ISO): $(BUILDDIR)/$(KERNEL)
	@mkdir -p isodir/boot/grub
	cp $(BUILDDIR)/$(KERNEL) isodir/boot/
	cp grub/grub.cfg isodir/boot/grub/
	xorriso -as mkisofs -R -b boot/grub/i386-pc/eltorito.img -no-emul-boot \
		-boot-load-size 4 -boot-info-table -input-charset utf8 \
		-o $(ISO) isodir

run: $(BUILDDIR)/$(KERNEL)
	qemu-system-i386 -kernel $(BUILDDIR)/$(KERNEL)

run-iso: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILDDIR) isodir $(ISO)

.PHONY: all clean run run-iso