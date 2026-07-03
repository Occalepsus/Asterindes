#ifndef PROJECTMANAGERSERVICE_H
#define PROJECTMANAGERSERVICE_H

// Asterindes

// Qt
#include <QObject>
#include <QUrl>

namespace Asterindes
{
	/**
	 * The ProjectManagerService class is responsible for managing the projects of the application: creating, loading, and managing recent projects.
	 */
	class ProjectManagerService : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(ProjectManagerService);

	public:

		/**
		 * Default constructor.
		 *
		 * @param parent The parent QObject, default is nullptr.
		 */
		explicit ProjectManagerService(QObject* parent = nullptr);

		/**
		 * Destructor.
		 */
		~ProjectManagerService() final = default;

		/**
		 * Creates a new project.
		 * 
		 * @param p_fileName The file name of the project to create, it should be a local file path where the project file will be created.
		 * If the file already exists, it will not be overwritten and the method will return false.
		 * 
		 * @return true if the project was created successfully, false otherwise.
		 */
		bool createProject(const QUrl& p_fileName);

		/**
		 * Loads a project from the given file path, if the project is already open it will just focus the project window.
		 *
		 * @param p_projectPath The path of the project to open, it should be a local file path pointing to a valid project file.
		 * 
		 * @return true if the project was loaded successfully, false otherwise.
		 */
		bool loadProject(const QUrl& p_projectPath);

		/**
		 * Gets the list of recent projects
		 * 
		 * @return the list of recent projects
		 */
		QList<QUrl> getRecentProjectList() const { return m_recentProjectList; };

		/**
		 * Gets the error string containing the last encountered error, it is set when a method fails and can be used to get more information about the error.
		 *
		 * @return The error string containing the last encountered error, it is empty if there was no error.
		 */
		QString getErrorString() const { return m_errorString; }

	signals:

		/**
		 * Emitted when the list of recent projects changes.
		 * 
		 * @param p_recentProjectList The new list of recent projects.
		 */
		void recentProjectListChanged(const QList<QUrl>& p_recentProjectList);

	private:
		
		/**
		 * The errorString containing the last encountered error, it is set when a method fails and can be used to get more information about the error.
		 */
		QString m_errorString{};

		/**
		 * The list of recent projects that will be updated and saved into registers
		 */
		QList<QUrl> m_recentProjectList{};

		/**
		 * Loads the list of recent projects, it reads the recent projects from the application settings.
		 */
		void loadRecentProjectList();

		/**
		 * Updates the list of recent projects given the last project path.
		 * 
		 * @param p_lastProjectPath The path of the last opened project.
		 */
		void updateRecentProjectList(const QUrl& p_lastProjectPath);
	};
}


#endif // !PROJECTMANAGERSERVICE_H
