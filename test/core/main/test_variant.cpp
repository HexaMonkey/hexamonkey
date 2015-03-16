
#include "test_variant.h"

#include <unordered_map>

#include "core/variant.h"
#include "core/objecttype.h"

void TestVariant::unknown()
{
    Variant var;
    QCOMPARE(var.isValueless(), true);
    QCOMPARE(var.type(), Variant::undefinedType);
    QCOMPARE(var.hasNumericalType(), false);

    QCOMPARE(var.toInteger(), 0LL);
    QCOMPARE(var.toUnsignedInteger(), 0ULL);
    QCOMPARE(var.toDouble(), double(0.0));
    QCOMPARE(var.toString(), std::string(""));
    // ObjectType ot = var.toObjectType();
    QCOMPARE(var.toBool(), false);

    var += 1; // invalid operation, the variable should stay unknown
    QCOMPARE(var.isValueless(), true);

    Variant var2(3);
    var2.clear();
    QCOMPARE(var2.isValueless(), true);
    QCOMPARE(var2.type(), Variant::undefinedType);
}

void TestVariant::boolean()
{
    Variant var1(true);
    QCOMPARE(var1.isNull(), false);
    QCOMPARE(var1.type(), Variant::integerType);
    QCOMPARE(var1.toBool(), true);
    QCOMPARE(var1.hasNumericalType(), true);

    Variant var2(false);
    QCOMPARE(var1 && var2, false);
    QCOMPARE(var1 || var2, true);
    QCOMPARE(var1 && !var2, true);

    Variant nvar2(!var2);
    QCOMPARE(nvar2.type(), Variant::integerType);

    Variant n(1000);
    QCOMPARE(n.toBool(), true);
}

void TestVariant::integer()
{
    Variant var(123);
    QCOMPARE(var.type(), Variant::integerType);
    QCOMPARE(var.hasNumericalType(), true);
    QCOMPARE(var+1, Variant(124));
    QCOMPARE(var.toInteger(), 123LL);
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
    QCOMPARE(var.type(), Variant::unsignedIntegerType);
    QCOMPARE(var.hasNumericalType(), true);
    QCOMPARE(var+1, Variant(124));
    QCOMPARE(var.toInteger(), 123LL);
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
    QCOMPARE(var.type(), Variant::floatingType);
    QCOMPARE(var.hasNumericalType(), true);
    QCOMPARE(var+1, Variant(124.4));
    QCOMPARE(var.toInteger(), 123LL);

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
    QCOMPARE(var.type(), Variant::stringType);
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
    QCOMPARE(var.type(), Variant::objectType);
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
    QCOMPARE(var.type(), Variant::integerType);

    QCOMPARE(Variant(12ULL), Variant(12LL));
    QCOMPARE(Variant(12LL), Variant(12ULL));

    Variant doubleVar(1.3), integerVar(1);
    QCOMPARE(integerVar == doubleVar, false);
    QCOMPARE(doubleVar == integerVar, false);
}

void TestVariant::unordered_map()
{
    std::unordered_map<Variant, Variant> hash_var;

    Variant key1("a key"), key2("another key"), val1(0), val2(0.3f);
    hash_var[key1] = val1;
    hash_var[key2] = val2;

    QCOMPARE(hash_var[key1], val1);
    QCOMPARE(hash_var[key2], val2);

    // test that the key is defined by the content of a variant, not the object itself
    Variant key1_bis("a key");
    QCOMPARE(hash_var[key1_bis], hash_var[key1]);

    key1_bis = key2;
    // hash_var[key1_bis] should now key2 value
    QCOMPARE(hash_var[key1_bis], hash_var[key2]);

    // test change of a value of the map
    hash_var[key1] = "new value";
    QCOMPARE(hash_var[key1], Variant("new value"));

    // test difference between keys "1" and 1
    Variant key_str("1"), key_int(1);
    hash_var[key_str] = "1 string";
    hash_var[key_int] = "1 int";
    QCOMPARE(hash_var[key_str] != hash_var[key_int], true);

    std::unordered_map<Variant,Variant>::size_type hash_size(4);
    QCOMPARE(hash_var.size(), hash_size);
}
