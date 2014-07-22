/************************************************************************/
/*Author:								*/
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>         */
/*				      <e.yllanescucho@gmail.com>        */
/************************************************************************/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>

namespace Ui {
class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT

public:
    Histogram(QWidget *parent = 0);
    ~Histogram();
    void setImage(QImage hist);
private:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
