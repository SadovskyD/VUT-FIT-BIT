#IPP 2023
#Dmytro Sadovskyi xsadov06

import xml.etree.ElementTree as XMLTree
import sys

from interpret_lib.errors import ExitCode
from interpret_lib.instruction_list import *

class XMLparser():
    def __init__(self, XMLpath):
        super().__init__()
        self.XMLpath = XMLpath
    
    def parse(self):
        #Nastavení cesty pro vstupy
        try:
            if self.XMLpath == 'sys.stdin' :
                myTree = XMLTree.parse(sys.stdin)
            else :
                myTree = XMLTree.parse(self.XMLpath)
        except FileNotFoundError:
            exit(ExitCode.inFileErr)
        except Exception:
            exit(ExitCode.xmlErr)

        try:
            self.root = myTree.getroot()
        except:
            exit(ExitCode.xmlErr)

        #Kontrola hlavíčky
        if ('language' not in self.root.attrib or self.root.attrib['language'].lower() != 'ippcode23' or (self.root.tag != 'program')):
            exit(ExitCode.xmlErr)
        
        for atribute in self.root.attrib :
            if atribute not in ['language', 'name', 'description'] :
                exit(ExitCode.syntaxErr)

        #Kontrola správnosti instrukcí
        order_tmp = 0
        for instruction in self.root :
            if ((instruction.tag != 'instruction') or ('order' not in instruction.attrib) or ('opcode' not in instruction.attrib)):
                exit(ExitCode.syntaxErr)
            try:
                instr_number = int(instruction.attrib['order'])
            except ValueError:
                exit(ExitCode.syntaxErr)
            if ((instr_number <= order_tmp) or (instr_number <= 0)):
                exit(ExitCode.syntaxErr)
            order_tmp = instr_number
            arg_counter = 0
            for argument in instruction :
                arg_counter += 1
                if (argument.tag != 'arg'+str(arg_counter) or ('type' not in argument.attrib) or (argument.attrib['type'] not in ['string', 'int', 'bool', 'label', 'type', 'nil', 'var'])) :
                    exit(ExitCode.syntaxErr)

    def importInstructions(self, instrList: InstructionList):
        #Ukládáme instrukcí a jejich atributy do InstructionList
        for instruction in self.root:
            if (len(list(instruction)) == 0) and (instruction.attrib['opcode'].upper() in ['CREATEFRAME', 'PUSHFRAME', 'POPFRAME', 'BREAK', 'RETURN']) :
                i = Instruction(instruction.attrib['opcode'].upper()) 
            elif (len(list(instruction)) == 1) and (instruction.attrib['opcode'].upper() in ['DPRINT', 'DEFVAR', 'CALL', 'PUSHS', 'POPS', 'LABEL', 'JUMP', 'WRITE', 'EXIT']) :
                i = Instruction(instruction.attrib['opcode'].upper(), instruction[0]) 
            elif (len(list(instruction)) == 2) and (instruction.attrib['opcode'].upper() in ['MOVE', 'INT2CHAR', 'READ', 'STRLEN', 'TYPE', 'NOT']) :
                i = Instruction(instruction.attrib['opcode'].upper(), instruction[0], instruction[1]) 
            elif (len(list(instruction)) == 3) and (instruction.attrib['opcode'].upper() in ['ADD', 'SUB', 'MUL', 'IDIV', 'LT', 'GT', 'EQ', 'AND', 'OR', 'JUMPIFEQ', 'JUMPIFNEQ', 'STRI2INT', 'CONCAT', 'GETCHAR', 'SETCHAR']) :
                i = Instruction(instruction.attrib['opcode'].upper(), instruction[0], instruction[1], instruction[2]) 
            else :
                exit(ExitCode.syntaxErr)
            instrList.insertInstruction(i)