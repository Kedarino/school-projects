<?php

namespace IPP\Student;

use IPP\Student\Exceptions\MyFrameAccessError;
use IPP\Student\Exceptions\MySemanticError;
use IPP\Student\Exceptions\MyVariableAccessError;
use IPP\Student\Variable;

/**
 * @class ProgramState
 * @brief Manages the state of the program, including variable frames and labels.
 * 
 * This class is responsible for handling all runtime environments such as global,
 * local, and temporary variable frames, as well as storing label definitions for jump instructions.
 */
class ProgramState {
    private $labels = []; ///< Associative array storing label names and their instruction indices
    private $globalFrame = []; ///< Array representing the global frame for variable storage
    private $localFrames = []; ///< Stack of local frames for managing variable scopes
    private $tempFrame = null; ///< Temporarily stores variables, used during function calls
    private Interpreter $interpreter; ///< Reference to the interpreter using this program state
    
        
    public function __construct(Interpreter $interpreter) {
        $this->interpreter = $interpreter;
        $this->globalFrame = [];
    }

    /**
    * Returns the interpreter instance associated with this program state.
    * @return Interpreter The interpreter using this program state.
    */
    public function getInterpreter(): Interpreter {
        return $this->interpreter;
    }
    
    /**
     * Updates or defines a variable in the specified frame.
     * @param  $var Variable identifier composed of frame prefix and name.
     * @param  $sourceValue New value to be set for the variable.
     * @param  $type The type of the variable to be updated.
     * @throws MyFrameAccessError If the specified frame does not exist.
     */
    public function updateVariableValue($var, $sourceValue, $type) {
        
        list($frame, $varName) = explode('@', $var, 2);
        
        switch ($frame) {
            case 'GF':
                $this->checkAndUpdate($this->globalFrame, $varName, $sourceValue, $type);
                break;
            case 'LF':
                if (empty($this->localFrames)) {
                    throw new MyFrameAccessError("Local frame stack is empty.");
                }
                $lastIndex = count($this->localFrames) - 1;
                $this->checkAndUpdate($this->localFrames[$lastIndex], $varName, $sourceValue, $type);
                break;
            case 'TF':
                if ($this->tempFrame === null) {
                    throw new MyFrameAccessError("Temporary frame is not set.");
                }
                $this->checkAndUpdate($this->tempFrame, $varName, $sourceValue, $type);
                break;
            default:
                throw new MyFrameAccessError("Unknown frame type: $frame");
        }
    }

    /**
    * Checks if a variable exists in the specified frame and updates its value and type.
    * @param array &$frame Reference to the frame in which the variable is to be updated.
    * @param string $name The name of the variable to update.
    * @param mixed $newValue The new value for the variable.
    * @param string $type The new type for the variable.
    * @throws MyVariableAccessError If the variable does not exist in the specified frame.
    */
    private function checkAndUpdate(&$frame, $name, $newValue, $type) {
        if (!isset($frame[$name])) {
            throw new MyVariableAccessError("Variable '$name' not defined in the specified frame");
        }
        $frame[$name]->setValue($newValue);
        $frame[$name]->setType($type);
    }


    /**
     * Sets a new global variable.
     * @param $name Name of the global variable.
     * @param Variable $variable The variable object to set in the global frame.
     * @throws MySemanticError If the variable is being redefined.
     */
    public function setGlobalVariable($name, Variable $variable) {
        if (isset($this->globalFrame[$name])) {
            throw new MySemanticError("Global variable redefinition: '$name'");
        }
        $this->globalFrame[$name] = $variable;
    }

    /**
     * Retrieves a global variable from the global frame.
     * @param $name Name of the global variable to retrieve.
     * @return * variable object from the global frame.
     * @throws MyVariableAccessError If the variable is not defined in the global frame.
     */
    public function getGlobalVariable($name) {
        if (!isset($this->globalFrame[$name])) {
            throw new MyVariableAccessError("Global variable '$name' not defined");
        }
        return $this->globalFrame[$name];
    }

