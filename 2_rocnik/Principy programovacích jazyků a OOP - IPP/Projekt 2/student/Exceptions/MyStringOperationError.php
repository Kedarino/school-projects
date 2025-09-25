<?php

namespace IPP\Student\Exceptions;

use IPP\Core\Exception\IPPException;


class MyStringOperationError extends IPPException {
    public function __construct($message = "String operation error", $code = 58) {
        parent::__construct($message, $code);
    }
}