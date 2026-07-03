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

	readonly property url defaultProjectPath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/AsterindesProjects"

	visible: startupWindowData ? startupWindowData.visible : false
	width: 640
	height: 480
	title: "Startup Window"

	StackView {
		id: windowStack
		anchors.fill: parent

		initialItem: mainStartupView
	}


	Component {
		id: mainStartupView
		
		ColumnLayout {
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

				model: startupWindow.startupWindowData ? startupWindow.startupWindowData.recentProjects : []

				delegate: Item {
					width: parent.width
					height: 40
					RowLayout {
						anchors.fill: parent
						spacing: 10
						Text {
							text: name
							font.bold: true
						}
						Text {
							text: path
							color: "gray"
						}
					}
					MouseArea {
						anchors.fill: parent
						onClicked: {
							startupWindow.startupWindowData.openProject(path)
						}
					}
				}
			}
			
			FileDialog {
				id: openProjectDialog
				title: "Open Project"
				currentFolder: startupWindow.defaultProjectPath
				fileMode: FileDialog.OpenFile
				nameFilters: ["Asterindes Project (*.asterindesproj)"]
				onAccepted: {
					if (startupWindow.startupWindowData) {
						startupWindow.startupWindowData.openProject(selectedFiles[0])
					}
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
				text: projectPathInput.text + "/" + projectNameInput.text
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
						if (startupWindow.startupWindowData) {
							startupWindow.startupWindowData.createProject(projectNameInput.text)
						}
					}
				}
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