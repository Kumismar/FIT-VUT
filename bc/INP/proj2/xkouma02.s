; Autor reseni: Ondřej Koumar, xkouma02
.data
login:          .asciiz "xkouma02"  ; sem doplnte vas login
cipher:         .space  17  ; misto pro zapis sifrovaneho loginu

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize "funkce" print_string)
; CODE SEGMENT
.text
                
    ; r11-r21-r6-r15-r0-r4
main:

xor r11, r11, r11

loop:
    lb r21, login(r11)
    ;if r21 < 'a' (its a number) stop
    addi r15, r21, -97 ; subtract 97 from r21
    bgez r15, continue ; if r15 >= 0, continue

    b end ; else end cipher 
 
continue:
    ;if r11 % 2 == 0 process even index else process odd index
    andi r6, r11, 1
    bnez r6, odd

even:
    ;r21 = r21 + key1
    addi r21, r21, 11

    ;if r21 > 'z' r21 = r21 - 'z' + 'a'
    addi r15, r21, -122
    beqz r15, storechar ; if r21 == 'z' storechar()

    bgez r15, continue2 ; if r21 >= 'z' continue2() else storechar()
    b storechar

continue2:
    ; r21 = r21 - 'z' + 'a'
    addi r21, r21, 97
    addi r21, r21, -122
    b storechar

odd:
    ;r21 = r21 - key2
    ;if r21 < 'a' r21 = r21 - 'a' + 'z' + 1
    addi r21, r21, -15
    addi r15, r21, -97
    bgez r15, storechar

    addi r21, r21, 122
    addi r21, r21, 1
    addi r21, r21, -97

storechar:
    sb r21, cipher(r11)
    addi r11, r11, 1
    j loop

end:   
    daddi   r4, r0, cipher   ; vozrovy vypis: adresa login: do r4
    jal     print_string    ; vypis pomoci print_string - viz nize

    syscall 0   ; halt

print_string:   ; adresa retezce se ocekava v r4
    sw      r4, params_sys5(r0)
    daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
    syscall 5   ; systemova procedura - vypis retezce na terminal
    jr      r31 ; return - r31 je urcen na return address

