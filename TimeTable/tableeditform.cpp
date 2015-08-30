#include "tableeditform.h"
#include "ui_tableeditform.h"
#include <QDebug>

TableEditForm::TableEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableEditForm)
{
    ui->setupUi(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    editIndex = QModelIndex();


}

void TableEditForm::setModel(QAbstractItemModel *model)
{
   mapper->setModel(model);
   mapper->addMapping(ui->nameLineEdit,1);
   mapper->addMapping(ui->descriptionPlainTextEdit,2);
   mapper->addMapping(ui->workingtimeSpinBox,3);
   mapper->addMapping(ui->comboBox,4);
}

TableEditForm::~TableEditForm()
{
    delete ui;
}

void TableEditForm::on_submitButton_clicked()
{
   mapper->submit();
   close();

}

void TableEditForm::on_cancelButton_clicked()
{
   close();
}

void TableEditForm::setIndex(QModelIndex &index)
{
    editIndex = index;
}

void TableEditForm::prepareData(const QAbstractItemModel *model,const QModelIndex& index)
{

    ui->comboBox->clear();
    ui->comboBox->addItem("0");
    QMap<int,QString> tmpMap;
    QString tmp;
    for(int i = 0; i < model->rowCount(); ++i)
    {

        if(i != index.row())
        {
        QModelIndex tmpIndex = model->index(i,0);
        tmpMap.insert(model->data(tmpIndex).toInt(),model->data(tmpIndex).toString());
        }
     }
    QMap<int,QString>::const_iterator mapIter = tmpMap.cbegin();
    while( mapIter != tmpMap.cend())
    {
        ui->comboBox->addItem(mapIter.value());
        ++mapIter;
    }
/*
    QString tmp;
    qDebug()<<index.row();
    for(int i = 0; i < model->rowCount(); ++i)
    {

        QModelIndex tmpIndex = model->index(i,0);
        int dependacyValue = model->data(tmpIndex).toInt();
        if(i != index.row())
        {
            tmp.setNum(dependacyValue);
            ui->comboBox->addItem(tmp);
        }
        ui->comboBox->addItem("0");
    }
    */
    QModelIndex firstItemIndex = model->index(0,0);
    int firstId = model->data(firstItemIndex).toInt();
    tmp.setNum(index.row()+firstId);

    ui->idValue->setText(tmp);
    QModelIndex currentIndex = model->index(index.row(),4);
    ui->comboBox->setCurrentText(model->data(currentIndex).toString());

}
