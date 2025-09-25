<?php

namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Core\AbstractInterpreter;
use IPP\Student\Exceptions\MySemanticError;
use IPP\Student\ProgramState;


/**
 * Class responsible for creating instruction objects based on opcode and arguments.
 */
class InstructionCreator {
    /**
     * Factory method to create instruction objects based on the provided opcode and arguments.
     * Throws exceptions if the arguments do not meet the expected requirements for each instruction type.
     *
     * @param ProgramState $programState The current state of the program which is passed to each instruction.
     * @param string $opcode The opcode that determines the type of instruction to create.
     * @param $args The arguments required for the instruction.
     * @return Instruction The created instruction object.
     * @throws MySemanticError If the number of arguments is incorrect or the opcode is unknown.
     */
    public static function createInstruction(ProgramState $programState, string $opcode, $args): Instruction {
        switch ($opcode) {
            case 'ADD':
                
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new ADDInstruction($programState, $args[0], $args[1], $args[2]);

            case 'SUB':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new SUBInstruction($programState, $args[0], $args[1], $args[2]);

            case 'MUL':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new MULInstruction($programState, $args[0], $args[1], $args[2]);

            case 'IDIV':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new IDIVInstruction($programState, $args[0], $args[1], $args[2]);

            case 'MOVE':
                
                if (count($args) != 2) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new MOVEInstruction($programState, $args[0], $args[1]);

            case 'DEFVAR':
                
                if (count($args) != 1) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new DEFVARInstruction($programState, $args[0]);
                
            case 'WRITE':
                
                if (count($args) != 1) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new WRITEInstruction($programState, $args[0]);

            case 'JUMP':
                if (count($args) != 1) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new JUMPInstruction($programState, $args[0]);

            case 'JUMPIFEQ':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new JUMPIFEQInstruction($programState, $args[0], $args[1], $args[2]);

            case 'JUMPIFNEQ':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new JUMPIFNEQInstruction($programState, $args[0], $args[1], $args[2]);

            case 'LT':
            case 'GT':
            case 'EQ':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new LogicalInstructions($programState, $opcode, $args[0], $args[1], $args[2]);

            case 'AND':
            case 'OR':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new LogicalInstructions($programState, $opcode, $args[0], $args[1], $args[2]);

            case 'NOT':
                if (count($args) != 2) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new LogicalInstructions($programState, 'NOT', $args[0], $args[1]);    

            case 'INT2CHAR':
                if (count($args) != 2) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new TypeConversionInstructions($programState, 'INT2CHAR', $args[0], $args[1]);

            case 'STRI2INT':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new TypeConversionInstructions($programState, 'STRI2INT', $args[0], $args[1], $args[2]);

            case 'GETCHAR':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new GETCHARInstruction($programState, $args[0], $args[1], $args[2]);

            case 'SETCHAR':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new SETCHARInstruction($programState, $args[0], $args[1], $args[2]);

            case 'CONCAT':
                if (count($args) != 3) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new CONCATInstruction($programState, $args[0], $args[1], $args[2]);

            case 'STRLEN':
                if (count($args) != 2) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new STRLENInstruction($programState, $args[0], $args[1]);

            case 'CREATEFRAME':
                if (count($args) != 0) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new CREATEFRAMEInstruction($programState);
            case 'PUSHFRAME':
                if (count($args) != 0) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new PUSHFRAMEInstruction($programState);
                
            case 'POPFRAME':
                if (count($args) != 0) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new POPFRAMEInstruction($programState);

            case 'CALL':
                if (count($args) != 1) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new CALLInstruction($programState, $args[0]);

            case 'RETURN':
                if (count($args) != 0) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new RETURNInstruction($programState);

            case 'EXIT':
                if (count($args) != 1) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new EXITInstruction($programState, $args[0]);

            case 'TYPE':
                if (count($args) != 2) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new TYPEInstruction($programState, $args[0], $args[1]);

            case 'READ':
                if (count($args) != 2) {
                    throw new MySemanticError("$opcode: Wrong number of arguments");
                }
                return new READInstruction($programState, $args[0], $args[1]['value']); 


            default:
                throw new MySemanticError("Unknown instruction: $opcode");

            
        }

    }
}



