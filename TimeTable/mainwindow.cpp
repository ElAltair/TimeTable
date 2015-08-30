#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edge.h"
#include <QGraphicsRectItem>
#include<QDebug>
#include <QtMath>
#include <QPropertyAnimation>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    scene = new  QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    edgeCount = 1;
    hasCircle = false;
    ui->gantsPushButton->setEnabled(false);




/*

    QVector<int>vector;
    for( int i = 0; i < 5; ++i)
        vector.push_back(i);
    QStringList list;
    QString tmp,tmp2;

    tmp.setNum(vector.at(0)+1);
    for( int i = 0; i < 5; ++i)
    {
      tmp.setNum(vector.at(i)+1);
     list<<tmp<<"Standart Work"+tmp<<"No description"+tmp<<tmp2.setNum(10)<<tmp;
    }
*/


    QStringList headLabels;
    headLabels<<"Id"<<"Name"<<"Description"<<"Working time"<<"Dependacy";
    myModel = new QStandardItemModel(1,5,this);
    myModel->setHorizontalHeaderLabels(headLabels);
    ui->tableView->setModel(myModel);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    editForm = new TableEditForm();
    editForm->setModel(myModel);
    editForm->setParent(this,Qt::Window);
    myModel->removeRow(0);




/*
    QStandardItem* item = 0;
    int jpos = 0, ipos = 0;
    for( int i = 0; i < list.length(); ++i)
    {
        if( i%5 == 0 && i!=0)
        {
            jpos++;
            ipos = 0;
        }
        item = new QStandardItem(list.at(i));
        myModel->setItem(jpos,ipos,item);
        ipos++;
       ui->textBrowser->clear();
      //  updateMatrix();
       // printMatrix();

    }

*/



     connect(myModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateMatrix()));
     connect(myModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateGraphics(QModelIndex)));
     gantForm = new GantForm(myModel);
     updateMatrix();

/*
     QString tmpI;
     GraphElem* elemRec = 0;
     Edge * elemEdge = 0;
     float angle = 360 / matrixCols;
     angle = qDegreesToRadians(angle);
     float radius = 200;
    for( int i = 0; i < matrixCols; ++i)
    {
        tmpI.setNum(i+1);
        elemRec = new GraphElem(tmpI,this);

        elemRec->setPos(cos(angle*i)*radius,sin(angle*i)*radius);
        elemRec->setFlag(QGraphicsItem::ItemIsMovable);
        scene->addItem(elemRec);
        graphElemIdList.push_back(new GraphElemId(elemRec,i+1));

      //  if(i !=0)
      // elemEdge = new Edge(graphElemIdList.at(i)->returnElem(),graphElemIdList.at(i-1)->returnElem());
      // scene->addItem(elemEdge);
       //rects.push_back(scene->addRect(i*100,0+i*100,100,100));
    }


*/



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    TableInsertForm insertForm(myModel);
    if(insertForm.exec()==QDialog::Accepted)
    {
        QString newElement;
         QPoint point = insertForm.insertDataToTheModel(*myModel);
        updateMatrix();
        addGeometry(point);
        scene->update();
    }


}
void MainWindow::on_deleteButton_clicked()
{
   QModelIndex index = ui->tableView->currentIndex();
   QModelIndex idIndex = myModel->index(index.row(),0);

   int row = index.row();
   int data = myModel->data(idIndex).toInt();
   qDebug()<<"data = "<<data;
   QVector<int> tmpvec;
   for( int i = 0; i < myModel->rowCount(); ++i)
   {
       QModelIndex tmpIndex = myModel->index(i,0);
       QModelIndex shouldShangeIndex = myModel->index(i,4);

       if(myModel->data(shouldShangeIndex).toInt() == data)
           myModel->setData(shouldShangeIndex,0);

       if(myModel->data(tmpIndex).toInt() == data)
       {
           tmpvec.push_back(i);
           qDebug()<<i<<" ";

       }
   }
   /*
   if(row > myModel->rowCount())
       myModel->removeRow(myModel->rowCount()-1);
   if(row == -1)
       row = myModel->rowCount()-1;
       */

   QModelIndex tmpIndex = myModel->index(row,0);
   int removeId = myModel->data(tmpIndex).toInt();

   for(int i = 0; i<tmpvec.size(); ++i)
   {
   myModel->removeRow(tmpvec.at(i)-i);

   }

   updateMatrix();
   deleteGeometry(removeId);
   scene->update();
}

