#include "BroadcastServer.h"

// Qt
#include <QFile>

using namespace Asterindes;

namespace
{
	const QByteArray sc_htmlContent{
R"(<!DOCTYPE html>
<html>
	<body>
		<h1>Welcome to Asterindes!</h1>
		<img src="/broadcasted-resource" alt="Broadcasted Resource" />
	</body>
</html>
)"
	};
}

BroadcastServer::BroadcastServer(QObject* p_parent)
	: QObject(p_parent)
{
	m_httpServer->route("/", QHttpServerRequest::Method::Get,
		[](const QHttpServerRequest& p_request)
		{
			return QHttpServerResponse("text/html", sc_htmlContent);
		}
	);

	m_httpServer->route("/broadcasted-resource", QHttpServerRequest::Method::Get,
		[](const QHttpServerRequest& p_request)
		{
			QFile l_test(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 3\Illustrations\Asterindes allongé.jpg)");
			if (l_test.open(QIODevice::ReadOnly))
			{
				QByteArray l_data = l_test.readAll();
				return QHttpServerResponse("image/jpeg", l_data);
			}

			return QHttpServerResponse("Hi there!");
		}
	);


	//// --- WebSocket Upgrade ---
	//// Filter: only allow upgrades on "/ws"
	//m_httpServer->addWebSocketUpgradeVerifier(&m_httpServer,
	//	[](const QHttpServerRequest& pRequest) {
	//		return pRequest.url().path() == "/ws";
	//	}
	//);

	//// Handle new WebSocket connections
	//QObject::connect(&m_httpServer, &QHttpServer::newWebSocketConnection, [&]() {
	//	QWebSocket* lClient = lHttpServer.nextPendingWebSocketConnection();
	//	lClients.append(lClient);

	//	qInfo() << "New WS client connected:" << lClient->peerAddress();

	//	QObject::connect(lClient, &QWebSocket::textMessageReceived,
	//		[lClient, &lClients](const QString& pMessage) {
	//			for (QWebSocket* lOther : lClients)
	//				lOther->sendTextMessage("Echo: " + pMessage);
	//		});

	//	QObject::connect(lClient, &QWebSocket::disconnected, [lClient, &lClients]() {
	//		qInfo() << "WS client disconnected";
	//		lClients.removeAll(lClient);
	//		lClient->deleteLater();
	//		});
	//	});

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
	if (m_tcpServer->isListening())
	{
		m_tcpServer->close();
	}
}