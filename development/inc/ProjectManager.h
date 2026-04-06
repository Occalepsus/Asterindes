#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

// Asterindes
#include "ResourcesManager.h"

// Qt
#include <QObject>
#include <QVariant>
#include <QQmlEngine>

namespace Asterindes
{
	class ProjectManager : public QObject
	{
		Q_OBJECT;

	public:

		/**
		 * Default constructor.
		 */
		using QObject::QObject;

		/**
		 * Destructor.
		 */
		~ProjectManager() override = default;

		/**
		 * Gets the resources manager of the project, it is used to manage the resources of the project and to display them in the UI.
		 * This reference is valid as long as the ProjectManager instance is alive.
		 */
		inline ResourcesManager& getResourcesManager() { return m_resourcesManager; }

		/**
		 * Loads a project from the given path, it is used to load the project when the user opens a project from the UI.
		 */
		void loadProject(const QString& projectPath);

	private:
		/**
		 * The resources manager of the project, it is used to manage the resources of the project and to display them in the UI.
		 */
		ResourcesManager m_resourcesManager{ this };
	};
}



#endif // PROJECTMANAGER_H
