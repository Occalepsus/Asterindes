#include "StartupWindow.h"

// Qt
#include <QQuickWindow>

using namespace Asterindes::Ui;

StartupWindow::StartupWindow(ProjectManagerService* p_projectManagerService, QObject* p_parent)
	: QObject(p_parent)
	, m_projectManagerService(p_projectManagerService)
{
	QObject::connect(m_projectManagerService, &ProjectManagerService::projectOpened, this, &StartupWindow::onProjectOpened);

	m_startupQmlEngine->setInitialProperties({
		{ "startupWindowData", QVariant::fromValue(this) },
		{ "projectManagerService", QVariant::fromValue(m_projectManagerService.data()) }
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

void StartupWindow::showStartupWindow()
{
	if (!m_isVisible) {
		m_isVisible = true;

		emit isVisibleChanged(m_isVisible);
	}

	// Raise the startup window to the front and request focus
	if (QQuickWindow* l_window = qobject_cast<QQuickWindow*>(m_startupQmlEngine->rootObjects().first()); l_window) {
		l_window->raise();
		l_window->requestActivate();
	}
}

void StartupWindow::hideStartupWindow()
{
	if (m_isVisible) {
		m_isVisible = false;
		emit isVisibleChanged(m_isVisible);
	}
}

void StartupWindow::onProjectOpened(const QUrl&)
{
	hideStartupWindow();
}