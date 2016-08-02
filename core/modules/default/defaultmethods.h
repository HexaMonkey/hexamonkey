#ifndef DEFAULTMETHODS_H
#define DEFAULTMETHODS_H

#include "core/modulemethod.h"

#include "core/variable/variable.h"
#include "core/variable/variablecollector.h"
#include "core/util/strutil.h"
#include "core/util/bitutil.h"
#include "core/util/formatutil.h"

#include "core/log/logmanager.h"

class SizeOfMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            int64_t size = args[0].value().toObjectType().fixedSize();
            if (size != -1) {
                return collector.copy(size);
            } else {
                return collector.null();
            }
        } else {
            return Variable();
        }
    }
};

class FormatStringMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs& kwargs, VariableCollector& collector) const override
    {
        return collector.copy(formatString(args, kwargs));
    }
};

class LogMethod : public ModuleMethod
{
public:
    LogMethod(LogLevel level) : _level(level)
    {
    }

private:
    virtual Variable call(VariableArgs& args, VariableKeywordArgs& kwargs, VariableCollector&) const override
    {
        LogManager::getInstance()->log(formatString(args, kwargs), _level);
        return Variable();
    }

    LogLevel _level;
};

class FormatDateMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            uint64_t secs = args[0].value().toUnsignedInteger();
            return collector.copy(formatDate(secs));
        } else {
            return Variable();
        }
    }
};

class FormatDurationMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            uint64_t secs = args[0].value().toUnsignedInteger();
            return collector.copy(formatDuration(secs));
        } else {
            return Variable();
        }
    }
};

std::vector<MethodArgument> toNumericSignature = {MethodArgument("value", true, Variant()), MethodArgument("base", true, 10)};

class ToIntMethod : public ModuleMethod
{
public:
    ToIntMethod() : ModuleMethod(toNumericSignature)
    {

    }

private:
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&kwargs, VariableCollector& collector) const override
    {
        fillNumberedArgs(args, kwargs, collector);

        const Variant& value = args[0].value();
        int base = (int) args[1].value().toInteger();
        if(value.canConvertTo(Variant::integerType))
        {
            Variant newValue = value.toInteger();
            newValue.setDisplayBase(base);
            return collector.copy(newValue);
        }
        else if(value.canConvertTo(Variant::stringType))
        {
            std::stringstream S;
            S<<value.toString();
            int64_t i;
            if(!(S>>i).fail()) {
                Variant newValue = i;
                newValue.setDisplayBase(base);
                return collector.copy(newValue);
            }
        }
        return Variable();
    }
};

class ToUIntMethod : public ModuleMethod
{
public:
    ToUIntMethod() : ModuleMethod(toNumericSignature)
    {

    }

private:
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&kwargs, VariableCollector& collector) const override
    {
        fillNumberedArgs(args, kwargs, collector);

        const Variant& value = args[0].value();
        int base = (int) args[1].value().toInteger();
        if(value.canConvertTo(Variant::unsignedIntegerType))
        {
            Variant newValue = value.toUnsignedInteger();
            newValue.setDisplayBase(base);
            return collector.copy(newValue);
        }
        else if(value.canConvertTo(Variant::stringType))
        {
            std::stringstream S;
            S<<value.toString();
            uint64_t i;
            if(!(S>>i).fail()) {
                Variant newValue = i;
                newValue.setDisplayBase(base);
                return collector.copy(newValue);
            }
        }
        return Variable();
    }
};

class ToFloatMethod : public ModuleMethod
{
public:
    ToFloatMethod() : ModuleMethod(toNumericSignature)
    {

    }

private:
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&kwargs, VariableCollector& collector) const override
    {
        fillNumberedArgs(args, kwargs, collector);

        const Variant& value = args[0].value();
        int base = (int) args[1].value().toInteger();
        if(value.canConvertTo(Variant::unsignedIntegerType))
        {
            Variant newValue = value.toDouble();
            newValue.setDisplayBase(base);
            return collector.copy(newValue);
        }
        else if(value.canConvertTo(Variant::stringType))
        {
            std::stringstream S;
            S<<value.toString();
            double d;
            if(!(S>>d).fail()) {
                Variant newValue = d;
                newValue.setDisplayBase(base);
                return collector.copy(newValue);
            }
        }
        return Variable();
    }
};

class FromAsciiMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            const char ch = args[0].value().toInteger();
            const std::string s(1, ch);
            return collector.copy(s);
        } else {
            return Variable();
        }
    }
};

class ToAsciiMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            const std::string& str = args[0].value().toString();
            if(str.empty())
                return Variable();
            else
                return collector.copy(str[0]);
        } else {
            return Variable();
        }
    }
};

class ToUpperMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            std::string str = toStr(args[0].value());
            std::transform(str.begin(), str.end(),str.begin(), ::toupper);
            return collector.copy(str);
        } else {
            return Variable();
        }
    }
};

class ToLowerMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            std::string str = toStr(args[0].value());
            std::transform(str.begin(), str.end(),str.begin(), ::tolower);
            return collector.copy(str);
        } else {
            return Variable();
        }
    }
};

class PopCountMethod : public ModuleMethod
{
    virtual Variable call(VariableArgs& args, VariableKeywordArgs&, VariableCollector& collector) const override
    {
        if (args.size() >= 1) {
            uint64_t word = args[0].value().toUnsignedInteger();
            return collector.copy(popCount(word));
        } else {
            return Variable();
        }
    }
};

std::vector<MethodArgument> substringSignature = {MethodArgument("string", true, Variant()), MethodArgument("start", true, Variant()), MethodArgument("size", true, Variant())};

class SubstringMethod : public ModuleMethod
{
public:
    SubstringMethod() : ModuleMethod(substringSignature)
    {}

private:
    virtual Variable call(VariableArgs& args, VariableKeywordArgs& kwargs, VariableCollector& collector) const override
    {

        fillNumberedArgs(args, kwargs, collector);

        if(   args[0].value().canConvertTo(Variant::stringType)
           && args[1].value().canConvertTo(Variant::unsignedIntegerType))
        {
            const std::string str = args[0].value().toString();

            uint64_t start;
            uint64_t size;
            if(args[2].value().canConvertTo(Variant::unsignedIntegerType)) {
                start = args[1].value().toUnsignedInteger();
                size = args[2].value().toUnsignedInteger();
            } else {
                start = 0;
                size = args[1].value().toUnsignedInteger();
            }

            if(start < str.size())
            {
                if(start + size < str.size())
                    return collector.copy(str.substr(start, size));
                else
                    return collector.copy(str.substr(start));
            }
        }

        return Variable();
    }
};

#endif // DEFAULTMETHODS_H
