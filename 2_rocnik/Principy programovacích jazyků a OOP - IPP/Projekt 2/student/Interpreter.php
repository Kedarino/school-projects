<?php

namespace IPP\Student;

use IPP\Core\AbstractInterpreter;
use IPP\Student\Instructions\InstructionCreator;
use IPP\Core\ReturnCode;
use IPP\Student\ProgramState;


/**
 * @class Interpreter
 * @brief Extends AbstractInterpreter to execute IPPcode24 instructions from an XML source.
 * 
 * This class is responsible for managing the execution lifecycle of the program,
 * including initializing program state, parsing XML instructions, and handling execution flow.
 */
class Interpreter extends AbstractInterpreter
{
    private $programState; ///< Holds the state of the program (variables, labels, etc.)
    private $instructions = [];  ///< Array to store executable instructions derived from XML
    private $callStack = [];    ///< Stack to manage call and return from functions

    //Initializes interpreter settings and prepares the initial program state.
    protected function init(): void {
        parent::init();
        $this->programState = new ProgramState($this);
    }

    /**
    * Main execution loop that processes all parsed instructions.
    * @return int Exit code indicating the result of the execution.
    */
    public function execute(): int
    {

        $dom = $this->source->getDOMDocument(); // Load the XML document from the source
        $index = 0; // Instruction index for LABEL tracking

        // First pass: Parse XML and populate instructions array
        foreach ($dom->documentElement->childNodes as $instructionNode) {

            if (!($instructionNode instanceof \DOMElement) || $instructionNode->nodeName !== 'instruction') {
                continue;
            }
            $opcode = $instructionNode->getAttribute('opcode');
            $args = [];
        
            // Extract arguments for each instruction
            foreach ($instructionNode->childNodes as $argNode) {

                if (!($argNode instanceof \DOMElement) || substr($argNode->nodeName, 0, 3) !== 'arg') {
                    continue;
                }
                $argType = $argNode->getAttribute('type');
                $argValue = $argNode->nodeValue;
                $args[] = ['type' => $argType, 'value' => $argValue];
            }

            // Special handling for LABEL instructions
            if ($opcode == 'LABEL') { 
                $this->programState->addLabel($args[0]['value'], $index);
            } else {
                $this->instructions[] = InstructionCreator::createInstruction($this->programState, $opcode, $args);
                $index++; // Only increment for actual executable instructions
            }
            
            
        }
        

        // Second pass: Execute all instructions
        $instructionPointer = 0;
        while ($instructionPointer < count($this->instructions)) {

                $instruction = $this->instructions[$instructionPointer];
                $result = $instruction->execute($this);
                
                if (is_array($result) && isset($result['exit']) && $result['exit']) {
                    $exitCode = $result['code'];
                    if ($exitCode >= 0 && $exitCode <= 9) {
                        
                        return $exitCode;
                    }
                } elseif (is_int($result) && $result >= 100) {
                    // Handle jumps
                    $instructionPointer = $result - 100;
                } else {
                    // Move to the next instruction
                    $instructionPointer++;
                }
        }
        
        
        return ReturnCode::OK;
    }

    /**
    * Retrieves standard output buffer.
    */
    public function getStdOut() {
        return $this->stdout;
    }


    /**
     * Gets the input reader object.
     * @return object Input reader instance.
     */
    public function getInputReader() {
        return $this->input;
    }

    /**
    * Handles the call to a function by storing the current instruction index.
    * @param int $instructionIndex Index of the instruction where the call occurs.
    */
    public function callFunction($instructionIndex) {
        array_push($this->callStack, $instructionIndex);
    }

    /**
    * Manages returning from a function by popping the last instruction index.
    * @return int Returns the last instruction index from the call stack.
    * @throws \Exception If the call stack is empty.
    */
    public function returnFromFunction() {
        if (empty($this->callStack)) {
            throw new \Exception("Call stack is empty, cannot return from function");
        }
        return array_pop($this->callStack);
    }



}
