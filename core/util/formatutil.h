#ifndef FORMATUTIL_H
#define FORMATUTIL_H

#include <ostream>
#include "core/variable/variable.h"

std::string formatString(const VariableArgs& args, const VariableKeywordArgs& kwargs);
std::ostream& outputString(std::ostream &output, const VariableArgs& args, const VariableKeywordArgs& kwargs);


#endif // FORMATUTIL_H
