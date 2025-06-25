#define VGA_ADDR 0xB8000
#define WHITE_ON_BLACK 0x0F

void kmain() {
    const char *str = "MyOS Kernel Loaded!";
    volatile char *vga = (volatile char*)VGA_ADDR;

    for (int i = 0; str[i]; i++) {
        vga[i*2] = str[i];
        vga[i*2 + 1] = WHITE_ON_BLACK;
    }

    while(1);
}