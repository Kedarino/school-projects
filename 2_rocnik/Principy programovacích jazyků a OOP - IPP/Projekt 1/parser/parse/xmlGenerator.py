import xml.etree.ElementTree as ET
import sys
from xml.dom import minidom

class XMLGenerator:
    def __init__(self, instructions):
        # Constructor for XML generator class that takes instances of INstruction classes as input
        self.instructions = instructions

        
    def xml_generate(self):
        # Generate XML representation of provided isntructions
        # Root element generation
        program = ET.Element("program", language="IPPcode24")

        # Iterating through instructions adding them as subelements
        for i, instruction in enumerate(self.instructions, start = 1):

            # Each instruction element has attributes for order and opcode
            instruction_elem = ET.SubElement(program, "instruction", order=str(i), opcode=instruction.opcode.upper())

            # Iterating through operands of instructions and adding them as subelements of isntructions
            for j, operand in enumerate(instruction.operands, start=1):
                operand_type = instruction.operands_types[j-1]
                operand_elem = ET.SubElement(instruction_elem, f"arg{j}", type=operand_type)
                
                # Spliting the operand to get value after '@' only if operand isnt variable othervise using the operand as is
                if operand_type != "var":
                    operand_value = operand.split('@', 1)[1] if '@' in operand else operand
                else:
                    operand_value = operand
                
                # Setting the text of operand elements
                operand_elem.text = operand_value
        # Convert the program element tree to a string with pretty formatting (indentation) using minidom
        xml_str = minidom.parseString(ET.tostring(program, 'utf-8')).toprettyxml(indent="   ", encoding="UTF-8")
        return xml_str.decode('utf-8') if isinstance(xml_str, bytes) else xml_str

    def xml_print(self):
        # Generation and print of XML output
        xml_tree = self.xml_generate()
        print(xml_tree)