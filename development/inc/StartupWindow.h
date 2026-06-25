#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

// Asterindes

// Asterindes UI

// Qt
#include <QObject>

namespace Asterindes::Ui
{
	/**
	 * StartupWindow is the class that handles the startup window of the application.
	 * It is responsible of telling the application to open a project or create a new one.
	 */
	class StartupWindow : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(StartupWindow);

	public:

		/**
		 * Default constructor.
		 * 
		 * @param parent The parent QObject, default is nullptr.
		 */
		//StartupWindow(QObject* parent = nullptr);
		using QObject::QObject;

		/**
		 * Destructor.
		 */
		~StartupWindow() final = default;



	public slots:

	private:

	};
}


#endif // !STARTUPWINDOW_H
