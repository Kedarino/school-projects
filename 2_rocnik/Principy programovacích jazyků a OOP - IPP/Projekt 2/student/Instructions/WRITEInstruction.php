<?php
namespace IPP\Student\Instructions;

use IPP\Student\Instructions\Instruction;
use IPP\Student\ProgramState;
use IPP\Core\ReturnCode;
use IPP\Core\AbstractInterpreter;


class WRITEInstruction extends Instruction {
    
    private $operand;

    public function __construct(ProgramState $programState, $operand) {
        parent::__construct($programState);
        $this->operand = $operand;
    }

    public function execute() {

            $valueToWrite = $this->resolveOperand($this->operand);
            $stdout = $this->programState->getInterpreter()->getStdOut();
            switch ($this->operand['type']) {
                case 'bool':
                    $stdout->writeString($valueToWrite ? 'true' : 'false');
                    break;
                case 'nil':
                    $stdout->writeString(''); // Output nothing for 'nil'
                    break;
                case 'string':
                    // Convert escape sequences to their actual characters
                    $formattedString = $this->unescapeString($valueToWrite);
                    $stdout->writeString($formattedString);
                    break;
                default:
                    $stdout->writeString((string)$valueToWrite);
                    break;


        return ReturnCode::OK; // Indicate successful execution
    }
    }

    private function unescapeString($inputString) {
        // Handle common escape sequences
        $replacements = [
            '\032' => ' ',  // Space
            '\10' => "\n",   // Newline
            '\092' => '\\'
        ];
    
        // Replace each escape sequence defined in the replacements array
        foreach ($replacements as $escape => $char) {
            $inputString = str_replace($escape, $char, $inputString);
        }
    
        // Handle numeric escape sequences \xyz (decimal values)
        return preg_replace_callback('/\\\\(\d{3})/', function ($matches) {
            return chr((int)$matches[1]);
        }, $inputString);
    }
}

    

