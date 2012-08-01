mod_negotiation_device.la: mod_negotiation_device.slo mobile_detect.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_negotiation_device.lo mobile_detect.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_negotiation_device.la
