<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;


class POPFRAMEInstruction extends Instruction {

    public function __construct(ProgramState $programState) {
        parent::__construct($programState);
    }

    public function execute() {
        
            
            $this->programState->popLocalFrame();
            return ReturnCode::OK;
    
}
}