void MainWindow::on_cleartButton_clicked()
{
  int rowCount = myModel->rowCount();
  myModel->removeRows(0,rowCount);
  updateMatrix();
  scene->clear();
  /*
  QList<Edge*>::iterator edgeIterator = EdgeSceneList.begin();
  while( edgeIterator != EdgeSceneList.end())
  {
      delete *edgeIterator;
      ++edgeIterator;
  }
  */
  EdgeSceneList.clear();
  graphElemIdList.clear();

  scene->update();


}

void MainWindow::updateMatrix()
{

    int maxValue = 0, value = 0;
    for( int i = 0; i < myModel->rowCount(); ++i)
    {
        QModelIndex index = myModel->index(i,4);
        QModelIndex idIndex = myModel->index(i,0);
        if((value = myModel->data(index).toInt())> maxValue)
            maxValue = value;
        if((value = myModel->data(idIndex).toInt()) > maxValue)
           maxValue = value;
    }


    //qDebug()<<"Max value = "<<maxValue<<"\n";

    matrixRows = myModel->rowCount();
    matrixCols = matrixRows;
    graphMatrix = new int*[matrixRows];
    timeMatrix = new int[matrixRows];
    vectorOfHeadValue = new int[matrixRows];
    for( int i = 0; i < matrixRows; ++i)
    {
        graphMatrix[i] = new int[matrixRows];
    }
    for(int i = 0; i < matrixRows; ++i)
    {
        for(int j = 0; j < matrixCols; ++j)
        {
             graphMatrix[i][j]=0;
        }
        timeMatrix[i] = 0;
        vectorOfHeadValue[i]=0;
    }
    fillMatrix();
    ui->textBrowser->clear();
    printMatrix();
}

void MainWindow::updateGraphics(QModelIndex changed)
{
    if( changed.column() == 3)
    {
        QModelIndex itemIndex = myModel->index(changed.row(),0);
        int itemData = myModel->data(itemIndex).toInt();
        QModelIndex dependacyIndex = myModel->index(changed.row(),4);
        QList<Edge*>::const_iterator edgeIterator = EdgeSceneList.cbegin();
        QString tmpString;

        while ( edgeIterator != EdgeSceneList.cend())
        {
            if( ((*edgeIterator)->returnStarId() == itemData))
            {
                (*edgeIterator)->setStringText(tmpString.setNum(-myModel->data(changed).toInt()));
            }
            ++edgeIterator;
        }
    }
    else if( changed.column() == 4)
    {
        QModelIndex itemIndex = myModel->index(changed.row(),0);
        QModelIndex timeIndex = myModel->index(changed.row(),3);
        int itemData = myModel->data(itemIndex).toInt();
        int destData = myModel->data(changed).toInt();
        QString time = myModel->data(timeIndex).toString();
        QList<Edge*>::iterator edgeIterator = EdgeSceneList.begin();

        while ( edgeIterator != EdgeSceneList.end())
        {
            if( (*edgeIterator)->returnEndId() == itemData)
            {
                scene->removeItem(*edgeIterator);
                edgeIterator = EdgeSceneList.erase(edgeIterator);

            }
            else
            ++edgeIterator;
        }
        if(destData !=0)
        {
            GraphElem * sourceElem = returnGraphElemById(itemData);
            GraphElem * destElem = returnGraphElemById(destData);
            EdgeSceneList.push_back(new Edge(destElem,sourceElem,destData,itemData,time));
            scene->addItem(EdgeSceneList.last());
        }
        ui->graphicsView->repaint();

    }

}

