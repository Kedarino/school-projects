import sys
import argparse
from returnCodes import *
from instructions import Instruction
from help import help_display
from xmlGenerator import *

lang_id = ".IPPcode24"

VALID_OPCODES = {
    "MOVE", "CREATEFRAME", "PUSHFRAME", "POPFRAME", "DEFVAR", 
    "CALL", "RETURN", "PUSHS", "POPS", "ADD", "SUB", "MUL", 
    "IDIV", "LT", "GT", "EQ", "AND", "OR", "NOT", "INT2CHAR", 
    "STRI2INT", "READ", "WRITE", "CONCAT", "STRLEN", "GETCHAR", 
    "SETCHAR", "TYPE", "LABEL", "JUMP", "JUMPIFEQ", "JUMPIFNEQ", 
    "EXIT", "DPRINT", "BREAK"
}

def create_parser():
    parser = argparse.ArgumentParser(description="Parse IPPcode24")
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--source', required=False)
    parser.add_argument('--help', action='store_true')
    return parser


def parse_line(line):
    
    line = line.split('#', 1)[0].strip()

    if not line:
        return None, []
    
    tokens = line.split()
    opcode = tokens[0]
    operands = tokens[1:]

    return opcode, operands

def main():
    parser = create_parser()
    args = parser.parse_args()
    
    

    if args.help:
        help_display()

    input_lines = [line for line in sys.stdin]

   

    for i, line in enumerate(input_lines):
        if line.strip():
            if line.strip() == lang_id:
                input_lines = input_lines[i+1:]
                
                break
        
            else:
                print(f"Error: Wrong or missing language identifier")
                sys.exit(LANG_ID_ERROR)

    for line in input_lines:
        opcode, operands = parse_line(line)
        if opcode:
            instruction = Instruction(opcode, operands)
            instruction.instruction_valid()
            print(f"Opcode: {instruction.opcode}, Operands: {instruction.operands}, Operands_types: {instruction.operands_types}")
    
    
            
    
       


    

if __name__ == "__main__":
    main()
    