#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

// Asterindes
#include "ProjectManagerService.h"

// Asterindes UI

// Qt
#include <QObject>
#include <QQmlApplicationEngine>


namespace Asterindes::Ui
{
	/**
	 * StartupWindow is the class that handles the startup window of the application.
	 * It is responsible of telling the application to open a project or create a new one.
	 */
	class StartupWindow : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(StartupWindow);

		Q_PROPERTY(bool visible READ isVisible NOTIFY isVisibleChanged);

		Q_PROPERTY(QList<QUrl> recentProjectList READ getRecentProjectList NOTIFY recentProjectListChanged);

	public:

		/**
		 * Default constructor.
		 * 
		 * @param p_projectManagerService The ProjectManagerService instance used to manage the projects of the application.
		 * @param p_parent The parent QObject, default is nullptr.
		 */
		explicit StartupWindow(ProjectManagerService* p_projectManagerService, QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~StartupWindow() final;

		/**
		 * Returns true if the startup window is visible, false otherwise.
		 *
		 * @return true if the startup window is visible, false otherwise.
		 */
		bool isVisible() const { return m_isVisible; };

		/**
		 * Sets the visibility of the startup window.
		 * 
		 * @param p_visible True to show the startup window, false to hide it.
		 */
		void setWindowVisible(bool p_visible);

		/**
		 * Returns the list of recent projects.
		 *
		 * @return The list of recent projects.
		 */
		QList<QUrl> getRecentProjectList() const { return m_projectManagerService ? m_projectManagerService->getRecentProjectList() : QList<QUrl>(); };

		/**
		 * Creates a new project given its path, if the project already exists it will not be overwritten and the method will return false.
		 */
		Q_INVOKABLE void createProject(const QUrl& p_projectPath) { m_projectManagerService ? m_projectManagerService->createProject(p_projectPath) : false; };

		/**
		 * Opens a project given its path, if the project is already open it will just focus the project window.
		 *
		 * @param p_projectPath The path of the project to open, it should be a local file path pointing to a valid project file.
		 */
		Q_INVOKABLE void openProject(const QUrl& p_projectPath) { m_projectManagerService ? m_projectManagerService->loadProject(p_projectPath) : false; };

	signals:

		/**
		 * Emitted when the startup window is visible/hidden.
		 * 
		 * @param p_visible the new visibility
		 */
		void isVisibleChanged(bool p_visible);

		/**
		 * Emitted when the list of recent projects changes.
		 * 
		 * @param p_recentProjectList The new list of recent projects.
		 */
		void recentProjectListChanged(const QList<QUrl>& p_recentProjectList);

	private:

		/**
		 * The ProjectManagerService instance used to manage the projects of the application.
		 */
		QPointer<ProjectManagerService> m_projectManagerService;

		/**
		 * The QML engine used to load the startup window QML file.
		 */
		QQmlApplicationEngine* m_startupQmlEngine{ new QQmlApplicationEngine() };

		/**
		 * True if the startup window is visible, false otherwise.
		 */
		bool m_isVisible{ false };
	};
}


#endif // !STARTUPWINDOW_H
