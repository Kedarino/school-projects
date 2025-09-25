<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandTypeError;
use IPP\Student\Exceptions\MySemanticError;

class JUMPIFEQInstruction extends Instruction {
    private $label;
    private $operand1;
    private $operand2;

    public function __construct(ProgramState $programState, $label, $operand1, $operand2) {
        parent::__construct($programState);
        $this->label = $label['value'];
        $this->operand1 = $operand1;
        $this->operand2 = $operand2;
    }

    public function execute() {
        if($this->resolveOperandType($this->operand1) != $this->resolveOperandType($this->operand2) && $this->resolveOperandType($this->operand1) == 'nil' || $this->resolveOperandType($this->operand2) == 'nil'){
            throw new MyOperandTypeError("JUMPIFEQ: Wrong operand types");
        }
        $value1 = $this->resolveOperand($this->operand1);
        $value2 = $this->resolveOperand($this->operand2);
        
        if ($value1 == $value2) {

            try {
                $labelIndex = $this->programState->getLabelIndex($this->label);
                }
            catch(MySemanticError $e) {
                    throw $e;
                }

            return (int)$labelIndex + 100; 
        }
        return ReturnCode::OK;
    }
}
