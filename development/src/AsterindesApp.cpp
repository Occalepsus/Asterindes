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
	
	QCoreApplication::setOrganizationName("Occalepsus");
	QCoreApplication::setOrganizationDomain("occalepsus.github.io");
	QCoreApplication::setApplicationName("Asterindes");

	AsterindesCore app(argc, argv);

	QQmlApplicationEngine engine;

	return AsterindesCore::exec();
}