#ifndef FROMFILEMODULE_H
#define FROMFILEMODULE_H

#include <memory>

#include "mapmodule.h"
#include "program.h"

class Interpreter;

class FromFileModule : public MapModule
{
public:
    FromFileModule(const std::string& path, Interpreter* interpreter);

protected:
    void addFormatDetection(StandardFormatDetector::Adder& formatAdder) override;
    void requestImportations(std::vector<std::string>& formatRequested) override;
    bool doLoad() override;
    int64_t doGetFixedSize(const ObjectType &type, const Module &module) const override;

private:  
    bool loadProgram(const std::string path);

    void loadFormatDetections(Program& formatDetections, StandardFormatDetector::Adder& formatAdder);
    void loadImports(Program& imports, std::vector<std::string>& formatRequested);

    void loadTemplates(Program &classDeclarations);
    void loadExtensions(Program &classDeclarations);
    void loadSpecifications(Program &classDeclarations);
    void addParsers(Program &classDeclarations);

    bool sizeDependency(const std::string& name) const;
    Program::const_iterator headerEnd(const std::string& name) const;
#if 0
    void guessSizes(Program &classDeclarations);
#endif
    Interpreter& interpreter() const;

    std::unique_ptr<Interpreter> _interpreter;
    std::map<std::string, Program> _definitions;
    mutable std::map<std::string, int64_t> _fixedSizes;
    mutable std::map<std::string, bool> _sizeDependency;
    mutable std::map<std::string, Program::const_iterator> _headerEnd;
    std::set<std::string> _nonApexTemplates;

    bool programLoaded;
};

#endif // FROMFILEMODULE_H
