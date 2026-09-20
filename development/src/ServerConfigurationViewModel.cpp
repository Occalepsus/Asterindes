#include "ServerConfigurationViewModel.h"

// Asterindes
#include "BroadcastServer.h"

// Qt
#include <QClipboard>
#include <QGuiApplication>
#include <QNetworkInterface>

using namespace Asterindes;
using namespace Asterindes::Ui;

ServerConfigurationViewModel::ServerConfigurationViewModel(BroadcastServer* p_broadcastServer, QObject* p_parent)
	: QObject(p_parent)
	, m_broadcastServer(p_broadcastServer)
{
	QObject::connect(m_broadcastServer, &BroadcastServer::serverPortChanged, this, [this](quint16 p_serverPort) { emit serverPortChanged(p_serverPort); });
	QObject::connect(m_broadcastServer, &BroadcastServer::serverStateChanged, this, [this](BroadcastServer::ServerState) { emit serverStateChanged(getServerState()); });
}

ServerConfigurationViewModel::~ServerConfigurationViewModel()
{
	QObject::disconnect(m_broadcastServer, &BroadcastServer::serverPortChanged, this, nullptr);
	QObject::disconnect(m_broadcastServer, &BroadcastServer::serverStateChanged, this, nullptr);
}

QString ServerConfigurationViewModel::getUsableIpAddress() const
{
	QHostAddress l_usableIpAddress{ QHostAddress::AnyIPv4 };

	for (const QNetworkInterface& l_interface : QNetworkInterface::allInterfaces())
	{
		// Break the loop if a usable IP address has been found
		if (l_usableIpAddress != QHostAddress::AnyIPv4)
		{
			break;
		}

		if (l_interface.flags().testFlag(QNetworkInterface::IsLoopBack)
			|| !l_interface.flags().testFlag(QNetworkInterface::IsUp))
		{
			continue;
		}

		for (const QNetworkAddressEntry& l_entry : l_interface.addressEntries())
		{
			if (QHostAddress l_ipAddress{ l_entry.ip() };
				l_ipAddress.protocol() == QAbstractSocket::IPv4Protocol && l_ipAddress != QHostAddress::AnyIPv4 && l_ipAddress != QHostAddress::LocalHost)
			{
				l_usableIpAddress = l_ipAddress;
			}
		}
	}

	// If no usable IP address was found, use the localhost address
	if (l_usableIpAddress == QHostAddress::AnyIPv4)
	{
		l_usableIpAddress = QHostAddress::LocalHost;
	}

	return l_usableIpAddress.toString();
}

void ServerConfigurationViewModel::copyTextToClipboard(const QString& p_text) const
{
	QGuiApplication::clipboard()->setText(p_text);
}

QString ServerConfigurationViewModel::getServerState() const
{
	QString l_stateString{ "No server" };

	if (m_broadcastServer)
	{
		switch (m_broadcastServer->getServerState())
		{
		case BroadcastServer::ServerState::Running:
			l_stateString = QString("Running on %1:%2").arg(getUsableIpAddress()).arg(m_broadcastServer->getServerPort());
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
