
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// This file is part of the frequency-analyzer application.
// It is licensed to you under the terms of the MIT license.
// http://opensource.org/licenses/MIT
//
// Copyright (c) 2014 Timur Kristóf

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import hu.timur 1.0

ApplicationWindow {
    visible: true
    width: 790
    height: 500
    title: "Frequency analyzer"

    Row {
        id: topRow
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        spacing: 10

        Button {
            text: "Start"
            enabled: !plot.isStarted
            onClicked: {
                if (!plot.start()) {
                    startError.open();
                }
            }
        }

        Button {
            text: "Stop"
            enabled: plot.isStarted
            onClicked: {
                plot.stop();
            }
        }
    }

    Item {
        anchors.top: topRow.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: rightPanel.left
        anchors.bottom: parent.bottom

        Waterfall {
            id: plot
            sensitivity: sensitivitySlider.value
            anchors.fill: parent

            Rectangle {
                color: "#88222222"
                radius: 5
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 10
                width: minFreq.width + 10
                height: minFreq.height + 10

                Text {
                    id: minFreq
                    anchors.centerIn: parent
                    color: "#fff"
                    text: "← " + plot.minFrequency + " Hz"
                }
            }

            Rectangle {
                color: "#88222222"
                radius: 5
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 10
                width: maxFreq.width + 10
                height: maxFreq.height + 10

                Text {
                    id: maxFreq
                    anchors.centerIn: parent
                    color: "#fff"
                    text: plot.maxFrequency + " Hz →"
                }
            }
        }
    }

    Item {
        id: rightPanel
        anchors.top: topRow.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 130

        Column {
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            Text {
                text: "Sensitivity"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Slider {
                id: sensitivitySlider
                anchors.horizontalCenter: parent.horizontalCenter
                height: 180
                orientation: Qt.Vertical

                minimumValue: 0.005
                maximumValue: 0.1
                stepSize: 0.005
                value: 0.05
            }
        }
    }

    Rectangle {
        color: "#66222222"
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topRow.bottom
        anchors.topMargin: 10
    }

    MessageDialog {
        id: startError
        icon: StandardIcon.Critical
        title: "Error with sound card"
        text: "We couldn't start the audio input device. Please verify if audio input is configured correctly on your computer and it supports 44100Hz."
    }

}