void MainWindow::printMatrix() const
{
   QString longString, tmp;
   QString spaces;
   spaces+="              ";
   tmp.setNum(matrixRows);
   longString = "Matrix row = " + tmp + "\n";
  // qDebug()<<longString;
   tmp.setNum(matrixCols);
   longString+="Matrix cols = " + tmp + "\n";

   longString +="Edges /\ Vertices";
   longString+=spaces;
           longString += "Time\n";
   longString+=spaces;
   longString+= "  ";
   for(int i = 0; i < matrixCols; ++i)
   {
       tmp.setNum(vectorOfHeadValue[i]);
       longString+=tmp+"   ";

   }
   longString+="\n";



   for(int i = 0; i < matrixRows; ++i)
   {
          tmp.setNum(i+1);
          longString+=tmp+spaces;
       for(int j = 0; j < matrixCols; ++j)
       {
          //tmp.number(graphMatrix[i][j],'g',2);
          tmp.setNum(graphMatrix[i][j]);
          if(graphMatrix[i][j] < 0)
          longString+=tmp + "  ";
          else
          longString+=" " + tmp + "  ";

       }
       tmp.setNum(timeMatrix[i]);
       longString += spaces;
       longString +=tmp;
   longString += "\n";
   }

   ui->textBrowser->setText(longString);


}

void MainWindow::fillMatrix()
{
    int value = 0;
    int startId = myModel->data(QModelIndex(myModel->index(0,0))).toInt();
    vectorOfHeadValue[0]=startId;
    for( int i = 0; i < myModel->rowCount(); ++i)
    {
        QModelIndex idIndex = myModel->index(i,0);
        QModelIndex index = myModel->index(i,4);
        QModelIndex timeIndex = myModel->index(i,3);
        int id = myModel->data(idIndex).toInt();
        if((value = myModel->data(index).toInt())==0)
        {
        vectorOfHeadValue[i]= id ;
        timeMatrix[i]=0;
            continue;
        }
        else
        {
            graphMatrix[i][value-startId] = -1;
            if(id-startId < 0)
            {

            }
            else
            graphMatrix[i][id-startId] = 1;

            for( int j = 0; j< myModel->rowCount(); ++j)
            {
                QModelIndex findIdIndex = myModel->index(j,0);
                if(myModel->data(findIdIndex).toInt() == value)
                {
                    QModelIndex findTimeIndex = myModel->index(j,3);
                    timeMatrix[i] = -myModel->data(findTimeIndex).toInt();
                    break;
                }

            }


        }
        vectorOfHeadValue[i]= id ;


    }



}
void MainWindow::deleteGeometry(int deletePos)
{
    if(hasCircle)
    {
        hasCircle = false;
        ui->optimizeButton->setEnabled(true);
    }

    QList<Edge*>::iterator edgeIterator = EdgeSceneList.begin();
    int i = 0;


    while( edgeIterator != EdgeSceneList.end())
    {


        if(((*edgeIterator)->returnStarId() == deletePos) || ((*edgeIterator)->returnEndId() == deletePos))
        {
            scene->removeItem(*edgeIterator);
            edgeIterator = EdgeSceneList.erase(edgeIterator);
        }
        else
        {
        ++edgeIterator;
        }
    }




        int deleteId = 0;
        i = 0;
        QList<GraphElemId*>::iterator elemIter = graphElemIdList.begin();
        while( elemIter != graphElemIdList.end())
        {
            if( (*elemIter)->returnId() == deletePos)
            {
                scene->removeItem((*elemIter)->returnElem());
                deleteId = i;

            }
            i++;
            ++elemIter;
        }
       graphElemIdList.removeAt(deleteId);
       scene->update();


    resetPos();
    ui->graphicsView->repaint();
}
void MainWindow::addGeometry(QPoint point)
{
    int id = point.x();
    int dependacy = point.y();
    QString name;
    name.setNum(id);
    Edge *tmpEdge = 0;
    GraphElem* tmpElem;
    QList<GraphElemId*>::const_iterator elemListIter = graphElemIdList.cbegin();
    if((tmpElem = returnGraphElemById(id)) != NULL)
    {

        name.setNum(timeMatrix[id-1]);
        GraphElem* destElem = returnGraphElemById(dependacy);
        tmpEdge = new Edge(destElem,tmpElem,dependacy,id,name);
        EdgeSceneList.push_back(tmpEdge);
        edgeCount++;
        scene->addItem(tmpEdge);


    }
    else
    {
            graphElemIdList.push_back(new GraphElemId(new GraphElem(name,this),id));
            graphElemIdList.last()->returnElem()->setFlag(QGraphicsItem::ItemIsMovable);
            scene->addItem(graphElemIdList.last()->returnElem());
            resetPos();
            tmpElem = returnGraphElemById(dependacy);
            if(tmpElem == NULL)
            {

            }
            else
            {
            name.setNum(timeMatrix[id-1]);
            tmpEdge = new Edge(tmpElem,graphElemIdList.last()->returnElem(),dependacy,id,name);
            EdgeSceneList.push_back(tmpEdge);
            edgeCount++;
            scene->addItem(tmpEdge);
            }

    }
    scene->update();
    ui->graphicsView->repaint();

}

