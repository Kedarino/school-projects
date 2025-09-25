<?php

namespace IPP\Student\Exceptions;

use IPP\Core\Exception\IPPException;


class MyOperandTypeError extends IPPException {
    public function __construct($message = "Operand type error", $code = 53) {
        parent::__construct($message, $code);
    }
}

