import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
	id: mainWindow
	width: 640
	height: 480
	visible: true

	RowLayout {
		anchors.fill: parent

		ResourcesGridPanel {}
		ResourcePreview {}
	}
}