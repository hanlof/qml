import QtQuick 2.6
import QtQuick.Controls 1.5

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: qsTr("Hello World")
    property real displayedTimeStart: sliderStart.value
    property real displayedTimeEnd: sliderEnd.value
    property real displayedTimeTotal: displayedTimeEnd - displayedTimeStart

    ListView {
        id: list
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        model: dataList
        spacing: 1
        height: mainWindow.height - 60
        property var colors: [ "#eeeeee", "#f8f8f8" ]
        delegate: Rectangle {
            height: 18
            color: list.colors[ index % list.colors.length ]
            //color: "#eeeeee"
            width: parent.width

            Rectangle {
                height: parent.height
                x: mainWindow.width * ((s - sliderStart.value) / mainWindow.displayedTimeTotal)
                property real rightX: mainWindow.width * ((e - sliderStart.value) / mainWindow.displayedTimeTotal)
                width: rightX - x > 1 ? rightX - x : 1
                color: "white"
                border.color: "black"
                border.width: 1
                Text {
                    text: name
                }
            }
        }
    }

    Rectangle {
        id: controlBar
        width: parent.width
        height: 60
        color: "lightblue"
        anchors.bottom: parent.bottom
        Row {
            anchors.centerIn: parent
            spacing: 5
            width: parent.width - spacing * 2
            Slider {
                id: sliderStart
                minimumValue: minMax.s
                maximumValue: minMax.e
                value: minimumValue
                width: (parent.width - parent.spacing) / 2
            }

            Slider {
                id: sliderEnd
                minimumValue: minMax.s
                maximumValue: minMax.e
                value: maximumValue
                width: (parent.width - parent.spacing) / 2
            }
        }
    }
//    Text { text: sliderStart.value + " " + sliderEnd.value }
}
