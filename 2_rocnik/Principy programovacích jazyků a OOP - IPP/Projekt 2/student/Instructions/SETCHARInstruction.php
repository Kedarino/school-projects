<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandTypeError;
use IPP\Student\Exceptions\MyStringOperationError;


class SETCHARInstruction extends Instruction {
    private $var;
    private $symb1;
    private $symb2;

    public function __construct(ProgramState $programState, $var, $symb1, $symb2) {
        parent::__construct($programState);
        $this->var = $var;
        $this->symb1 = $symb1;
        $this->symb2 = $symb2;
    }

    public function execute() {
            if ($this->resolveOperandType($this->var) != 'string' || $this->resolveOperandType($this->symb1) != 'int' || $this->resolveOperandType($this->symb2) != 'string' ){
                throw new MyOperandTypeError("SETCHAR: Wrong operand types");
            }

            $string = $this->resolveOperand($this->var);
            $position = $this->resolveOperand($this->symb1);
            $replacement = $this->resolveOperand($this->symb2);

            if ($position < 0 || $position >= mb_strlen($string)) {
                throw new MyStringOperationError("Position out of bounds");
            }
            $string[$position] = $replacement[0];
            $this->programState->updateVariableValue($this->var['value'], $string, 'string');
            return ReturnCode::OK;
    }
}