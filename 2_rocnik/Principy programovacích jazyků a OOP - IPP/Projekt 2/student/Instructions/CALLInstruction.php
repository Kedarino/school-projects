<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;

class CALLInstruction extends Instruction {
    private $label;

    public function __construct(ProgramState $programState, $label) {
        parent::__construct($programState);
        $this->label = $label;
    }

    public function execute() {
        $index = $this->programState->getLabelIndex($this->label['value']);
        $this->programState->getInterpreter()->callFunction($index);
        return ReturnCode::OK;
    }
}


