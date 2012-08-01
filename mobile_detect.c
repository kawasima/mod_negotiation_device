#include <stdio.h>
#include <string.h>
#include <apr_general.h>
#include "mobile_detect.h"

#define ENGINE_WEB_KIT "webkit"
#define DEVICE_IPHONE  "iphone"
#define DEVICE_IPOD    "ipod"
#define DEVICE_IPAD    "ipad"
#define DEVICE_MAC_PPC "macintosh"

#define DEVICE_ANDROID "android"
#define DEVICE_GOOGLE_TV "googletv"
#define DEVICE_XOOM    "xoom"
#define DEVICE_HTC_FLYER "htc_flyer"

#define DEVICE_NUVIFONE "nuvifone"

#define DEVICE_SYMBIAN  "symbian"
#define DEVICE_S60      "series60"
#define DEVICE_S70      "series70"
#define DEVICE_S80      "series80"
#define DEVICE_S90      "series90"

#define DEVICE_WIN_PHONE7 "windows phone os 7"
#define DEVICE_WIN_MOB    "windows ce"
#define DEVICE_WINDOWS    "windows"
#define DEVICE_IE_MOB     "iemobile"
#define DEVICE_PPC        "ppc"
#define ENGINE_PIE        "wm5 pie"

#define DEVICE_BB             "blackberry"
#define VND_RIM               "vnd.rim"
#define DEVICE_BB_STORM       "blackberry95"
#define DEVICE_BB_BOLD        "blackberry97"
#define DEVICE_BB_BOLD_TOUCH  "blackberry 99"
#define DEVICE_BB_TOUR        "blackberry96"
#define DEVICE_BB_CURVE       "blackberry89"
#define DEVICE_BB_CURVE_TOUCH "blackberry 938"
#define DEVICE_BB_TORCH       "blackberry 98"
#define DEVICE_BB_PLAYBOOK    "playbook"

#define DEVICE_PALM      "palm"
#define DEVICE_WEB_OS    "webos"
#define DEVICE_WEB_OS_HP "hpwos"

#define ENGINE_BLAZER "blazer"
#define ENGINE_XIINO  "xiino"

#define DEVICE_KINDLE "kindle"
#define ENGINE_SILK   "silk"

#define VNDWAP  "vnd.wap"
#define WML     "wml"

#define DEVICE_TABLET      "tablet"
#define DEVICE_BREW        "brew"
#define DEVICE_DANGER      "danger"
#define DEVICE_HIPTOP      "hiptop"
#define DEVICE_PLAYSTATION "playstation"
#define DEVICE_NINTENDO_DS "nitro"
#define DEVICE_NINTENDO    "nintendo"
#define DEVICE_WII         "wii"
#define DEVICE_XBOX        "xbox"
#define DEVICE_ARCHOS      "archos"

#define ENGINE_OPERA      "opera"
#define ENGINE_NETFRONT   "netfront"
#define ENGINE_UP_BROWSER "up.browser"
#define ENGINE_OPEN_WEB   "openweb"
#define ENGINE_MIDP       "midp"
#define UPLINK            "up.link"
#define ENGINE_TELECA_Q   "teleca q"

#define DEVICE_PDA "pda"
#define MINI       "mini"
#define MOBILE     "mobile"
#define MOBI       "mobi"

#define MAEMO      "maemo"
#define LINUX      "linux"
#define QTEMBEDDED "qt embedded"
#define MYLOCOM2   "com2"

#define MANU_SONY_ERICSSON "sonyericsson"
#define MANU_ERICSSON      "ericsson"
#define MANU_SAMSUNGL      "sec-sgh"
#define MANU_SONY          "sony"
#define MANU_HTC           "htc"

#define SVC_DOCOMO    "docomo"
#define SVC_KDDI      "kddi"
#define SVC_VODAPHONE "vodafone"

#define DIS_UPDATE "update"

int mdetect_iphone(const char *ua)
{
    if (strstr(ua, DEVICE_IPHONE) != NULL) {
	return mdetect_ipad(ua) || mdetect_ipod(ua);
    } else {
	return FALSE;
    }
}

int mdetect_ipod(const char *ua) {
    return strstr(ua, DEVICE_IPOD) != NULL;
}

int mdetect_ipad(const char *ua) {
    return strstr(ua, DEVICE_IPAD) != NULL && mdetect_webkit(ua);
}

int mdetect_iphone_or_ipod(const char *ua) {
    return strstr(ua, DEVICE_IPHONE) != NULL || strstr(ua, DEVICE_IPOD) != NULL;
}

int mdetect_ios(const char *ua) {
    return mdetect_iphone_or_ipod(ua) || mdetect_ipad(ua);
}

int mdetect_android(const char *ua) {
    if (strstr(ua, DEVICE_ANDROID) != NULL || mdetect_google_tv(ua))
	return TRUE;
    return strstr(ua, DEVICE_HTC_FLYER) != NULL;
}

int mdetect_android_phone(const char *ua) {
    if (mdetect_android(ua) && strstr(ua, MOBILE) != NULL)
	return TRUE;
    if (mdetect_opera_android_phone(ua))
	return TRUE;
    return strstr(ua, DEVICE_HTC_FLYER) != NULL;
}

