import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.private 1.0
import Sailfish.Contacts 1.0
import Sailfish.Telephony 1.0
import Sailfish.TransferEngine 1.0
import org.nemomobile.contacts 1.0
import org.nemomobile.thumbnailer 1.0
import org.nemomobile.ofono 1.0
import org.nemomobile.commhistory 1.0
import com.jolla.connection 1.0
import MeeGo.QOfono 0.2
import org.nemomobile.dbus 2.0
import hu.mm.vCardSerializer 1.0

Page {
    id: newMessagePage

    property url source
    property variant content: ({})
    property string methodId
    property string displayName
    property int accountId
    property string accountName

    property var recipients: []

    allowedOrientations: Orientation.All
    _clickablePageIndicators: !(isLandscape && recipientField.activeFocus)
    onStatusChanged: {
        if (status === PageStatus.Active) {
            recipientField.forceActiveFocus()
        }
    }

    function updateRecipients(selectedContacts) {
        var remoteUids = [ ]
        var person = null

        for (var i = 0; i < selectedContacts.count; i++) {
            var contact = selectedContacts.get(i)
            var remote

            if (contact.property !== undefined && contact.propertyType === "phoneNumber") {
                // Should we check that this is a correctly formatted phone number?
                remote = contact.property.number
                person = contact.person
            } else {
                continue
            }
            remoteUids.push(remote)
        }

        recipients = remoteUids
    }


    DBusInterface {
        id: smartMessagingIf
        service: "org.ofono"
        iface: "org.ofono.SmartMessaging"
        path: modemManager.voiceModemPath
        bus: DBus.SystemBus
    }


    DBusInterface {
        id: smsIf
        service: "org.nemomobile.qmlmessages"
        iface: "org.nemomobile.qmlmessages"
        path: "/"
    }

    PeopleModel {
        id: peopleModel
        filterType: PeopleModel.FilterNone
    }

    VCardSerializer {
        id: id_VCardSerializer
    }

    onContentChanged: {
        if (content.type === "text/vcard")
            id_VCardSerializer.setVCardData(JSON.stringify(content.data));
        updateTextItem(id_shareMode.currentIndex);
    }

    SilicaFlickable {
        id: flickable
        focus: true
        opacity: enabled ? 1.0 : 0.0
        Behavior on opacity { FadeAnimation {} }
        contentHeight: contentColumn.y + contentColumn.height
        anchors.fill: parent

        PullDownMenu
        {
            id: menu
            MenuItem
            {
                //% "Send"
                text: qsTrId("sms-share-la-send")
                onClicked: send()
            }
        }

        Column {
            id: contentColumn
            y: newMessagePage.isLandscape ? Theme.paddingMedium : 0
            width: flickable.width
            Item {

                width: flickable.width
                Column {
                    id: recipientHeader
                    width: parent.width
                    PageHeader {
                        //% "Share contact via SMS"
                        title: qsTrId("sms-share-la-new_message")
                        visible: newMessagePage.isPortrait
                    }
                    ComboBox
                    {
                        id:  id_shareMode
                        //% "Method"
                        label: qsTrId("sms-share-la-method")
                        currentIndex: 0
                        menu: ContextMenu
                        {
                            MenuItem
                            {
                                id: shareTypePlainTextShort
                                //% "Short plaintext"
                                text: qsTrId("sms-share-la-short_plaintext")
                                onClicked:
                                {
                                    textInput.visible = true;
                                    updateTextItem(0);
                                }
                            }
                            MenuItem
                            {
                                id: shareTypePlainText
                                //% "Full plaintext"
                                text: qsTrId("sms-share-la-full_plaintext")
                                onClicked:
                                {
                                    textInput.visible = true;
                                    updateTextItem(1);
                                }
                            }
                            MenuItem
                            {
                                id: shareTypevCARD
                                //% "SmartMessaging vCard"
                                text: qsTrId("sms-share-la-smartmessaging_vcard")
                                visible: (content.type === "text/vcard")
                                onClicked:
                                {
                                    textInput.visible = false;
                                }
                            }
                        }
                    }

                    RecipientField {
                        id: recipientField
                        requiredProperty: PeopleModel.PhoneNumberRequired
                        contactSearchModel: peopleModel
                        // XXX Group messaging is not supported yet
                        multipleAllowed: false
                        enabled: visible

                        //: A single recipient
                        //% "Recipient"
                        placeholderText: qsTrId("sms-share-ph-recipient")

                        //: Summary of all selected recipients, e.g. "Bob, Jane, 75553243"
                        //% "Recipients"
                        summaryPlaceholderText: qsTrId("sms-share-ph-recipients")

                        onFinishedEditing: {
                            textInput.forceActiveFocus()
                        }

                        onSelectionChanged: updateRecipients(selectedContacts)
                    }

                    TextArea {
                        id: textInput
                        width: parent.width
                    }
                }
            }


            Loader {
                id: menuContainer
                width: parent.width
                active: false

                sourceComponent: Component {
                    id: simSelectorComponent
                    ContextMenu {
                        id: simSelector
                        // TODO: remove once Qt.inputMethod.animating has been implemented JB#15726
                        property Item lateParentItem
                        property bool noKeyboard: lateParentItem && ((isLandscape && pageStack.width === Screen.width) ||
                                                                     (!isLandscape && pageStack.height === Screen.height))
                        onNoKeyboardChanged: {
                            if (noKeyboard) {
                                show(lateParentItem)
                                lateParentItem = null
                            }
                        }

                        onClosed: menuContainer.active = false

                        function showMenu(parentItem) {
                            // close keyboard if necessary
                            if (Qt.inputMethod.visible) {
                                textInput.editorFocus = false
                                lateParentItem = parentItem
                            } else {
                                show(parentItem)
                            }
                        }

                        SimPicker {
                            actionType: Telephony.Message
                            onSimSelected: {
                                simSelector.hide()
                            }
                        }
                    }
                }
            }
        }
        VerticalScrollDecorator {}
    }

    function stoa(str)
    {
        var bytes = [];
        var charCode;

        for (var i = 0; i < str.length; ++i)
        {
            charCode = str.charCodeAt(i);
            bytes.push(Number((charCode & 0xFF00) >> 8));
            bytes.push(Number(charCode & 0xFF));
        }
        return bytes;
    }

    function send() {
        console.log(modemManager.voiceModemPath)
        console.log(source)
        console.log(JSON.stringify(content))
        if (id_shareMode.currentIndex == 3) {
            // share via SmartMessaging
            if (recipients.length < 0)
                return
            smartMessagingIf.typedCall(
                        "SendBusinessCard",
                        [
                            {
                                'type': 's',
                                'value': recipients[0]
                            },
                            {
                                'type': 'ay',
                                'value': stoa(JSON.stringify(content))
                            }
                        ]
            );
        } else {
            // send via SMS
            smsIf.typedCall("startSMS",
                            [
                                {"type":"as", "value":[recipients[0]]},
                                {"type":"s", "value": textInput.text}
                            ]
            );
        }

        pageStack.pop()
    }

    function updateTextItem(mode)
    {
        if (content.type === "text/vcard") {
            switch (mode) {
            case 0:
                textInput.text = id_VCardSerializer.serialize_vCardShort();
                break;
            case 1:
                textInput.text = id_VCardSerializer.serialize_vCardFull();
                break;
            }
        } else if (content.type === "text/x-url"){
            textInput.visible = true
            switch (mode) {
            case 0:
                textInput.text = content.status
                break;
            case 1:
                textInput.text = content.linkTitle + "\n" + content.status
                break;
            }
        }
    }

    OfonoModemManager {
        id: modemManager
        readonly property string voiceModemPath: valid ? defaultVoiceModem : ""
    }

    OfonoNetworkRegistration {
        id: networkRegistration
        modemPath: modemManager.voiceModemPath
        readonly property bool isRoaming: valid && status === "roaming"
    }

    OfonoConnMan {
        id: connectionManager
        modemPath: modemManager.voiceModemPath
        readonly property bool roamingIsAllowed: valid && roamingAllowed
    }

    ConnectionAgent {
        id: connectionAgent
    }
}
