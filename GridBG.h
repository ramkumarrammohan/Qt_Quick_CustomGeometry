#ifndef GRIDBG_H
#define GRIDBG_H

#include <QQuickPaintedItem>

class GridBG : public QQuickPaintedItem
{
public:
    GridBG(QQuickPaintedItem *parent = nullptr);

    void paint(QPainter *painter);

private:
    bool rightAligned;
};

#endif // GRIDBG_H
