// Asterindes
#include "AsterindesCore.h"

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

	// Try to start the application, if it fails (another instance is running) exit the application.
	if (!app.start())
	{
		return 0;
	}

	return AsterindesCore::exec();
}