class Statistics:
    def __init__(self):
        # Initialize statistics dictionary to count statisctics
        self.statistics = {"loc": 0, "comments": 0, "labels": 0, "jumps": 0, "fwjumps": 0, "backjumps": 0, "badjumps": 0}
        
        # Dictionary to count occurences of each opcode
        self.opcode_counts = {}

    def increment_stat(self, stat):
        # Incrementing count of one statistic
        if stat in self.statistics:
            self.statistics[stat] += 1

    def write_stats(self, stats_config):
        # Writing statisctics to file based on specifications from stats_config
        for file_path, stats in stats_config.items():
            with open(file_path, 'w') as f:
                for stat in stats:
                    # Write the custom string
                    if stat[0] == 'print':
                        f.write(f"{stat[1]}\n")
                    # Write a newline
                    elif stat[0] == 'eol':
                        f.write("\n")
                    # Write opcodes based on number of occurencies in descending order
                    elif stat[0] == 'frequent':
                        f.write(f"{self.frequent_opcodes()}\n")
                    # Write other statictics
                    else:
                        f.write(f"{self.statistics[stat[0]]}\n") 

    def analyze_stats(self, instructions):
        # Storing positions of labes to later determine type of jumps
        label_positions = {}

        # First pass through the instructions to get label positions and opcode counts
        for i, instruction in enumerate(instructions):
            opcode = instruction.opcode.upper()
            # Incrementing opcode count
            if opcode not in self.opcode_counts:
                self.opcode_counts[opcode] = 1
            else:
                self.opcode_counts[opcode] += 1

            # Store definition of label
            if opcode == "LABEL":
                label = instruction.operands[0] 
                label_positions[label] = i

        # Second pass to determine type of jumps
        for i, instruction in enumerate(instructions):
            opcode = instruction.opcode.upper()
            
            # Looking at jump instructions
            if opcode in ["JUMP", "JUMPIFEQ", "JUMPIFNEQ", "CALL"]:
                target_label = instruction.operands[0] 

                # Determining of jump type based on label position (forward or back jump)
                if target_label in label_positions:
                    if label_positions[target_label] > i:
                        self.increment_stat('fwjumps')
                    else:
                        self.increment_stat('backjumps')
                
                # If label definition is not present jump is determined as bad jump
                else:
                    self.increment_stat('badjumps')

    def frequent_opcodes(self):
        # Returns comma-separated string of opcodes sorted by frequency
        sorted_opcodes = sorted(self.opcode_counts.items(), key=lambda x: (-x[1], x[0]))
        return ','.join([opcode for opcode, _ in sorted_opcodes])

        

        

        



