import re 
import sys
from returnCodes import *

class Instruction:

    # Regex paterns for different types of operands
    OPERAND_PATTERNS = {
    "VARIABLE": re.compile(r'^(GF|LF|TF)@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$'),
    "SYMBOL": re.compile(
        
        r'(int@-?\d+|'
        r'bool@(true|false)|'
        r'string@.*$|'
        r'nil@nil$|'
        r'(GF|LF|TF)@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*)'
    ),
    #"LABEL": re.compile(r'((GF|LF|TF)@)?[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*'),
    "LABEL": re.compile(r'(?<!GF@)(?<!LF@)(?<!TF@)[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*'),
    "TYPE_SPEC": re.compile(r'^(int|string|bool)$'), 
    }

    # Regex patern for valid opcodes
    OPCODE_PATTERN = re.compile(r'^[a-zA-Z0-9]+$')

    # Pattern for every instruction
    INSTRUCTION_SPECS = {
        "MOVE": ["VARIABLE", "SYMBOL"],
        "CREATEFRAME": [],
        "PUSHFRAME": [],
        "POPFRAME": [],
        "DEFVAR": ["VARIABLE"],
        "CALL": ["LABEL"],
        "RETURN": [],
        "PUSHS": ["SYMBOL"],
        "POPS": ["VARIABLE"],
        "ADD": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "SUB": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "MUL": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "IDIV": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "LT": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "GT": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "EQ": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "AND": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "OR": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "NOT": ["VARIABLE", "SYMBOL"],
        "INT2CHAR": ["VARIABLE", "SYMBOL"],
        "STRI2INT": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "READ": ["VARIABLE", "TYPE_SPEC"],
        "WRITE": ["SYMBOL"],
        "CONCAT": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "STRLEN": ["VARIABLE", "SYMBOL"],
        "GETCHAR": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "SETCHAR": ["VARIABLE", "SYMBOL", "SYMBOL"],
        "TYPE": ["VARIABLE", "SYMBOL"],
        "LABEL": ["LABEL"],
        "JUMP": ["LABEL"],
        "JUMPIFEQ": ["LABEL", "SYMBOL", "SYMBOL"],
        "JUMPIFNEQ": ["LABEL", "SYMBOL", "SYMBOL"],
        "EXIT": ["SYMBOL"],
        "DPRINT": ["SYMBOL"],
        "BREAK": [],    
    }

    def __init__(self, opcode, operands):
        # Initialize an Instruction instance with opcode and operands
        self.opcode = opcode
        self.operands = operands
        self.operands_types = [] # Storing types of operands

        # Validating opcode upon inicialization
        if not self.opcode_valid():
            print("Error: Invalid opcode")
            sys.exit(OPCODE_ERROR)

    def operands_valid(self):
        # Validation of operands against their expcted patterns based on isntruction opcode
        expected_operands = Instruction.INSTRUCTION_SPECS.get(self.opcode.upper(), [])

        # Handling instruction with no operands
        if not expected_operands and not self.operands:
            return True

        # If number of operands match the expectation
        if len(self.operands) != len(expected_operands):
            print("Error: Invalid operands")
            sys.exit(SYNTAX_ERROR)

        # Validate each operand against ist expected pattern
        for operand, expected_operand in zip(self.operands, expected_operands):
            pattern = Instruction.OPERAND_PATTERNS.get(expected_operand)
            if not bool(pattern.match(operand)):
                print("Error: Invalid operand")
                sys.exit(SYNTAX_ERROR)

        return True
    
    def string_valid(self, string):
        # Validate string operands
        i = 0
        while i < len(string):
            # Validation of escape sequences
            if string[i] == '\\':
                if i + 4 > len(string):
                    print(f"Error: wrong string {string}")
                    sys.exit(SYNTAX_ERROR) 
                if not string[i+1:i+4].isdigit():
                    print(f"Error: wrong string {string}")
                    sys.exit(SYNTAX_ERROR)
                i += 4
            else:
                i += 1
        return

    def int_valid(self, int):
        # Vvalidation of integer operands, that include octal numbers
        if int.startswith(('0o', '0O')):
            octal_part = int[2:]
            if any(char not in '01234567' for char in octal_part):
                print("Error: Invalid operand")
                sys.exit(SYNTAX_ERROR)

        # Validation of integer operands, that innclude octal numbers
        elif int.startswith(('0x', '0X')):
            hex_part = int[2:]
            if any(char not in '0123456789abcdefABCDEF' for char in hex_part):
                print("Error: Invalid operand")
                sys.exit(SYNTAX_ERROR)
        
        return

    def operands_type(self):
        # Validation of each operand type
        for operand in self.operands:
        
            if operand.startswith('int@') :
                int = operand.split('@')[1]
                self.int_valid(int)

                self.operands_types.append('int')

            elif operand.startswith('bool@'):
                self.operands_types.append('bool')

            elif operand.startswith('string@') or operand == 'string@':
                string = operand[7:]
                self.string_valid(string)
                self.operands_types.append('string')
        
            elif operand.startswith('nil@'):
                self.operands_types.append('nil')
            
            elif (self.opcode.upper() == 'LABEL' or self.opcode.upper() == 'JUMP' or self.opcode.upper() == 'JUMPIFEQ' or self.opcode == 'JUMPIFNEQ' or self.opcode == 'CALL') and operand == self.operands[0]:
                self.operands_types.append('label')
            
            elif operand == 'int' or operand == 'string' or operand == 'bool':
                self.operands_types.append('type')

            elif operand.startswith('LF@') or operand.startswith('TF@') or operand.startswith('GF@'):
                self.operands_types.append('var')
            
            else:
                    print("Error: wrong operand")
                    sys.exit(SYNTAX_ERROR)
        
        return

    def opcode_valid(self):
        # Validation of opcode against expected pattern
        if not Instruction.OPCODE_PATTERN.match(self.opcode):
            sys.exit(SYNTAX_ERROR)
        return self.opcode.upper() in Instruction.INSTRUCTION_SPECS
    
    def instruction_valid(self):
        # Validation of entire instruciton
        self.operands_valid()
        self.operands_type()    

            
        
