#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "waterfallitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<WaterfallItem>("hu.timur", 1, 0, "Waterfall");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///desktop/main.qml")));

    return app.exec();
}
