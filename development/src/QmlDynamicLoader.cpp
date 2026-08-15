#include "QmlDynamicLoader.h"

// Qt
#include <QFileInfo>
#include <QDir>

namespace
{
	QString formatQmlErrors(const QList<QQmlError>& p_errors)
	{
		QStringList l_lines;

		for (const QQmlError& l_error : p_errors)
		{
			l_lines.append(QString("%1:%2:%3: %4")
				.arg(QDir::toNativeSeparators(l_error.url().toLocalFile()))
				.arg(l_error.line())
				.arg(l_error.column())
				.arg(l_error.description()));
		}

		return l_lines.join('\n');
	}
}

using namespace Asterindes::Ui;

QmlDynamicLoader::QmlDynamicLoader(QQmlApplicationEngine* p_qmlEngine, QObject* p_parent)
	: QObject(p_parent)
	, m_qmlEngine(p_qmlEngine)
{
	m_reloadTimer->setSingleShot(true);
	m_reloadTimer->setInterval(100);

	QObject::connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &QmlDynamicLoader::onQmlFileChanged);
	QObject::connect(m_reloadTimer, &QTimer::timeout, this, &QmlDynamicLoader::onReloadTimerTimeout);

	QObject::connect(m_qmlEngine, &QQmlApplicationEngine::objectCreated, this, &QmlDynamicLoader::onQmlFileLoaded);
	QObject::connect(m_qmlEngine, &QQmlApplicationEngine::warnings, this, [this](const QList<QQmlError>& p_warnings) {
		m_lastQmlError = formatQmlErrors(p_warnings);
	});
}

QmlDynamicLoader::~QmlDynamicLoader()
{
	clearQmlFile();

	QObject::disconnect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &QmlDynamicLoader::onQmlFileChanged);

	QObject::disconnect(m_qmlEngine, &QQmlApplicationEngine::objectCreated, this, &QmlDynamicLoader::onQmlFileLoaded);
}

bool QmlDynamicLoader::loadQmlFile(const QUrl& p_qmlFilePath)
{
	if (!m_currentQmlFilePath.isEmpty()) {
		qWarning() << "A QML file is already loaded. Please clear the current QML file before loading a new one by calling clearQmlFile().";
		return false;
	}

	if (!p_qmlFilePath.isLocalFile())
	{
		qCritical() << "The specified QML file is not a local file:" << p_qmlFilePath.toString();
		return false;
	}

	m_currentQmlFilePath = QFileInfo(p_qmlFilePath.toLocalFile()).absoluteFilePath();
	m_qmlEngine->load(m_currentQmlFilePath);

	m_fileWatcher->addPath(m_currentQmlFilePath);

	return true;
}

void QmlDynamicLoader::clearQmlFile()
{
	if (!m_currentQmlFilePath.isEmpty())
	{
		m_fileWatcher->removePath(m_currentQmlFilePath);
		m_currentQmlFilePath.clear();

		if (m_currentQmlObject)
		{
			m_currentQmlObject->deleteLater();
			m_currentQmlObject = nullptr;
		}
	}
}

void QmlDynamicLoader::onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url)
{
	if (p_url.toLocalFile() == m_currentQmlFilePath)
	{
		if (!p_qmlObject)
		{
			qCritical() << "Failed to load QML file:" << p_url.toLocalFile();
			emit qmlLoadError(m_lastQmlError);
		}
		else
		{
			qInfo() << "QML file loaded successfully:" << p_url.toLocalFile();
			
			if (m_currentQmlObject)
			{
				// Only delete the previous QML object if the new one is successfully loaded to avoid closing the application window.
				m_currentQmlObject->deleteLater();
			}

			m_currentQmlObject = p_qmlObject;
		}
	}
}

void QmlDynamicLoader::onQmlFileChanged(const QString& p_path)
{
	if (p_path == m_currentQmlFilePath)
	{
		m_reloadTimer->start();
	}
	else
	{
		qWarning() << "Received file change notification for a file that is not currently loaded:" << p_path;
	}
}

void QmlDynamicLoader::onReloadTimerTimeout()
{
	if (!m_currentQmlFilePath.isEmpty())
	{
		m_qmlEngine->clearComponentCache();
		m_qmlEngine->clearSingletons();
		m_qmlEngine->load(QUrl::fromLocalFile(m_currentQmlFilePath));

		// Re-add the file to the watcher (some editors delete/recreate files on save)
		m_fileWatcher->addPath(m_currentQmlFilePath);
	}
}