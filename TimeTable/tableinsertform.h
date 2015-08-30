#ifndef TABLEINSERTFORM_H
#define TABLEINSERTFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDialog>

namespace Ui {
class TableInsertForm;
}

class TableInsertForm : public QDialog
{
    Q_OBJECT

public:
    explicit TableInsertForm(QAbstractItemModel* _model,QWidget *parent =0);
    ~TableInsertForm();
    QPoint insertDataToTheModel(QStandardItemModel & model);

private slots:
    void on_submitButton_clicked();
    void on_cancelButton_clicked();
    void on_existingCheckBox_toggled(bool checked);
    void on_existingComboBox_activated(const QString &arg1);


    void on_existingComboBox_currentIndexChanged(int index);

private:
    void fillDependacyComboBox(void);
    Ui::TableInsertForm *ui;
    QAbstractItemModel *model;
    QMap<int,QString> map;
};

#endif // TABLEINSERTFORM_H
