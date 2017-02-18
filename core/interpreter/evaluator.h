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
    Evaluator(const Variable& scope);
    Evaluator(const Variable& scope, const Module& module);

    /**
     * @brief Evaluate a right value tagged \link Program program node\endlink
     */
    Variable rightValue(const Program& program, int modifiable = false, int createIfNeeded = false) const;

    /**
     * @brief Get the \link VariablePath variable path\endlink of a variable
     * tagged \link Program program node\endlink.
     */
    VariablePath variablePath(const Program& program) const;

    /**
     * @brief Evaluate a type tagged \link Program program node\endlink
     */
    ObjectType type(const Program& program) const;
    std::shared_ptr<ObjectType> sharedType(const Program& program) const;
private:
    VariableCollector& collector() const;

    Variable unaryOperation(int op, const Variable& a) const;
    Variable binaryOperation(int op, const Variable& a, const Variable& b) const;
    Variable ternaryOperation(int op, const Variable& a, const Variable& b, const Variable& c) const;
    Variable variable(const Program& program, bool modifiable, bool createIfNeeded) const;
    Variable assignField(const Program& path, const Program& rightValue) const;
    Variable arrayScope(const Program& program) const;
    Variable mapScope(const Program& program) const;
    Variable methodEvaluation(const Program& program) const;

    const Variable& scope;
    const Module& module;
};

#endif // EVALUATOR_H
