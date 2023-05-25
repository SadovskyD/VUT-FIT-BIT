# Projekt 1

Server komunikující prostřednictvím protokolu HTTP, který poskytuje různé informace o systému. Server naslouchá na zadaném portu a podle url vrací požadované informace. 

## Spuštění

```
./hinfosvc [port]

```
port - UDP port, na kterém bude server naslouchat.


## Typy dotazů

Server zpracovává následující tři typy požadavků odeslaných na server pomocí příkazu GET:

1. Získání doménového jména

Vrací síťové jméno počítače včetně domény, například:

GET http://servername:12345/hostname

merlin.fit.vutbr.cz

2. Získání informací o CPU 

Vrací informaci o procesoru, například:

GET http://servername:12345/cpu-name

3. Aktuální zátěž 

Vrací aktuální informace o zátěži. Tento vyžaduje výpočet z hodnot uvedených v souboru /proc/stat (viz odkaz níže). Výsledek je například:

GET http://servername:12345/load

65%

#### Autor
- Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>