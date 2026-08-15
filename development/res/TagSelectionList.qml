import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

pragma ComponentBehavior: Bound

Item {
	id: tagSelectionListRoot

	required property var selectedTagList
	required property var allTagList
	property bool canCreateNewTag: false

	signal tagSelected(string tagName, bool isSelected)
	signal newTagCreated(string tagName)

	property string mSearchText: ""

	function mIsTagSelected(pTagName) {
		return (selectedTagList ?? []).indexOf(pTagName) !== -1
	}

	function mMatchesSearch(pTagName) {
		if (!mSearchText || mSearchText.length === 0) {
			return true
		}
		return String(pTagName).toLowerCase().indexOf(mSearchText.toLowerCase()) !== -1
	}

	function mBuildSelectedTags() {
		const lResult = []
		for (let lIndex = 0; lIndex < (selectedTagList ?? []).length; lIndex++) {
			const lTag = selectedTagList[lIndex]
			if (mMatchesSearch(lTag)) {
				lResult.push(lTag)
			}
		}
		lResult.sort((a, b) => a.localeCompare(b))
		return lResult
	}

	function mBuildNotSelectedTags() {
		const lResult = []
		for (let lIndex = 0; lIndex < (allTagList ?? []).length; lIndex++) {
			const lTag = allTagList[lIndex]
			if ((selectedTagList ?? []).indexOf(lTag) === -1 && mMatchesSearch(lTag)) {
				lResult.push(lTag)
			}
		}
		lResult.sort((a, b) => a.localeCompare(b))
		return lResult
	}

	function mClearSearch() {
		mSearchText = ""
		tagSearchInput.clear()
	}

	anchors.fill: parent

	component TagRow: Rectangle {
		id: tagSelectionListDelegate
		
		required property string tagName
		property bool isTagToAdd: false
			
		property int horPadding: 6
		property int vertPadding: 2

		implicitWidth: tagCheckBox.implicitWidth + horPadding * 2
		implicitHeight: tagCheckBox.implicitHeight + vertPadding * 2

		radius: implicitHeight / 3
		color: "#cccccc"

		CheckBox {
			id: tagCheckBox

			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: parent.horPadding

			text: tagSelectionListDelegate.isTagToAdd ? "Add " + tagSelectionListDelegate.tagName + "..." : tagSelectionListDelegate.tagName
			font.italic: tagSelectionListDelegate.isTagToAdd
			checked: tagSelectionListRoot.mIsTagSelected(text)
			
			onVisibleChanged: {
				implicitIndicatorWidth: 16
				implicitIndicatorHeight: 16
			}

			onToggled: {
				tagSelectionListRoot.tagSelected(tagSelectionListDelegate.tagName, checked)
			}
		}
	}
	Component {
		id: tagRowDelegate
		TagRow {
			required property var modelData
			tagName: String(modelData)
		}
	}

	ColumnLayout {
		anchors.fill: parent
		
		spacing: 6

		// TODO: Add a "Select all" / "Deselect all" button
		TextField {
			id: tagSearchInput
			Layout.fillWidth: true
			placeholderText: "Search" + (tagSelectionListRoot.canCreateNewTag ? " or create" : "") + " tag"
			rightPadding: clearSearchButton.visible ? (clearSearchButton.implicitWidth + 10) : 6

			onTextChanged: {
				tagSelectionListRoot.mSearchText = text
			}

			onAccepted: {
				if (tagSearchInput.text.length > 0 && tagSelectionListRoot.canCreateNewTag) {
					tagSelectionListRoot.newTagCreated(tagSearchInput.text)
				}
			}

			ToolButton {
				id: clearSearchButton
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter
				
				width: parent.implicitHeight
				height: parent.implicitHeight

				visible: tagSearchInput.text.length > 0
				text: "✕"

				onClicked: {
					tagSearchInput.clear()
					tagSearchInput.forceActiveFocus()
				}
			}
		}
		
		ListView {
			id: selectedTagsListView
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.minimumHeight: 0
			Layout.preferredHeight: contentHeight
			Layout.maximumHeight: contentHeight
			
			clip: true
			spacing: 2
			model: tagSelectionListRoot.mBuildSelectedTags()
			delegate: tagRowDelegate
		}

		Rectangle {
			Layout.fillWidth: true
			Layout.preferredHeight: 1
			color: "gray"
		}

		ListView {
			id: notSelectedTagsListView
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.minimumHeight: 0
			Layout.preferredHeight: contentHeight
			Layout.maximumHeight: contentHeight

			clip: true
			spacing: 2
			model: tagSelectionListRoot.mBuildNotSelectedTags()
			delegate: tagRowDelegate
		}
		
		TagRow {
			id: newTagRow
			tagName: tagSearchInput.text.trim()
			isTagToAdd: true

			visible: tagSearchInput.text.trim().length > 0 && tagSelectionListRoot.canCreateNewTag && (tagSelectionListRoot.allTagList.indexOf(tagSearchInput.text.trim()) === -1)
		}

		// Spacer
		Item {
			Layout.fillWidth: true
			Layout.fillHeight: true
		}
	}
}