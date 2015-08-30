#ifndef GRAPHELEMID_H
#define GRAPHELEMID_H
#include "graphelem.h"
class GraphElemId
{
public:

    GraphElemId(GraphElem * elem, int id);
    GraphElem* returnElem()const;
    int returnId()const;

private:

    GraphElem* graphElem;
    int idElem;

};
#endif // GRAPHELEMID_H

