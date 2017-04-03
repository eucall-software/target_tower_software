import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QmlTango 1.0 // import types from the plugin

TopLevelColumn
{
   id: root
   deviceName: "test/camera/1"

   property string savedSelectedCamera: ""
   property string savedSelectedMode: ""

   Item{
      CommandExecuter{ // The abort command excecuter
        id: abortCmd
        deviceName: root.deviceName
        commandName: 'abort'
      }

      StateReader{
         id: statusReader
         deviceName: root.deviceName

         // The function is used only with str === value, but when value was not a parameter,
         // for an unknown reason, the old value was used here during onValueChanged.
         function stringContains(str, subRegex)
         {
            return (typeof str !== 'undefined' && str.search(subRegex) >= 0)
         }

         property var prevValue: undefined
         property bool isInit: stringContains(value, 'INIT') // in init state
         property bool isDisable: stringContains(value, 'DISABLE') // is configured
         property bool isClose: stringContains(value, 'CLOSE')  // is homing
         property bool isStandby: stringContains(value, 'STANDBY') // is homed and ready to move
         property bool isOpen: stringContains(value, 'OPEN') // is analysing
         property bool isRunning: stringContains(value, 'RUNNING') // is aligning
         property bool isMoving: stringContains(value, 'MOVING') // in move -> abs, rel, run commands

         property bool isCameraConnected: stringContains(value, 'DISABLE|CLOSE|OPEN|STANDBY|RUNNING|MOVING')
         property bool isServerConnected: stringContains(value, 'INIT|DISABLE|CLOSE|OPEN|STANDBY|RUNNING|MOVING')
         property bool isMovementReady: stringContains(value, 'STANDBY')
         property bool isAbortReady: stringContains(value, 'CLOSE|OPEN|RUNNING|MOVING')
         property bool isConfigurationPossible: stringContains(value, 'INIT')
         property bool isAnalysePossible: isHolderAligned && isStandby
         property bool isCenteringPossible: isHolderAnalyzed && isStandby
         // This also occurs when the device name changes, because the Status briefly changes to undefined.
         property bool isHolderAligned: false
         property bool isHolderAligning: false
         property bool isHolderAnalyzed: false
         property bool isHolderAnalyzing: false
         onValueChanged:
         {
            if(typeof prevValue !== 'undefined' && prevValue === value)
               return
            prevValue = value
         }
      }
   }

   Column{

    AttributeLabel{
      id: status
      attributeName: "Status"
      deviceName: root.deviceName
//      Layout.row: 1
//      Layout.column: 1
      unitVisible: false
      valueLabel.width: 150
      color: nameDevice.color
      valueLabel.font.pixelSize: 10
      infoLabel.font.pixelSize: 10
   }

    RowLayout{ // image + buttons on the right next to image

        ImageViewer{
        id: imageViewer
//      Layout.row: 2
//      Layout.column: 1
        attributeName: "cameraFeed"
        deviceName: root.deviceName
        requestedHeight: 300
        requestedWidth: 700
        pixelFormat: ""

        }

        Rectangle{
            id: rectTower
            width: 250
            anchors.top: imageViewer.top
            anchors.topMargin: 25
            color: "white"
            border.width: 1

            CommandExecuter{ // defined in C++
               id: alignExecuter
               deviceName: root.deviceName
               commandName: 'align'
            }

            //----------------------------------------------------
            Button{ // load Conf button
                id: loadConfigurationButton
                width: 200
                height: 48
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.topMargin: 5
                enabled: statusReader.isConfigurationPossible
                text: "Load Configuration"

                FileDialog {
                    id: qmlFileDialog
                    modality: Qt.WindowModal
                    nameFilters: ["XML File (*.xml)"]
                    selectMultiple: false

                    property string curStr: "" // just for processing

                    onAccepted: {
                       curStr = fileUrl; //get the path
                       curStr = curStr.replace("file:///home/", "/home/") // remove the prefix
                       if(curStr.length >= 0){
                             //confError.visible = false;
                             result = loadConfExecuter.runWithParam(curStr)
                             if(!result){
//                                 confError.visible = true;
//                                 confError.color = "red";
                             }else{
//                                 confError.visible = true;
//                                 confError.color = "green";
                             }
                       }
                    }
                    onRejected: {
                    }
                }
                onClicked:{
                    qmlFileDialog.open();
                }

                CommandExecuter{ // defined in C++
                   id: loadConfExecuter
                   deviceName: root.deviceName
                   commandName: 'loadConfiguration'
                }

            }
            //----------------------------------------------------
            Button{ // Home button
                id: homeButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: loadConfigurationButton.bottom
                anchors.topMargin: 5
                enabled: statusReader.isDisable
                text: "Homing"

                CommandExecuter{ // defined in C++
                   id: homingExecuter
                   deviceName: root.deviceName
                   commandName: 'home'
                }

                onClicked: {
                    homingExecuter.runAsynch();
                }
            }
            //----------------------------------------------------
            Button{ // Align button
                id: alignButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: homeButton.bottom
                anchors.topMargin: 5
                enabled: statusReader.isStandby
                text: "Align Frame"

                onClicked: {
                    var movement = ["0","0","0"];
                    if(alignX.checked){
                        movement[0] = "1";
                    }
                    if(alignY.checked){
                        movement[1] = "1";
                    }

                    alignExecuter.runAsynchWithParam(movement)
                }
            }
            CheckBox{
                id: alignX;
                anchors.left: alignButton.right
                anchors.leftMargin: 5
                anchors.top: alignButton.top
                anchors.topMargin: 5

                checked:  false;
                text: "Align X"
            }
            CheckBox{
                id: alignY;
                anchors.left: alignButton.right
                anchors.leftMargin: 5
                anchors.bottom: alignButton.bottom
                anchors.bottomMargin: 5

                checked:  true;
                text: "Align y"
            }
            //----------------------------------------------------
            Button{ // Align button
                id: alignHolderButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: alignButton.bottom
                anchors.topMargin: 5
                enabled: statusReader.isStandby
                text: "Align Holder"

                onClicked: {
                    var movement = ["0","0","1"];
                    if(alignHolderX.checked){
                        movement[0] = "1";
                        statusReader.isHolderAligned = true;
                        statusReader.isHolderAligning = true;
                    }
                    if(alignHolderY.checked){
                        movement[1] = "1";
                        statusReader.isHolderAligned = true;
                        statusReader.isHolderAligning = true;
                    }

                    alignExecuter.runAsynchWithParam(movement)
                }
            }
            CheckBox{
                id: alignHolderX;
                anchors.left: alignHolderButton.right
                anchors.leftMargin: 5
                anchors.top: alignHolderButton.top
                anchors.topMargin: 5

                checked:  false;
                text: "Align X"
            }
            CheckBox{
                id: alignHolderY;
                anchors.left: alignHolderButton.right
                anchors.leftMargin: 5
                anchors.bottom: alignHolderButton.bottom
                anchors.bottomMargin: 5

                checked:  true;
                text: "Align y"
            }
            //----------------------------------------------------
            Button{ // analyse button
                id: analyseButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: alignHolderButton.bottom
                anchors.topMargin: 10
                enabled: statusReader.isAnalysePossible
                text: "Analyze"
                CommandExecuter{ // defined in C++
                   id: analyseExecuter
                   deviceName: root.deviceName
                   commandName: 'analyse'
                }
                onClicked: {
                    if(analyseExecuter.runWithParam("1")){
                        statusReader.isHolderAnalyzed = true;
                        statusReader.isHolderAnalyzing = true;
                    }else{
                        statusReader.isHolderAnalyzing = false;
                    }
                }
            }
            //----------------------------------------------------
            Button{ // focus button
                id: focusImageButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: analyseButton.bottom
                anchors.topMargin: 10
                enabled: statusReader.isStandby
                text: "Focus now!"

                CommandExecuter{ // defined in C++
                   id: focusExecuter
                   deviceName: root.deviceName
                   commandName: 'focus'
                }

                onClicked: {
//                    var str = "~/Documents/";
//                    str += imageText.text;
                    focusExecuter.run();
                }
            }
            //----------------------------------------------------
            Button{ // clear Configuration button
                id: clearConfButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: focusImageButton.bottom
                anchors.topMargin: 50
                enabled: statusReader.isDisable
                text: "Clear Configuration"

                CommandExecuter{ // defined in C++
                   id: clearConfExecuter
                   deviceName: root.deviceName
                   commandName: 'clearConfiguration'
                }

                onClicked: {
                    clearConfExecuter.run();
                }
            }
            //----------------------------------------------------
            Button{ // disconnect button
                id: disconnectButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: clearConfButton.bottom
                anchors.topMargin: 5
                enabled: statusReader.isStandby
                text: "Disconnect"

                CommandExecuter{ // defined in C++
                   id: disconnectExecuter
                   deviceName: root.deviceName
                   commandName: 'disconnect'
                }

                onClicked: {
                    disconnectExecuter.run();
                }
            }
            //----------------------------------------------------
            //save button stuff
            Label{
                id: imageLabel
                text: "The image name: "

                anchors.bottom: saveImageButton.top
                anchors.bottomMargin: 5
                anchors.left: saveImageButton.left
                enabled: statusReader.isCameraConnected
            }
            TextEdit {
                id: imageText
                text: "nameOfImage"
                width: 200

                anchors.bottom: saveImageButton.top
                anchors.bottomMargin: 5
                anchors.left: imageLabel.right
                enabled: statusReader.isCameraConnected
            }
            Button{ // save Image button
                id: saveImageButton
                width: 200
                height: 48
                anchors.horizontalCenter: loadConfigurationButton.horizontalCenter
                anchors.top: disconnectButton.bottom
                anchors.topMargin: 100
                enabled: statusReader.isCameraConnected
                text: "Save Image"

                CommandExecuter{ // defined in C++
                   id: sameImageExecuter
                   deviceName: root.deviceName
                   commandName: 'saveImage'
                }

                onClicked: {
//                    var str = "~/Documents/";
//                    str += imageText.text;
                    sameImageExecuter.runWithParam(imageText.text);
                }
            }
            //----------------------------------------------------
        }
    }

    RowLayout{ // Tab + buttons bar --------------------------------------
       TabView{
           id: tabMain
           implicitWidth: 600
           implicitHeight: 600
//           Layout.row: 3
//           Layout.column: 1

           Tab{ // interface TAB
               title: "Movement"

               Grid{
                   id: axesGrid
                   rows: 5
                   columns: 1
                   rowSpacing: 10

                   anchors.horizontalCenter: parent.horizontalCenter
                   anchors.top: parent.top
                   anchors.topMargin: 5

                   Column{
                       id: interfaceX
//                       rows: 4
//                       columns: 1
//                       rowSpacing: 5
                       spacing: 5

                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.top: parent.top
                       anchors.topMargin: 5
                       //horizontalItemAlignment: Grid.AlignHCenter

                       Label{
//                           anchors.horizontalCenter: parent.horizontalCenter
                           id:axisXinfo
                           text: "AXIS X"
                           font.pixelSize: 20
                       }

                       Row{
                           id: interfaceInternalX
//                           columns: 6
//                           rows: 2

                           //level 1 - buttons and textField
                           CommandExecuter{ // defined in C++
                             id: absMoveButtonExecuterX
                             deviceName: root.deviceName
                             commandName: 'moveAbs'
                           }
                           CommandExecuter{ // defined in C++
                             id: relMoveButtonExecuterX
                             deviceName: root.deviceName
                             commandName: 'moveRel'
                           }
                           CommandExecuter{ // defined in C++
                             id: runButtonExecuterX
                             deviceName: root.deviceName
                             commandName: 'run'
                           }

                           Button{
                               id:moveRelativeButtonX
                               text: "Move relative"
                               enabled: statusReader.isStandby
                               width: 100;
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountX.length > 0){ // move 1 step relativelly
                                       var movement = ["0",moveRelativeAmountX.text]
                                       relMoveButtonExecuterX.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           TextField{
                               id:moveRelativeAmountX
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:moveAbsoluteButtonX
                               text: "Move absolute"
                               enabled: statusReader.isStandby
                               width: 100;
                               onClicked: {
                                   if(statusReader.isStandby && moveAbsoluteAmountX.length > 0){ // move steps
                                       var movement = ["0",moveAbsoluteAmountX.text]
                                       absMoveButtonExecuterX.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           TextField{
                               id:moveAbsoluteAmountX
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:runRelativeButtonX
                               text: "Run"
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby){ // run
                                       if(runDirectionX.checked){
                                           var movement = ["0","1"];
                                           runButtonExecuterX.runAsynchWithParam(movement);
                                       }else{
                                           var movement = ["0","0"];
                                           runButtonExecuterX.runAsynchWithParam(movement);
                                       }
                                    }
                               }
                           }
                           CheckBox{
                               id:runDirectionX
                               text: "Positive"
                               checked: true
                               enabled: statusReader.isStandby
                           }

                       } // grid interface end

                       Row{ // steps and stop row
                           spacing: 5
                           Button{
                               id:xAbsLeft
                               text: "<<"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountX.length > 0){ // move 1 step relativelly
                                       var movement = ["0", Math.abs(moveRelativeAmountX.text)]
                                       relMoveButtonExecuterX.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{
                               id:xAbsRight
                               text: ">>"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountX.length > 0){ // move 1 step relativelly
                                       var movement = ["0", -1 * Math.abs(moveRelativeAmountX.text)]
                                       relMoveButtonExecuterX.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{ // stop button
                               id: stopButtonX
                               width: 420;
                               enabled: statusReader.isMoving

                               Image {
                                   id: stopImageX
                                   source: "./img/stop.png"
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter: parent.horizontalCenter
                                   width: 32
                                   height: 32
                               }

                               Label{
                                   id: stopLabelX
                                   text: "STOP"
                                   anchors.left: stopImageX.right
                                   anchors.leftMargin: 10
                                   anchors.verticalCenter: parent.verticalCenter
                               }

                               onClicked:{
                                  var result = abortCmd.run()
                               }
                           }

                       }

                       AttributeLabel{
                          id: positionX
                          attributeName: "positionX"
                          deviceName: root.deviceName
                          Layout.row: 1
                          Layout.column: 1
                          unitVisible: true
                          valueLabel.width: 150
                          valueLabel.font.pixelSize: 10
                          color: "black"
                          infoLabel.font.pixelSize: 10
                       }

                   } //grid movement interface end

                   Grid{
                       id: interfaceY
                       rows: 4
                       columns: 1
                       rowSpacing: 5

                       anchors.left: interfaceX.left
                       anchors.top: interfaceX.bottom
                       anchors.topMargin: 5
                       //horizontalItemAlignment: Grid.AlignHCenter

                       Label{
//                           anchors.horizontalCenter: parent.horizontalCenter
                           id:axisYinfo
                           text: "AXIS Y"
                           font.pixelSize: 20
                       }

                       Row{
                           id: interfaceInternalY

                           CommandExecuter{ // defined in C++
                             id: absMoveButtonExecuterY
                             deviceName: root.deviceName
                             commandName: 'moveAbs'
                           }
                           CommandExecuter{ // defined in C++
                             id: relMoveButtonExecuterY
                             deviceName: root.deviceName
                             commandName: 'moveRel'
                           }
                           CommandExecuter{ // defined in C++
                             id: runButtonExecuterY
                             deviceName: root.deviceName
                             commandName: 'run'
                           }

                           Button{
                               id:moveRelativeButtonY
                               text: "Move relative"
                               enabled: statusReader.isStandby
                               width: 100
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountY.length > 0){ // move 1 step relativelly
                                       var movement = ["1",moveRelativeAmountY.text]
                                       relMoveButtonExecuterY.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           TextField{
                               id:moveRelativeAmountY
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:moveAbsoluteButtonY
                               text: "Move absolute"
                               enabled: statusReader.isStandby
                               width: 100
                               onClicked: {
                                   if(statusReader.isStandby && moveAbsoluteAmountY.length > 0){ // move steps
                                       var movement = ["1",moveAbsoluteAmountY.text]
                                       absMoveButtonExecuterY.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           TextField{
                               id:moveAbsoluteAmountY
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:runRelativeButtonY
                               text: "Run"
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby){ // run
                                       if(runDirectionY.checked){
                                           var movement = ["1","1"];
                                           runButtonExecuterY.runAsynchWithParam(movement);
                                       }else{
                                           var movement = ["1","0"];
                                           runButtonExecuterY.runAsynchWithParam(movement);
                                       }
                                    }
                               }
                           }
                           CheckBox{
                               id:runDirectionY
                               text: "Positive"
                               checked: true
                               enabled: statusReader.isStandby
                           }
                       } // grid interface end

                       Row{ // steps and stop row
                           spacing: 5
                           Button{
                               id:yAbsLeft
                               text: "<<"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountY.length > 0){ // move 1 step relativelly
                                       var movement = ["1", Math.abs(moveRelativeAmountY.text)]
                                       relMoveButtonExecuterY.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{
                               id:yAbsRight
                               text: ">>"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountY.length > 0){ // move 1 step relativelly
                                       var movement = ["1", -1 * Math.abs(moveRelativeAmountY.text)]
                                       relMoveButtonExecuterY.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{ // stop button
                               id: stopButtonY
                               width: 420;
                               enabled: statusReader.isMoving

                               Image {
                                   id: stopImageY
                                   source: "./img/stop.png"
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter: parent.horizontalCenter
                                   width: 32
                                   height: 32
                               }

                               Label{
                                   id: stopLabelY
                                   text: "STOP"
                                   anchors.left: stopImageY.right
                                   anchors.leftMargin: 10
                                   anchors.verticalCenter: parent.verticalCenter
                               }

                               onClicked:{
                                  var result = abortCmd.run()
                               }
                           }

                       }

                       AttributeLabel{
                          id: positionY
                          attributeName: "positionY"
                          deviceName: root.deviceName
                          Layout.row: 1
                          Layout.column: 1
                          unitVisible: true
                          valueLabel.width: 150
                          valueLabel.font.pixelSize: 10
                          color: "black"
                          infoLabel.font.pixelSize: 10
                       }

                   } //grid movement interface end

                   Grid{
                       id: interfaceZ
                       rows: 4
                       columns: 1
                       rowSpacing: 5


                       anchors.left: interfaceY.left
                       anchors.top: interfaceY.bottom
                       anchors.topMargin: 5
                       //horizontalItemAlignment: Grid.AlignHCenter

                       Label{
                           //anchors.horizontalCenter: parent.horizontalCenter
                           id:axisZinfo
                           text: "AXIS Z"
                           font.pixelSize: 20
                       }

                       Row{
                           id: interfaceInternalZ
//                           columns: 6

                           //level 1 - buttons and textField
                           CommandExecuter // defined in C++
                           {
                             id: absMoveButtonExecuterZ
                             deviceName: root.deviceName
                             commandName: 'moveAbs'
                           }
                           CommandExecuter // defined in C++
                           {
                             id: relMoveButtonExecuterZ
                             deviceName: root.deviceName
                             commandName: 'moveRel'
                           }
                           CommandExecuter // defined in C++
                           {
                             id: runButtonExecuterZ
                             deviceName: root.deviceName
                             commandName: 'run'
                           }

                           Button{
                               id:moveRelativeButtonZ
                               text: "Move relative"
                               enabled: statusReader.isStandby
                               width: 100
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountZ.length > 0){ // move 1 step relativelly
                                       var movement = ["2",moveRelativeAmountZ.text]
                                       relMoveButtonExecuterZ.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           TextField{
                               id:moveRelativeAmountZ
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:moveAbsoluteButtonZ
                               text: "Move absolute"
                               enabled: statusReader.isStandby
                               width: 100

                               onClicked: {
                                   if(statusReader.isStandby && moveAbsoluteAmountZ.length > 0){ // move steps
                                       var movement = ["2",moveAbsoluteAmountZ.text]
                                         absMoveButtonExecuterZ.runAsynchWithParam(movement)
                                    }
                               }

                           }
                           TextField{
                               id:moveAbsoluteAmountZ
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:runRelativeButtonZ
                               text: "Run"
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby){ // run
                                       if(runDirectionZ.checked){
                                           var movement = ["2","1"];
                                           runButtonExecuterZ.runAsynchWithParam(movement);
                                       }else{
                                           var movement = ["2","0"];
                                           runButtonExecuterZ.runAsynchWithParam(movement);
                                       }
                                    }
                               }
                           }
                           CheckBox{
                               id:runDirectionZ
                               text: "Positive"
                               checked: true
                               enabled: statusReader.isStandby
                           }

                       } // grid interface end

                       Row{ // steps and stop row
                           spacing: 5
                           Button{
                               id:zAbsLeft
                               text: "<<"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountZ.length > 0){ // move 1 step relativelly
                                       var movement = ["2", Math.abs(moveRelativeAmountZ.text)]
                                       relMoveButtonExecuterZ.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{
                               id:zAbsRight
                               text: ">>"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountZ.length > 0){ // move 1 step relativelly
                                       var movement = ["2", -1 * Math.abs(moveRelativeAmountZ.text)]
                                       relMoveButtonExecuterZ.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{ // stop button
                               id: stopButtonZ
                               width: 420;
                               enabled: statusReader.isMoving

                               Image {
                                   id: stopImageZ
                                   source: "./img/stop.png"
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter: parent.horizontalCenter
                                   width: 32
                                   height: 32
                               }

                               Label{
                                   id: stopLabelZ
                                   text: "STOP"
                                   anchors.left: stopImageZ.right
                                   anchors.leftMargin: 10
                                   anchors.verticalCenter: parent.verticalCenter
                               }

                               onClicked:{
                                  var result = abortCmd.run()
                               }
                           }

                       }

                       AttributeLabel{
                          id: positionZ
                          attributeName: "positionZ"
                          deviceName: root.deviceName
                          Layout.row: 1
                          Layout.column: 1
                          unitVisible: true
                          valueLabel.width: 150
                          valueLabel.font.pixelSize: 10
                          color: "black"
                          infoLabel.font.pixelSize: 10
                       }

                   }

                   Grid{
                       id: interfaceT
                       rows: 4
                       columns: 1
                       rowSpacing: 5

                       anchors.left: interfaceZ.left
                       anchors.top: interfaceZ.bottom
                       anchors.topMargin: 5
                       //horizontalItemAlignment: Grid.AlignHCenter

                       Label{
//                           anchors.horizontalCenter: parent.horizontalCenter
                           id:axisTinfo
                           text: "AXIS T"
                           font.pixelSize: 20
                       }

                       Row{
                           id: interfaceInternalT
//                           columns: 6

                           //level 1 - buttons and textField
                           CommandExecuter // defined in C++
                           {
                             id: absMoveButtonExecuterT
                             deviceName: root.deviceName
                             commandName: 'moveAbs'
                           }
                           CommandExecuter // defined in C++
                           {
                             id: relMoveButtonExecuterT
                             deviceName: root.deviceName
                             commandName: 'moveRel'
                           }
                           CommandExecuter // defined in C++
                           {
                             id: runButtonExecuterT
                             deviceName: root.deviceName
                             commandName: 'run'
                           }

                           Button{
                               id:moveRelativeButtonT
                               text: "Move relative"
                               enabled: statusReader.isStandby
                               width: 100

                               onClicked: {

                                   if(statusReader.isStandby && moveRelativeAmountT.length > 0){ // move 1 step relativelly
                                       var movement = ["3",moveRelativeAmountT.text]
                                       relMoveButtonExecuterT.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           TextField{
                               id:moveRelativeAmountT
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }

                           Button{
                               id:moveAbsoluteButtonT
                               text: "Move absolute"
                               enabled: statusReader.isStandby
                               width: 100

                               onClicked: {
                                   if(statusReader.isStandby && moveAbsoluteAmountT.length > 0){ // move steps
                                       var movement = ["3",moveAbsoluteAmountT.text]
                                       absMoveButtonExecuterT.runAsynchWithParam(movement)
                                    }
                               }

                           }
                           TextField{
                               id:moveAbsoluteAmountT
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:runRelativeButtonT
                               text: "Run"
                               enabled: statusReader.isStandby
                           //    width: 50

                               onClicked: {
                                   if(statusReader.isStandby){ // run
                                       if(runDirectionT.checked){
                                           var movement = ["3","1"];
                                           runButtonExecuterT.runAsynchWithParam(movement);
                                       }else{
                                           var movement = ["3","0"];
                                           runButtonExecuterT.runAsynchWithParam(movement);
                                       }
                                    }
                               }
                           }
                           CheckBox{
                               id:runDirectionT
                               text: "Positive"
                               checked: true
                               enabled: statusReader.isStandby
                           }

                       } // grid interface end

                       Row{ // steps and stop row
                           spacing: 5
                           Button{
                               id:tAbsLeft
                               text: "<<"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountT.length > 0){ // move 1 step relativelly
                                       var movement = ["3", Math.abs(moveRelativeAmountT.text)]
                                       relMoveButtonExecuterT.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{
                               id:tAbsRight
                               text: ">>"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountT.length > 0){ // move 1 step relativelly
                                       var movement = ["3", -1 * Math.abs(moveRelativeAmountT.text)]
                                       relMoveButtonExecuterT.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{ // stop button
                               id: stopButtonT
                               width: 420;
                               enabled: statusReader.isMoving

                               Image {
                                   id: stopImageT
                                   source: "./img/stop.png"
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter: parent.horizontalCenter
                                   width: 32
                                   height: 32
                               }

                               Label{
                                   id: stopLabelT
                                   text: "STOP"
                                   anchors.left: stopImageT.right
                                   anchors.leftMargin: 10
                                   anchors.verticalCenter: parent.verticalCenter
                               }

                               onClicked:{
                                  var result = abortCmd.run()
                               }
                           }

                       }

                       AttributeLabel{
                          id: positionT
                          attributeName: "positionT"
                          deviceName: root.deviceName
                          Layout.row: 1
                          Layout.column: 1
                          unitVisible: true
                          valueLabel.width: 150
                          valueLabel.font.pixelSize: 10
                          color: "black"
                          infoLabel.font.pixelSize: 10
                       }

                   }

                   Grid{
                       id: interfaceP
                       rows: 4
                       columns: 1
                       rowSpacing: 5

                       anchors.left: interfaceT.left
                       anchors.top: interfaceT.bottom
                       anchors.topMargin: 5
                       //horizontalItemAlignment: Grid.AlignHCenter

                       Label{
//                           anchors.horizontalCenter: parent.horizontalCenter
                           id:axisPinfo
                           text: "AXIS P"
                           font.pixelSize: 20
                       }

                       Row{
                           id: interfaceInternalP
//                           columns: 6


                           //level 1 - buttons and textField
                           CommandExecuter // defined in C++
                           {
                             id: absMoveButtonExecuterP
                             deviceName: root.deviceName
                             commandName: 'moveAbs'
                           }
                           CommandExecuter // defined in C++
                           {
                             id: relMoveButtonExecuterP
                             deviceName: root.deviceName
                             commandName: 'moveRel'
                           }
                           CommandExecuter // defined in C++
                           {
                             id: runButtonExecuterP
                             deviceName: root.deviceName
                             commandName: 'run'
                           }

                           Button{
                               id:moveRelativeButtonP
                               text: "Move relative"
                               enabled: statusReader.isStandby
                               width: 100

                               onClicked: {

                                   if(statusReader.isStandby && moveRelativeAmountP.length > 0){ // move 1 step relativelly
                                       var movement = ["4",moveRelativeAmountP.text]
                                       relMoveButtonExecuterP.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           TextField{
                               id:moveRelativeAmountP
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }
                           Button{
                               id:moveAbsoluteButtonP
                               text: "Move absolute"
                               enabled: statusReader.isStandby
                               width: 100

                               onClicked: {
                                   if(statusReader.isStandby && moveAbsoluteAmountP.length > 0){ // move steps
                                       var movement = ["4",moveAbsoluteAmountP.text]
                                       absMoveButtonExecuterP.runAsynchWithParam(movement)
                                    }
                               }

                           }
                           TextField{
                               id:moveAbsoluteAmountP
                               text: "10"
                               validator: DoubleValidator{bottom: -99999.999; top: 99999.999}
                               maximumLength: 9
                               enabled: statusReader.isStandby
                               width: 90

                               onActiveFocusChanged: {
                                 if (activeFocus == true) {
                                   selectAll();
                                 }
                                 else {
                                   select(0,0);
                                 }
                               }
                           }

                           Button{
                               id:runRelativeButtonP
                               text: "Run"
                               enabled: statusReader.isStandby
                           //    width: 50

                               onClicked: {
                                   if(statusReader.isStandby){ // run
                                       if(runDirectionP.checked){
                                           var movement = ["4","1"];
                                           runButtonExecuterP.runAsynchWithParam(movement);
                                       }else{
                                           var movement = ["4","0"];
                                           runButtonExecuterP.runAsynchWithParam(movement);
                                       }
                                    }
                               }
                           }
                           CheckBox{
                               id:runDirectionP
                               text: "Positive"
                               checked: true
                               enabled: statusReader.isStandby
                           }

                       } // grid interface end

                       Row{ // steps and stop row
                           spacing: 5
                           Button{
                               id:pAbsLeft
                               text: "<<"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountP.length > 0){ // move 1 step relativelly
                                       var movement = ["4", Math.abs(moveRelativeAmountP.text)]
                                       relMoveButtonExecuterP.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{
                               id:pAbsRight
                               text: ">>"
                               width: 48;
                               enabled: statusReader.isStandby
                               onClicked: {
                                   if(statusReader.isStandby && moveRelativeAmountP.length > 0){ // move 1 step relativelly
                                       var movement = ["4", -1 * Math.abs(moveRelativeAmountP.text)]
                                       relMoveButtonExecuterP.runAsynchWithParam(movement)
                                    }
                               }
                           }
                           Button{ // stop button
                               id: stopButtonP
                               width: 420;
                               enabled: statusReader.isMoving

                               Image {
                                   id: stopImageP
                                   source: "./img/stop.png"
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter: parent.horizontalCenter
                                   width: 32
                                   height: 32
                               }

                               Label{
                                   id: stopLabelP
                                   text: "STOP"
                                   anchors.left: stopImageP.right
                                   anchors.leftMargin: 10
                                   anchors.verticalCenter: parent.verticalCenter
                               }

                               onClicked:{
                                  var result = abortCmd.run()
                               }
                           }

                       }

                       AttributeLabel{
                          id: positionP
                          attributeName: "positionP"
                          deviceName: root.deviceName
                          Layout.row: 1
                          Layout.column: 1
                          unitVisible: true
                          valueLabel.width: 150
                          valueLabel.font.pixelSize: 10
                          color: "black"
                          infoLabel.font.pixelSize: 10
                       }

                   }

               } // axes grid end
           } // interface tab end
           Tab{ // interface TAB
               title: "Centering"
               enabled: statusReader.isCenteringPossible

           } // interface tab end
       } // tabMain end

       // rectangle with the abortButton---------------------------------------
       Rectangle{
           id: rectButtons

           width: 150
           height: tabMain.height - 32

           anchors.bottom: tabMain.bottom
           border.color: "gray"
           border.width: 1

           Button{ // abort button
               id: abortButton
               width: 128
               height: 48
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               anchors.topMargin: 5
               enabled: statusReader.isAbortReady

               Image {
                   id: abortImage
                   source: "./img/abort.png"
                   //anchors.fill: parent
                   anchors.left: parent.left
                   anchors.leftMargin: 10
                   anchors.verticalCenter: parent.verticalCenter
                   width: 32
                   height: 32
               }

               Label{
                   id: abortLabel
                   text: "ABORT"
                   anchors.left: abortImage.right
                   anchors.leftMargin: 10
                   anchors.verticalCenter: parent.verticalCenter

               }

               onClicked:{
                  var result = abortCmd.run()

                   if(statusReader.isHolderAligning){
                       statusReader.isHolderAligning = false;
                       statusReader.isHolderAligned = false;
                   }

                   if(statusReader.isHolderAnalyzing){
                       statusReader.isHolderAnalyzed = false;
                       statusReader.isHolderAnalyzing = false;
                   }

               }
           }
       }
   }
   }
}
