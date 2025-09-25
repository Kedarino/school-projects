<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandTypeError;

class STRLENInstruction extends Instruction {
    private $var;
    private $symb;

    public function __construct(ProgramState $programState, $var, $symb) {
        parent::__construct($programState);
        $this->var = $var;
        $this->symb = $symb;
    }

    public function execute() {

            $value = $this->resolveOperand($this->symb);
            if ($this->resolveOperandType($this->symb) != 'string') {
                throw new MyOperandTypeError("Operand must be a string");
            }
            $length = mb_strlen($value, 'UTF-8');
            $this->programState->updateVariableValue($this->var['value'], $length, 'int');
            return ReturnCode::OK;

    }
}

