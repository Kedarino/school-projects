<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;


class PUSHFRAMEInstruction extends Instruction {

    public function __construct(ProgramState $programState) {
        parent::__construct($programState);
    }

    public function execute() {
 
            //echo "Pushing frame\n";
            $this->programState->pushLocalFrame();
            return ReturnCode::OK;

    }
}
