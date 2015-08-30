#include "gantform.h"
#include "ui_gantform.h"
#include <QDebug>
#include <QGraphicsItem>

GantForm::GantForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GantForm)
{
    ui->setupUi(this);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
   // scene->setSceneRect(0,0,500,300);

}
GantForm::GantForm(QStandardItemModel* _model, QWidget *parent):
    QDialog(parent),
    ui(new Ui::GantForm)
{
    ui->setupUi(this);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
  //  scene->setSceneRect(0,0,500,300);
    model = _model;

}

void GantForm::visualize()
{

    int deltaX = 20;
    int deltaY = 40;
    int defaultSquareHeight = 40;
    int defaultSquareWidth = 40;

    QMap<int,int> map;
    for(int i = 0; i < model->rowCount(); ++i)
    {
        QModelIndex idIndex = model->index(i,0);
        QModelIndex timeIndex = model->index(i,3);
        int time = model->data(timeIndex).toInt();
        int id = model->data(idIndex).toInt();
        map.insert(id,time);

    }
    QMap<int,int>::iterator mapIterator = map.begin();


    int position = 0;
    int startObjectValueX = 0;
    int startObjectValueY = 0;
    int squareWidth = 0;
    int squareHeight = defaultSquareHeight;
    int maxHeight = map.size() * (defaultSquareHeight + deltaY);
    QGraphicsRectItem * rectItem;
    QGraphicsTextItem *textItem;
    QString stringToConvert;
    QString fullString;
    scene->addLine(deltaX,0,deltaX,500,QPen(Qt::DotLine));

    while( mapIterator != map.end())
    {


        startObjectValueX = defaultSquareWidth*(-distances.at(position + 1));
        startObjectValueY = position * (defaultSquareHeight + deltaY);
        if(mapIterator.value() == 0)
            squareWidth = defaultSquareHeight;
        else
            squareWidth = mapIterator.value()*defaultSquareWidth;

    rectItem = new QGraphicsRectItem(deltaX + startObjectValueX ,deltaY + startObjectValueY ,squareWidth,squareHeight);
    rectItem->setBrush(QBrush(Qt::blue));
    scene->addItem(rectItem);
    scene->addLine(deltaX + startObjectValueX + squareWidth,0,deltaX + startObjectValueX + squareWidth,maxHeight,QPen(Qt::DotLine));
    scene->addLine(0,deltaY + startObjectValueY + squareHeight,900,deltaY + startObjectValueY + squareHeight,QPen(Qt::DotLine));
    fullString += "id = ";
    stringToConvert.setNum(mapIterator.key());
    fullString += stringToConvert;
    textItem = new QGraphicsTextItem(fullString);
    fullString.clear();
    textItem->setPos(deltaX + startObjectValueX,deltaY + startObjectValueY - deltaY/2);
    scene->addItem(textItem);
    fullString += "time = ";
    stringToConvert.setNum(mapIterator.value());
    fullString += stringToConvert;
    textItem = new QGraphicsTextItem(fullString);
    textItem->setPos(deltaX + startObjectValueX,deltaY + startObjectValueY + squareHeight);
    scene->addItem(textItem);
    fullString.clear();

    ++mapIterator;
    ++position;
    }




}

void GantForm::setDistancesVector(QVector<int> dist)
{
    distances = dist;

}

void GantForm::setVerticesVector(QVector<int> vert)
{
    vertices = vert;
}


GantForm::~GantForm()
{
    delete ui;
}

void GantForm::on_okButton_clicked()
{
    accept();
    scene->clear();
}

void GantForm::on_GantForm_rejected()
{
    scene->clear();
}
