<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;

class RETURNInstruction extends Instruction {

public function __construct(ProgramState $programState) {
    parent::__construct($programState);
}

public function execute() {

    return $this->programState->getInterpreter()->returnFromFunction();

}
}