    /**
     * Pushes the current temporary frame to the stack of local frames.
     * @throws MyFrameAccessError If there is no temporary frame set when attempting to push.
     */
    public function pushLocalFrame() {
        if ($this->tempFrame === null) {
            throw new MyFrameAccessError("Temporary frame is not set.");
        }
        array_push($this->localFrames, $this->tempFrame);
        $this->tempFrame = null;
    }

    /**
     * Pops the top local frame from the stack and sets it as the temporary frame.
     * @throws MyFrameAccessError If there are no local frames to pop.
     */
    public function popLocalFrame() {
        if (empty($this->localFrames)) {
            throw new MyFrameAccessError("No local frames to pop");
        }
        $this->tempFrame = array_pop($this->localFrames);
    }

        /**
     * Sets a local variable in the current local frame.
     * @param  $name Name of the local variable.
     * @param Variable $variable The variable object to set in the local frame.
     * @throws MyFrameAccessError If no local frame is available to set the variable.
     * @throws MySemanticError If the variable is being redefined in the current local frame.
     */
    public function setLocalVariable($name, Variable $variable) {
        if (empty($this->localFrames)) {
            throw new MyFrameAccessError("No local frame available");
        }
        
        $currentFrameIndex = count($this->localFrames) - 1;
        if (isset($this->localFrames[$currentFrameIndex][$name])) {
            throw new MySemanticError("Local variable redefinition: '$name'");
        }
        $this->localFrames[$currentFrameIndex][$name] = $variable;
    }

    /**
     * Retrieves a local variable from the current local frame.
     * @param $name Name of the local variable to retrieve.
     * @return Variable The variable object from the local frame.
     * @throws MyFrameAccessError If no local frame is available to retrieve the variable.
     */
    public function getLocalVariable($name) {
        if (empty($this->localFrames)) {
            throw new MyFrameAccessError("No local frame available");
        }
        $currentFrame = end($this->localFrames);
        if (!isset($currentFrame[$name])) {
            throw new MyVariableAccessError("Local variable '$name' not defined");
        }
        return $currentFrame[$name];
    }

    /**
     * Creates a new temporary frame, resetting the existing one if any.
     */
    public function createTempFrame() {
        $this->tempFrame = [];
    }

    /**
     * Sets a temporary variable in the current temporary frame.
     * @param $name Name of the temporary variable.
     * @param Variable $variable The variable object to set in the temporary frame.
     * @throws MyFrameAccessError If the temporary frame is not set when trying to define a variable.
     * @throws MySemanticError If the temporary variable is being redefined.
     */

    public function setTempVariable($name, Variable $variable) {
        if ($this->tempFrame === null) {
            throw new MyFrameAccessError("Temporary frame is not set. Cannot set temporary variable.");
        }
        if (isset($this->tempFrame[$name])) {
            throw new MySemanticError("Temporary variable redefinition: '$name'");
        }
        $this->tempFrame[$name] = $variable;
    }

    /**
     * Retrieves a temporary variable from the current temporary frame.
     * @param string $name Name of the temporary variable to retrieve.
     * @return Variable The variable object from the temporary frame.
     * @throws MyFrameAccessError If the temporary frame is unset when trying to retrieve a variable.
     */
    public function getTempVariable($name) {
        if ($this->tempFrame === null) {
            throw new MyFrameAccessError("Temporary frame is not set or has been unset.");
        }
        if (!isset($this->tempFrame[$name])) {
            throw new MyVariableAccessError("Temporary variable '$name' not defined in temporary frame.");
        }
        return $this->tempFrame[$name];
    }



    /**
     * Adds a label to the label collection with its corresponding instruction index.
     * @param string $label Name of the label.
     * @param int $index Instruction index associated with the label.
     */
    public function addLabel($label, $index) {
        $this->labels[$label] = $index;
    }

    /**
     * Retrieves the instruction index associated with a label.
     * @param string $label Name of the label.
     * @return int Instruction index of the label.
     * @throws MySemanticError If the label is not found in the label collection.
     */
    public function getLabelIndex($label) {

        if (!isset($this->labels[$label])) {
            throw new MySemanticError("Label not found: $label");
        }

        return $this->labels[$label];
    }


}