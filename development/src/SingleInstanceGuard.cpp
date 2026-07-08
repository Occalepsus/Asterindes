#include "SingleInstanceGuard.h"

// Asterindes
#include "AsterindesCore.h"

using namespace Asterindes;

SingleInstanceGuard::SingleInstanceGuard(AsterindesCore* p_coreApp, QObject* p_parent)
	: QObject(p_parent)
	, m_coreApp(p_coreApp)
{
}

SingleInstanceGuard::~SingleInstanceGuard()
{
	m_localServer->close();
}

bool SingleInstanceGuard::checkAndSendIfRunning(const QString& p_projectPath)
{
	QLocalSocket l_clientSocket;

	l_clientSocket.connectToServer(msc_serverName);
	if (l_clientSocket.waitForConnected(2000))
	{
		// Another instance is running, send the project path to it.
		QByteArray l_dataToSend(!p_projectPath.isEmpty() ? p_projectPath.toUtf8() : QString("none").toUtf8());
		l_clientSocket.write(l_dataToSend);
		l_clientSocket.flush();
		l_clientSocket.waitForBytesWritten(500);
		l_clientSocket.disconnectFromServer();
		return true;
	}
	else
	{
		qInfo() << "Failed to connect to another instance: " << l_clientSocket.errorString() << ". Starting normally.";
		return false;
	}
}

void SingleInstanceGuard::startListeningForConnections()
{
	QObject::connect(m_localServer, &QLocalServer::newConnection, this, &SingleInstanceGuard::onNewConnection);

	m_localServer->listen(msc_serverName);
}

void SingleInstanceGuard::onNewConnection()
{
	// If there is already a connected socket, we ignore the new connection and wait for the current one to be processed.
	if (m_connectedSocket)
	{
		return;
	}

	m_connectedSocket = m_localServer->nextPendingConnection();

	QObject::connect(m_connectedSocket, &QLocalSocket::disconnected, [this]()
		{
			m_connectedSocket->deleteLater();
			m_connectedSocket = nullptr;
		}
	);

	QObject::connect(m_connectedSocket, &QLocalSocket::readyRead, [this]()
		{
			QString l_receivedProjectPath{ QString::fromUtf8(m_connectedSocket->readAll()) };
		
			// TODO: Multiple instances are not supported yet.
			//if (l_receivedProjectPath == "none")
			//{
			//	if (m_coreApp)
			//	{
			//		m_coreApp->openStartupWindow();
			//	}
			//}
			//else
			//{
			//	if (m_coreApp)
			//	{
			//		m_coreApp->openProject(QUrl::fromUserInput(l_receivedProjectPath));
			//	}
			//}
		}
	);
}
