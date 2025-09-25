<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;

class READInstruction extends Instruction {
    private $var;
    private $type;

    public function __construct(ProgramState $programState, $var, $type) {
        parent::__construct($programState);
        $this->var = $var;
        $this->type = $type;
    }

    public function execute() {
        $input = $this->programState->getInterpreter()->getInputReader();
        $value = null;

        
        switch ($this->type) { // Now just using the type directly as a string
            case 'int':
                $value = $input->readInt();
                break;
            case 'bool':
                $value = $input->readBool();
                break;
            case 'string':
                $value = $input->readString();
                break;
            default:
                $value = 'nil'; // If type is unsupported, default to 'nil'
                break;
        }

        // If reading failed or unsupported type, set variable to nil
        if ($value === null) {
            $value = 'nil';
        }

        // Update the variable with the read or default value
        
        $this->programState->updateVariableValue($this->var['value'], $value, $this->type);
       

        return ReturnCode::OK;
    }
}
