<?php
namespace IPP\Student\Instructions;

use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MySemanticError;

class JUMPInstruction extends Instruction {
    private $label;

    public function __construct(ProgramState $programState, $label) {
        parent::__construct($programState);
        $this->label = $label['value'];
    }

    public function execute() {
            try{
            $labelIndex = $this->programState->getLabelIndex($this->label);
            }
            catch(MySemanticError $e)
            {
                throw $e;
            }
            return (int)$labelIndex + 100; 

    }
}

