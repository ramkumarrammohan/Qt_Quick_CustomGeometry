#ifndef SGNODE_H
#define SGNODE_H

#include <QQuickItem>

class SGNode : public QQuickItem
{
    Q_OBJECT
public:
    explicit SGNode(QQuickItem *parent = nullptr);
    ~SGNode();

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

signals:
    void buttonClicked();

private slots:
    void onButtonClicked();

};

#endif // SGNODE_H
