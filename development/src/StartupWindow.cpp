#include "StartupWindow.h"

using namespace Asterindes::Ui;

StartupWindow::StartupWindow(ProjectManagerService* p_projectManagerService, QObject* p_parent)
	: QObject(p_parent)
	, m_projectManagerService(p_projectManagerService)
{
	if (m_projectManagerService) {
		QObject::connect(m_projectManagerService, &ProjectManagerService::recentProjectsChanged, this, &StartupWindow::recentProjectsChanged);
	}

	m_startupQmlEngine->setInitialProperties({
		{ "startupWindowData", QVariant::fromValue(this) }
	});

	// Load the QML file for the startup window
	m_startupQmlEngine->loadFromModule("Asterindes", "StartupWindow");
	if (m_startupQmlEngine->rootObjects().isEmpty()) {
		qFatal("Cannot load QML component 'Asterindes/StartupWindow'.");
	}
}

StartupWindow::~StartupWindow()
{
	m_startupQmlEngine->deleteLater();
}

void StartupWindow::setWindowVisible(bool visible)
{
	if (m_isVisible != visible) {
		m_isVisible = visible;
		emit isVisibleChanged(m_isVisible);
	}
}
