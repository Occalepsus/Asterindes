#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("Asterindes", "Main");
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Impossible de charger le composant QML principal 'Asterindes/Main'.";
        return -1;
    }

	return QGuiApplication::exec();
}