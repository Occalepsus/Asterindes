#ifndef PROJECTVIEWMODEL_H
#define PROJECTVIEWMODEL_H

// Asterindes
#include "AsterindesProject.h"

// Qt
#include <QObject>
#include <QUrl>

namespace Asterindes::Ui
{
	/**
	 * ProjectViewModel class is responsible for exposing the project data and operations to the QML UI.
	 * It acts as a bridge between the AsterindesProject and the QML views, providing properties, methods, and signals for UI interaction.
	 */
	class ProjectViewModel : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(ProjectViewModel);

		/**
		 * The URL of the currently opened project, empty if no project is opened. This is used to display the project path in the UI and to check if a project is currently opened.
		 */
		Q_PROPERTY(QUrl currentProjectPath READ getCurrentProjectPath NOTIFY currentProjectPathChanged);

	public:
		/**
		 * Constructor.
		 *
		 * @param p_projectManager Reference to the business logic manager.
		 * @param p_parent Parent QObject.
		 */
		explicit ProjectViewModel(QPointer<AsterindesProject> p_projectManager, QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~ProjectViewModel() override = default;

		/**
		 * Loads a project from the given path, it is used to load the project when the user opens a project from the UI.
		 */
		void loadProject(const QUrl& p_projectPath);

		/**
		 * Gets the URL of the currently opened project, empty if no project is opened. This is used to display the project path in the UI and to check if a project is currently opened.
		 *
		 * @return The URL of the currently opened project.
		 */
		inline QUrl getCurrentProjectPath() const { return m_projectPath; };

	signals:
		/**
		 * Signal emitted when the current project path changes.
		 */
		void currentProjectPathChanged();

	private:
		/**
		 * Reference to the business logic manager.
		 */
		QPointer<AsterindesProject> m_projectManager;

		/**
		 * The URL of the currently opened project, empty if no project is opened. This is used to display the project path in the UI and to check if a project is currently opened.
		 */
		QUrl m_projectPath;

	private slots:
		/**
		 * Slot called when a project is loaded. This will update the current project path and emit the currentProjectPathChanged signal to update the UI.
		 */
		void onProjectLoaded(const QUrl& p_projectPath);
	};
}

#endif // PROJECTVIEWMODEL_H