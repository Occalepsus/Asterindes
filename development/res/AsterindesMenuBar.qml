import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
	id: menuBarRoot

	implicitHeight: barLayout.implicitHeight

	signal serverConfigurationClicked()

	Rectangle {
		anchors.fill: parent

		color: "grey"

		RowLayout {
			id: barLayout
			Button {
				id: startupWindowButton
				text: "🏠"
				onClicked: {
					projectWindow.showStartupWindow()
				}
			}

			// Spacer
			Item {
				Layout.fillWidth: true
			}

			Button {
				id: serverConfigurationButton
				
				function getServerStateText() {
					let text = "No server";

					if (projectWindow) {
						text = projectWindow.serverConfiguration.serverState;

						if (!text.startsWith("Running") && text !== "Stopped") {
							text = "Error";
						}
					}

					return text;
				}

				text: "🌐 : " + getServerStateText()
				onClicked: {
					menuBarRoot.serverConfigurationClicked()
				}
			}
		}
	}
}