<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyStringOperationError;
use IPP\Student\Exceptions\MyOperandTypeError;

class TypeConversionInstructions extends Instruction {
    private $operation;
    private $resultVariable;
    private $sourceOperand;
    private $indexOperand;

    public function __construct(ProgramState $programState, $operation, $resultVariable, $sourceOperand, $indexOperand = null) {
        parent::__construct($programState);
        $this->operation = $operation;
        $this->resultVariable = $resultVariable;
        $this->sourceOperand = $sourceOperand;
        $this->indexOperand = $indexOperand; // Used only for STRI2INT
    }

    public function execute() {
        $value = $this->resolveOperand($this->sourceOperand);
        $valueType = $this->resolveOperandType($this->sourceOperand);

        
            if ($this->operation === 'INT2CHAR') {
                if ($valueType !== 'int' || $value < 0 || $value > 1114111) {
                    throw new MyOperandTypeError("Invalid Unicode value for INT2CHAR");
                }
                $char = mb_chr($value, 'UTF-8');
                $this->programState->updateVariableValue($this->resultVariable['value'], $char, 'char');
            } elseif ($this->operation === 'STRI2INT') {
                if ($valueType !== 'string') {
                    throw new MyOperandTypeError("Expected string for STRI2INT");
                }
                $index = $this->resolveOperand($this->indexOperand);
                $indexType = $this->resolveOperandType($this->indexOperand);
                if ($indexType !== 'int' || $index < 0 || $index >= mb_strlen($value, 'UTF-8')) {
                    throw new MyStringOperationError("Invalid index for STRI2INT");
                }
                $char = mb_substr($value, $index, 1, 'UTF-8');
                $ordValue = mb_ord($char, 'UTF-8');
                $this->programState->updateVariableValue($this->resultVariable['value'], $ordValue, 'int');
            } else {
                throw new MyOperandTypeError("Unknown type conversion operation");
            }


        return ReturnCode::OK;
    }
}