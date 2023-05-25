; Vernamova sifra na architekture DLX
; Dmytro Sadovskyi xsadov06

        .data 0x04          ; zacatek data segmentu v pameti
login:  .asciiz "xsadov06"  ; <-- nahradte vasim loginem
cipher: .space 9 ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            ; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          ; adresa zacatku programu v pameti
        .global main        ; 

main: 	addi r20, r20, 0	;cnt cyklusu

loop:	lb r24, login(r20)	;nacist symbl
	
	slti r29, r24, 97	;ak cislo tak konec
	bnez r29, loopend

	andi r29, r20, 1	; zjistime jestli r20 sude nebo liche
	bnez r29, minus

	addi r8, r0, 1
	lb r2, login(r8)
	add r24, r24, r2	;precteme klic
	subi r24, r24, 96
	
	slti r29, r24, 123	;z = 122 dal nejsou pismena
	bnez r29, sfr
	nop
	subi r24, r24, 26
	j sfr

minus:	addi r8, r0, 2
	lb r2, login(r8)
	addi r24, r24, 96	;odecteme klic
	sub r24, r24, r2
	
	slti r29, r24, 97	; a= 97 pred ni nejsou pismena
	beqz r29, sfr
	nop
	addi r24, r24, 26

sfr:	sb cipher(r20), r24	;zapiseme shifrovane pismeno
	
	addi r20, r20, 1	; cnt cyklusu ++

	slti r29, r20, 8
	bnez r20, loop		;konec cyklusu

loopend: add r8, r0, r0	
	sb cipher(r20), r8	;precteme 0 na konec

end:    addi r14, r0, caddr ; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  ; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  ; ukonceni simulace
