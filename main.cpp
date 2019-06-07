#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Game.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    
    Game game(640, 480, 6, std::chrono::milliseconds(30));
    engine.rootContext()->setContextProperty("Game", &game);
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
