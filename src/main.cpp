#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include "myglitem.h"

int main(int argc, char *argv[])
{
    QLocale::setDefault(QLocale::German);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QTranslator translator;
    if(translator.load("Weekeewachee.qm",":/translations"))
        app.installTranslator(&translator);

    QQmlApplicationEngine engine;
    app.setWindowIcon(QIcon(":/icons/icon.ico"));

    qmlRegisterType<MyGLItem>("GlComponents",1,0,"MyGLItem");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
