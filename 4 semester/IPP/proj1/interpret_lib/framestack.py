#IPP 2023
#Dmytro Sadovskyi xsadov06

from interpret_lib.errors import *
import interpret_lib.instruction_list as i

class Framestack :
    def __init__(self):
        super().__init__()
        #polozka 0 uchovava GF, polozka 1 TF
        self.frameStack = [{},{}]
        self.tmpFrameDefined = False

    def getFrame(self, type: str) -> dict:
        if type == 'GF':
            return self.frameStack[0]
        elif type == 'LF':
            return (self.frameStack[-1] if len(self.frameStack) > 2 else None)
        elif type == 'TF':
            return (self.frameStack[1] if self.tmpFrameDefined else None)
        else:
            return None

    def createFrame(self) :
        self.frameStack[1] = {}
        self.tmpFrameDefined = True

    def pushFrame(self) :
        if self.tmpFrameDefined :
            self.frameStack.append(self.frameStack[1])
            self.tmpFrameDefined = False
        else :
            exit(ExitCode.runErrMissingFrame)

    def popFrame(self) :
        if len(self.frameStack) :
            self.frameStack[1] = self.frameStack.pop()
            self.tmpFrameDefined = True
        else :
            exit(ExitCode.runErrMissingFrame)

    def setVar(self, argument, typee, data) :
        frame, name = i.Instruction.splitVar(argument)
        frameObj = self.getFrame(frame)
        if frameObj is None :
            exit(ExitCode.runErrMissingFrame)
        if name not in frameObj :
            exit(ExitCode.runErrMissingVar)
        frameObj[name]['type'] = typee
        frameObj[name]['data'] = data

    def defVar(self, argument) :
        frame, name = i.Instruction.splitVar(argument)
        frameObj = self.getFrame(frame)
        if frameObj is None :
            exit(ExitCode.runErrMissingFrame)
        else :
            if name in frameObj :
                exit(ExitCode.semanticErr)
            else :
                frameObj[name] = {'data': None, 'type': None}



