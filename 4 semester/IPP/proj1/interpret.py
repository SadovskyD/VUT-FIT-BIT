#IPP 2023
#Dmytro Sadovskyi xsadov06
import sys

from interpret_lib.errors import *
from interpret_lib.instruction_list import *
from interpret_lib.XMLparser import *

def start():
    #kontrola poctu argumentu
    if len(sys.argv) > 3 or len(sys.argv) < 2 :
            exit(ExitCode.badParam)
    #vypis help
    if sys.argv[1] == '--help' or sys.argv[1] == '-h':
        print("Program načte XML reprezentaci programu a tento program s využitím vstupu dle parametrů příkazové řádky")
        print("interpretuje a generuje výstup. Vstupní XML je reprezentaci zdrojového kódu IPPcode23")
        print("--source=file vstupní soubor s XML reprezentací zdrojového kódu")
        print("--input=file soubor se vstupy pro samotnou interpretaci zadaného zdrojového kódu")
        exit(0)
    #defoltni nastaveni vstupu
    srcXML ='sys.stdin'
    srcIn = ''
    #kontrola argumentu
    for arg in sys.argv[1:]:
        if arg.startswith('--source='):
            srcXML = arg[9:]

        elif arg.startswith('--input='):
            srcIn = arg[8:]

        else :
            sys.stderr.write('Chybny argument: ' + arg + '\n')
            exit(ExitCode.badParam)
    
    #vytvoreni seznamu pro instrukce
    instructionList = InstructionList()

    #zpracovani XML vstupu
    parser = XMLparser(srcXML)
    parser.parse()

    #Ulozeni instrukci
    parser.importInstructions(instructionList)

    #zkontrolovat řetězce a nahradit znak "\"
    instructionList.checkStrings()

    #Interpretace
    instructionList.execute(srcIn)
    

if __name__ == '__main__':
    start()