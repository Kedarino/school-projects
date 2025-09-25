<?php

namespace IPP\Student\Instructions;

use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyVariableAccessError;
use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Student\Exceptions\VariableAccessError;

class TYPEInstruction extends Instruction {
    
    private $resultVariable;
    private $symbol;

    public function __construct(ProgramState $programState, $resultVariable, $symbol) {
        parent::__construct($programState);
        $this->resultVariable = $resultVariable;
        $this->symbol = $symbol;
    }

    public function execute() {
        
        try {
            $type = $this->resolveOperandType($this->symbol);
        } catch (MyVariableAccessError$e) {
        
            $type = '';
        }

      
        if (strpos($this->resultVariable['value'], '@') === false) {
            throw new \Exception("Invalid variable identifier: " . $this->resultVariable['value']);
        }

      
        $this->programState->updateVariableValue($this->resultVariable['value'], $type, 'string');

        return ReturnCode::OK;
    }
}
