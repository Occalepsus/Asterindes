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

	public slots:

		/**
		 * Slot called when a project is opened.
		 *
		 * @param p_projectPath The path of the opened project.
		 */
		void onProjectOpened(const QUrl& p_projectPath);

	signals:

		/**
		 * Emitted when the startup window is visible/hidden.
		 * 
		 * @param p_visible the new visibility
		 */
		void isVisibleChanged(bool p_visible);

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
