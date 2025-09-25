import sys
import argparse
from returnCodes import * # Importing custom return codes for error handling
from instructions import Instruction # Importing Instruction class to handle instructions
from help import help_display # Importing function to display help
from xmlGenerator import * # Importing XML generator logics
from stats import Statistics # Importing the Statistics class

lang_id = ".IPPcode24" # Definition of language identifier

class ProgramParser:

    def __init__(self):
        # Initialize the parser with empty instructions list and a new Statistics object
        self.intructions = []
        self.statistics = Statistics()

        # Setting up argument parser for command-line options
        self.parser = argparse.ArgumentParser(description="Parse IPPcode24", add_help=False)

        # Defining command-line arguments
        self.parser.add_argument('--help', action='store_true')
        self.parser.add_argument('--stat', required=False, action="append", metavar='FILE')
        self.parser.add_argument('--loc', action='store_true')
        self.parser.add_argument('--comments', action='store_true')
        self.parser.add_argument('--labels', action='store_true')
        self.parser.add_argument('--jumps', action='store_true')
        self.parser.add_argument('--eol', action='store_true')
        self.parser.add_argument('--print', required=False, type=str, metavar='STRING')
        self.parser.add_argument('--fwjumps', action="store_true")
        self.parser.add_argument('--backjumps', action="store_true")
        self.parser.add_argument('--badjumps', action="store_true")
        self.parser.add_argument('--frequent', action="store_true")
        self.stats_config = []

    def parse_arrgs(self):
        # Parse command-line arguments
        args = self.parser.parse_args()
        return args




    
    def parse_line(self, line):
        # Parsing single line of code

        # Incrementing the comment statistics count 
        if '#' in line:
            self.statistics.increment_stat('comments')
 
        # Removing the comments from line if there are any
        line = line.split('#', 1)[0].strip()

        # Skipping empty lines 
        if not line:
            return None, []
        
        tokens = line.split()
        opcode = tokens[0]
        operands = tokens[1:]

        return opcode, operands
    
    def process_input(self):
        # Processing input form the stdin
        lang_id_pressent = False

        # Iterating through code lines to determine if language identifier is pressent and valid
        for i, line in enumerate(sys.stdin):

            # Incrementing the comment statistics count 
            if '#' in line:
                self.statistics.increment_stat('comments')

            if line.strip() and not line.strip().startswith('#'):
                if  line.split('#', 1)[0].strip() == lang_id:
                    lang_id_pressent = True
                    break
                else:
                    
                    sys.exit(LANG_ID_ERROR)
        if not lang_id_pressent:
            
            sys.exit(LANG_ID_ERROR)
        
        # Iterating through lines of code after language identifier
        for line in sys.stdin:
            opcode, operands = self.parse_line(line)
            if opcode:
                
                # Incrementing specific opcode statistics
                if opcode.upper() in ["LABEL"]:
                    self.statistics.increment_stat('labels')
                elif opcode.upper() in ["JUMP", "JUMPIFEQ", "JUMPIFNEQ", "CALL"]:  # Add other jump/call opcodes as needed
                    self.statistics.increment_stat('jumps')
                
                # Incrementing number of code lines statistics
                self.statistics.increment_stat('loc')
                
                # Handling instructions
                instruction = Instruction(opcode, operands)
                instruction.instruction_valid()
                self.intructions.append(instruction)
        return

    def xml_generate(self):
        # Generation of XML ouput for parsed instrucitons
        xml_generator = XMLGenerator(self.intructions)
        xml_generator.xml_print()
    
    def write_statistics(self):
        # Writing statistics based on specified arguments
        self.statistics.analyze_stats(self.intructions)
        self.statistics.write_stats(self.stats_config)

    def parse_stat_arg(self):
        # Parsing of statistic-related arguments
        self.stats_config = {}
        current_stats_file = None

        # Seperate parsing logic for statistic-related arguments
        for arg in sys.argv[1:]:
            if arg.startswith('--stat='):
                current_stats_file = arg.split('=')[1]
                if current_stats_file in self.stats_config:
                    
                    sys.exit(STAT_FILE_ERROR)
                self.stats_config[current_stats_file] = []
            elif current_stats_file:
                if arg.startswith('--print='):
                    self.stats_config[current_stats_file].append(('print', arg.split('=')[1]))
                elif arg == '--eol':
                    self.stats_config[current_stats_file].append(('eol', None))
                else:
                    self.stats_config[current_stats_file].append((arg.lstrip('--'), None))
        
            else:
                
                sys.exit(HELP_ERROR)


    def parse(self):
        # Main parsing logic
        args = self.parse_arrgs()
        if args.help:
            help_display()
        self.parse_stat_arg()
        self.process_input()
        self.xml_generate()
        self.write_statistics()


    
if __name__ == "__main__":
    ProgramParser = ProgramParser()
    ProgramParser.parse()
    sys.exit(OK)
