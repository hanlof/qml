import QtQuick 2.6
import QtQuick.Controls 1.5

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    property real displayedTimeStart: sliderStart.value
    property real displayedTimeEnd: sliderEnd.value
    property real displayedTimeTotal: displayedTimeEnd - displayedTimeStart

    ListModel {
      id: testModel
      property real minTime: 1491313851.123363
      property real maxTime: 1491313852.329373
      ListElement { s: 3.5; e: 4.7 }
      ListElement { s: 9.5; e: 9.7 }
      ListElement { s: 4.5; e: 6.7 }
      ListElement { s: 7.5; e: 9.7 }
      ListElement { s: 5.5; e: 11.7 }
    }

    ListView {
        id: list
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        model: dataList
        spacing: 1
        height: mainWindow.height - 60
        property var colors: [ "darkblue", "blue" ]
        delegate: Rectangle {
            height: 18
         //   color: list.colors[ index % list.colors.length ]
            color: "#eeeeee"
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
            Slider {
                id: sliderStart
      minimumValue: 1491831099.5 // 1491313851.123363
      maximumValue: 1491831102.99 // 1491313852.429373
                //minimumValue: 0
                //maximumValue: 30
                value: testModel.minTime
                width: 600
            }
            Rectangle {
                width: 20
                height: 10
                color: "transparent"
            }

            Slider {
                id: sliderEnd
      minimumValue: 1491831099.5 // 1491313851.123363
      maximumValue: 1491831102.99 // 1491313852.429373
      //          minimumValue: 0
       //         maximumValue: 30
                value: testModel.maxTime
                width: 600
            }
        }
    }

}
