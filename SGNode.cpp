#include "SGNode.h"
#include "CSVReader.h"

#include <QDebug>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QSGSimpleRectNode>

static const int SegmentCount = 2;
static const int HeadSize = 20;
static const double GainFactor = 0.08;
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
    QSGNode *parentNode = nullptr;
    QSGGeometryNode *geometryNode = nullptr;
    QSGGeometry *geometry = nullptr;
    QSGSimpleRectNode *rectNode = nullptr;

    int ht = int(height());
    int htRef = int(ht/2);
    int wt = int(width());

    if (!oldNode) {
        // Init all new
        parentNode = new QSGNode;
        geometryNode = new QSGGeometryNode;
        rectNode = new QSGSimpleRectNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), SegmentCount);

        // Geometry settings
        geometry->setLineWidth(2);
        geometry->setDrawingMode(GL_LINE_STRIP);
        for (int i = 0; i < geometry->vertexCount(); ++i) {
            geometry->vertexDataAsPoint2D()[i].set(i, (m_csvReader->getData(m_readCursor)*GainFactor)+htRef);
            m_readCursor++;
            m_xLocation++;
        }

        // Material design settings
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(Qt::red);

        // geometryNode settings
        geometryNode->setGeometry(geometry);
        geometryNode->setFlag(QSGGeometryNode::OwnsGeometry);
        geometryNode->setMaterial(material);
        geometryNode->setFlag(QSGNode::OwnsMaterial);

        // Rectnode settings;
        rectNode->setRect(m_xLocation, 0, HeadSize, ht);
        rectNode->setColor(Qt::lightGray);

        parentNode->appendChildNode(geometryNode);
        parentNode->appendChildNode(rectNode);

    } else {
        // Change plots in oldNode
        parentNode = static_cast<QSGNode *>(oldNode);
        geometryNode = static_cast<QSGGeometryNode *>(parentNode->childAtIndex(0));
        rectNode = static_cast<QSGSimpleRectNode *>(parentNode->childAtIndex(1));

        geometry = geometryNode->geometry();
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

                int data = (m_csvReader->getData(m_readCursor)*GainFactor)+htRef;
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

            int data = (m_csvReader->getData(m_readCursor)*GainFactor) + htRef;
            m_readCursor++;
            if (data > ht)
                data = ht;
            geometry->vertexDataAsPoint2D()[m_xLocation].set(m_xLocation, data);
            m_xLocation++;
        }
        rectNode->setRect(m_xLocation, 0, HeadSize, ht);

        rectNode->markDirty(QSGNode::DirtyGeometry);
        geometryNode->markDirty(QSGNode::DirtyGeometry);
    }
    return parentNode;
}
