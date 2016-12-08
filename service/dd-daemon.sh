#!/bin/sh

gdbus-codegen --generate-c-code=dd-daemon \
	--annotate "com.dd.service.ioctl()[pin]" org.gtk.GDBus.C.ForceGVariant "ay" \
	--annotate "com.dd.service.ioctl()[pout]" org.gtk.GDBus.C.ForceGVariant "ay" \
	--annotate "com.dd.service::notification[data]" org.gtk.GDBus.C.ForceGVariant "ay" \
	dd-daemon.xml
