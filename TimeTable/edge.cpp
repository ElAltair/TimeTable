#include "edge.h"
#include "graphelem.h"
#include <QPainter>
#include <QDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(GraphElem *sourseElem, GraphElem *destElem,int idstart, int idend,QString text)
{

    start = sourseElem;
    end = destElem;
    start->addEdge(this);
    end->addEdge(this);
    startId = idstart ;
    endId = idend;
    edgeIdString = text;


}

int Edge::returnStarId() const
{
    return startId;

}

int Edge::returnEndId() const
{
    return endId;

}

void Edge::setStringText(QString text)
{
    edgeIdString = text;

}
Edge::~Edge()
{

}

QRectF Edge::boundingRect() const
{

   return QRectF(startPoint, QSizeF(endPoint.x() - startPoint.x(),endPoint.y() - startPoint.y()));


}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QLineF tmpLine(mapFromItem(start, 0, 0), mapFromItem(end, 0, 0));
    QPoint centerPoint(35,35);

    qreal dx = tmpLine.dx() / tmpLine.length();
    qreal dy = tmpLine.dy() / tmpLine.length();
    //qDebug()<<dx;

    startPoint = tmpLine.p1() + QPoint(dx*35,dy*35) + centerPoint;
    endPoint = tmpLine.p2() - QPoint(dx*35,dy*35) + centerPoint;

    QLineF line(startPoint, endPoint);

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
    QPointF centerLInePoint(startPoint.x()+((endPoint.x()-startPoint.x())/2),startPoint.y()-5+((endPoint.y()-startPoint.y())/2));
   // painter->drawText(centerLInePoint,edgeIdString.setNum(edgeId));
    painter->drawText(centerLInePoint,edgeIdString);

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;


    int arrowSize = 10;
    QPointF destArrowP1 = endPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = endPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);


    painter->setBrush(Qt::black);
   // painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);


}
