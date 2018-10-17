#ifndef SGNODE_H
#define SGNODE_H

#include <QQuickItem>
#include <QTimer>

class CSVReader;
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
    void onTimerTimeout();

private:
    int m_readCursor;
    int m_xLocation;
    QTimer m_timer;
    CSVReader *m_csvReader;
};

#endif // SGNODE_H
