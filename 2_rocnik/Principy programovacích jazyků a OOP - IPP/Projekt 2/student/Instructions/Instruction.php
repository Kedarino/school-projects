<?php

namespace IPP\Student\Instructions;
use IPP\Student\ProgramState;
use IPP\Core\Interface\OutputWriter;
use IPP\Student\Exceptions\MyFrameAccessError;
use IPP\Student\Interpreter;

/**
 * Abstract class representing a general instruction.
 * This class provides the foundational methods needed for handling instructions within the interpreter.
 */
abstract class Instruction {
    protected $programState;
    

    public function __construct(ProgramState $programState) {
        $this->programState = $programState;
    }

    /**
     * Resolves the type of an operand, handling variable type retrieval from different frames.
     *
     * @param array $operand The operand whose type needs to be resolved.
     * @return string The type of the operand.
     * @throws MyFrameAccessError If the specified frame type is unknown.
     */
    protected function resolveOperandType($operand) {
        if ($operand['type'] == 'var') {
            list($frame, $varName) = explode('@', $operand['value'], 2);
    
            // Decide which frame to access based on the prefix of the variable name
            switch (strtoupper($frame)) {
                case 'GF':
                    return $this->programState->getGlobalVariable($varName)->getType();
                case 'LF':
                    return $this->programState->getLocalVariable($varName)->getType();
                case 'TF':
                    return $this->programState->getTempVariable($varName)->getType();
                default:
                    throw new MyFrameAccessError("Unknown frame type: $frame @ $varName");
            }
        } else {
            return $operand['type'];
        }
    }


     /**
     * Resolves the value of an operand, retrieving variable values from different frames if necessary.
     *
     * @param array $operand The operand whose value needs to be resolved.
     * @return mixed The value of the operand.
     * @throws MyFrameAccessError If the specified frame type is unknown.
     */
    protected function resolveOperand($operand) {
        if ($operand['type'] == 'var') {
            list($frame, $varName) = explode('@', $operand['value'], 2);
    
            // Decide which frame to access based on the prefix of the variable name
            switch (strtoupper($frame)) {
                case 'GF':
                    return $this->programState->getGlobalVariable($varName)->getValue();
                case 'LF':
                    return $this->programState->getLocalVariable($varName)->getValue();
                case 'TF':
                    return $this->programState->getTempVariable($varName)->getValue();
                default:
                    throw new MyFrameAccessError("Unknown frame type: $frame @ $varName");
            }
        } else {
            return $operand['value'];
        }
    }
    


    abstract public function execute();
}