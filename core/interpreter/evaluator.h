#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "core/module.h"
#include "core/variable/variable.h"
#include "core/variable/variablepath.h"

class Program;

/**
 * @brief Provides functions to evaluate \link Program program nodes\endlink
 */
class Evaluator
{
public:
    Evaluator();
    Evaluator(const Variable& scope);
    Evaluator(const Module &module);
    Evaluator(const Variable& scope, const Module& module);

    /**
     * @brief Evaluate a right value tagged \link Program program node\endlink
     */
    Variable rightValue(const Program& program, int modifiable = false) const;

    /**
     * @brief Get the \link VariablePath variable path\endlink of a variable
     * tagged \link Program program node\endlink.
     */
    VariablePath variablePath(const Program& program) const;

    /**
     * @brief Evaluate a type tagged \link Program program node\endlink
     */
    ObjectType type(const Program& program) const;
private:
    Variable unaryOperation(int op, const Variable& a) const;
    Variable binaryOperation(int op, const Variable& a, const Variable& b) const;
    Variable ternaryOperation(int op, const Variable& a, const Variable& b, const Variable& c) const;
    Variable function(const Program& program) const;
    Variable variable(const Program& program, bool modifiable) const;
    Variable assignField(const Program& path, const Program& rightValue) const;

    const Variable& scope;
    const Module& module;
};

#endif // EVALUATOR_H
