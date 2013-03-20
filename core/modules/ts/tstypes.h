#ifndef TSTYPES_H
#define TSTYPES_H

#include "objecttypetemplate.h"
#include "objecttype.h"

/*!
 * \brief tsTypes
 */
namespace tsTypes
{
    const ObjectTypeTemplate tsFile("TsFile");
    const ObjectTypeTemplate transport_packet("transport_packet");
    const ObjectTypeTemplate junk("junk");
    const ObjectTypeTemplate adaptation_field("adaptation_field");
    const ObjectTypeTemplate bslbf("bslbf", {"size"});
    const ObjectTypeTemplate uimsbf("uimsbf", {"size"});
    const ObjectTypeTemplate tcimsbf("tcimsbf", {"size"});
}

#endif // TSTYPES_H
