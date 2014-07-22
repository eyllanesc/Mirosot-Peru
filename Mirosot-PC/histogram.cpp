/************************************************************************/
/*Author:								*/
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>         */
/*				      <e.yllanescucho@gmail.com>        */
/************************************************************************/

#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::setImage(QImage hist)
{
    ui->myframe->setPixmap(QPixmap::fromImage(hist));
}
