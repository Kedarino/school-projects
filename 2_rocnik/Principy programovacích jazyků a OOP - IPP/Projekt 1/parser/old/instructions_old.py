import re
import sys
from returnCodes import *

OPERAND_PATTERNS = {
    "VARIABLE": re.compile(r'^(GF|LF|TF)@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$'),
    "SYMBOL": re.compile(
        r'(int@-?\d+|'
        r'bool@(true|false)|'
        r'string@.*|'
        r'nil@|'
        r'(GF|LF|TF)@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*)'
    ),
    "LABEL": re.compile(r'((GF|LF|TF)@)?[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*'),
    "TYPE_SPEC": re.compile(r'^(int|string|bool)$'), 
}

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
def operands_types(opcode, operands):
    detected_types = []
    
    for operand in operands:
        print(operand)
        if operand.startswith('int@') :
            detected_types.append((operand, 'int'))

        elif operand.startswith('bool@'):
            detected_types.append((operand, 'bool'))

        elif operand.startswith('string@') or operand == 'string@':
            detected_types.append((operand, 'string'))
     
        elif operand.startswith('nil@'):
            detected_types.append((operand, 'nil'))
        
        elif operand == 'int' or operand == 'string' or operand == 'bool':
            detected_types.append((operand, 'type'))
            
        elif (opcode.upper() == 'LABEL' or opcode.upper() == 'JUMP' or opcode.upper() == 'JUMPIFEQ' or opcode.upper() == 'JUMPIFNEQ') and operand == operands[0]:
            detected_types.append((operand, 'label'))

        elif operand.startswith('LF@') or operand.startswith('TF@') or operand.startswith('GF@'):
            detected_types.append((operand, 'var'))
        
        else:
                print("Error: wrong operand111")
                sys.exit(SYNTAX_ERROR)
        
    return detected_types


def operand_valid(operand, expected_operand):
    pattern = OPERAND_PATTERNS.get(expected_operand)
    
    return bool(pattern.match(operand))

def instruction_valid(opcode, operands):
    expected_operands = INSTRUCTION_SPECS.get(opcode.upper(), [])

    if not expected_operands and not operands:
        
        return True


    if len(operands) != len(expected_operands):
       print("Error: Invalid operand")
       sys.exit(SYNTAX_ERROR) 
    
    types = operands_types(opcode, operands)

    for operands, expected_operands in zip(operands, expected_operands):
 
        if not operand_valid(operands, expected_operands):
            
            print(f"Error: Invalid operand")
            sys.exit(SYNTAX_ERROR)
    

    
    return types







