#ifndef QMLDYNAMICLOADER_H
#define QMLDYNAMICLOADER_H

// Qt
#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QTimer>

namespace Asterindes::Ui
{
	/**
	 * QmlDynamicLoader is a class responsible for loading QML components and handling the hot reloading (only for debug builds).
	 */
	class QmlDynamicLoader : public QObject
	{
		Q_OBJECT;

	public:

		/**
		 * Default constructor.
		 *
		 * @param p_qmlEngine Pointer to the QQmlApplicationEngine used for loading QML components.
		 * @param p_parent Pointer to the parent QObject.
		 */
		explicit QmlDynamicLoader(QQmlApplicationEngine* p_qmlEngine, QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~QmlDynamicLoader() override;

		/**
		 * Loads a QML file by its file path and watches for changes to enable hot reloading.
		 *
		 * @param p_qmlFile The local file path of the QML file to load.
		 * 
		 * @return False if a QML file is already loaded or the QML file path is invalid, true otherwise.
		 */
		bool loadQmlFile(const QUrl& p_qmlFilePath);

		/**
		 * Clears the currently loaded QML file and stops watching for changes.
		 * Destroy the QML component and free up resources.
		 */
		void clearQmlFile();

	signals:

		/**
		 * Signal emitted when a QML file is successfully loaded.
		 *
		 * @param p_url The URL of the loaded QML file.
		 */
		void qmlLoadError(const QString& p_errorString);

	private:

		/**
		 * Watches for changes in QML files and triggers reloads when changes are detected.
		 */
		QFileSystemWatcher* m_fileWatcher{ new QFileSystemWatcher(this) };

		/**
		 * The QML engine for loading and managing QML components, given by the constructor.
		 */
		QPointer<QQmlApplicationEngine> m_qmlEngine;

		/**
		 * The current QML file being loaded and watched for changes.
		 */
		QString m_currentQmlFilePath;

		/**
		 * The current QML object loaded from the QML file. It is a QPointer to ensure safe deletion and avoid dangling pointers.
		 */
		QPointer<QObject> m_currentQmlObject;

		/**
		 * Timer used to delay the reload of the QML file when changes are detected. This is to avoid multiple reloads in quick succession.
		 */
		QTimer* m_reloadTimer{ new QTimer(this) };

		/**
		 * The formatted string containing the last error in the QML
		 */
		QString m_lastQmlError{ "" };

	private slots:

		/**
		 * Slot triggered when a QML file is loaded. It is used to check if the QML failed to load or keep a reference on it otherwise.
		 */
		void onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url);

		/**
		 * Slot triggered when a QML file changes. It reloads the QML file to reflect the changes.
		 *
		 * @param p_path The path of the changed QML file.
		 */
		void onQmlFileChanged(const QString& p_path);

		/**
		 * Slot triggered when the reload timer times out. It reloads the QML file to reflect the changes.
		 */
		void onReloadTimerTimeout();

	};
}

#endif // !QMLDYNAMICLOADER_H
