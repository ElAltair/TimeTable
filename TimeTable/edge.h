#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class GraphElem;
class Edge : public QGraphicsItem
{
public:
    Edge(GraphElem* sourseElem, GraphElem* destElem, int idstart, int idend,QString text);
    int returnStarId(void)const;
    int returnEndId(void)const;
    void setStringText(QString text);
    ~Edge();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
   // void mousePressEvent(QGraphicsSceneMouseEvent *event);
   // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    GraphElem * start, *end;
    QPointF startPoint;
    QPointF endPoint;
    QString edgeIdString;
    int startId;
    int endId;

};

#endif // EDGE_H
