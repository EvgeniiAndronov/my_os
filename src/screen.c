#include "screen.h"
#include "ports.h"

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

// Текущая позиция курсора
static int cursor_x = 0;
static int cursor_y = 0;

// Обновление аппаратного курсора
static void move_cursor() {
    unsigned short pos = cursor_y * MAX_COLS + cursor_x;

    // VGA контроллер: регистр 14 (high byte), 15 (low byte)
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// Очистка экрана
void clear_screen() {
    volatile char *video_memory = (volatile char*)VIDEO_ADDRESS;

    for (int i = 0; i < MAX_ROWS * MAX_COLS * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = WHITE_ON_BLACK;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// Инициализация экрана
void init_screen() {
    clear_screen();
}

// Вывод символа на экран
static void kputchar(char c) {
    volatile char *video_memory = (volatile char*)VIDEO_ADDRESS;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        int offset = (cursor_y * MAX_COLS + cursor_x) * 2;
        video_memory[offset] = c;
        video_memory[offset+1] = WHITE_ON_BLACK;
        cursor_x++;
    }

    // Перенос строки, если вышли за границу
    if (cursor_x >= MAX_COLS) {
        cursor_x = 0;
        cursor_y++;
    }

    // Прокрутка экрана, если достигли конца
    if (cursor_y >= MAX_ROWS) {
        // Сдвигаем все строки вверх на одну
        for (int row = 1; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLS; col++) {
                int src_offset = (row * MAX_COLS + col) * 2;
                int dst_offset = ((row-1) * MAX_COLS + col) * 2;
                video_memory[dst_offset] = video_memory[src_offset];
                video_memory[dst_offset+1] = video_memory[src_offset+1];
            }
        }

        // Очищаем последнюю строку
        for (int col = 0; col < MAX_COLS; col++) {
            int offset = ((MAX_ROWS-1) * MAX_COLS + col) * 2;
            video_memory[offset] = ' ';
            video_memory[offset+1] = WHITE_ON_BLACK;
        }

        cursor_y = MAX_ROWS - 1;
    }

    move_cursor();
}

// Вывод строки
void kprint(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        kputchar(str[i]);
    }
}

void int_to_str(uint32_t num, char *str) {
    char *ptr = str;
    int len = 0;
    uint32_t tmp = num;

    // Определяем длину числа
    do {
        len++;
        tmp /= 10;
    } while (tmp);

    ptr = str + len;
    *ptr = '\0';

    do {
        *--ptr = '0' + (num % 10);
        num /= 10;
    } while (num);
}

void print_hex(uint32_t n) {
    char hex_chars[] = "0123456789ABCDEF";
    char hex[9];
    hex[8] = '\0';

    for (int i = 7; i >= 0; i--) {
        hex[i] = hex_chars[n & 0xF];
        n >>= 4;
    }

    kprint("0x");
    kprint(hex);
}