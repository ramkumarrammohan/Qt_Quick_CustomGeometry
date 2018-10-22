#include "SGNode.h"
#include "CSVReader.h"

#include <QDebug>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QSGSimpleRectNode>

static const int SegmentCount = 2;
static const int HeadSize = 20;
static const int NoOfLeads = 12;
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
    m_timer.start(40);
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

void SGNode::createChildNodes(QSGNode *parent)
{
    if (!parent)
        return;

    int ht = int(height());
    int htRef = int((ht / NoOfLeads)); // (AvailHt/NoOfLeads)
    int htRefHalf = htRef / 2;

    QSGSimpleRectNode *rectNode = new QSGSimpleRectNode;
    // Rectnode settings;
    rectNode->setRect(m_xLocation, 0, HeadSize, ht);
    rectNode->setColor(Qt::lightGray);

    // Adding rect node to parent node
    parent->appendChildNode(rectNode);

    for (int childIndex = 1; childIndex <= NoOfLeads; childIndex++) {
        QSGGeometry *childGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), SegmentCount);
        QSGGeometryNode *childGeometryNode = new QSGGeometryNode;

        // Geometry settings
        childGeometry->setLineWidth(2);
        childGeometry->setDrawingMode(GL_LINE_STRIP);

        for (int i = 0; i < childGeometry->vertexCount(); i++) {
            childGeometry->vertexDataAsPoint2D()[i].set(i, int((m_csvReader->getData(m_readCursor) * GainFactor) + ((htRef * childIndex) - htRefHalf)));
        }

        // Material design settings
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(Qt::red);

        // geometryNode settings
        childGeometryNode->setGeometry(childGeometry);
        childGeometryNode->setFlag(QSGGeometryNode::OwnsGeometry);
        childGeometryNode->setMaterial(material);
        childGeometryNode->setFlag(QSGNode::OwnsMaterial);

        parent->appendChildNode(childGeometryNode);
    }
    m_readCursor++;
    m_xLocation++;
}

QSGNode *SGNode::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGNode *parentNode = nullptr;
    QSGGeometryNode *geometryNode = nullptr;
    QSGGeometry *geometry = nullptr;
    QSGSimpleRectNode *rectNode = nullptr;

    int ht = int(height());
    int htRef = int(ht / NoOfLeads); // AvailHt/NoOfLeads
    int htRefHalf = htRef / 2;
    int wt = int(width());

    if (!oldNode) {
        // Init all new
        parentNode = new QSGNode;
        createChildNodes(parentNode);
    } else {
        // Change plots in oldNode
        parentNode = static_cast<QSGNode *>(oldNode);
        rectNode = static_cast<QSGSimpleRectNode *>(parentNode->childAtIndex(0));

        for (int childNodeIndex = 1; childNodeIndex < parentNode->childCount(); childNodeIndex++) {
            int childHtRef = (htRef * childNodeIndex) - htRefHalf;

            geometryNode = static_cast<QSGGeometryNode *>(parentNode->childAtIndex(childNodeIndex));
            geometry = geometryNode->geometry();

            int oldVertexCount = geometry->vertexCount();
            int maxdataAvail = m_csvReader->maxData();

            if (oldVertexCount <= wt) {
                m_readCursor = 0;
                m_xLocation = 0;
                for (int i=0; i<=oldVertexCount; i++) {
                    if (m_readCursor >= maxdataAvail) {
                        m_readCursor = 0;
                    }

                    int data = int ((m_csvReader->getData(m_readCursor) * GainFactor) + childHtRef);
                    m_readCursor++;
                    if (data > childHtRef)
                        data = childHtRef;

                    geometry->allocate(oldVertexCount+1);
                    geometry->vertexDataAsPoint2D()[i].set(i, data);
                    m_xLocation++;
                }
            } else {
                if (m_xLocation > wt) {
                    m_xLocation = 0;
                }

                if (m_readCursor >= maxdataAvail) {
                    m_readCursor = 0;
                }

                int data = int ((m_csvReader->getData(m_readCursor)*GainFactor) + childHtRef);
                if (data > childHtRef)
                    data = childHtRef;
                geometry->vertexDataAsPoint2D()[m_xLocation].set(m_xLocation, data);
            }
            geometryNode->markDirty(QSGNode::DirtyGeometry);
        }
        m_readCursor++;
        m_xLocation++;

        rectNode->setRect(m_xLocation, 0, HeadSize, ht);
        rectNode->markDirty(QSGNode::DirtyGeometry);
    }
    return parentNode;
}
