#ifndef ENERGYDISTVIEW_H
#define ENERGYDISTVIEW_H

#include <QDialog>
#include <QImage>

namespace Ui {
class EnergyDistView;
}

class EnergyDistView : public QDialog
{
    Q_OBJECT
public slots:
    void receiveEnergyDist(QImage &i);

public:
    explicit EnergyDistView(QWidget *parent = 0);
    ~EnergyDistView();

private:
    Ui::EnergyDistView *ui;
};

#endif // ENERGYDISTVIEW_H
