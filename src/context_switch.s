; switch.S
; Assembly function to switch context
; Input: ESP of the previous task, ESP of the next task

[GLOBAL perform_task_switch]
perform_task_switch:
    push ebp
    mov ebp, esp

    ; Save the ESP of the previous task
    mov eax, [ebp+8]
    mov [eax], esp

    ; Load the ESP of the next task
    mov eax, [ebp+12]
    mov esp, [eax]

    pop ebp
    ret
