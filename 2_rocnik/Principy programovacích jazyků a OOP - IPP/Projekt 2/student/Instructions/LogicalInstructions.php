<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MyOperandTypeError;
use IPP\Student\Exceptions\MySemanticError;
use mysqli;

class LogicalInstructions extends Instruction {
    private $operation;
    private $resultVariable;
    private $operand1;
    private $operand2;

    public function __construct(ProgramState $programState, $operation, $resultVariable, $operand1, $operand2 = null) {
        parent::__construct($programState);
        $this->operation = $operation;
        $this->resultVariable = $resultVariable;
        $this->operand1 = $operand1;
        $this->operand2 = $operand2; // Might be null for NOT operation
    }

    public function execute() {
        $type1 = $this->resolveOperandType($this->operand1);
        $type2 = $this->operand2 ? $this->resolveOperandType($this->operand2) : null;
        $value1 = $this->resolveOperand($this->operand1);
        $value2 = $this->operand2 ? $this->resolveOperand($this->operand2) : null;

                // Type checking for NOT operation, should only have one operand and be of type bool
        if ($this->operation === 'NOT' && ($this->operand2 !== null || $type1 !== 'bool')) {
            throw new MyOperandTypeError("NOT operation should only have one operand of type bool.");
        }

        // Type checking for AND, OR operations, both operands should be of type bool
        if (in_array($this->operation, ['AND', 'OR']) && ($type1 !== 'bool' || $type2 !== 'bool')) {
            throw new MyOperandTypeError("AND/OR operations require both operands to be of type bool.");
        }

        // Type checking for LT, GT, EQ operations
        if (in_array($this->operation, ['LT', 'GT', 'EQ']) && ($type1 !== $type2 || ($type1 !== 'int' && $type1 !== 'string' && $type1 !== 'bool'))) {
            if ($type1 === 'nil' || $type2 === 'nil') {
                if ($this->operation !== 'EQ') {
                    throw new MyOperandTypeError("Only EQ operation is allowed with 'nil' type.");
                }
            } else {
                throw new MyOperandTypeError("LT/GT/EQ operations require both operands to be of the same type (int, string, bool).");
            }
        }

        switch ($this->operation) {
            case 'LT':
                $result = $value1 < $value2;
                break;
            case 'GT':
                $result = $value1 > $value2;
                break;
            case 'EQ':
                $result = $value1 == $value2;
                break;
            case 'AND':
                $result = $value1 && $value2;
                break;
            case 'OR':
                $result = $value1 || $value2;
                break;
            case 'NOT':
                $result = !$value1;
                break;
            default:
                throw new MySemanticError("Unknown operation: " . $this->operation);
        }

        if ($this->operation === 'NOT' && $this->operand2 !== null) {
            throw new MySemanticError("NOT operation should only have one operand.");
        }


        
        $this->programState->updateVariableValue($this->resultVariable['value'], $result, 'bool');
        return ReturnCode::OK;
    }
}
