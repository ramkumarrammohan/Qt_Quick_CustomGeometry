#include "SGNode.h"

#include <QDebug>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QSGSimpleRectNode>

static const int SegmentCount = 2;
static const int TotalSamples = 20;
static const int xPixelsScale = 15;
static const int yPixelsScale = 20;

SGNode::SGNode(QQuickItem *parent) : QQuickItem (parent)
{
    qDebug() << "SGNode init";
    qDebug() << "mock data count: " << m_mockData.count();

    // Register signals here
    connect(this, &SGNode::buttonClicked, this, &SGNode::onButtonClicked);

    setFlag(ItemHasContents, true);
}

SGNode::~SGNode()
{
}

void SGNode::onButtonClicked()
{
    update();
}

QSGNode *SGNode::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = nullptr;
    QSGGeometry *geometry = nullptr;

    if (!oldNode) {
        // Init all new
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), SegmentCount);

        // Geometry settings
        geometry->setLineWidth(2);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        geometry->vertexDataAsPoint2D()[0].set(0, m_mockData[0]*yPixelsScale);
        geometry->vertexDataAsPoint2D()[1].set(xPixelsScale, m_mockData[1]*yPixelsScale);

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

        if (oldVertexCount <= TotalSamples) {
            for (int i=0; i<=oldVertexCount; i++) {
                geometry->allocate(oldVertexCount+1);
                geometry->vertexDataAsPoint2D()[i].set(i*xPixelsScale, m_mockData[i]*yPixelsScale);
            }

            qDebug() << "geometry->vertexCount(): " << geometry->vertexCount();
            for (int i=0; i<geometry->vertexCount(); i++){
                qDebug() << "point " << i << "  X: " << geometry->vertexDataAsPoint2D()[i].x << " y: " << geometry->vertexDataAsPoint2D()[i].y;
            }
            qDebug() << "  ";
        }
        node->markDirty(QSGNode::DirtyGeometry);
    }

    return node;
}
