# Symbian Signing Requirements #

**NOTE: You will not be able to use "Open Signed Online" to sign this application**

Unfortunately, the project requires the following capabilities:


---


LocalServices

NetworkServices

ProtServ

ReadDeviceData

ReadUserData

SwEvent

WriteDeviceData

WriteUserData

UserEnvironment

MultimediaDD


---


Symbian "Open Signed Online" will not sign .sis files that have the following capabilities:


---


CommDD

MultimediaDD

NetworkControl

DiskAdmin

DRM

AllFiles

TCB


---


So, the MultimediaDD capability will cause a problem. I have been signing the application using the "Open Signed Offline", however, this will require to have a developer certificate.