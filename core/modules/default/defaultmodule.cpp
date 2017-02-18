//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <unordered_set>

#include "core/modules/default/defaultmodule.h"


#include "core/modules/default/filetypetemplate.h"
#include "core/modules/default/integertypetemplate.h"
#include "core/modules/default/arraytypetemplate.h"
#include "core/modules/default/tupletypetemplate.h"
#include "core/modules/default/datatypetemplate.h"
#include "core/modules/default/floattypetemplate.h"
#include "core/modules/default/stringtypetemplate.h"
#include "core/modules/default/enumtypetemplate.h"
#include "core/modules/default/structtypetemplate.h"

#include "core/modules/default/defaultmethods.h"



bool DefaultModule::doLoad()
{
    addTemplate(new FileTypeTemplate);
    addTemplate(new ArrayTypeTemplate);
    addTemplate(new TupleTypeTemplate);
    addTemplate(new DataTypeTemplate);
    addTemplate(new StructTypeTemplate);
    addTemplate(new EnumTypeTemplate);
    addTemplate(new IntegerTypeTemplate);
    addTemplate(new UIntegerTypeTemplate);
    addTemplate(new ByteTypeTemplate);
    addTemplate(new BitsetTypeTemplate);
    addTemplate(new UuidTypeTemplate);
    addTemplate(new FloatTypeTemplate);
    addTemplate(new DoubleTypeTemplate);
    addTemplate(new FixedFloatTypeTemplate);
    addTemplate(new StringTypeTemplate);
    addTemplate(new WStringTypeTemplate);

    addMethod("SizeOf", new SizeOfMethod);
    addMethod("Format", new FormatStringMethod);
    addMethod("Log", new LogMethod(LogLevel::Info));
    addMethod("Warning", new LogMethod(LogLevel::Warning));
    addMethod("Error", new LogMethod(LogLevel::Error));
    addMethod("FormatDate", new FormatDateMethod);
    addMethod("FormatDuration", new FormatDurationMethod);
    addMethod("ToInt", new ToIntMethod);
    addMethod("ToUInt", new ToUIntMethod);
    addMethod("ToFloat", new ToUIntMethod);
    addMethod("FromAscii", new FromAsciiMethod);
    addMethod("ToAscii", new ToAsciiMethod);
    addMethod("ToUpper", new ToUpperMethod);
    addMethod("ToLower", new ToLowerMethod);
    addMethod("PopCount", new PopCountMethod);
    addMethod("Substring", new SubstringMethod);

    return true;
}
