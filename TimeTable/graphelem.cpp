#include "graphelem.h"
#include "edge.h"

GraphElem::GraphElem(QString&text,QObject* parent)
{
    Text = text;
    result = "nan";
}

QRectF GraphElem::boundingRect() const
{
    return QRectF(0,0,70,70);

}

void GraphElem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    painter->drawEllipse(0,0,70,70);
    painter->drawText(30,40,result);
    painter->drawText(30,90,Text);


}
void GraphElem::setResultString(QString text)
{
    result = text;
}

void GraphElem::addEdge(Edge *edge)
{
    edgeList<<edge;

}

GraphElem::~GraphElem()
{

}

void GraphElem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void GraphElem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update(boundingRect());
    QGraphicsItem::mouseReleaseEvent(event);

}

