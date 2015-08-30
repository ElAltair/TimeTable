#include "delegate.h"
#include <QApplication>
#include <QDebug>

Delegate::Delegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
Delegate::Delegate(QObject *parent, QMap<QString, int> &_map):QStyledItemDelegate(parent)
{
    map = _map;

}


Delegate::~Delegate()
{

}

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   QComboBox* editor = new QComboBox(parent);
    QMap<QString,int>::ConstIterator mapIterator = map.cbegin();
    while(mapIterator != map.cend())
    {
        if((index.row()+1) != mapIterator.value())
        {
        editor->insertItem(mapIterator.value(),mapIterator.key());
        }
        ++mapIterator;
    }
   return editor;
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox * comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(index.row()-1);


}

void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox * comboBox = static_cast<QComboBox*>(editor);
    model->setData(index,comboBox->currentIndex()+1);

}

void Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);

}

