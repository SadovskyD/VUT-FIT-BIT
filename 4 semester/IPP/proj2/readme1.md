Implementační dokumentace k 1. úloze do IPP 2022/2023
Jméno a příjmení: Dmytro Sadovskyi
Login: xsadov06
## Úloha
Cílem bylo vytvořit skript, který zkontroluje, zda je daný IPPcode23 kód lexikálně a syntakticky správný a vytiskne jeho XML reprezentaci na standardní výstup.

## Řešení
Řešení se skládá ze 4 souborů:

1. V souboru def.php jsou různé define chybové kódy, tokenové kódy, a pole, které uvádí všechny instrukce. 

2. Ve skriptu parse.php dojde k otevření STDIN a STDOUT, ověření argumentů skriptu, v případě potřeby se zobrazí pomoc. Dále, pokud je vše dobře volá se funkce check_syntax (),
která se nachází v souboru syntax.php.

3. Funkce check_syntax () na základě přijatých tokenů z funkce scan_next () (která bude popsána dále) připravuje XML reprezentaci kódu. Nejprve se zkontroluje správná hlavička,
pak v cyklu dokud nezískáme token konce souboru pomocí dat přenášených v tokenu a DOMdocument vytvoříme XML reprezentaci kódu.

4. V souboru scanner.php je funkce scan_next () která čte ze standardního vstupu kód IPPCode23 a pomocí regulárních výrazů určuje typ tokenu a vrací token jako pole.

Pokud se za běhu skriptu objeví chyba v kódu přijatém na vstup - skript dokončí práci s kódem odpovídající chybě.