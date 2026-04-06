import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
	id: mainWindow
	width: 640
	height: 480
	visible: true
	
	Item {
		anchors.fill: parent

		ResourcesGridPanel {}
	}
}