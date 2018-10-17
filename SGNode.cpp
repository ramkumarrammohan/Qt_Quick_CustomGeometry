#include "SGNode.h"
#include "CSVReader.h"

#include <QDebug>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QSGSimpleRectNode>

static const int SegmentCount = 2;
//static const int TotalSamples = 20;
//static const int xPixelsScale = 15;
//static const int yPixelsScale = 20;

SGNode::SGNode(QQuickItem *parent) :
    QQuickItem(parent),
    m_csvReader(nullptr)
{
    qDebug() << "SGNode init";
    m_readCursor = 0;
    m_xLocation = 0;
    m_csvReader = new CSVReader();

    // Register signals here
    connect(this, &SGNode::buttonClicked, this, &SGNode::onButtonClicked);
    connect(&m_timer, &QTimer::timeout, this, &SGNode::onTimerTimeout);

    setFlag(ItemHasContents, true);
}

SGNode::~SGNode()
{
}

void SGNode::onButtonClicked()
{
    qDebug() << "Height: " << height();
    qDebug() << "Width: " << width();
    m_timer.start(40);
}

void SGNode::onTimerTimeout()
{
    update();
}

QSGNode *SGNode::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = nullptr;
    QSGGeometry *geometry = nullptr;

    int ht = int(height());
    int htRef = int(ht/2);
    int wt = int(width());

    if (!oldNode) {
        // Init all new
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), SegmentCount);

        // Geometry settings
        geometry->setLineWidth(2);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        for (int i = 0; i < geometry->vertexCount(); ++i) {
            geometry->vertexDataAsPoint2D()[i].set(i, m_csvReader->getData(m_readCursor)+htRef);
            m_readCursor++;
        }

        // Material design settings
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));

        // node settings
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);

    } else {
        // Change plots in oldNode
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        int oldVertexCount = geometry->vertexCount();
        int maxdataAvail = m_csvReader->maxData();

        if (oldVertexCount <= wt) {
            m_readCursor = 0;
            m_xLocation = 0;
            for (int i=0; i<=oldVertexCount; i++) {
                if (m_readCursor >= maxdataAvail) {
//                    qDebug() << "Reset cursor";
                    m_readCursor = 0;
                }

                int data = m_csvReader->getData(m_readCursor)+htRef;
                m_readCursor++;
                if (data > ht)
                    data = ht;

                geometry->allocate(oldVertexCount+1);
                geometry->vertexDataAsPoint2D()[i].set(i, data);
                m_xLocation++;
            }
//            for (int i=0; i<geometry->vertexCount(); i++){
//                qDebug() << "point " << i << "  X: " << geometry->vertexDataAsPoint2D()[i].x << " y: " << geometry->vertexDataAsPoint2D()[i].y;
//            }
//            qDebug() << "  ";
//            qDebug() << "geometry->vertexCount(): " << geometry->vertexCount();
        } else {
            if (m_xLocation > wt) {
//                qDebug() << "m_xLocation reset";
                m_xLocation = 0;
            }

            if (m_readCursor >= maxdataAvail) {
//                qDebug() << "Reset cursor";
                m_readCursor = 0;
            }

            int data = m_csvReader->getData(m_readCursor) + htRef;
            m_readCursor++;
            if (data > ht)
                data = ht;
            geometry->vertexDataAsPoint2D()[m_xLocation].set(m_xLocation, data);
            m_xLocation++;
        }
        node->markDirty(QSGNode::DirtyGeometry);
    }

    return node;
}
