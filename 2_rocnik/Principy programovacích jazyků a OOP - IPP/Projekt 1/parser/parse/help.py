import sys
from returnCodes import *

def help_text():
    # Prints help
    print("Usage: parse.py [OPTIONS]")
    print("--help                  Display this help message.")
    print("--source=file           Specify input file with IPPcode24 source code.")
    print("--stat=file             Specify file for statistics output.")
    print("--loc                   Count the number of lines of code (excluding empty lines and comments).")
    print("--comments              Count the number of lines containing comments.")
    print("--labels                Count the number of label definitions.")
    print("--jumps                 Count the total number of jumps (both conditional and unconditional).")
    print("--fwjumps               Count the number of forward jumps.")
    print("--backjumps             Count the number of backward jumps.")
    print("--badjumps              Count the number of jumps to undefined labels.")
    print("--frequent              Output the most frequently used instructions.")
    print("--eol                   Insert an end-of-line character in the statistics output.")
    print("--print=STRING          Output a custom string in the statistics output.")
    return

def help_display():
    # Display help only if --help is one and only argument
    if len(sys.argv) != 2:
        
        sys.exit(HELP_ERROR)
    help_text()
    sys.exit(OK)