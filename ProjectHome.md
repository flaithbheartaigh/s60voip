# What is the s60voip project #

The outcome of this project is an installable piece of software that allows you to place a voip-based call on certain Symbian S60 phones, i.e. Nokia smartphones such as the E71 and 5800 Xpress.

# What does the software look like #

The software consists of a regular _.sis_ executable that can be installed on Nokia Series 60 3rd and 5th edition phones. For a listing of these phones please consult: http://www.symbian.org/devices

Once installed, the software's interface consists of two main screens, titled: **Contacts** and **Log**.

The **Contacts** screen lists all the contacts stored on the phone and allows the user to add/edit/delete contacts. The user can also send a text-message, place a normal GSM call, or place a voip-based call from this screen, e.g.

![http://s60voip.googlecode.com/svn/trunk/images/contacts.jpg](http://s60voip.googlecode.com/svn/trunk/images/contacts.jpg)

The **Log** screen provides a running status of the VoIP operations being performed, e.g.

![http://s60voip.googlecode.com/svn/trunk/images/log.jpg](http://s60voip.googlecode.com/svn/trunk/images/log.jpg)

The voip settings are configurable from the **Options** menu, e.g.

![http://s60voip.googlecode.com/svn/trunk/images/voip_settings.jpg](http://s60voip.googlecode.com/svn/trunk/images/voip_settings.jpg)

# How do I run the software on my phone #

Under the downloads tab you can download the unsigned _.sis_ executable. This _.sis_ executable will need to be signed before it can be installed on your phone. You can sign this _.sis_ executable for your phone via the Symbian Signed website:  https://www.symbiansigned.com
**Note:** The MultimediaDD capability prevents this application from being **Open Signed**.

# How does the software work #

The software provides an easy-to-use S60 graphical user interface for an externally developed voip software library called **PJSIP**.

_**PJSIP** is a professionally supported open source, portable, small footprint multimedia communication libraries written in C language for building portable VoIP applications._

The PJSIP software library consists of:

  * **pjlib** A cross-platforms portability and framework library,
  * **pjlib-util** An adjunct library to pjlib which provides various utility functions,
  * **pjnath** A NAT traversal helper library,
  * **pjmedia** A multimedia communications library,
  * **pjmedia-codec** A placeholder library to support various multimedia codecs,
  * **pjsip** A SIP protocol stack collections.

For more information on the **PJSIP** software library, see http://www.pjsip.org/

This project's software statically links to a collection of **PJSIP** software libraries.

## Thanks ##

Many thanks to [pjsip](http://www.pjsip.org) team, without whom none of this would be possible.

## Disclaimer ##

This software is provided as-is, without any express or implicit warranty.