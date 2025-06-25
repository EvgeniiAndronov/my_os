.global irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
.global irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

# Обработчики IRQ
irq0:
    cli
    push $0       # Код ошибки
    push $0       # Номер прерывания (IRQ0)
    jmp irq_common

irq1:
    cli
    push $0
    push $1       # Номер прерывания (IRQ1)
    jmp irq_common

irq2:
    cli
    push $0       # Код ошибки
    push $2       # Номер прерывания
    jmp irq_common

irq3:
    cli
    push $0
    push $3
    jmp irq_common

irq4:
    cli
    push $0       # Код ошибки
    push $4       # Номер прерывания
    jmp irq_common

irq5:
    cli
    push $0
    push $5
    jmp irq_common

irq6:
    cli
    push $0       # Код ошибки
    push $6       # Номер прерывания
    jmp irq_common

irq7:
    cli
    push $0
    push $7
    jmp irq_common

irq8:
    cli
    push $0       # Код ошибки
    push $8       # Номер прерывания
    jmp irq_common

irq9:
    cli
    push $0
    push $9
    jmp irq_common

irq10:
    cli
    push $0       # Код ошибки
    push $10       # Номер прерывания
    jmp irq_common

irq11:
    cli
    push $0
    push $11
    jmp irq_common

irq12:
    cli
    push $0       # Код ошибки
    push $12       # Номер прерывания
    jmp irq_common

irq13:
    cli
    push $0
    push $13
    jmp irq_common

irq14:
    cli
    push $0       # Код ошибки
    push $14       # Номер прерывания
    jmp irq_common

irq15:
    cli
    push $0
    push $15
    jmp irq_common

irq_common:
    # Сохраняем регистры процессора
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

    # Передаем указатель на структуру regs в C-обработчик
    mov %esp, %eax
    push %eax
    call irq_handler
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