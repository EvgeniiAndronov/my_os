.global idt_load
.extern idtp

idt_load:
    lidt (idtp)
    ret

# Макрос для прерываний без кода ошибки
.macro ISR_NOERRCODE num
    .global isr\num
    isr\num:
        cli
        pushl $0                     # Fake error code
        pushl $\num                  # Interrupt number
        jmp isr_common
.endm

# Макрос для прерываний с кодом ошибки
.macro ISR_ERRCODE num
    .global isr\num
    isr\num:
        cli
        pushl $\num                  # Interrupt number
        jmp isr_common
.endm

# Создаем обработчики
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_ERRCODE   30
ISR_NOERRCODE 31

# Общий обработчик
.extern isr_handler

isr_common:
    # Сохраняем регистры
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    # Загружаем сегмент данных ядра
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Передаем указатель на структуру с регистрами
    push %esp
    call isr_handler
    add $4, %esp

    # Восстанавливаем регистры
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    # Очищаем номер прерывания и код ошибки
    add $8, %esp

    # Возврат из прерывания
    iret