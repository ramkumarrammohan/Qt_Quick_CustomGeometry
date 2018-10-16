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

private:
    QList<int> m_mockData{0, 5, 7, 2, 8, 4, 9, 6, 2, 3,
                         8, 4, 1, 4, 8, 6, 4, 8, 9, 2, 6};
};

#endif // SGNODE_H
