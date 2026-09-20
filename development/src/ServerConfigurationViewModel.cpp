#include "ServerConfigurationViewModel.h"

// Asterindes
#include "BroadcastServer.h"

// Qt
#include <QNetworkInterface>

using namespace Asterindes;
using namespace Asterindes::Ui;

ServerConfigurationViewModel::ServerConfigurationViewModel(BroadcastServer* p_broadcastServer, QObject* p_parent)
	: QObject(p_parent)
	, m_broadcastServer(p_broadcastServer)
{
	QObject::connect(m_broadcastServer, &BroadcastServer::serverAddressChanged, this, [this](const QHostAddress& p_hostAddress) { emit serverAddressChanged(p_hostAddress); });
	QObject::connect(m_broadcastServer, &BroadcastServer::serverPortChanged, this, [this](quint16 p_serverPort) { emit serverPortChanged(p_serverPort); });
	QObject::connect(m_broadcastServer, &BroadcastServer::serverStateChanged, this, [this](BroadcastServer::ServerState) { emit serverStateChanged(getServerState()); });
}

ServerConfigurationViewModel::~ServerConfigurationViewModel()
{
	QObject::disconnect(m_broadcastServer, &BroadcastServer::serverAddressChanged, this, nullptr);
	QObject::disconnect(m_broadcastServer, &BroadcastServer::serverPortChanged, this, nullptr);
	QObject::disconnect(m_broadcastServer, &BroadcastServer::serverStateChanged, this, nullptr);
}

QHash<QString, QVariant> ServerConfigurationViewModel::getAvailableIpAddresses() const
{
	QHash<QString, QVariant> l_ipAddresses{ {"All interfaces (0.0.0.0)", QVariant("0.0.0.0")}};

	for (const QNetworkInterface& l_interface : QNetworkInterface::allInterfaces())
	{
		for (const QNetworkAddressEntry& l_entry : l_interface.addressEntries())
		{
			if (l_entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
			{
				l_ipAddresses.insert(QString("%1 (%2)").arg(l_interface.humanReadableName()).arg(l_entry.ip().toString()), l_entry.ip().toString());
			}
		}
	}

	return l_ipAddresses;
}

QString ServerConfigurationViewModel::getServerState() const
{
	QString l_stateString{ "No server" };

	if (m_broadcastServer)
	{
		switch (m_broadcastServer->getServerState())
		{
		case BroadcastServer::ServerState::Running:
			l_stateString = QString("Running on %1:%2").arg(m_broadcastServer->getHostAddress().toString()).arg(m_broadcastServer->getServerPort());
			break;
		case BroadcastServer::ServerState::Stopped:
			l_stateString = "Stopped";
			break;
		default:
			l_stateString = QString("Error: %1").arg(m_broadcastServer->getLastErrorString());
			break;
		}
	}

	return l_stateString;
}
