#ifndef VARIABLE_H
#define VARIABLE_H

class Variant;

class Variable
{
public:
    ~Variable();

    Variant& value();
    const Variant& constValue() const;

    static Variable* copy(const Variant& value);
    static Variable* move(Variant* value);
    static Variable* reference(Variant& value);
    static Variable* constReference(const Variant& value);
private:
    Variable(bool own, bool constant , Variant* var, const Variant* constVar);
    Variable() = delete;
    Variable(const Variable&) = delete;
    Variable& operator =(const Variable&) = delete;

    bool _own;
    bool _constant;
    Variant* _var;
    const Variant* _constVar;

};

#endif // VARIABLE_H
