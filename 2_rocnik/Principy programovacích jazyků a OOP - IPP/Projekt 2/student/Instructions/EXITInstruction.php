<?php

namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandValueError;
use IPP\Student\Exceptions\MyOperandTypeError;

class EXITInstruction extends Instruction {
    private $exitCodeOperand;

    public function __construct(ProgramState $programState, $exitCodeOperand) {
        parent::__construct($programState);
        $this->exitCodeOperand = $exitCodeOperand;
    }

    public function execute() {
        if ($this->resolveOperandType($this->exitCodeOperand) != 'int'){
            throw new MyOperandTypeError("EXIT: Wrong exit code type");
        }
        $exitCode = $this->resolveOperand($this->exitCodeOperand);

        // Check if the exit code is an integer and within the specified range
        if (is_numeric($exitCode) && $exitCode >= 0 && $exitCode <= 9) {
            // Cast to integer to ensure proper type
            $exitCode = (int) $exitCode;
            
            return ['exit' => true, 'code' => $exitCode];; // Terminate the program with the specified exit code
        } else {
            throw new MyOperandValueError("EXIT: Invalid exit code $exitCode");
        }
    }
}