/*!
 * \class main
 * \brief Hauptklasse für das Spiel.
 * \author Baranov Kostyantyn
 * \author Behrenbeck David
 * \author Peddinghaus Mike Frank
 * \version 1.0
 * \date 27 April 2020
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include "myglitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QLocale::Language sysLangId = QLocale::system().language();
    QTranslator translator;
    if(sysLangId == QLocale::German)
    {
        if(translator.load("Weekeewachee_de.qm",":/translations"))
            app.installTranslator(&translator);
    }

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