int mdetect_android_tablet(const char *ua) {
    if (!mdetect_android(ua) || mdetect_opera_mobile(ua) || strstr(ua, DEVICE_HTC_FLYER) != NULL)
	return FALSE;
    
    return strstr(ua, MOBILE) != NULL;
}

int mdetect_android_web_kit(const char *ua) {
    return mdetect_android(ua) && mdetect_webkit(ua);
}

int mdetect_google_tv(const char *ua) {
    return strstr(ua, DEVICE_GOOGLE_TV) != NULL;
}

int mdetect_webkit(const char *ua) {
    return strstr(ua, ENGINE_WEB_KIT) != NULL;
}

int mdetect_s60_oss_browser(const char *ua) {
    if (mdetect_webkit(ua))
	return strstr(ua, DEVICE_S60) != NULL || strstr(ua, DEVICE_SYMBIAN) != NULL;
    else
	return FALSE;
}

int mdetect_symbian_os(const char *ua) {
    return strstr(ua, DEVICE_SYMBIAN) != NULL
	|| strstr(ua, DEVICE_S60) != NULL
	|| strstr(ua, DEVICE_S70) != NULL
	|| strstr(ua, DEVICE_S80) != NULL
	|| strstr(ua, DEVICE_S90) != NULL;
}

int mdetect_windows_phone7(const char *ua) {
    return strstr(ua, DEVICE_WIN_PHONE7) != NULL;
}

int mdetect_windows_mobile(const char *ua) {
    if (mdetect_windows_phone7(ua))
	return FALSE;
    if (strstr(ua, DEVICE_WIN_MOB) != NULL
	|| strstr(ua, DEVICE_IE_MOB) != NULL
	|| strstr(ua, ENGINE_PIE) != NULL)
	return TRUE;
    if (strstr(ua, DEVICE_PPC) != NULL && strstr(ua, DEVICE_MAC_PPC) == NULL)
	return TRUE;
    return strstr(ua, MANU_HTC) != NULL && strstr(ua, DEVICE_WINDOWS) != NULL;
}

int mdetect_black_berry(const char *ua) {
    if (strstr(ua, DEVICE_BB) != NULL)
	return TRUE;
    return strstr(ua, VND_RIM) != NULL;
}

int mdetect_black_berry_tablet(const char *ua) {
    return strstr(ua, DEVICE_BB_PLAYBOOK) != NULL;
}

int mdetect_black_berry_web_kit(const char *ua) {
    return mdetect_black_berry && strstr(ua, ENGINE_WEB_KIT) != NULL;
}

int mdetect_black_berry_touch(const char *ua) {
    return mdetect_black_berry(ua)
	&& (strstr(ua, DEVICE_BB_STORM) != NULL
	    || strstr(ua, DEVICE_BB_TORCH) != NULL
	    || strstr(ua, DEVICE_BB_BOLD_TOUCH) != NULL
	    || strstr(ua, DEVICE_BB_CURVE_TOUCH) != NULL);
}

int mdetect_black_berry_high(const char *ua) {
    if (mdetect_black_berry_web_kit(ua))
	return FALSE;

    if (mdetect_black_berry(ua)) {
	return mdetect_black_berry_touch(ua)
	    || strstr(ua, DEVICE_BB_BOLD) != NULL
	    || strstr(ua, DEVICE_BB_TOUR) != NULL
	    || strstr(ua, DEVICE_BB_CURVE) != NULL;
    } else
	return FALSE;
}

int mdetect_black_berry_low(const char *ua) {
    if (mdetect_black_berry(ua)) {
	return !(mdetect_black_berry_high(ua) || mdetect_black_berry_web_kit(ua));
    } else
	return FALSE;
}

int mdetect_palm_os(const char *ua) {
    if (strstr(ua, DEVICE_PALM) != NULL
	|| strstr(ua, ENGINE_BLAZER) != NULL
	|| strstr(ua, ENGINE_XIINO) != NULL) {
	return !mdetect_palm_web_os(ua);
    } else
	return FALSE;
}

int mdetect_palm_web_os(const char *ua) {
    return strstr(ua, DEVICE_WEB_OS) != NULL;
}

int mdetect_web_os_tablet(const char *ua) {
    return strstr(ua, DEVICE_WEB_OS_HP) != NULL && strstr(ua, DEVICE_TABLET) != NULL;
}

int mdetect_garmin_nuvifone(const char *ua) {
    return strstr(ua, DEVICE_NUVIFONE) != NULL;
}

int mdetect_smartphone(const char *ua) {
    return mdetect_iphone_or_ipod(ua)
	|| mdetect_android_phone(ua)
	|| mdetect_s60_oss_browser(ua)
	|| mdetect_symbian_os(ua)
	|| mdetect_windows_mobile(ua)
	|| mdetect_windows_phone7(ua)
	|| mdetect_black_berry(ua)
	|| mdetect_palm_web_os(ua)
	|| mdetect_palm_os(ua)
	|| mdetect_garmin_nuvifone(ua);
}

int mdetect_opera_mobile(const char *ua) {
    if (strstr(ua, ENGINE_OPERA) != NULL) {
	return strstr(ua, MINI) != NULL || strstr(ua, MOBI) != NULL;
    } else
	return FALSE;
}

int mdetect_opera_android_phone(const char *ua) {
    return strstr(ua, ENGINE_OPERA) != NULL
	&& strstr(ua, DEVICE_ANDROID) != NULL
	&& strstr(ua, MOBI) != NULL;
}
