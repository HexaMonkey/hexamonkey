#ifndef SCOPE_H
#define SCOPE_H

class Variant;

class Scope
{
public:
    virtual ~Scope() {}
    virtual Variant* get(const Variant& key) const = 0;
    virtual const Variant* cget(const Variant& key) const = 0;
    Variant *declare(const Variant& key) const;
    Scope *getScope(const Variant& key) const;
protected:
    virtual Variant *doDeclare(const Variant& key) const;
    virtual Scope *doGetScope(const Variant&) const;
};

class MutableScope : public Scope
{
public:
    virtual ~MutableScope() {}
    Variant* get(const Variant& key) const override;
    const Variant* cget(const Variant& key) const override;
protected:
    virtual Variant* doGet(const Variant& key) const = 0;
};

class ConstScope : public Scope
{
public:
    virtual ~ConstScope() {}
    virtual Variant* get(const Variant& key) const override;
    virtual const Variant* cget(const Variant& key) const override;
protected:
    virtual const Variant* doCget(const Variant& key) const = 0;
};

class EmptyScope : public MutableScope
{
protected:
    Variant* doGet(const Variant &key) const override;
};

#endif // SCOPE_H
