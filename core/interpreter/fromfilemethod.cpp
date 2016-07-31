#include "fromfilemethod.h"

#include "core/interpreter/blockexecution.h"
#include "core/interpreter/evaluator.h"

#include "core/variable/localscope.h"
#include "core/variable/methodscope.h"

Variable FromFileMethod::call(VariableArgs &args, VariableKeywordArgs &kwargs, VariableCollector &collector) const
{
    fillArgs(args, kwargs, collector);

    Variable scope(new LocalScope(Variable(new MethodScope(args, kwargs, collector), true), _module), true);

    Evaluator eval(scope, _module);
    BlockExecution blockExecution(_definition, eval, scope, nullptr);

    blockExecution.execute();

    return blockExecution.returnValue();
}
