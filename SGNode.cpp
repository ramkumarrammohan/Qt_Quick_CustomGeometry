#include "SGNode.h"

#include <QDebug>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QSGSimpleRectNode>

static const int SegmentCount = 2;
static const int Samples = 20;
static const int xPixelsScale = 15;
static const int yPixelsScale = 20;

SGNode::SGNode(QQuickItem *parent) : QQuickItem (parent)
{
    qDebug() << "SGNode init";
//    createMockData();

    // Register signals here
    connect(this, &SGNode::buttonClicked, this, &SGNode::onButtonClicked);

    setFlag(ItemHasContents, true);
}

SGNode::~SGNode()
{
}

void SGNode::createMockData()
{
    m_mockData.clear();
    for (int i=0; i<Samples; i++) {
        int rand = int(qrand()%10);
        m_mockData.append(rand);
    }
    qDebug() << "mockdata: " << m_mockData;
}

void SGNode::onButtonClicked()
{
//    qDebug() << "Button click detected";
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

        int pointy0 = int(qrand()%10) * yPixelsScale;
        int pointy1 = int(qrand()%10) * yPixelsScale;

        // Geometry settings
        geometry->setLineWidth(2);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        geometry->vertexDataAsPoint2D()[0].set(0, pointy0);
        geometry->vertexDataAsPoint2D()[1].set(1*xPixelsScale, pointy1);
        qDebug() << "Point 1 -- " << "x: " << geometry->vertexDataAsPoint2D()[0].x << " y: " << geometry->vertexDataAsPoint2D()[0].y;
        qDebug() << "Point 2 -- " << "x: " << geometry->vertexDataAsPoint2D()[1].x << " y: " << geometry->vertexDataAsPoint2D()[1].y;

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
        geometry->allocate(oldVertexCount + SegmentCount);

        geometry->vertexDataAsPoint2D()[oldVertexCount].set(oldVertexCount*xPixelsScale, (int(qrand()%10) * yPixelsScale));
        geometry->vertexDataAsPoint2D()[oldVertexCount+1].set((oldVertexCount+1)*xPixelsScale, (int(qrand()%10) * yPixelsScale));

        qDebug() << "new points added " << "X: " << geometry->vertexDataAsPoint2D()[oldVertexCount].x << " y: " << geometry->vertexDataAsPoint2D()[oldVertexCount].y;
        qDebug() << "new points added " << "X: " << geometry->vertexDataAsPoint2D()[oldVertexCount+1].x << " y: " << geometry->vertexDataAsPoint2D()[oldVertexCount+1].y;
    }

    qDebug() << "geometry->vertexCount(): " << geometry->vertexCount();
    for (int i=0; i<geometry->vertexCount(); i++){
        qDebug() << "point " << i << "  X: " << geometry->vertexDataAsPoint2D()[i].x << " y: " << geometry->vertexDataAsPoint2D()[i].y;
    }
    qDebug() << "  ";

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
