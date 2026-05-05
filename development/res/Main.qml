import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
	id: mainWindow
	width: 640
	height: 480
	visible: true

	menuBar: AsterindesMenuBar {}

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
}