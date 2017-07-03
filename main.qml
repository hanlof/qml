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
    /*    MouseArea {
            anchors.fill: parent
            onWheel: function() {
                         sliderStart.value = sliderStart.minimumValue;
                         sliderStart.accessibleIncreaseAction(1);
                         return 0;
                     }
        } */


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
                x: parent.width * ((s - sliderStart.value) / mainWindow.displayedTimeTotal)
                property real rightX: parent.width * ((e - sliderStart.value) / mainWindow.displayedTimeTotal)
                width: rightX - x > 3 ? rightX - x : 3
                color: "white"
                border.color: "black"
                border.width: 1
                Text {
                    height: parent.height
                    text: Math.round( (e - s) * 1000) + "ms " + name
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    Repeater {
        property int lineSpacing: 100
        property int lineCount: Math.round(mainWindow.width / lineSpacing)

        property real timeSpacing: mainWindow.displayedTimeTotal / lineCount
        property real log: Math.ceil(Math.log(mainWindow.displayedTimeTotal, 10))

        id: rep
        model: lineCount

        delegate: Rectangle {
            color: "blue"
            x: modelData * rep.lineSpacing

            y: 0
            width: 1
            height: parent.height
            Text {
                height: childrenRect.height
                //x: parent.x + 3
                text: "hej: " + rep.log + " " + 1000 * index * Math.pow(10, rep.log) + "ms"
                color: "blue"
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
