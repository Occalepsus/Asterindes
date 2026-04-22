// Asterindes
#include "AsterindesCore.h"

// Qt
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>
#include <QTimer>

using namespace Asterindes;

int main(int argc, char* argv[])
{
	qSetMessagePattern(
#ifdef QT_DEBUG
		"%{time} %{type} %{function}(): %{message}"
#else
		"%{type}: %{message}"
#endif
	);

	AsterindesCore app(argc, argv);

	QQmlApplicationEngine engine;

	app.getProjectManager().loadProject("");

	return AsterindesCore::exec();
}