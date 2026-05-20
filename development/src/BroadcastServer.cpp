#include "BroadcastServer.h"

// Qt
#include <QFile>

using namespace Asterindes;

BroadcastServer::BroadcastServer(QObject* p_parent)
	: QObject(p_parent)
{
	m_httpServer->route("/", QHttpServerRequest::Method::Get,
		[](const QHttpServerRequest& p_request)
		{
			return QHttpServerResponse::fromFile(":/html/ClientPage.html");
		}
	);

	// Route to get the broadcasted resource, return only headers for an HEAD request.
	m_httpServer->route("/broadcasted-resource", QHttpServerRequest::Method::Head, [this]() { return getBroadcastResourceResponse(true); });
	m_httpServer->route("/broadcasted-resource", QHttpServerRequest::Method::Get, [this]() { return getBroadcastResourceResponse(false); });


	m_httpServer->addWebSocketUpgradeVerifier(
		m_httpServer,
		[](const QHttpServerRequest& p_request)
		{
			if (p_request.url().path() == "/ws")
			{
				return QHttpServerWebSocketUpgradeResponse::accept();
			}
			else
			{
				return QHttpServerWebSocketUpgradeResponse::passToNext();
			}
		}
	);

	QObject::connect(m_httpServer, &QHttpServer::newWebSocketConnection, this,
		[this]()
		{
			// Use release to transfer ownership to the connection list
			if (QWebSocket* l_connection{ m_httpServer->nextPendingWebSocketConnection().release() })
			{
				m_webSocketConnections.append(l_connection);

				QObject::connect(l_connection, &QWebSocket::disconnected, this,
					[this, l_connection]()
					{
						m_webSocketConnections.removeAll(l_connection);
						l_connection->deleteLater();
					}
				);
			}
		}
	);
}

BroadcastServer::~BroadcastServer()
{
	stop();
}

void BroadcastServer::setHostAddress(const QHostAddress& p_hostAddress)
{
	if (p_hostAddress != m_hostAddress)
	{
		m_hostAddress = p_hostAddress;
	}
}

void BroadcastServer::setServerPort(qint16 p_serverPort)
{
	if (p_serverPort != m_serverPort)
	{
		m_serverPort = p_serverPort;
	}
}

bool BroadcastServer::start()
{
	bool l_success{
		m_tcpServer->listen(m_hostAddress, m_serverPort)
	&&	m_httpServer->bind(m_tcpServer) };

	qInfo("Broadcast server started on %s:%d", qPrintable(m_hostAddress.toString()), m_serverPort);

	return l_success;
}

void BroadcastServer::stop()
{
	for (const auto& l_webSocketConnection : m_webSocketConnections)
	{
		l_webSocketConnection->deleteLater();
	}
	m_webSocketConnections.clear();

	if (m_tcpServer->isListening())
	{
		m_tcpServer->close();
	}
}

void BroadcastServer::setBroadcastResourceUrl(const QUrl& p_broadcastResourceUrl)
{
	if (p_broadcastResourceUrl != m_broadcastResourceUrl)
	{
		m_broadcastResourceUrl = p_broadcastResourceUrl;
		notifyBroadcastResourceChanged();
	}
}

// TODO: Refacto
// TODO: Proxy requests?
QHttpServerResponse BroadcastServer::getBroadcastResourceResponse(bool p_withoutContent) const
{
	using enum QHttpHeaders::WellKnownHeader;

	if (m_broadcastResourceUrl.isEmpty())
	{
		QHttpServerResponse l_response{ QHttpServerResponse::StatusCode::NoContent };
		return l_response;
	}
	else if (m_broadcastResourceUrl.isLocalFile())
	{
		QHttpServerResponse l_response{ QHttpServerResponse::fromFile(m_broadcastResourceUrl.toLocalFile()) };

		if (p_withoutContent)
		{
			QHttpServerResponse l_headersOnlyResponse{ l_response.statusCode() };
			l_headersOnlyResponse.setHeaders(l_response.headers());
			return l_headersOnlyResponse;
		}

		return l_response;
	}
	else
	{
		QHttpServerResponse l_response{ QHttpServerResponse::StatusCode::PermanentRedirect };

		QHttpHeaders l_headers;
		l_headers.append(Location, m_broadcastResourceUrl.toEncoded());

		l_response.setHeaders(l_headers);

		return l_response;
	}
}

void BroadcastServer::notifyBroadcastResourceChanged() const
{
	for (const auto l_webSocketConnection : m_webSocketConnections)
	{
		if (l_webSocketConnection)
		{
			l_webSocketConnection->sendTextMessage("broadcasted-resource-changed");
		}
	}
}
