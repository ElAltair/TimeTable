#ifndef GANTFORM_H
#define GANTFORM_H
#include <QDialog>
#include<QGraphicsScene>
#include <QStandardItemModel>

namespace Ui {
class GantForm;
}

class GantForm : public QDialog
{
    Q_OBJECT

public:
    explicit GantForm(QWidget *parent = 0);
    GantForm(QStandardItemModel *_model, QWidget *parent = 0);
    void setDistancesVector(QVector<int> dist);
    void setVerticesVector(QVector<int> vert);
    void visualize(void);
    ~GantForm();

private slots:
    void on_okButton_clicked();

    void on_GantForm_rejected();

private:
    Ui::GantForm *ui;
    QGraphicsScene* scene;
    QVector<int> distances;
    QVector<int> vertices;
    QStandardItemModel* model;
};

#endif // GANTFORM_H
