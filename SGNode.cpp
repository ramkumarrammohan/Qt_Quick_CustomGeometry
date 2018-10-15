#include "SGNode.h"

#include <QDebug>
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QSGSimpleRectNode>

static int SegmentCount = 2;

SGNode::SGNode(QQuickItem *parent) : QQuickItem (parent)
{
    qDebug() << "SGNode init";
    connect(this, &SGNode::buttonClicked, this, &SGNode::onButtonClicked);

    setFlag(ItemHasContents, true);
}

SGNode::~SGNode()
{
}

void SGNode::onButtonClicked()
{
    qDebug() << "Button click detected";
    update();
}

QSGNode *SGNode::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
//    QSGSimpleRectNode *n = static_cast<QSGSimpleRectNode *>(oldNode);
//    if (!n) {
//        n = new QSGSimpleRectNode();
//        n->setColor(Qt::red);
//    }
//    n->setRect(boundingRect());
//    return n;

    QSGGeometryNode *node = nullptr;
    QSGGeometry *geometry = nullptr;
    int ht = int(height());
    int wt = int(width());

    if (!oldNode) {
        // Init all new
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), SegmentCount);

        // Geometry settings
        geometry->setLineWidth(2);
        geometry->setDrawingMode(GL_LINES);
        geometry->vertexDataAsPoint2D()[0].set(0, ht/2);
        geometry->vertexDataAsPoint2D()[1].set(wt/2, ht/2);
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

        qDebug() << "oldVertexCount: " << oldVertexCount;
//        geometry->vertexDataAsPoint2D()[2].set(wt/2, ht/2);
//        geometry->vertexDataAsPoint2D()[3].set(wt, ht/2);
        geometry->vertexDataAsPoint2D()[oldVertexCount].set(wt/2, ht/2);
        geometry->vertexDataAsPoint2D()[oldVertexCount+1].set(wt, ht/2);
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
