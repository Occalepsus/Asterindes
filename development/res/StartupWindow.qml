import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

import QtCore

pragma ComponentBehavior: Bound

Window {
	id: startupWindow
	required property var startupWindowData
	required property var projectManagerService

	readonly property url defaultProjectPath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/AsterindesProjects"

	visible: startupWindowData ? startupWindowData.visible : false
	width: 640
	height: 480
	title: "Startup Window"

	onVisibilityChanged: {
		windowStack.clear()
		windowStack.push(mainStartupView)
	}

	onClosing: (close) => {
		// If the window is not visible, it means the window is being closed by the application, so we don't need to hide it again.
		if (!visible)
		{
			close.accepted = true
			return
		}

		console.log("StartupWindow closing, hide instead of close")
		if (startupWindowData)
		{
			// Calls the C++ function to hide the window instead of closing it, so that it can be reopened later. Set the close.accepted to false to prevent the window from closing.
			startupWindowData.hideStartupWindow()
			close.accepted = false
		}
	}

	StackView {
		id: windowStack
		anchors.fill: parent

		// Not used, the initialItem is set in the onVisibilityChanged
		initialItem: mainStartupView
	}


	Component {
		id: mainStartupView
		
		ColumnLayout {
			id: mainStartupLayout
			
			function openProject(file) {
				if (startupWindow.projectManagerService) {
					const res = startupWindow.projectManagerService.openProject(file)
					if (!res) {
						errorText.text = startupWindow.projectManagerService.errorString;
						errorText.visible = true;
					}
				}
			}

			RowLayout {
				Layout.fillWidth: true
				spacing: 10
		 
				Text {
					text: "Asterindes"
				}

				Button {
					id: newProjectButton
					text: "New Project"
					onClicked: {
						windowStack.push(newProjectView)
					}
				}

				Button {
					id: openProjectButton
					text: "Open Project"
					onClicked: {
						openProjectDialog.open()
					}
				}
			}

			ListView {
				id: recentProjectsListView

				Layout.fillWidth: true
				Layout.fillHeight: true

				model: startupWindow.projectManagerService ? startupWindow.projectManagerService.recentProjectList : []

				delegate: Item {
					id: recentProjectItem
					required property url modelData

					width: parent.width
					height: 40
					RowLayout {
						anchors.fill: parent
						spacing: 10
						Text {
							text: recentProjectItem.modelData.toString().split("/").pop()
							font.bold: true
						}
						Text {
							text: recentProjectItem.modelData.toString()
							color: "gray"
						}
					}
					MouseArea {
						anchors.fill: parent
						onClicked: {
							mainStartupLayout.openProject(recentProjectItem.modelData)
						}
					}
				}
			}

			Text {
				id: errorText
				color: "red"
				visible: false
			}
			
			FileDialog {
				id: openProjectDialog
				title: "Open Project"
				currentFolder: startupWindow.defaultProjectPath
				fileMode: FileDialog.OpenFile
				nameFilters: ["Asterindes Project (*.asterindesproj)"]
				onAccepted: {
					mainStartupLayout.openProject(selectedFiles[0])
				}
			}
		}
	}

	Component {
		id: newProjectView

		ColumnLayout {
			id: newProjectLayout

			property url projectPath: startupWindow.defaultProjectPath
			property string projectName: ""

			TextField {
				id: projectNameInput
				Layout.fillWidth: true

				placeholderText: "Project Name..."
				
				text: newProjectLayout.projectName
			
				onTextChanged: {
					newProjectLayout.projectName = text
				}
			}

			RowLayout {
				TextField {
					id: projectPathInput
					Layout.fillWidth: true

					// Remove "file:///" prefix from URL
					text: newProjectLayout.projectPath.toString().substring("file:///".length) 
					validator: RegularExpressionValidator { regularExpression: /^(.+)\/([^\/]+)$/ }

					onTextChanged: {
						newProjectLayout.projectPath = "file:///" + text
					}
				}

				Button {
					text: "..."
					onClicked: {
						newProjectDirectoryDialog.open()
					}
				}
			}

			Text {
				id: fullPathText
				text: projectPathInput.text + "/" + projectNameInput.text + ".asterindesproj"
			}

			RowLayout {
				Button {
					text: "Cancel"
					onClicked: {
						windowStack.pop()
					}
				}

				Button {
					text: "Create Project"
					onClicked: {
						if (startupWindow.projectManagerService) {
							const res = startupWindow.projectManagerService.createProject(newProjectLayout.projectPath + "/" + newProjectLayout.projectName + ".asterindesproj")
							if (!res) {
								errorText.text = startupWindow.projectManagerService.errorString;
								errorText.visible = true;
							}
						}
					}
				}
			}

			Text {
				id: errorText
				color: "red"
				visible: false
			}

			FolderDialog {
				id: newProjectDirectoryDialog
				title: "Select Project Directory"
				currentFolder: startupWindow.defaultProjectPath
				onAccepted: {
					newProjectLayout.projectPath = selectedFolder
				}
			}
		}
	}
}