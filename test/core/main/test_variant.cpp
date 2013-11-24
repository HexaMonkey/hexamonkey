
#include "test_variant.h"

#include "core/variant.h"
#include "core/objecttype.h"

void TestVariant::unknown()
{
    Variant var;
    QCOMPARE(var.isNull(), true);
    QCOMPARE(var.type(), Variant::Type::unknown);
    QCOMPARE(var.hasNumericalType(), false);

    QCOMPARE(var.toInteger(), 0LL);
    QCOMPARE(var.toUnsignedInteger(), 0ULL);
    QCOMPARE(var.toDouble(), double(0.0));
    QCOMPARE(var.toString(), std::string(""));
    // ObjectType ot = var.toObjectType();
    QCOMPARE(var.toBool(), false);

    var += 1; // invalid operation, the variable should stay unknown
    QCOMPARE(var.isNull(), true);

    Variant var2(3);
    var2.clear();
    QCOMPARE(var2.isNull(), true);
    QCOMPARE(var2.type(), Variant::Type::unknown);
}

void TestVariant::boolean()
{
    Variant var1(true);
    QCOMPARE(var1.isNull(), false);
    QCOMPARE(var1.type(), Variant::Type::integer);
    QCOMPARE(var1.toBool(), true);
    QCOMPARE(var1.hasNumericalType(), true);

    Variant var2(false);
    QCOMPARE(var1 && var2, false);
    QCOMPARE(var1 || var2, true);
    QCOMPARE(var1 && !var2, true);

    Variant nvar2(!var2);
    QCOMPARE(nvar2.type(), Variant::Type::integer);
}

void TestVariant::integer()
{
    Variant var(123);
    QCOMPARE(var.type(), Variant::Type::integer);
    QCOMPARE(var.hasNumericalType(), true);
    QCOMPARE(var+1, Variant(124));
    QCOMPARE(var.toInteger(), int(123));
    QCOMPARE(var.toDouble(), double(123.0));

    QCOMPARE(var++, Variant(123));
    QCOMPARE(var, Variant(124));
    QCOMPARE(++var, Variant(125));

    QCOMPARE(var--, Variant(125));
    QCOMPARE(var, Variant(124));
    QCOMPARE(--var, Variant(123));

    var*=2;
    QCOMPARE(var, Variant(246));
    var/=3;
    QCOMPARE(var, Variant(82));
    var%=4;
    QCOMPARE(var, Variant(2));
    QCOMPARE(-var, Variant(-2));

    // var == 2
    QCOMPARE(var|=1, Variant(3));
    QCOMPARE(var^=5, Variant(6));
    QCOMPARE(var&=10, Variant(2));
    QCOMPARE(var<<=2, Variant(8));
    QCOMPARE(var>>=3, Variant(1));

    var.setValue(62);
    QCOMPARE(~var, Variant(~62));
    QCOMPARE(!var, false);

    var.setValue(50);
    QCOMPARE(var+14, Variant(64));
    QCOMPARE(var-12, Variant(38));
    QCOMPARE(var-120, Variant(-70));
    QCOMPARE(var*2, Variant(100));
    QCOMPARE(var/3, Variant(16));
    QCOMPARE(var%3, Variant(2));

    var.setValue(62);
    QCOMPARE(var|1, Variant(62|1));
    QCOMPARE(var&3, Variant(62&3));
    QCOMPARE(var^32, Variant(62^32));
    QCOMPARE(var<<1, Variant(62<<1));
    QCOMPARE(var>>2, Variant(62>>2));

    var.setValue(943);
    QCOMPARE(var == 943, true);
    QCOMPARE(var != 43,  true);
    QCOMPARE(var <  10,  false);
    QCOMPARE(var <= 943, true);
    QCOMPARE(var >  2,   true);
    QCOMPARE(var >= 944, false);

    QCOMPARE(var && 0, false);
    QCOMPARE(var || false, true);
}

void TestVariant::unsignedInteger()
{
    Variant var(123ULL);
    QCOMPARE(var.type(), Variant::Type::unsignedInteger);
    QCOMPARE(var.hasNumericalType(), true);
    QCOMPARE(var+1, Variant(124));
    QCOMPARE(var.toInteger(), int(123));
    QCOMPARE(var.toDouble(), double(123.0));

    QCOMPARE(var++, Variant(123));
    QCOMPARE(var, Variant(124));
    QCOMPARE(++var, Variant(125));

    QCOMPARE(var--, Variant(125));
    QCOMPARE(var, Variant(124));
    QCOMPARE(--var, Variant(123));

    var*=2;
    QCOMPARE(var, Variant(246));
    var/=3;
    QCOMPARE(var, Variant(82));
    var%=4;
    QCOMPARE(var, Variant(2));
    QCOMPARE(-var, Variant(-2));

    // var == 2
    QCOMPARE(var|=1, Variant(3));
    QCOMPARE(var^=5, Variant(6));
    QCOMPARE(var&=10, Variant(2));
    QCOMPARE(var<<=2, Variant(8));
    QCOMPARE(var>>=3, Variant(1));

    var.setValue(62ULL);
    QCOMPARE(~var, Variant(~62));
    QCOMPARE(!var, false);

    var.setValue(50ULL);
    QCOMPARE(var+14, Variant(64));
    QCOMPARE(var-12, Variant(38));
    QCOMPARE((var-120).toInteger(), -70LL);
    QCOMPARE(var*2, Variant(100));
    QCOMPARE(var/3, Variant(16));
    QCOMPARE(var%3, Variant(2));

    var.setValue(62ULL);
    QCOMPARE(var|1, Variant(62|1));
    QCOMPARE(var&3, Variant(62&3));
    QCOMPARE(var^32, Variant(62^32));
    QCOMPARE(var<<1, Variant(62<<1));
    QCOMPARE(var>>2, Variant(62>>2));

    var.setValue(943ULL);
    QCOMPARE(var == 943, true);
    QCOMPARE(var != 43,  true);
    QCOMPARE(var <  10,  false);
    QCOMPARE(var <= 943, true);
    QCOMPARE(var >  2,   true);
    QCOMPARE(var >= 944, false);

    QCOMPARE(var && 0, false);
    QCOMPARE(var || false, true);
}

