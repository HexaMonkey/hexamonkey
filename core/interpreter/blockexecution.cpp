#include "blockexecution.h"


BlockExecution::BlockExecution(Program::const_iterator begin,
                               Program::const_iterator end,
                               const Module &module,
                               Interpreter &interpreter,
                               ContainerParser *parser)
    : begin(begin),
      end(end),
      current(begin),
      module(module),
      interpreter(interpreter),
      parser(parser)
{
}
