<?php

namespace IPP\Student\Exceptions;

use IPP\Core\Exception\IPPException;

class MyFrameAccessError extends IPPException {
    public function __construct($message = "Frame access error", $code = 55) {
        parent::__construct($message, $code);
    }
}

