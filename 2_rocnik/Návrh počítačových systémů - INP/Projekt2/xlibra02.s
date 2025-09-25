; Autor reseni: Radek Libra xlibra02
; Pocet cyklu k serazeni puvodniho retezce: 4512
; Pocet cyklu razeni sestupne serazeneho retezce: 4570
; Pocet cyklu razeni vzestupne serazeneho retezce: 4400
; Pocet cyklu razeni retezce s vasim loginem: 944
; Implementovany radici algoritmus: Bubble sort
; ------------------------------------------------

; DATA SEGMENT
                .data

login:          .asciiz "xlibra02"            ; SEM DOPLNTE VLASTNI LOGIN
                                                ; A POUZE S TIMTO ODEVZDEJTE

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize - "funkce" print_string)


; CODE SEGMENT
.text

main:
    daddi r4, r0, login          ; Načte adresu řetězce do r4
    daddiu r5, r0, 0             ; Inicializuje délku řetězce na 0 

lengthCalculation:
    daddu r6, r4, r5             ; Vypočítá adresu aktuálního znaku
    lb r7, 0(r6)                 ; Načte byte na aktuální pozici
    beqz r7, prepareSorting      ; Pokud je byte nula, začne řadit
    daddiu r5, r5, 1             ; Inkrementuje délku
    j lengthCalculation

prepareSorting:
    daddiu r8, r5, -1            ; Nastaví r8 = délka - 1 
    daddiu r9, r0, 0             ; Inicializuje vnější počítadlo cyklu

sortingLoop:
    daddiu r11, r0, 0            ; Inicializuje vnitřní počítadlo
    daddi r17, r0, 0             ; Inicializuje příznak pro výměnu

compareAndSwap:
    sltu r10, r11, r8            ; Porovná vnitřní počítadlo cyklu s r8 
    beqz r10, updateOuterCycle   ; Pokud je počítadlo  >= r8, aktualizuje vnější cyklus
    daddu r12, r4, r11
    lb r13, 0(r12)               ; načte aktuální prvek
    daddiu r14, r11, 1
    daddu r15, r4, r14
    lb r16, 0(r15)               ; Načte další prvek
    sltu r10, r13, r16           ; Porovná prvky
    bnez r10, skipSwap           ; POkud je aktuální < další neprovádí výměnu
    sb r13, 0(r15)               ; Provede výměnu prvků
    sb r16, 0(r12)
    daddi r17, r0, 1             ; Nastaví příznak pro výměnu na 1 

skipSwap:
    daddiu r11, r11, 1           ; Inkrementuje vnitřní počítadlo cykllu
    j compareAndSwap

updateOuterCycle:
    beqz r17, endSort            ; Pokud nedošlo k žádným výměnám, poe je seřazeno 
    daddiu r9, r9, 1             ; Inkrementuje vnější počítadlo
    daddiu r8, r8, -1            ; Sníží počet porovnání 
    j sortingLoop

endSort:
    jal print_string             ; Volá funkci print_string pro výpis
    syscall 0                    ; Ukončí program

print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
