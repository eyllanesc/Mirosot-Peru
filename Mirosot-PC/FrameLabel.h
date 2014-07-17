/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef FRAMELABEL_H
#define FRAMELABEL_H

#include "Structures.h"

// Qt header files
#include <QtGui>

class FrameLabel : public QLabel
{
    Q_OBJECT

public:
    FrameLabel(QWidget *parent = 0);
    void setMouseCursorPos(QPoint);
    QPoint getMouseCursorPos();
private:
    MouseData mouseData;
    QPoint startPoint;
    QPoint mouseCursorPos;
    bool drawBox;
    QRect *box;
    QPoint initial;
    QPoint final;
protected:
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);
signals:
    void newMouseData(struct MouseData mouseData);
    void onMouseMoveEvent();
};

#endif // FRAMELABEL_H
