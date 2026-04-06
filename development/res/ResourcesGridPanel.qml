import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs

Item {
	id: resourcesGridPanel
	objectName: "resourcesGridPanel"
	anchors.fill: parent

	signal qmlResourceAddRequest(url resourceUrl)

	Component {
		id: resourceGridDelegate

		Column {
			width: resourceGridView.cellWidth
			height: resourceGridView.cellHeight

			Image {
				width: parent.width
				height: parent.height - 20
				anchors.horizontalCenter: parent.horizontalCenter

				fillMode: Image.PreserveAspectFit

				source: resourceUrl
			}

			Text {
				width: parent.width
				height: 20
				anchors.horizontalCenter: parent.horizontalCenter
				
				text: name
			}
		}
	}

	Column {
		anchors.fill: parent
		
		GridView {
			id: resourceGridView
			objectName: "resourceGridView"

			//model: resourcesListModel

			width: parent.width
			height: parent.height - addResourceButton.height

			cellWidth: 120
			cellHeight: 120

			delegate: resourceGridDelegate
		
			highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
			focus: true
		}

		FileDialog {
			id: resourceFileDialog
			title: "Select Resource File"
			nameFilters: ["Images (*.png *.jpg *.jpeg *.webp)"]

			onAccepted: resourcesGridPanel.qmlResourceAddRequest(selectedFile)
		}

		Button {
			id: addResourceButton
			text: "Add Resource"

			width: parent.width
			height: 42

			onClicked: {
				resourceFileDialog.open()
			}
		}
	}
}