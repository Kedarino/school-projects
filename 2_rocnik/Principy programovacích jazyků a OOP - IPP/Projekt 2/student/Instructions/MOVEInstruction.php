<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Interpreter;

class MOVEInstruction extends Instruction {
    
    private $destinationVariable;
    private $sourceOperand;

    public function __construct(ProgramState $programState, $destinationVariable, $sourceOperand) {
        parent::__construct($programState);
        $this->destinationVariable = $destinationVariable;
        $this->sourceOperand = $sourceOperand;
    }

    public function execute() {
 
        $sourceValue = $this->resolveOperand($this->sourceOperand);
        
        $varName = $this->destinationVariable['value'];
        
        $type = $this->resolveOperandType($this->sourceOperand);
        if ($type == 'nil'){
            $sourceValue = '';
        }
        $this->programState->updateVariableValue($varName, $sourceValue, $type);

        
        return ReturnCode::OK;
    }
}