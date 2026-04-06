// Asterindes
#include "AsterindesApp.h"

// Qt
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>
#include <QTimer>

using namespace Asterindes;

int main(int argc, char* argv[])
{
	AsterindesApp app(argc, argv);

	QQmlApplicationEngine engine;

	app.getProjectManager().loadProject("");

		// Expose the resources list model to QML
	//engine.rootContext()->setContextProperty("resourcesListModel", app.getProjectManager().getResourcesManager().getDisplayedResourcesListModel());


	ResourcesManager& resourcesManager{ app.getProjectManager().getResourcesManager() };

	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [&resourcesManager] (QObject* object, const QUrl& url) {
		// Expose the resources list model to QML
		QVariant aaa = QVariant::fromValue(resourcesManager.getDisplayedResourcesListModel());
		QObject* bbb = object->findChild<QObject*>(QAnyStringView("resourceGridView"));
		bbb->setProperty("model", aaa);

		// Connect the signal from the QML resources panel to the slot in the resources manager to handle the resource add request
		const QObject* lResourcesPanel{ object->findChild<QObject*>("resourcesGridPanel") };
		auto a = QObject::connect(lResourcesPanel, SIGNAL(qmlResourceAddRequest(QUrl)), &resourcesManager, SLOT(addResource(QUrl)));

		qInfo("%s", a ? "Connection successful" : "Connection failed");
	});

	engine.loadFromModule("Asterindes", "Main");
	if (engine.rootObjects().isEmpty()) {
		qCritical() << "Impossible de charger le composant QML principal 'Asterindes/Main'.";
		return -1;
	}

	return AsterindesApp::exec();
}