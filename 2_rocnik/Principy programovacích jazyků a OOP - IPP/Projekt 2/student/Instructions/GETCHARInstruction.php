<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandTypeError;
use IPP\Student\Exceptions\MyOperandValueError;
use IPP\Student\Exceptions\MyStringOperationError;

class GETCHARInstruction extends Instruction {
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

        $var1 = $this->resolveOperandType($this->symb1);
        $var2 = $this->resolveOperandType($this->symb2);

            if ($this->resolveOperandType($this->symb1) != 'string' || $this->resolveOperandType($this->symb2) != 'int' ) {
                throw new MyOperandTypeError("GETCHAR: Wrong operand types");
            }

            

            $string = $this->resolveOperand($this->symb1);
            $position = (int)$this->resolveOperand($this->symb2);


            if($string == '' || $position == ''){
                throw new MyOperandValueError("GETCHAR: Wrong operand value");
            }
            if ($position < 0 || $position >= mb_strlen($string)) {
                
                throw new MyStringOperationError("GETCHAR: Position out of bounds");
            }
            $char = mb_substr($string, $position, 1);
            $this->programState->updateVariableValue($this->var['value'], $char, 'string');
            return ReturnCode::OK;
    }
}

