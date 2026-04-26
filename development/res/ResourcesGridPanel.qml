import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
	id: resourcesGridPanel
	//Layout.minimumWidth: 300
	Layout.fillWidth: true
	Layout.fillHeight: true

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
	
	ColumnLayout {
		anchors.fill: parent
		
		// ===== Toolbar with search and sort =====
		ToolBar {
			Layout.fillWidth: true
			
			RowLayout {
				anchors.fill: parent
				spacing: 10

				TextField {
					id: searchField
					Layout.fillWidth: true
					placeholderText: "Search resources..."
					
					// Bind to ViewModel's search filter
					text: "aa"//resourcesViewModel.searchFilter
					//onTextChanged: resourcesViewModel.searchFilter = text
				}

				ComboBox {
					id: sortComboBox
					Layout.preferredWidth: 150
					
					model: ["Name (A-Z)", "Name (Z-A)", "Date Added", "File Size"]
					
					// Bind to ViewModel's sort order
					//currentIndex: resourcesViewModel.sortOrder
					onCurrentIndexChanged: {
						//resourcesViewModel.sortOrder = currentIndex;
					}
				}

				Button {
					text: "Clear Filters"
					enabled: searchField.text !== "" || sortComboBox.currentIndex !== 0
					//onClicked: resourcesViewModel.clearFilters()
				}

				Label {
					text: "bb"/*resourcesViewModel.filteredCount */+ " / " + resourcesViewModel.displayedResourceListCount
					color: "blue"//resourcesViewModel.filteredCount < resourcesViewModel.displayedResourceListCount ? "blue" : "black"
				}
			}
		}
			
		// ===== Grid view =====
		DropArea {
			Layout.fillWidth: true
			Layout.fillHeight: true

			enabled: !resourcesViewModel.isLoading

			onEntered: (drag) => {
				let lCanDrop = true;
				if (drag.hasUrls) {
					for (let i = 0; i < drag.urls.length; i++) {
						if (!resourcesViewModel.canAddResource(drag.urls[i])) {
							lCanDrop = false;
							break;
						}
					}
				} else {
					lCanDrop = false;
				}
				drag.action = Qt.LinkAction;
				drag.accepted = lCanDrop;
			}

			onDropped: (drag) => {
				if (drag.hasUrls) {
					for (let i = 0; i < drag.urls.length; i++) {
						resourcesViewModel.addResource(drag.urls[i]);
					}
				}
			}

			GridView {
				id: resourceGridView
				anchors.fill: parent
				anchors.margins: 10

				// Bind to ViewModel's model (already filtered and sorted)
				model: resourcesViewModel.displayedResourceListModel

				cellWidth: 120
				cellHeight: 120

				delegate: resourceGridDelegate
				highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
				// No move animation when changing selection
				highlightMoveDuration: 0
				focus: true
				
				currentIndex: resourcesViewModel.selectedResourceId
				onCurrentIndexChanged: {
					resourcesViewModel.setSelectedResourceId(currentIndex);
				}

				// Handle clicks to select resources
				MouseArea {
					anchors.fill: parent
					onClicked: (mouse) => {
						let posInGridView = Qt.point(mouse.x, mouse.y)
						let posInContentItem = mapToItem(resourceGridView.contentItem, posInGridView)
						resourcesViewModel.setSelectedResourceId(resourceGridView.indexAt(posInContentItem.x, posInContentItem.y))
					}
				}

				// Empty state
				Label {
					anchors.centerIn: parent
					visible: parent.count === 0 && !resourcesViewModel.isLoading
					text: false//resourcesViewModel.searchFilter !== "" 
							? "No resources match your search"
							: "Drop images here or click 'Add Resource'"
					font.pixelSize: 16
					color: "gray"
				}
			}

			BusyIndicator {
				anchors.centerIn: parent
				running: resourcesViewModel.isLoading
				visible: running
			}
		}

		// ===== Bottom toolbar =====
		ToolBar {
			Layout.fillWidth: true
			
			RowLayout {
				anchors.fill: parent

				FileDialog {
					id: resourceFileDialog
					title: "Select Resource File"
					nameFilters: ["Images (*.png *.jpg *.jpeg *.webp)"]
					onAccepted: resourcesViewModel.addResource(selectedFile)
				}

				Button {
					text: "Add Resource"
					enabled: !resourcesViewModel.isLoading
					onClicked: resourceFileDialog.open()
				}

				Button {
					text: "Clear All"
					enabled: false//!resourcesViewModel.isLoading && resourcesViewModel.displayedResourceListCount > 0
					//onClicked: confirmClearDialog.open()
				}

				Item { Layout.fillWidth: true } // Spacer
			}
		}

		// Error dialog
		Connections {
			target: resourcesViewModel
			function onErrorOccurred(errorMessage) {
				errorDialog.text = errorMessage;
				errorDialog.open();
			}
		}

		Dialog {
			id: errorDialog
			title: "Error"
			property alias text: errorText.text
		
			Text { id: errorText }
			standardButtons: Dialog.Ok
		}
	}
}