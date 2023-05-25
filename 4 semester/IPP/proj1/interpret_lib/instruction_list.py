#IPP 2023
#Dmytro Sadovskyi xsadov06

import sys

from interpret_lib.errors import *
from interpret_lib.framestack import Framestack

class Instruction():

    def __init__(self, type, arg1=None, arg2=None, arg3=None):
        super().__init__()
        self.type = type
        if arg1 is not None :
            self.arg1 = {'type': arg1.attrib['type'], 'data': (arg1.text if arg1.text is not None else '')}
        if arg2 is not None :
            self.arg2 = {'type': arg2.attrib['type'], 'data': (arg2.text if arg2.text is not None else '')}
        if arg3 is not None :
            self.arg3 = {'type': arg3.attrib['type'], 'data': (arg3.text if arg3.text is not None else '')}

    @staticmethod
    def splitVar(variable) -> (str, str) :
        return variable['data'].split('@', 1)

    def getArgTypeAndData(self, argument: dict, frameClassObj: Framestack) -> (str, str):
        if argument['type'] in ['int', 'bool', 'string', 'type', 'label', 'nil'] :
            return(argument['type'], argument['data'])
        else :
            #
            frame, data = self.splitVar(argument)
            frameObj = frameClassObj.getFrame(frame)
            if frameObj is None :
                exit(ExitCode.runErrMissingFrame)
            if data not in frameObj :
                exit(ExitCode.runErrMissingVar)
            else :
                if (frameObj[data]['type'] is None or frameObj[data]['data'] is None) :
                    exit(ExitCode.runErrMissingValue)
                return(frameObj[data]['type'], frameObj[data]['data'])

    def getType(self, argument: dict, frameObj: Framestack) -> str:
        if argument['type'] in ['int', 'bool', 'string', 'type', 'label', 'nil'] :
            return argument['type']
        else :
            frame, data = self.splitVar(argument)
            frameObj = frameObj.getFrame(frame)
            if frameObj is None :
                exit(ExitCode.runErrMissingFrame)
            if data not in frameObj :
                exit(ExitCode.runErrMissingVar)
            else :
                if frameObj[data]['type'] is None :
                    return None
                return frameObj[data]['type']

