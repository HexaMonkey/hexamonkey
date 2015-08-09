#include "test_variable.h"

#include <vector>

#include "core/variable/variable.h"
#include "core/variable/variablecollector.h"



class CollectorTestVariableImplementation : public VariableImplementation
{
public:
    CollectorTestVariableImplementation(VariableCollector& collector, int& collected, int i)
        : VariableImplementation(collector),
          collected(collected),
          i(i)
    {
    }

    ~CollectorTestVariableImplementation() {
        collected = 1;
    }

    void collect(const std::function<void(VariableMemory&)>& addAccessible) {
        for (auto& reference : references) {
            addAccessible(reference);
        }
    }

    void addReference(const Variable& reference) {
        references.push_back(reference);
    }
private:
    int& collected;
    int i;
    std::vector<VariableMemory> references;
};

void TestVariable::testCollector()
{
    VariableCollector collector;

    std::vector<CollectorTestVariableImplementation*> implementations;
    std::vector<Variable> toCollect;
    std::vector<int> collected =         {0,0,0,0,0,0,0,0,0,0};
    std::vector<int> shouldBeCollected = {0,0,0,0,0,1,1,1,1,1};

    for (int i = 0; i < 10; ++i) {
        auto implementation = new CollectorTestVariableImplementation(collector, collected[i], i);
        implementations.push_back(implementation);
        toCollect.emplace_back(implementation, true);
    }


    implementations[3]->addReference(toCollect[4]);
    implementations[4]->addReference(toCollect[2]);
    implementations[7]->addReference(toCollect[8]);
    implementations[8]->addReference(toCollect[9]);
    implementations[9]->addReference(toCollect[7]);

    toCollect.emplace_back(toCollect[3]);


    toCollect[9]  = Variable();
    toCollect[5]  = Variable();
    toCollect[10] = Variable();
    toCollect[4]  = Variable();
    toCollect[7]  = Variable();
    toCollect[6]  = Variable();
    toCollect[8]  = Variable();

    collector.collect();

    QCOMPARE(collected, shouldBeCollected);

    collector.collect();

    QCOMPARE(collected, shouldBeCollected);

    for (int i = 0; i < 10; ++i) {
        if (!collected[i]) {
            shouldBeCollected[i] = 1;
            toCollect[i] = Variable();
        }
    }
    collector.collect();
    QCOMPARE(collected, shouldBeCollected);
}
