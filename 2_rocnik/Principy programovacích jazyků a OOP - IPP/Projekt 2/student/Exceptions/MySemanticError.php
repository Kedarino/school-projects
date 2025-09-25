<?php

namespace IPP\Student\Exceptions;

use IPP\Core\Exception\IPPException;

class MySemanticError extends IPPException {
    public function __construct($message = "Semantic error", $code = 52) {
        parent::__construct($message, $code);
    }
}


