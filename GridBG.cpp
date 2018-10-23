#include "GridBG.h"

#include <QDebug>
#include <QPainter>

GridBG::GridBG(QQuickPaintedItem *parent) :
    QQuickPaintedItem(parent),
    rightAligned(true)
{
    qDebug() << "GridLines init ";
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void GridBG::paint(QPainter *painter)
{
    int wt = int (width());
    int ht = int (height());
    qDebug() << "Ht: " << ht;
    qDebug() << "Wt: " << wt;

    QVector<QLineF> normalLines;
    QVector<QLineF> boldLines;

    // vertical lines
    int vi = 0;
    while (vi <= ht) {
        if (vi % 25 == 0)
            boldLines.append(QLineF(0, vi, wt, vi));
        else
            normalLines.append(QLineF(0, vi, wt, vi));

        vi+=5;
    }

    // horizontal lines
    int hi = 0;
    while (hi <= wt) {
        if (hi % 25 == 0)
            boldLines.append(QLineF(hi, 0, hi, ht));
        else
            normalLines.append(QLineF(hi, 0, hi, ht));

        hi+=5;
    }


    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor("#f4f4f4"), 1, Qt::SolidLine));
    painter->drawLines(normalLines);


    painter->setPen(QPen(QColor("#dedede"), 1, Qt::SolidLine));
    painter->drawLines(boldLines);
}
