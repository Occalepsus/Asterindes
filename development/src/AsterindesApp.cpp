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
	AsterindesCore app(argc, argv);

	QQmlApplicationEngine engine;

	app.getProjectManager().loadProject("");


	ResourcesManager& resourcesManager{ app.getProjectManager().getResourcesManager() };

	return AsterindesCore::exec();
}