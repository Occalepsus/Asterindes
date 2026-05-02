import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
	id: resourcesGridPanel

	anchors.fill: parent

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
					text: "bb"/*resourcesViewModel.filteredCount */+ " / " + (resourcesViewModel ? resourcesViewModel.displayedResourceListCount : 0)
					color: "blue"//resourcesViewModel.filteredCount < resourcesViewModel.displayedResourceListCount ? "blue" : "black"
				}
			}
		}
			
		// ===== Grid view =====
		DropArea {
			Layout.fillWidth: true
			Layout.fillHeight: true

			enabled: resourcesViewModel ? !resourcesViewModel.isLoading : true

			onEntered: (drag) => {
				let lCanDrop = true;
				if (resourcesViewModel && drag.hasUrls) {
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
				if (resourcesViewModel && drag.hasUrls) {
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
				model: resourcesViewModel ? resourcesViewModel.displayedResourceListModel : null

				cellWidth: 120
				cellHeight: 120

				delegate: resourceGridDelegate
				highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
				// No move animation when changing selection
				highlightMoveDuration: 0
				focus: true
				
				currentIndex: resourcesViewModel ? resourcesViewModel.selectedResourceIndex : -1
				onCurrentIndexChanged: {
					if (resourcesViewModel) {
						resourcesViewModel.setSelectedResourceIndex(currentIndex);
					}
				}

				// Handle clicks to select resources
				MouseArea {
					anchors.fill: parent
					onClicked: (mouse) => {
						if (resourcesViewModel) {
							let posInGridView = Qt.point(mouse.x, mouse.y)
							let posInContentItem = mapToItem(resourceGridView.contentItem, posInGridView)
							resourcesViewModel.setSelectedResourceIndex(resourceGridView.indexAt(posInContentItem.x, posInContentItem.y))
						}
					}
					onDoubleClicked: (mouse) => {
						if (resourcesViewModel) {
							let posInGridView = Qt.point(mouse.x, mouse.y)
							let posInContentItem = mapToItem(resourceGridView.contentItem, posInGridView)
							let index = resourceGridView.indexAt(posInContentItem.x, posInContentItem.y)

							if (index >= 0) {
								let resource = resourcesViewModel.getResourceAtIndex(index);
								if (resource && resource.resourceUrl) {
									resourcesViewModel.setBroadcastedResourceUrl(resource.resourceUrl);
								}
							}
						}
					}
				}

				// Empty state
				Label {
					anchors.centerIn: parent
					visible: parent.count === 0 && (resourcesViewModel ? !resourcesViewModel.isLoading : true)
					text: false//resourcesViewModel.searchFilter !== "" 
							? "No resources match your search"
							: "Drop images here or click 'Add Resource'"
					font.pixelSize: 16
					color: "gray"
				}
			}

			BusyIndicator {
				anchors.centerIn: parent
				running: resourcesViewModel ? resourcesViewModel.isLoading : false
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
					onAccepted: {
						if (resourcesViewModel) {
							resourcesViewModel.addResource(selectedFile)
						}
					}
				}

				Button {
					text: "Add Resource"
					enabled: resourcesViewModel ? !resourcesViewModel.isLoading : false
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