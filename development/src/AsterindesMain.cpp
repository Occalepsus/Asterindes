// Asterindes
#include "AsterindesApp.h"

// Qt
#include <QQmlApplicationEngine>
#include <QQmlContext>

using namespace Asterindes;

int main(int argc, char* argv[])
{
	AsterindesApp app(argc, argv);

	QQmlApplicationEngine engine;

	app.getProjectManager().loadProject("");

	// Expose the resources list model to QML
	engine.rootContext()->setContextProperty("resourcesListModel", app.getProjectManager().getResourcesManager().getDisplayedResourcesListModel());

	engine.loadFromModule("Asterindes", "Main");
	if (engine.rootObjects().isEmpty()) {
		qCritical() << "Impossible de charger le composant QML principal 'Asterindes/Main'.";
		return -1;
	}

	return AsterindesApp::exec();
}