import sys
from returnCodes import *

def help_text():
    # Prints help
    print("Usage: parse.py [OPTIONS]")
    print("--help                  Display this help message.")
    print("--source=file           Specify input file with IPPcode24 source code.")
    print("Test test picus")
    return

def help_display():
    # Display help only if --help is one and only argument
    if len(sys.argv) != 2:
        print(f"Error: --help cannot be conbined with other parameters")
        sys.exit(HELP_ERROR)
    help_text()
    sys.exit(OK)