/* 
**  mod_negotiation_device.c -- Apache negotiation by device module
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_negotiation_device.c
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /negotiation_device in as follows:
**
**    #   apache2.conf
**    LoadModule negotiation_device_module modules/mod_negotiation_device.so
**    <Location /s>
**    DeviceNegotiation /ios     +iphone_or_ipod
**    DeviceNegotiation /android +android_phone
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
*/ 

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_log.h"
#include "ap_config.h"
#include "mobile_detect.h"

typedef struct {
    apr_array_header_t *entries;
} neg_device_dir_config;

typedef struct {
    char *path;
    apr_array_header_t *conds;
} neg_device_entry;

module AP_MODULE_DECLARE_DATA negotiation_device_module;

static void *config_perdir_create(apr_pool_t *p, char *path) {
    neg_device_dir_config *dconf;

    dconf = (neg_device_dir_config*)apr_palloc(p, sizeof(neg_device_dir_config));
    dconf->entries = apr_array_make(p, 2, sizeof(neg_device_entry*));
    return (void *)dconf;
}
static const char *set_neg_device(cmd_parms *parms, void *dummy, const char *arg) {
    neg_device_dir_config *dconf = (neg_device_dir_config*) dummy;
    neg_device_entry **newentry;

    char action, *w;
    int (**mdetect)(const char*);

    newentry = apr_array_push(dconf->entries);
    *newentry = (neg_device_entry*)apr_pcalloc(parms->pool, sizeof(neg_device_entry));
    (*newentry)->conds = apr_array_make(parms->pool, 10, sizeof(void*));

    while(*arg) {
        w = ap_getword_conf(parms->temp_pool, &arg);
        action = '\0';

        if ((*w == '+') || (*w == '-')) {
            action = *(w++);
        } else {
	    continue;
	}

	mdetect = apr_array_push((*newentry)->conds);
        if (strcmp(w, "smartphone") == 0) {
	} else if (strcmp(w, "iphone_or_ipod") == 0) {
	    *mdetect = mdetect_iphone_or_ipod;
	} else if (strcmp(w, "ipad") == 0) {
	    *mdetect = mdetect_ipad;
	} else if (strcmp(w, "ios") == 0) {
	    *mdetect = mdetect_ios;
        } else if (strcmp(w, "android") == 0) {
	    *mdetect = mdetect_android;
	} else if (strcmp(w, "android_phone") == 0) {
	    *mdetect = mdetect_android_phone;
	} else if (strcmp(w, "android_tablet") == 0) {
	    *mdetect = mdetect_android_tablet;
	} else if (strcmp(w, "android_web_kit") == 0) {
	    *mdetect = mdetect_android_web_kit;
	} else if (strcmp(w, "opera_mobile") == 0) {
	    *mdetect = mdetect_opera_mobile;
	} else {
	    return (char*)apr_pstrcat(parms->pool, "Invalid detect method: ", w, NULL);
	}
    }
    if (*w == '/') {
	(*newentry)->path = (char*)apr_pstrdup(parms->pool, w);
    } else {
	return "Last token must be a forward path starts with /.";
    }
    return NULL;
}

static int fix_request(request_rec *r)
{
    const char *user_agent = NULL;
    const char *ua_lower = NULL;
    neg_device_dir_config *dconf;
    neg_device_entry *entry;
    int (*mdetect)(const char *);
    int i, j, match;

    if (!ap_is_initial_req(r)) {
	return DECLINED;
    }

    dconf = ap_get_module_config(r->per_dir_config, &negotiation_device_module);

    if (r->headers_in != NULL) {
	user_agent = apr_table_get(r->headers_in, "User-Agent");
	if (user_agent != NULL) {
	    ua_lower = apr_pstrdup(r->pool, user_agent);
	    ap_str_tolower(ua_lower);
	    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "entries->nelts=%d", dconf->entries->nelts);
	    for(i = 0; i < dconf->entries->nelts; i++) {
		entry = ((neg_device_entry**)(dconf->entries->elts))[i];
		match = TRUE;
		ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "conds->nelts=%d", entry->conds->nelts);
		for (j = 0; j < entry->conds->nelts; j++) {
		    mdetect = ((void**)(entry->conds->elts))[j];
		    match &= (*mdetect)(ua_lower);
		}
		ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "UA=%s, match=%d", ua_lower, match);
		if (match) {
		    char *uri = (char*)apr_pstrcat(r->pool, entry->path, r->uri, r->args ? "?" : NULL, r->args, NULL);
		    ap_internal_redirect(uri, r);
		    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
				  "r->status=%d", r->status);
		    return OK;
		}
	    }
	}
    }

    return OK;
}

static const command_rec negotiation_device_cmds[] = 
    {   
	AP_INIT_RAW_ARGS("NegotiationDevice", set_neg_device, NULL, OR_ALL,
		     "negotiate rules by device"),
	{NULL}
    };

static void neg_device_register_hooks(apr_pool_t *p)
{
    ap_hook_fixups(fix_request, NULL, NULL, APR_HOOK_LAST);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA negotiation_device_module = {
    STANDARD20_MODULE_STUFF, 
    config_perdir_create,  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    negotiation_device_cmds,   /* table of config file commands       */
    neg_device_register_hooks  /* register hooks                      */
};

