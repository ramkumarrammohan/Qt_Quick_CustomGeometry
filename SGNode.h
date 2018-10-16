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
    void createMockData();

    QList<int> m_mockData;

};

#endif // SGNODE_H
