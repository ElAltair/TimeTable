#include "tableinsertform.h"
#include "ui_tableinsertform.h"
#include <QDebug>
TableInsertForm::TableInsertForm(QAbstractItemModel* _model,QWidget *parent) : QDialog(parent),
    ui(new Ui::TableInsertForm)
{
    ui->setupUi(this);
    model = _model;



    QMap<int,QString> tmpMap;
    QString tmp;
    for(int i = 0; i < model->rowCount(); ++i)
    {

        QModelIndex tmpIndex = model->index(i,0);
        tmpMap.insert(model->data(tmpIndex).toInt(),model->data(tmpIndex).toString());
     }
    QMap<int,QString>::const_iterator mapIter = tmpMap.cbegin();
    while( mapIter != tmpMap.cend())
    {
        ui->existingComboBox->addItem(mapIter.value());
        ui->dependacyComboBox->addItem(mapIter.value());
        ++mapIter;
    }
    ui->dependacyComboBox->addItem("0");
    map = tmpMap;

    ui->existingCheckBox->setChecked(false);
    ui->existingComboBox->setEnabled(false);
    ui->existingComboBox->setCurrentText("0");

}

TableInsertForm::~TableInsertForm()
{
    delete ui;
}

QPoint TableInsertForm::insertDataToTheModel(QStandardItemModel &model)
{
    QString tmpString,returnString;
    QStandardItem* tmp = 0;
    QList<QStandardItem*> list;
    int id = 0,dependacy = 0;
    int tmpI = 0;

    if( ui->existingCheckBox->isChecked())
    {
        for( int i = 0; i< model.rowCount(); ++i)
        {
            QModelIndex tmpIndex = model.index(i,0);
            if(model.data(tmpIndex).toString() == ui->existingComboBox->currentText())
            {


                for( int j = 0; j < model.columnCount()-2; ++j)
                {
                    QModelIndex columnIndex = model.index(i,j);

                    tmp = new QStandardItem(model.data(columnIndex).toString());
                    list<<tmp;
                }

                id = model.data(tmpIndex).toInt();
                tmpI = i;
                break;
            }


        }

        tmpString.setNum(ui->workingtimeSpinBox->value());
        tmp = new QStandardItem(tmpString);
        list<<tmp;
        tmp = new QStandardItem(ui->dependacyComboBox->currentText());
        list<<tmp;
        model.appendRow(list);
        dependacy =  ui->dependacyComboBox->currentText().toInt();
        return QPoint( id,dependacy);



    }
    else
    {
        int maxValue = INT_MIN;

        if(model.rowCount()!=0)
        {
            for( int i = 0; i < model.rowCount(); ++i)
            {
                QModelIndex tmp = model.index(i,0);
                int value = model.data(tmp).toInt();
                if(value > maxValue )
                {
                    maxValue = value;
                }
            }
            maxValue++;
        }
        else
            maxValue = 1;

        returnString.setNum(maxValue);
        tmp = new QStandardItem(returnString);
        list<<tmp;
        tmp = new QStandardItem(ui->nameLineEdit->text());
        list<<tmp;
        tmp = new QStandardItem(ui->descriptionPlainTextEdit->toPlainText());
        list<<tmp;
        tmpString.setNum(ui->workingtimeSpinBox->value());
        tmp = new QStandardItem(tmpString);
        list<<tmp;
        tmp = new QStandardItem(ui->dependacyComboBox->currentText());
        list<<tmp;
        model.appendRow(list);
        id = maxValue;
        dependacy = ui->dependacyComboBox->currentText().toInt();
        return QPoint( id,dependacy);
    }
}


void TableInsertForm::on_submitButton_clicked()
{
    accept();

}

void TableInsertForm::on_cancelButton_clicked()
{
    reject();

}

void TableInsertForm::on_existingCheckBox_toggled(bool checked)
{
    if(checked)
    {
    ui->existingComboBox->setEnabled(true);
    ui->nameLineEdit->setEnabled(false);
    ui->dependacyComboBox->setEnabled(false);
    ui->workingtimeSpinBox->setEnabled(false);
    ui->descriptionPlainTextEdit->setEnabled(false);
    }
    else
    {


        ui->dependacyComboBox->clear();
        fillDependacyComboBox();
       ui->dependacyComboBox->addItem("0");


        ui->dependacyComboBox->setEnabled(true);
        ui->nameLineEdit->setEnabled(true);
        ui->workingtimeSpinBox->setEnabled(true);
        ui->descriptionPlainTextEdit->setEnabled(true);
        ui->existingComboBox->setEnabled(false);
    }

}


void TableInsertForm::on_existingComboBox_activated(const QString &arg1)
{
   /* ui->dependacyComboBox->clear();


       int count =  ui->dependacyComboBox->count();

    for( int i = 0; i < count; ++i)
        if(ui->dependacyComboBox->itemText(i) == ui->existingComboBox->currentText())
               ui->dependacyComboBox->removeItem(i);

    //ui->dependacyComboBox->removeItem(count-1);

    ui->dependacyComboBox->setEnabled(true);
    */


}

void TableInsertForm::on_existingComboBox_currentIndexChanged(int index)
{
    ui->dependacyComboBox->clear();
    fillDependacyComboBox();

    int count = ui->dependacyComboBox->count();
    for( int i = 0; i < count; ++i)
        if(ui->dependacyComboBox->itemText(i) == ui->existingComboBox->currentText())
               ui->dependacyComboBox->removeItem(i);

    ui->dependacyComboBox->setEnabled(true);


}

void TableInsertForm::fillDependacyComboBox()
{
    QMap<int,QString>::const_iterator mapIter = map.cbegin();
    while( mapIter != map.cend())
    {
        ui->dependacyComboBox->addItem(mapIter.value());
        ++mapIter;
    }


}
