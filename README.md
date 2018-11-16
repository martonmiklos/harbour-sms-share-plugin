# harbour-sms-share-plugin
Share via SMS plugin for SailfishOS

Supports sharing mime-types "text/vcard" ie. contacts.

The plugin supports 3 sending modes:

 * Short plaintext: only the contact name and it's phone numbers will be generated
 * Full plaintext: all contact details will be added
 * SmartMessaging vCard: this is a special format developed originally by Nokia. Old Nokia feature phones, and Sailfish OS supports it: the sent data could be imported on the receiver side as a simple vCard. Android known to be ignoring this method, iOS support is unknown.

## Translations:

The plugin serialization backend uses the strings of the contacts app. (jolla-contacts.ts on the Pootle) If any of the placeholders in the generated SMS are incorrct head to the Pootle.

The UI strings could be translated, feel free to open up a Github PR or Issue.

## Bug reporting:

Open up a terminal.

```
set LANG=<locale code>

start jolla-contacts

Select your contact try to share with the plugin

```
Share the vCard data from the command line output, and the resulted SMS text in a Github Issue. (Feel free to mangle personal details, the vCard keys are the most important.)
