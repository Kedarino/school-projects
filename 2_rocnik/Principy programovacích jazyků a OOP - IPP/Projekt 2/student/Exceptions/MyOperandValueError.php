<?php

namespace IPP\Student\Exceptions;

use IPP\Core\Exception\IPPException;

class MyOperandValueError extends IPPException {
    public function __construct($message = "Operand value error", $code = 57) {
        parent::__construct($message, $code);
    }
}


