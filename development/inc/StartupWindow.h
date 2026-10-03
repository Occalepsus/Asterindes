#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

// Asterindes

// Asterindes UI

// Qt
#include <QObject>
#include <QQmlApplicationEngine>

namespace Asterindes
{
	class ProjectManagerService;
}

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

		/**
		 * True if the startup window is visible, false otherwise.
		 */
		Q_PROPERTY(bool visible READ isVisible NOTIFY isVisibleChanged);

		/**
		 * True if the startup window is loading a project, false otherwise.
		 */
		Q_PROPERTY(bool loading READ isLoading NOTIFY isLoadingChanged);

		/**
		 * The errorString containing the last encountered error, it is set when a method fails and can be used to get more information about the error.
		 */
		Q_PROPERTY(QString projectOpenErrorString READ getProjectOpenErrorString NOTIFY projectOpenErrorStringChanged);

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
		 * Returns true if the startup window is loading a project, false otherwise.
		 *
		 * @return true if the startup window is loading a project, false otherwise.
		 */
		bool isLoading() const { return m_isLoading; };

		/**
		 * Shows the startup window, it will load the QML file and display the window.
		 */
		void showStartupWindow();

		/**
		 * Hides the startup window.
		 */
		Q_INVOKABLE void hideStartupWindow();

		/**
		 * Gets the error string containing the last encountered error when opening a project, it is set when a method fails and can be used to get more information about the error.
		 *
		 * @return The error string containing the last encountered error when opening a project, it is empty if there was no error.
		 */
		QString getProjectOpenErrorString() const;

	public slots:

		/**
		 * Slot called when a project is opened.
		 *
		 * @param p_projectPath The path of the opened project.
		 */
		void onProjectOpening(const QUrl& p_projectPath);

		/**
		 * Slot called when a project is opened.
		 *
		 * @param p_projectPath The path of the opened project.
		 */
		void onProjectOpened(const QUrl& p_projectPath);

		/**
		 * Slot called when a project failed to open.
		 *
		 * @param p_errorString The error string containing the reason of the failure.
		 */
		void onProjectOpenError(const QString& p_errorString);

	signals:

		/**
		 * Emitted when the startup window is visible/hidden.
		 * 
		 * @param p_visible the new visibility
		 */
		void isVisibleChanged(bool p_visible);
		
		/**
		 * Emitted when the startup window is loading a project.
		 *
		 * @param p_loading true if a project is loading, false otherwise.
		 */
		void isLoadingChanged(bool p_loading);

		/**
		 * Emitted when a project failed to open.
		 *
		 * @param p_errorString The error string containing the reason of the failure.
		 */
		void projectOpenErrorStringChanged(const QString& p_errorString);

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

		/**
		 * True if a project is loading, false otherwise.
		 */
		bool m_isLoading{ false };
	};
}


#endif // !STARTUPWINDOW_H
