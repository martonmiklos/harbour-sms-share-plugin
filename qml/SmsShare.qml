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
        if (recipients.length == 1 && person) {
            textInput.contactName = person.firstName
        } else {
            textInput.contactName = ""
        }
    }


    DBusInterface {
        id: smsIf
        service: "org.ofono"
        iface: "org.ofono.SmartMessaging"
        path: "/modem0"
    }

    PeopleModel {
        id: peopleModel
        filterType: PeopleModel.FilterNone
    }

    SilicaFlickable {
        id: flickable
        focus: true
        opacity: enabled ? 1.0 : 0.0
        Behavior on opacity { FadeAnimation {} }
        contentHeight: contentColumn.y + contentColumn.height
        anchors.fill: parent

        Column {
            id: contentColumn
            y: newMessagePage.isLandscape ? Theme.paddingMedium : 0
            width: flickable.width
            Item {
                width: flickable.width
                height: Math.max(recipientHeader.height, flickable.height - bottomArea.height - contentColumn.y)

                Column {
                    id: recipientHeader
                    width: parent.width
                    PageHeader {
                        //% "New message"
                        title: qsTrId("sms-share-la-new_message")
                        visible: newMessagePage.isPortrait
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

                    Item { height: Theme.paddingLarge; width: 1 }
                }
            }

            Row {
                id: bottomArea
                width: parent.width
                ChatTextInput {
                    id: textInput
                    width: parent.width - x
                    enabled: recipientField && !recipientField.empty && visible
                    onSendMessage: {
                        if (Telephony.voiceSimUsageMode == Telephony.AlwaysAskSim) {
                            menuContainer.active = true
                            menuContainer.item.showMenu(menuContainer)
                        } else {
                            newMessagePage.send()
                        }
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
                                newMessagePage.send(simInfo(sim))
                                simSelector.hide()
                            }
                        }
                    }
                }
            }
        }
        VerticalScrollDecorator {}
    }

    function send(sim) {
        console.log(modemManager.voiceModemPath)
        console.log(source)
        console.log(JSON.stringify(content))
        if (recipients.length < 0)
            return
        smsIf.call("SendBusinessCard", [recipients[0], JSON.stringify(content)]);
        /*var attachments = [ ]

        if (source != '') {
            attachments.push({ "contentId": "1", "path": source })
        } else if ('data' in content) {
            // Android does not recognize the text/vcard type
            var type = ('type' in content) ? content.type.replace("text/vcard", "text/x-vCard") : ""
            var name = ('name' in content) ? content.name : "1"
            attachments.push({ "contentId": name, "contentType": type, "freeText": content.data })
        } else {
            console.log("BUG: No content for sms share message!")
        }

        if (textInput.text != '') {
            attachments.push({ "contentId": "2", "contentType": "text/plain", "freeText": textInput.text })
        }

        if (simImsi) {
            // IMSI, To, CC, BCC, Subject, Parts
            smsHelper.sendMessage(simImsi, recipients, [], [], "", attachments)
        } else {
            // To, CC, BCC, Subject, Parts
            smsHelper.sendMessage(recipients, [], [], "", attachments)
        }*/

        pageStack.pop()
    }

    MmsHelper {
        id: smsHelper
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
