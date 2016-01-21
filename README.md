# README #

This project attempts to provide a S60 GUI for the PJSIP project.

(Note: this project is basically a wrapper on the PJSIP S60 port, please
familiarise yourself with the PJSIP project http://www.pjsip.org/ before continuing).

This project has been created using:

- Carbide.c++ Version 2.4.0 Build v201001151122\_90 (Subclispe plugin 1.6.10)
- S60\_5th\_Edition\_SDK\_v1.0 (with API Plug-in for the S60 5th Edition SDK)

## PJSIP Dependencies ##

The project requires the following static library files are required:

pjsua\_lib.lib
pjsip\_ua.lib
pjsip\_simple.lib
pjsip.lib
libgsmcodec.lib
libspeexcodec.lib
libg7221codec.lib
libpassthroughcodec.lib
pjmedia.lib
pjmedia\_audiodev.lib
pjsdp.lib
pjnath.lib
pjlib\_util.lib
pjlib.lib
libsrtp.lib
libresample.lib

These libraries were compiled from version 1.5.5 of the PJSIP project
http://www.pjsip.org/

Note: the following method was added to the pjsip v1.5.5:

pjsua\_core.c line: 2519

> PJ\_DECL(pj\_status\_t) pjsua\_is\_connected();

## Nokia Dependencies ##

This project relies on the VoIP Audio Service (VAS) SDK API Plug-in http://wiki.forum.nokia.com/index.php/SDK_API_Plug-in

The API Plug-in for the S60 5th Edition SDK needs to be installed (i.e. added to the base S60 5th Edition SDK installed on your development machine) and the following SIS file needs to be installed on your target device:

For S60 3rd edition feature pack 1 and 2 phones you can install:

- VAS Plugin v1.0 for 3rd Edition:
VoIPAudioSrvc\_3132\_0x10207b9e.SISX

- VAS Plugin v2.0 for 3rd Edition (recommended):
VoIPAudioSrvc\_v20\_SA\_BK\_3rd.SISX

For S60 5th edition phones you do not need to install anything, since they ship with the VAS Plugin v2.0 installed.

NOTE: we will be using the VAS Plugin v2.0 in this project since it provides more functionality than v1.0

## Tested Devices ##

This project has been tested on:

- Nokia E71 (3rd FP1 edition phone)
- Nokia 5800 (5th edition phone)