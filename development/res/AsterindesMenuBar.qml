import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
	id: menuBar

	implicitHeight: barLayout.implicitHeight

	Rectangle {
		anchors.fill: parent

		color: "grey"

		RowLayout {
			id: barLayout
			Button {
				id: startupWindowButton
				text: "🏠"
				onClicked: {
					// TODO: Implement navigation to the startup window
				}
			}
		}
	}
}