<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Student\Exceptions\MySemanticError;
use IPP\Student\Interpreter;
use IPP\Student\Variable;


class DEFVARInstruction extends Instruction {
    
    private $variableIdentifier;

    public function __construct(ProgramState $programState, $variableIdentifier) {
        parent::__construct($programState);
        $this->variableIdentifier = $variableIdentifier;
    }

    public function execute() {          
            list($frame, $varName) = explode('@', $this->variableIdentifier['value'], 2);
            $variable = new Variable(null); 
            switch ($frame) {
                case 'GF':
                    $this->programState->setGlobalVariable($varName, $variable);
                    break;
                case 'LF':
                    $this->programState->setLocalVariable($varName, $variable);
                    break;
                case 'TF':
                    $this->programState->setTempVariable($varName, $variable);
                    break;
                default:
                    throw new MySemanticError("Invalid frame type specified: $frame");
            }


        return ReturnCode::OK;
    }
}
