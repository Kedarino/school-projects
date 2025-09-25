<?php

namespace IPP\Student\Instructions;
use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandTypeError;


class SUBInstruction extends Instruction {
    private $resultVariable;
    private $operand1;
    private $operand2;

    public function __construct(ProgramState $programState, $resultVariable, $operand1, $operand2) {
        parent::__construct($programState);
        $this->resultVariable = $resultVariable;
        $this->operand1 = $operand1;
        $this->operand2 = $operand2;
    }

    public function execute() {

        if ($this->resolveOperandType($this->operand1) != "int" || $this->resolveOperandType($this->operand1) != "int")
        {
            throw new MyOperandTypeError();
        }
        $operand1Value = $this->resolveOperand($this->operand1);
        $operand2Value = $this->resolveOperand($this->operand2);
        $result = $operand1Value - $operand2Value;

            
        $this->programState->updateVariableValue($this->resultVariable['value'], $result, 'int');


        return ReturnCode::OK;
    }
}
