<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandTypeError;

class CONCATInstruction extends Instruction {
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

            if($this->resolveOperandType($this->symb1) != 'string' || $this->resolveOperandType($this->symb2) != 'string' ){
                throw new MyOperandTypeError("CONCAT: Wrong operand types");
            }
            
            $value1 = $this->resolveOperand($this->symb1);
            $value2 = $this->resolveOperand($this->symb2);

            $result = $value1 . $value2;
            $this->programState->updateVariableValue($this->var['value'], $result, 'string');
            return ReturnCode::OK;
    }
}

