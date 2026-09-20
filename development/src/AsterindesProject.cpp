#include "AsterindesProject.h"

// Asterindes
#include "AsterindesCore.h"
#include "BroadcastServer.h"

// Qt
#include <QFile> // Used for file reading
#include <QSaveFile> // Used for safe file writing
#include <QJsonDocument>
#include <QTimer>

using namespace Asterindes;

//TODO handle invalid url
AsterindesProject::AsterindesProject(const QUrl& p_projectPath, AsterindesCore* p_parent)
	: QObject(p_parent)
	, m_projectPath(p_projectPath)
{
	QObject::connect(m_resourcesRegistry, &ResourceRegistry::resourcesChanged, this, &AsterindesProject::saveProject);

	QObject::connect(m_broadcastServer, &BroadcastServer::serverPortChanged, this, [this](quint16) { this->saveProject(); });
}

AsterindesProject::~AsterindesProject()
{

}

bool AsterindesProject::loadProject()
{
	Q_ASSERT(m_resourcesRegistry);
	Q_ASSERT(m_broadcastServer);

	if (m_isLoaded)
	{
		qWarning("Project is already loaded: %s", qUtf8Printable(m_projectPath.toString()));
		return false;
	}

	if (!m_projectPath.isLocalFile())
	{
		return false;
	}

	QJsonObject l_projectData;

	if (QFile l_projectFile{ m_projectPath.toLocalFile() };
		l_projectFile.open(QIODevice::ReadOnly))
	{
		l_projectData = QJsonDocument::fromJson(l_projectFile.readAll()).object();
	}
	else
	{
		qCritical("Failed to open project file: %s", qUtf8Printable(m_projectPath.toLocalFile()));
		return false;
	}

	// Load resources from the project data
	if (!l_projectData.contains("resources") || !l_projectData["resources"].isArray())
	{
		qCritical("Invalid project file format: missing 'resources' array");
		return false;
	}
	bool l_resourcesLoaded{ m_resourcesRegistry->loadResourcesFromJson(l_projectData["resources"].toArray()) };

	// Load server port from the project data
	if (!l_projectData.contains("server_port") || !l_projectData["server_port"].isDouble())
	{
		qWarning("Invalid project file format: missing 'server_port' value, using default port");
		m_broadcastServer->setServerPort(BroadcastServer::msc_defaultServerPort);
	}
	else
	{
		quint16 l_serverPort{ static_cast<quint16>(l_projectData["server_port"].toInt(BroadcastServer::msc_defaultServerPort)) };
		m_broadcastServer->setServerPort(l_serverPort);
		m_broadcastServer->start();
	}

	m_isLoaded = l_resourcesLoaded;

	return m_isLoaded;
}

bool AsterindesProject::saveProject() const
{
	if (!m_isLoaded)
	{
		qWarning("Project is not loaded, cannot save: %s", qUtf8Printable(m_projectPath.toString()));
		return false;
	}

	QJsonArray l_resourcesJsonArray(m_resourcesRegistry->getResourcesAsJson());

	QJsonObject l_projectJsonObject{
		{ "resources", l_resourcesJsonArray },
		{ "server_port", m_broadcastServer ? m_broadcastServer->getServerPort() : BroadcastServer::msc_defaultServerPort }
	};

	if (QSaveFile l_projectFile{ m_projectPath.toLocalFile() };
		l_projectFile.open(QIODevice::WriteOnly))
	{
		l_projectFile.write(QJsonDocument(l_projectJsonObject).toJson());
		if (!l_projectFile.commit())
		{
			qCritical("Failed to save project file: %s", qUtf8Printable(m_projectPath.toString()));
			return false;
		}
	}
	else
	{
		qCritical("Failed to open project file for writing: %s", qUtf8Printable(m_projectPath.toString()));
		return false;
	}

	return true;
}