void MainWindow::resetPos()
{


    float allElements =  graphElemIdList.count();
    float angle = qDegreesToRadians(360/allElements);
    float radius = 200;
    QList<GraphElemId*>::const_iterator graphElemIterator = graphElemIdList.cbegin();
    int i = 0;
    while( graphElemIterator != graphElemIdList.cend())
    {


        /*
    QPropertyAnimation *animation = new QPropertyAnimation((*graphElemIterator)->returnElem(),"pos");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setEndValue(QRectF(i*radius,i*radius));
  //  animation->setEndValue(QRectf(cos(angle*i)*radius,sin(angle*i)*radiu));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    */

       //(*graphElemIterator)->returnElem()->setPos(i*radius,i*radius);
       (*graphElemIterator)->returnElem()->setPos(cos(angle*i)*radius,sin(angle*i)*radius);
       // qDebug()<< "X = "<<cos(angle*i)*radius << "Y = "<<sin(angle*i)*radius;


        ++graphElemIterator;
        i++;
    }
    ui->graphicsView->repaint();

}

void MainWindow::printAllLists()
{

    int i = 0;
    qDebug()<<" edge list\n";
    QList<Edge*>::const_iterator edgeIterator = EdgeSceneList.cbegin();
    while( edgeIterator != EdgeSceneList.cend())
    {
        qDebug()<< "i = " << i <<" start = "<<(*edgeIterator)->returnStarId() << "end  = " << (*edgeIterator)->returnEndId();
        ++edgeIterator;
        ++i;
    }
    qDebug()<<"\n";

    i = 0;
    qDebug()<<" item list \n";
    QList<GraphElemId*>::const_iterator itemIterator = graphElemIdList.cbegin();
    while( itemIterator != graphElemIdList.cend())
    {
        qDebug()<< "i = " << i << "id = " << (*itemIterator)->returnId();
        ++itemIterator;
        ++i;
    }
    qDebug() << "\n";

    i = 0;
    QList<QGraphicsItem*> sceneList;
    qDebug()<<"\n";
    qDebug()<<"scene list\n";
    sceneList = scene->items();
    QList<QGraphicsItem*>::const_iterator sceneIterator = sceneList.cbegin();
    while( sceneIterator != sceneList.cend())
    {
        qDebug()<<"i = "<< i <<"\n";
        ++sceneIterator;
        ++i;
    }

}

QVector<edge> MainWindow::form_vector(QVector<int> vertices)
{
    QVector<edge> edges;
    int startId = vectorOfHeadValue[0];
    int a = 0, b = 0;
    for( int i = 0; i < matrixRows; ++i)
    {
        for(int j = 0; j < matrixCols; ++j)
        {
            if( graphMatrix[i][j] == 1)
            {
                b = j+startId;
            }
            else if( graphMatrix[i][j] == -1)
            {
                a = j+startId;
            }
            else
            {
                continue;
            }
        }
        if( a == 0 || b == 0)
            continue;
        edges.push_back(edge(a,b,timeMatrix[i]));


    }


    for(int i = 0; i < vertices.size(); ++i)
    {
        edges.push_back(edge(0,vertices.at(i),0));
    }
    return edges;


}
    int zeroEqual(int num, int difference)
    {
        if(num == 0)
            return difference;
        else
            return num;

    }

