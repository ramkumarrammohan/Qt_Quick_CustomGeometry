#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScreen>

#include "SGNode.h"
#include "GridBG.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<SGNode>("SGNode", 1, 0, "SGNode");
    qmlRegisterType<GridBG>("GridBG", 1, 0, "GridBG");

    qDebug() << "Screen info: " << app.primaryScreen()->logicalDotsPerInch();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
