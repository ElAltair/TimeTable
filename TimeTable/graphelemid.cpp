#include "graphelemid.h"


GraphElemId::GraphElemId(GraphElem *elem, int id)
{
    graphElem = elem;
    idElem = id;

}


GraphElem *GraphElemId::returnElem() const
{
    return graphElem;

}


int GraphElemId::returnId() const
{
    return idElem;

}
