#ifndef EDITFORM_H
#define EDITFORM_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QModelIndex>

namespace Ui {
class TableEditForm;
}

class TableEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit TableEditForm(QWidget *parent = 0);
    void setModel(QAbstractItemModel * model);
    ~TableEditForm();
    QDataWidgetMapper* mapper;
    void setIndex(QModelIndex &index);
    void prepareData(const QAbstractItemModel *model, const QModelIndex &index);

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::TableEditForm *ui;
    QModelIndex editIndex;
};

#endif // EDITFORM_H
