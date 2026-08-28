import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore

ApplicationWindow {
	id: mainWindow
	required property var projectWindow
	required property var projectManagerService
	property var qmlLoader
	
	Settings {
		id: uiSettings
		category: "MainWindow"
		property int x
		property int y
		property int width: 800
		property int height: 600

		property string fullscreenMode: "windowed"
	}
	
	function saveWindowGeometry() {
		if (mainWindow.visibility === Window.Windowed)
		{
			uiSettings.x = mainWindow.x;
			uiSettings.y = mainWindow.y;
			uiSettings.width = mainWindow.width;
			uiSettings.height = mainWindow.height;
		}
	}
	
	Timer {
		id: saveGeometryTimer
		interval: 200
		repeat: false
		onTriggered: mainWindow.saveWindowGeometry()
	}
	
	function posOrSizeChanged() {
		saveGeometryTimer.restart();
	}

	x: uiSettings.x
	y: uiSettings.y
	width: uiSettings.width
	height: uiSettings.height
	
	onXChanged: posOrSizeChanged()
	onYChanged: posOrSizeChanged()
	onWidthChanged: posOrSizeChanged()
	onHeightChanged: posOrSizeChanged()

	visibility: uiSettings.fullscreenMode === "maximized" ? Window.Maximized : Window.Windowed
	onVisibilityChanged: (visibility) => {
		if (visibility === Window.Maximized || visibility === Window.FullScreen)
		{
			uiSettings.fullscreenMode = "maximized";
		}
		else if (visibility === Window.Windowed)
		{
			uiSettings.fullscreenMode = "windowed";
		}
	}
	
	Dialog {
		id: qmlLoadErrorDialog
		property string errorString: "unknown error"
		
		title: "Hot reload failed"

		ColumnLayout {
			Text {
				text: "The following QML errors happened:"
			}

			Text {
				id: errorText
				text: ""
				color: "darkred"

			}
		}
		
		onErrorStringChanged: {
			errorText.text = "- " + errorString.replace("\n", "\n- ")
		}
	}
	
	Connections {
		target: mainWindow.qmlLoader
		
		function onQmlLoadError(errorString) {
			console.log(errorString)
			qmlLoadErrorDialog.errorString = errorString
			qmlLoadErrorDialog.open()
		}
	}
	
	//menuBar: AsterindesMenuBar {}

	RowLayout {
		anchors.fill: parent

		Item {
			Layout.fillWidth: true
			Layout.fillHeight: true
			ResourcesGridPanel {}
		}

		ColumnLayout {
			Layout.fillWidth: true
			Layout.fillHeight: true

			Item {
				Layout.fillWidth: true
				Layout.fillHeight: true
				ResourcePreviewPanel {}
			}

			Item {
				Layout.fillWidth: true
				Layout.fillHeight: true
				ResourceBroadcastPanel {}
			}
		}
	}

	footer: AsterindesMenuBar {}
}