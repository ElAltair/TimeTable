#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QVector>
#include <QGraphicsScene>
#include <QDataWidgetMapper>
#include "delegate.h"
#include "graphelem.h"
#include "graphelemid.h"
#include "tableeditform.h"
#include "tableinsertform.h"
#include "gantform.h"
namespace Ui {
class MainWindow;
}
struct edge {
    int a, b, cost;
    edge(int _a,int _b,int _cost):a(_a),b(_b),cost(_cost){}
    edge():a(0),b(0),cost(0){}
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_cleartButton_clicked();

    void updateMatrix(void);

    void updateGraphics(QModelIndex changed);


    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_optimizeButton_clicked();

    void on_gantsPushButton_clicked();

private:
    void initMatrix();
    void printMatrix()const;
    void fillMatrix();
    void deleteGeometry(int deletePos);
    void addGeometry(QPoint point);
    void resetPos(void);
    void printAllLists(void);

    void Belman_Ford(int verticesNumber, QVector<int> vertices, int startNumber);
    QVector<edge> form_vector(QVector<int> vertices);


    GraphElem* returnGraphElemById( int id);


    Ui::MainWindow *ui;
    QStandardItemModel* myModel;
    QGraphicsScene * scene;
    QList<GraphElemId*> graphElemIdList;
    QList<Edge*> EdgeSceneList;
    QDataWidgetMapper * mapper;
    TableEditForm * editForm;
    GantForm * gantForm;
    int ** graphMatrix;
    int * timeMatrix;
    int * vectorOfHeadValue;
    int matrixRows;
    int matrixCols;
    int edgeCount;
    bool hasCircle;

};

#endif // MAINWINDOW_H