void TestVariant::floating()
{
    Variant var(123.4);
    QCOMPARE(var.type(), Variant::Type::floating);
    QCOMPARE(var.hasNumericalType(), true);
    QCOMPARE(var+1, Variant(124.4));
    QCOMPARE(var.toInteger(), int(123));

    QCOMPARE(var++, Variant(123.4));
    QCOMPARE(var, Variant(124.4));
    QCOMPARE(++var, Variant(125.4));

    QCOMPARE(var--, Variant(125.4));
    QCOMPARE(var, Variant(124.4));
    QCOMPARE(--var, Variant(123.4));

    var*=2;
    QCOMPARE(var, Variant(246.8));
    var.setValue(246.6);
    var/=3;
    QCOMPARE(var, Variant(82.2));
    QCOMPARE(-var, Variant(-82.2));


    var.setValue(2.0);
    // QCOMPARE(var|=1, Variant(3.2));
    // QCOMPARE(var^=5, Variant(6));
    // QCOMPARE(var&=10, Variant(2));
    // QCOMPARE(var<<=2, Variant(8));
    // QCOMPARE(var>>=3, Variant(1));

    var.setValue(62.0);
    // QCOMPARE(~var, Variant(~62.0));
    QCOMPARE(!var, false);

    var.setValue(50.0);
    QCOMPARE(var+14, Variant(64.0));
    QCOMPARE(var-12, Variant(38.0));
    QCOMPARE((var-120.0).toDouble(), -70.0);
    QCOMPARE(var*2, Variant(100.0));
    QCOMPARE(var/4, Variant(12.5));
    // QCOMPARE(var%3, Variant(2));

    // var.setValue(62.0);
    // QCOMPARE(var|1, Variant(62|1));
    // QCOMPARE(var&3, Variant(62&3));
    // QCOMPARE(var^32, Variant(62^32));
    // QCOMPARE(var<<1, Variant(62<<1));
    // QCOMPARE(var>>2, Variant(62>>2));

    var.setValue(943.5);
    QCOMPARE(var == 943.5, true);
    QCOMPARE(var != 43.0,  true);
    QCOMPARE(var <  10.0,  false);
    QCOMPARE(var <= 943.5, true);
    QCOMPARE(var >  2,   true);
    QCOMPARE(var >= 944.0, false);

    QCOMPARE(var && 0, false);
    QCOMPARE(var || false, true);
}

void TestVariant::string()
{
    Variant var("hello world!");
    QCOMPARE(var.type(), Variant::Type::string);
    QCOMPARE(var.hasNumericalType(), false);

    QCOMPARE(var+" test", Variant("hello world! test"));
    QCOMPARE(var+1, Variant("hello world!1"));
    QCOMPARE(var+1.2, Variant("hello world!1.2"));

    QCOMPARE(Variant("123abc").toInteger(), 0LL); // no info extracted

    QCOMPARE(!var, false);

    QCOMPARE(var == "hello world!", true);
    QCOMPARE(var != "Hello world!",  true);
    QCOMPARE(var <  "gello world!",  false);
    QCOMPARE(var <= "iello world!", true);
    QCOMPARE(var >  "a",   true);
    QCOMPARE(var >= "w", false);

    QCOMPARE(var && 0, false);
    QCOMPARE(var || false, true);
}

void TestVariant::objectType()
{
    ObjectType ot;
    Variant var(ot);
    QCOMPARE(var.type(), Variant::Type::objectType);
    QCOMPARE(var.hasNumericalType(), false);
    QCOMPARE(var.toInteger(), 0LL);

    ObjectType ot2 = var.toObjectType();
    QCOMPARE(ot2.isNull(), true);

    // a ObjectType can't be created from a variant other than an ObjectType ?
    // var.setValue(123);
    // ObjectType ot3 = var.toObjectType();
}

void TestVariant::conversion()
{
    // setValue resets the Variant (including the type)
    Variant var(1ULL);
    var.setValue(3);
    QCOMPARE(var.type(), Variant::Type::integer);

    QCOMPARE(Variant(12ULL), Variant(12LL));
    QCOMPARE(Variant(12LL), Variant(12ULL));

    Variant doubleVar(1.3), integerVar(1);
    QCOMPARE(integerVar == doubleVar, false);
    QCOMPARE(doubleVar == integerVar, false);
}