class InstructionList() :
    def __init__(self):
        super().__init__()
        self.list = {}
        self.instructionCounter = 0
        self.current = 1
        self.callstack = []
        self.labels = {}
    
    def insertInstruction(self, instruction: Instruction) :
        self.instructionCounter += 1
        self.list[self.instructionCounter] = instruction

        if instruction.type == 'LABEL' :
            name = instruction.arg1['data']
            if name not in self.labels :
                self.labels[name] = self.instructionCounter
            else :
                exit(ExitCode.semanticErr)

    def storePosition(self) :
        self.callstack.append(self.current)

    def restorePosition(self) :
        if len(self.callstack) :
            self.current = self.callstack.pop()
        else :
            exit(ExitCode.runErrMissingValue)

    def jump(self, argument: dict) :
        name = argument['data']
        if name in self.labels :
            self.current = self.labels[name]
        else :
            exit(ExitCode.semanticErr)

    def checkLabel(self, argument: dict) :
        name = argument['data']
        if name not in self.labels :
            exit(ExitCode.semanticErr)

    def fixString(self, string: str) -> str :
        index: int = string.find('\\')
        while(index != -1) :
            string = string.replace(string[index:index+4], chr(int(string[index+1:index+4])))
            index = string.find('\\', index + 1)
        return string
        
    def checkStrings(self) :
        for ins in self.list:
            instruction = self.list[ins]
            if hasattr(instruction, 'arg1') :
                if instruction.arg1['type'] == 'string' :
                    instruction.arg1['data'] = self.fixString(instruction.arg1['data'])
            if hasattr(instruction, 'arg2') :
                if instruction.arg2['type'] == 'string' :
                    instruction.arg2['data'] = self.fixString(instruction.arg2['data'])
            if hasattr(instruction, 'arg3') :
                if instruction.arg3['type'] == 'string' :
                    instruction.arg3['data'] = self.fixString(instruction.arg3['data'])

    def execute(self, srcIn):
        frame = Framestack()
        dataStack = []
        lineCounter = 0
    
        while self.current <= self.instructionCounter :
            self.current += 1
            instruction = self.list[self.current - 1]

            if instruction.type == 'WRITE' or instruction.type == 'DPRINT':
                aType, aData = instruction.getArgTypeAndData(instruction.arg1, frame)
                if aData == None:
                    exit(ExitCode.runErrMissingValue)
                else :
                    if (aType == 'nil' and aData == 'nil') :
                        aData = ''

                    if instruction.type == 'WRITE' :
                        print(aData, end='')
                    else:
                        sys.stderr.write(aData)

            elif instruction.type == 'BREAK' :
                pass

            elif instruction.type == 'CREATEFRAME' :
                frame.createFrame()
            
            elif instruction.type == 'PUSHFRAME' :
                frame.pushFrame()

            elif instruction.type == 'POPFRAME' :
                frame.popFrame()
            
            elif instruction.type == 'DEFVAR' :
                frame.defVar(instruction.arg1)

            elif instruction.type == 'PUSHS' :
                typee, data = instruction.getArgTypeAndData(instruction.arg1, frame)
                dataStack.append((typee, data))
            
            elif instruction.type == 'POPS' :
                try:
                    typee, data = dataStack.pop()
                except IndexError :
                    exit(ExitCode.runErrMissingValue)
                frame.setVar(instruction.arg1, typee, data)

            elif instruction.type == 'MOVE' :
                typee, data = instruction.getArgTypeAndData(instruction.arg2, frame)
                frame.setVar(instruction.arg1, typee, data)

            elif instruction.type == 'CALL' :
                self.storePosition()
                self.jump(instruction.arg1)

            elif instruction.type == 'RETURN' :
                self.restorePosition()

            elif instruction.type in ['ADD', 'SUB', 'MUL', 'IDIV'] :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)
                if type1 == type2 == 'int' :
                    if instruction.type == 'ADD':
                        frame.setVar(instruction.arg1, 'int', str(int(data1)+int(data2)))
                    elif instruction.type == 'SUB':
                        frame.setVar(instruction.arg1, 'int', str(int(data1) - int(data2)))
                    elif instruction.type == 'MUL':
                        frame.setVar(instruction.arg1, 'int', str(int(data1) * int(data2)))
                    else:
                        if int(data2) == 0:
                            exit(ExitCode.runErrZeroDivision)
                        else:
                            frame.setVar(instruction.arg1, 'int', str(int(data1) // int(data2)))
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type in ['LT', 'GT', 'EQ'] :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)

                if type1 == type2 :
                    if instruction.type == 'EQ' :
                        frame.setVar(instruction.arg1, 'bool', 'true' if data1 == data2 else 'false')
                    elif (instruction.type in ['GT', 'LT'] and (type1 == 'nil' and type2 == 'nil')) :
                        exit(ExitCode.runErrBadType)
                    elif instruction.type == 'LT' :
                        if type1 == 'string' :
                            frame.setVar(instruction.arg1, 'bool', 'true' if data1 < data2 else 'false')
                        elif type1 == 'nil' :
                            frame.setVar(instruction.arg1, 'bool', 'false')
                        elif type1 == 'bool':
                            frame.setVar(instruction.arg1, 'bool', 'true' if data1 == 'false' and data2 == 'true' else 'false')
                        else :
                            frame.setVar(instruction.arg1, 'bool', 'true' if int(data1) < int(data2) else 'false')
                    else :
                        if type1 == 'string' :
                            frame.setVar(instruction.arg1, 'bool', 'true' if data1 > data2 else 'false')
                        elif type1 == 'nil' :
                            frame.setVar(instruction.arg1, 'bool', 'false')
                        elif type1 == 'bool':
                            frame.setVar(instruction.arg1, 'bool', 'true' if data1 == 'true' and data2 == 'false' else 'false')
                        else :
                            frame.setVar(instruction.arg1, 'bool', 'true' if int(data1) > int(data2) else 'false')

                elif instruction.type == 'EQ' and (type1 == 'nil' or type2 == 'nil') :
                    frame.setVar(instruction.arg1, 'bool', 'false')
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type in ['AND', 'OR'] :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)

                if type1 == type2 == 'bool' :
                    if instruction.type == 'AND' :
                        frame.setVar(instruction.arg1, 'bool', 'true' if data1 == data2 == 'true' else 'false')
                    else :
                        frame.setVar(instruction.arg1, 'bool', 'true' if data1 == 'true' or data2 == 'true' else 'false')
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type == 'NOT' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                if type1 == 'bool':
                    frame.setVar(instruction.arg1, 'bool', 'true' if data1 == 'false' else 'false')
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type == 'INT2CHAR' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                if type1 == 'int' :
                    try:
                        char = chr(int(data1))
                    except ValueError:
                        exit(ExitCode.runErrBadStringOperation)
                    frame.setVar(instruction.arg1, 'string', char)
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type == 'STRI2INT' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)

                if type1 == 'string' and type2 == 'int' :
                    i = int(data2)
                    if i >= 0 and i <= len(data1) - 1 :
                        ordd = ord(data1[i])
                        frame.setVar(instruction.arg1, 'int', ordd)
                    else :
                        exit(ExitCode.runErrBadStringOperation)
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type == 'READ' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)

                if len(srcIn) :
                    try :
                        with open(srcIn) as file :
                            uis = file.read().splitlines()
                    except FileNotFoundError :
                        exit(ExitCode.outFileErr)
                    
                    try:
                        userInput = uis[lineCounter]
                    except IndexError:
                        sys.stderr.write('Chybejici udaj do promenne' + '\n')
                        frame.setVar(instruction.arg1, 'nil', '')
                        continue
                    finally :
                        lineCounter += 1
                else :
                    try :
                        userInput = input()
                    except Exception :
                        exit(ExitCode.inFileErr)
                
                if data1 == 'int' :
                    try:
                        number = str(int(userInput))
                    except :
                        sys.stderr.write('Byl zadan chybny udaj do promenne typu int' + '\n')
                        frame.setVar(instruction.arg1, 'nil', '')
                    else :
                        frame.setVar(instruction.arg1, 'int', number)
                elif data1 == 'bool' :
                    if userInput.lower() == 'true' :
                        frame.setVar(instruction.arg1, 'bool', 'true')
                    elif userInput.lower() == 'false' :
                        frame.setVar(instruction.arg1, 'bool', 'false')
                    else :
                        sys.stderr.write('Byl zadan chybny udaj do promenne typu bool' + '\n')
                        frame.setVar(instruction.arg1, 'bool', 'false')
                else :
                    frame.setVar(instruction.arg1, 'string', userInput)
            
            elif instruction.type == 'CONCAT' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)

                if type1 == type2 == 'string' :
                    data1 = '' if data1 is None else data1
                    data2 = '' if data2 is None else data2
                    frame.setVar(instruction.arg1, 'string', data1 + data2)
                else :
                    exit(ExitCode.runErrBadType)
                    
            elif instruction.type == 'STRLEN' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)

                if type1 == 'string' :
                    frame.setVar(instruction.arg1, 'int', len(data1))
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type == 'GETCHAR' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)

                if type1 == 'string' and type2 == 'int' :
                    number = int(data2)
                    if number >= 0 and number < len(data1) :
                        frame.setVar(instruction.arg1, 'string', data1[number])
                    else :
                        exit(ExitCode.runErrBadStringOperation)
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type == 'SETCHAR' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)
                dataV: str
                typeV, dataV = instruction.getArgTypeAndData(instruction.arg1, frame)

                if type1 == 'int' and type2 == 'string' and typeV == 'string':
                    number = int(data1)
                    if number < 0 or number >= len(dataV) or dataV == '' :
                        exit(ExitCode.runErrBadStringOperation)
                    if data2 == '' :
                        exit(ExitCode.runErrBadStringOperation)
                    else :
                        data_list = list(dataV)
                        data_list[number] = data2[0]
                        dataV = "".join(data_list)
                        frame.setVar(instruction.arg1, 'string', dataV)
                else :
                    exit(ExitCode.runErrBadType)

            elif instruction.type == 'TYPE' :
                type1 = instruction.getType(instruction.arg2, frame)
                if type1 is None :
                    type1 = ''
                frame.setVar(instruction.arg1, 'string', type1)
            
            elif instruction.type == 'LABEL' :
                continue
            
            elif instruction.type in ['JUMPIFEQ', 'JUMPIFNEQ'] :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg2, frame)
                type2, data2 = instruction.getArgTypeAndData(instruction.arg3, frame)

                self.checkLabel(instruction.arg1)

                if (type1 == type2 or type1 == 'nil' or type2 == 'nil') :
                    if instruction.type == 'JUMPIFEQ' and data1 == data2 :
                        self.jump(instruction.arg1)
                    elif instruction.type == 'JUMPIFNEQ' and data1 != data2 :
                        self.jump(instruction.arg1)
                    else :
                        pass
                else :
                    exit(ExitCode.runErrBadType)
            
            elif instruction.type == 'JUMP' :
                self.jump(instruction.arg1)
            
            elif instruction.type == 'EXIT' :
                type1, data1 = instruction.getArgTypeAndData(instruction.arg1, frame)

                if type1 != 'int' :
                    exit(ExitCode.runErrBadType)
                else :
                    number = int(data1)
                    if number < 0 or number > 49 :
                        exit(ExitCode.runErrZeroDivision)
                    else :
                        exit(number)


