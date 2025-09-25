import sys
from instructions import Instruction
from returnCodes import *

class SyntaxAnalyzer:
    def  __init__(self, instructions):
        self.instructions = instructions
        self.declared_labels = set()
        self.declared_variables = set()

    def analyze(self):

        for instruction in self.instructions:
            if instruction.opcode.upper() == "LABEL":
                self.label_declaration(instruction)

            elif instruction.opcode.upper() == "DEFVAR":
                self.var_declaration(instruction)
        
        for instruction in self.instructions:
            if instruction.opcode.upper() in ["JUMP", "JUMPIFEQ", "JUMPIFNEQ"]:
                self.label_usage(instruction)
            self.var_usage(instruction)
    
    def label_declaration(self, instruction):
        label = instruction.operands[0]
        if label in self.declared_labels:
            print(f"Error: Duplicate label declaration")
            sys.exit(SYNTAX_ERROR)
        else:
            self.declared_labels.add(label)
    
    def label_usage(self, instruction):
        label = instruction.operands[0]
        if label not in self.declared_labels:
            print(f"Error: Undefined label {label} used")
            sys.exit(SYNTAX_ERROR)
    
    def var_declaration(self, instruction):
        var = instruction.operands[0]
        self.declared_variables.add(var)

    def var_usage(self, instruction):
        for i, operand in enumerate(instruction.operands):
            if instruction.operands_types[i] == 'var' and operand not in self.declared_variables:
                print(f"Error: Undeclared variable {operand} used")
                sys.exit(SYNTAX_ERROR)

    