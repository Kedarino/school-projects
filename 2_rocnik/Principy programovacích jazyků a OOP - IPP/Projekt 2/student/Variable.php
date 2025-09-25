<?php

namespace IPP\Student;

/**
 * Class representing a variable with a type and a value.
 */
class Variable {
    private $type;
    private $value;

    /**
     * Constructs a new Variable instance.
     *
     * @param string $type The data type of the variable.
     * @param mixed $value Initial value of the variable, defaults to null.
     */
    public function __construct($type, $value = null) {
        $this->type = $type;
        $this->value = $value;
    }

    /**
     * Sets the value of the variable.
     *
     * @param $value The new value to set.
     */
    public function setValue($value) {
        $this->value = $value;
    }

    /**
     * Gets the value of the variable.
     *
     * @return mixed The current value of the variable.
     */
    public function getValue() {
        return $this->value;
    }

    /**
     * Gets the type of the variable.
     *
     * @return string The current type of the variable.
     */
    public function getType() {
        return $this->type;
    }


    /**
     * Sets the type of the variable.
     *
     * @param string $type The new type to set.
     */
    public function setType($type) {
        $this->type = $type;
    }
}
