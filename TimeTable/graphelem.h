#ifndef GRAPHELEM_H
#define GRAPHELEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>

class Edge;
class GraphElem : public QGraphicsItem
{
public:
    GraphElem(QString &text,QObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addEdge(Edge* edge);
    void setResultString(QString text);
    QList<Edge*> edgeList;
    QString Text;

    ~GraphElem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QString result;
};

#endif // GRAPHELEM_H