void MainWindow::Belman_Ford(int verticesNumber,QVector<int> vertices,int startNumber = 0)
{

    QVector<edge> edges;
    edges = form_vector(vertices);

    QVector<int> distances;
    for(int i = 0; i < verticesNumber; ++i)
    {
        if(i == startNumber)
            distances.push_back(0);
        else
            distances.push_back(INT_MAX);
    }


    int x = -1;
    int difference =vectorOfHeadValue[0] - 1;
    for (int i = 0; i < verticesNumber; ++i)
    {
        x = -1;



        for( int j = 0; j < edges.size(); ++j)
        {
            if ( distances[zeroEqual(edges[j].a,difference) - difference] < INT_MAX)
            {
                if(distances[zeroEqual(edges[j].b,difference) - difference] > distances[zeroEqual(edges[j].a,difference) - difference] + edges[j].cost)
                {
                    distances[zeroEqual(edges[j].b,difference) - difference] = distances[zeroEqual(edges[j].a,difference) - difference] + edges[j].cost;
                    x = edges[j].b;
                }

            }
        }

    }
    if( x != -1)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error message"," This graph has circle! \n Please change your graph before you can try again");
        messageBox.setFixedSize(500,500);
        hasCircle = true;
        ui->optimizeButton->setEnabled(false);
    }


    QList<GraphElemId*>::iterator elemIterator = graphElemIdList.begin();
    QString tempResultString;
    int i = 1;
    while( elemIterator != graphElemIdList.end())
    {

        if((*elemIterator)->returnId() == i + difference )
        {
            tempResultString.setNum(distances.at( i ));
            (*elemIterator)->returnElem()->setResultString(tempResultString);
            (*elemIterator)->returnElem()->update();
        }
        ++elemIterator;
        ++i;
    }
    ui->graphicsView->repaint();

    if(!hasCircle)
    ui->gantsPushButton->setEnabled(true);

    gantForm->setDistancesVector(distances);
    gantForm->setVerticesVector(vertices);





}

GraphElem *MainWindow::returnGraphElemById(int id)
{
    QList<GraphElemId*>::const_iterator listIterator = graphElemIdList.cbegin();

    while( listIterator != graphElemIdList.cend())
    {
        if( (*listIterator)->returnId() ==  id)
            return (*listIterator)->returnElem();

        ++listIterator;
    }
    return NULL;

}

/*
void MainWindow::updateGraphics(int flag,int deleteId)
{

    qDebug()<<"flag = "<<flag<<"Delete id = "<<deleteId<<"\n";
    qDebug()<<currentGraphicsCount<<"\n";
    if(flag==0)
    {
       // rects.push_back(scene->addRect(100,100,100,100));
      //  rects.last()->setFlag(QGraphicsItem::ItemIsMovable);
    }
    else if(flag == 1)
    {
        if(currentGraphicsCount >= 0)
        {
            scene->removeItem(graphElemList.at(deleteId));
            graphElemList.removeAt(deleteId);
        }
    }
    else if(flag == 2)
    {
            scene->clear();
    }




}

*/

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    editForm->mapper->setCurrentModelIndex(index);
    editForm->prepareData(myModel,index);
   editForm->show();
}

void MainWindow::on_optimizeButton_clicked()
{
   QVector<int> vertices;
   for(int i = 0; i < myModel->rowCount(); ++i)
   {
       QModelIndex idModelIndex = myModel->index(i,0);
       int data = myModel->data(idModelIndex).toInt();

       if(vertices.size() == 0)
       {
           vertices.push_back(data);
           continue;
       }
       else
       {
           bool isExist = false;
           for(int j = 0; j < vertices.size(); ++j)
           {
               if( vertices.at(j) == data)
               {
                   isExist =true;
                   break;
               }
           }
           if(!isExist)
               vertices.push_back(data);
       }
   }
   Belman_Ford(vertices.size()+1,vertices,0);
}

void MainWindow::on_gantsPushButton_clicked()
{
    gantForm->setModal(false);
    gantForm->visualize();
    gantForm->exec();

}
