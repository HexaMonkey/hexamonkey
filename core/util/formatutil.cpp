#include "formatutil.h"

#include "core/util/strutil.h"




std::ostream &outputString(std::ostream &output, const VariableArgs &args, const VariableKeywordArgs &kwargs)
{
    if (args.size() > 0)
    {


        if (args.size() == 1 && kwargs.size() == 0) {
            output << args[0].value();
        } else {
            const std::string input = toStr(args[0].value());
            size_t n = input.size();
            if (n > 0) {
                size_t defaultPlaceHolderIndex = 1;

                size_t i = 0;
                while (true) {
                    size_t j = i;
                    for (; j < n; ++j) {
                       if (input[j] == '{') {
                           break;
                       }
                    }
                    output << input.substr(i, j-i);

                    if (j == n) {
                        break;
                    }

                    j = i = j + 1;
                    if (j == n || input[j] != '{') {
                        for (;j < n; ++j) {
                            const char c = input[j];
                            if (c == ':' || c == '}') {
                                break;
                            }
                        }
                        const std::string id = input.substr(i, j-i);
                        Variant value;
                        if (id.empty()) {
                            if (defaultPlaceHolderIndex < args.size()) {
                                value = args[defaultPlaceHolderIndex].value();
                            }
                            ++defaultPlaceHolderIndex;
                        } else if ('0' <= id[0] && id[0] <= '9') {
                            size_t index = strTo<size_t>(id) + 1;
                            if (index) {
                                if (index < args.size()) {
                                    value = args[index].value();
                                }
                            }
                        } else {
                            auto it = kwargs.find(id);
                            if (it != kwargs.cend()) {
                                value = it->second.value();
                            }
                        }
                        i = j;

                        if (i < n && input[i] == ':') {
                            ++i;
                            if (i == n) {
                                output << value;
                                break;
                            } else {
                                char c = input[i];

                                switch (c) {
                                    case 'd':
                                    case 'x':
                                    case 'o':
                                    case 'b':
                                    {
                                        ++i;
                                        j = i;
                                        for (;j < n; ++j) {
                                            if (input[j] == '}') {
                                                break;
                                            }
                                        }
                                        int width = 0;
                                        if (j != i) {
                                            width = strTo<int>(input.substr(i, j-i));
                                        }


                                        std::stringstream S;
                                        S<<std::setbase((c == 'b') ? 2 : (c == 'o') ? 8 : (c == 'x') ? 16 : 10)<<std::setw(width)<<std::setfill('0');
                                        value.display(S, false);
                                        output << S.str();

                                        i = j;

                                        break;
                                    }

                                case 'f':
                                case 'e':
                                {
                                    ++i;

                                    std::stringstream S;
                                    if (c == 'e') {
                                        S << std::scientific;
                                    } else if (i + 1 < n && input[i] == '.') {
                                        S << std::fixed;
                                        i += 1;

                                    }

                                    j = i;
                                    for (;j < n; ++j) {
                                        if (input[j] == '}') {
                                            break;
                                        }
                                    }

                                    if (j != i) {
                                        S<< std::setprecision(strTo<int>(input.substr(i, j-i)));
                                    }

                                    S << value.toDouble();

                                    output << S.str();

                                    i = j;

                                    break;
                                }

                                    case '<':
                                    case '>':
                                    case '^':
                                    {
                                        ++i;

                                        char fill = ' ';
                                        if (i + 2 < n && input[i] == '@') {
                                            fill = input[i + 1];
                                            i += 2;
                                        }

                                        j = i;

                                        for (;j < n; ++j) {
                                            if (input[j] == '}') {
                                                break;
                                            }
                                        }
                                        size_t width = 0;
                                        if (j != i) {
                                            width = strTo<size_t>(input.substr(i, j-i));
                                        }
                                        const std::string valueRepresentation = toStr(value);
                                        if (valueRepresentation.size() >= width) {
                                            output << valueRepresentation;
                                        } else {
                                            size_t missing = width - valueRepresentation.size();
                                            size_t left = c == '<' ? 0 : c == '>' ? missing : missing / 2;
                                            output << std::string(left, fill) << valueRepresentation << std::string(missing - left, fill);
                                        }

                                        i = j;

                                        break;
                                    }

                                    default:
                                        output << value;
                                        break;


                                }
                            }
                        } else {
                            output << value;
                        }

                        if (j == n) {
                            break;
                        }

                        i = j+1;
                    }

                    if (i == n) {
                        break;
                    }
                }
            }
        }
    }

    return output;
}

std::string formatString(const VariableArgs &args, const VariableKeywordArgs &kwargs)
{
    std::stringstream S;
    outputString(S, args, kwargs);
    return S.str();
}
