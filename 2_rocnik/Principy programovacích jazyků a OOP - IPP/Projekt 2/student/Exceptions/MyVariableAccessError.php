<?php

namespace IPP\Student\Exceptions;

use IPP\Core\Exception\IPPException;

class MyVariableAccessError extends IPPException {
    public function __construct($message = "Variable access error", $code = 54) {
        parent::__construct($message, $code);
    }
}