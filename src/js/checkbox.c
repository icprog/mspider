#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "browser.h"
#include "interface.h"
#include "html.h"
#include "dw_widget.h"
#include "mgwidget.h"
#include "spidermonkey.h"
#include "jsmisc.h"
#include "mgdconfig.h"
#ifdef JS_SUPPORT

/* local data and function definition */
enum checkbox_prop {
	JSP_CHECKBOX_ALT,
	JSP_CHECKBOX_CHECKED,
	JSP_CHECKBOX_DISABLED,
	JSP_CHECKBOX_DEFCHECKED,
	JSP_CHECKBOX_FORM,
	JSP_CHECKBOX_ID,
	JSP_CHECKBOX_NAME,
	JSP_CHECKBOX_TYPE,
	JSP_CHECKBOX_VALUE
};
static JSBool checkbox_get_property(JSContext *ctx, JSObject *obj, jsval id, jsval *vp);
static JSBool checkbox_set_property(JSContext *ctx, JSObject *obj, jsval id, jsval *vp);
static int get_checkbox_state(jsobject *jsobj);
static void set_checkbox_state(int boolean, jsobject *jsobj);
static JSBool	checkbox_blur(JSContext *ctx, JSObject *obj, uintN argc,jsval *argv, jsval *rval);
static JSBool	checkbox_click(JSContext *ctx, JSObject *obj, uintN argc,jsval *argv, jsval *rval);
static JSBool 	checkbox_focus(JSContext *ctx, JSObject *obj, uintN argc,jsval *argv, jsval *rval);
/* export data sructure */
const JSClass checkbox_class = {
	"checkbox",
	JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub,
	checkbox_get_property, checkbox_set_property,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

const JSPropertySpec checkbox_props[] = {
    { "alt",      	JSP_CHECKBOX_ALT,       	JSPROP_ENUMERATE },
    { "checked",  	JSP_CHECKBOX_CHECKED,   	JSPROP_ENUMERATE },
    { "disabled",  	JSP_CHECKBOX_DISABLED,   	JSPROP_ENUMERATE },
    { "defaultChecked",  	JSP_CHECKBOX_DEFCHECKED,   	JSPROP_ENUMERATE },
    { "form",   	JSP_CHECKBOX_FORM,   	JSPROP_ENUMERATE |JSPROP_READONLY  },
    { "id",     	JSP_CHECKBOX_ID,     	JSPROP_ENUMERATE },
    { "name",    	JSP_CHECKBOX_NAME,   	JSPROP_ENUMERATE },
    { "type",   	JSP_CHECKBOX_TYPE,   	JSPROP_ENUMERATE },
    { "value",  	JSP_CHECKBOX_VALUE,   	JSPROP_ENUMERATE },
	{ NULL }
};

const JSFunctionSpec checkbox_funcs[] = {
	{ "blur",	checkbox_blur,		0 },
	{ "click",	checkbox_click,	0 },
	{ "focus",	checkbox_focus,		0 },
	{ NULL }
};

const char *checkbox_propidx[] = {
	/* props */
	"accept",		
	"accessKey",	
	"align",		
	"alt",			
	"checked",
	"defaultChecked",
	"disabled",		
	"form",			
	"id",
	"name",
	"tabIndex",
	"type",
	"value",
	/* events */
	"onBlur",
	"onClick",
	"onFocus",
	"onMouseDown",
	"onMouseUp"
};
const int checkbox_propidxlen = sizeof(checkbox_propidx)/sizeof(char*);
/* checkbox objects property and method */
static JSBool
checkbox_get_property(JSContext *ctx, JSObject *obj, jsval id, jsval *vp)
{
	int ret = 0;
	struct jsval_property prop;
    int temp;

	jsobject *jsobj = NULL;


	jsobj = JS_GetPrivate(ctx, obj);
	set_prop_undef(&prop);
	if (JSVAL_IS_STRING(id)) {
		union jsval_union v;
		jsval_to_value(ctx, &id, JSTYPE_STRING, &v);
		return JS_TRUE;
	} else if (!JSVAL_IS_INT(id)) {
		return JS_TRUE;
	}

	switch (JSVAL_TO_INT(id)) {
		case JSP_CHECKBOX_ALT:
            break;
        case JSP_CHECKBOX_CHECKED:
            temp=get_checkbox_state(jsobj);
            set_prop_boolean(&prop, temp);
            break;
        case JSP_CHECKBOX_DISABLED:
            ret = get_jsobj_disabled(jsobj);
            if ( ret != -1 ) {
                set_prop_boolean(&prop, ret); 
            }
            break;
        case JSP_CHECKBOX_DEFCHECKED:
            ret=get_props_index(checkbox_propidx,checkbox_propidxlen,"defaultChecked");
            if (jsobj->jsprops[ret]== NULL)
                jsobj->jsprops[ret]=strdup("false");
            if (strcasecmp(jsobj->jsprops[ret], "false") == 0)
                set_prop_boolean(&prop, 0); 
            else
                set_prop_boolean(&prop, 1); 

            break;
        case JSP_CHECKBOX_FORM:
            if((((jsobject*)jsobj->jsparent)->jstype == jsform)
                    &&(((jsobject*)jsobj->jsparent)->smobj!=NULL))
                set_prop_object(&prop,(JSObject *)(((jsobject*)jsobj->jsparent)->smobj)); 
            break;
        case JSP_CHECKBOX_ID:
            set_prop_string(&prop,(unsigned char*)(jsobj->jsid));
            break;
        case JSP_CHECKBOX_NAME:
            set_prop_string(&prop,(unsigned char*)(jsobj->jsname));
            break;
        case JSP_CHECKBOX_TYPE:
            set_prop_string(&prop,(unsigned char*)("checkbox"));
            break;
        case JSP_CHECKBOX_VALUE:
            ret=get_props_index(checkbox_propidx,checkbox_propidxlen,"value");
            set_prop_string(&prop,(unsigned char*)jsobj->jsprops[ret]);
            break;
        default:
            return JS_TRUE;
    }

	value_to_jsval(ctx, vp, &prop);
	return JS_TRUE;
}

static JSBool
checkbox_set_property(JSContext *ctx, JSObject *obj, jsval id, jsval *vp)
{
	union jsval_union v;
	jsobject *jsobj = NULL;

	jsobj = JS_GetPrivate(ctx, obj);

	if (JSVAL_IS_STRING(id)) {
		union jsval_union v;
		jsval_to_value(ctx, &id, JSTYPE_STRING, &v);
		return JS_TRUE;
	} else if (!JSVAL_IS_INT(id)) {
		return JS_TRUE;
	}

	switch (JSVAL_TO_INT(id)) {
        case JSP_CHECKBOX_ALT:
            break;
        case JSP_CHECKBOX_CHECKED:
            jsval_to_value(ctx, vp, JSTYPE_BOOLEAN, &v);
            set_checkbox_state(v.boolean, jsobj);
            break;
        case JSP_CHECKBOX_DISABLED:
            jsval_to_value(ctx, vp, JSTYPE_BOOLEAN, &v);
            set_jsobj_disabled(v.boolean, jsobj);
            break;
        case JSP_CHECKBOX_DEFCHECKED:
            jsval_to_value(ctx, vp, JSTYPE_STRING, &v);
            set_jsobj_props(checkbox_propidx,checkbox_propidxlen,jsobj,"defaultChecked",(char*)v.string);
            if (strcasecmp((char*)v.string, "false") == 0)
                set_checkbox_state(0, jsobj);
            else if ( strcasecmp((char*)v.string, "true") == 0 ) 
                set_checkbox_state(1, jsobj);

            break;
        case JSP_CHECKBOX_ID:
            jsval_to_value(ctx, vp, JSTYPE_STRING, &v);
            if(jsobj->jsid!=NULL)
                free(jsobj->jsid); 
            jsobj->jsid=strdup((char*)v.string);
            break;
        case JSP_CHECKBOX_NAME:
            jsval_to_value(ctx, vp, JSTYPE_STRING, &v);
            if(jsobj->jsname!=NULL)
                free(jsobj->jsname); 
            jsobj->jsname=strdup((char*)v.string);
            break;
        case JSP_CHECKBOX_VALUE:
            jsval_to_value(ctx, vp, JSTYPE_STRING, &v);
            set_jsobj_props(checkbox_propidx, checkbox_propidxlen, jsobj, "value", (char*)v.string);
            if ( strcasecmp((char*)v.string, "false") == 0 ) {
                set_checkbox_state(0, jsobj);
            } else if ( strcasecmp((char*)v.string, "true") == 0 ) {
                set_checkbox_state(1, jsobj);
            } 
            break;
        default:
            return JS_TRUE;
	}

	return JS_TRUE;
}

static JSBool	checkbox_blur(JSContext *ctx, JSObject *obj, uintN argc,jsval *argv, jsval *rval)
{
    HWND hwnd;
    jsobject *jsobj = NULL;

    jsobj = JS_GetPrivate(ctx, obj);
    if (!jsobj || !(jsobj->htmlobj)){
        return JS_TRUE;
    }
    hwnd = ((DwMgWidget *)jsobj->htmlobj)->window;
    if (!hwnd){
        return JS_TRUE;
    }
    if (IsWindowEnabled (hwnd))
#if ((MINIGUI_MAJOR_VERSION >= 2 && MINIGUI_MICRO_VERSION >= 3 && MINIGUI_MINOR_VERSION >= 0))
        SetNullFocus(GetParent(hwnd));
#else
        SendMessage(hwnd, MSG_KILLFOCUS, 0 , 0);
#endif
	return JS_TRUE;
}
static JSBool	checkbox_click(JSContext *ctx, JSObject *obj, uintN argc,jsval *argv, jsval *rval)
{
	HWND hwnd;
	jsobject *jsobj = NULL;
    int temp;

	jsobj = JS_GetPrivate(ctx, obj);
	if ( !jsobj || !(jsobj->htmlobj) ) {
		return JS_TRUE;
	}
	hwnd = ((DwMgWidget*)jsobj->htmlobj)->window;
	if ( !hwnd ) {
        return JS_TRUE;
	}

    if (!IsWindowEnabled (hwnd))
        return JS_TRUE;

    temp=SendMessage(hwnd,BM_GETCHECK,0,0);
    if(temp)
        SendMessage(hwnd,BM_SETCHECK,0,0);
    else
        SendMessage(hwnd,BM_SETCHECK,BM_CLICK,0);

    return JS_TRUE;
}
static JSBool 	checkbox_focus(JSContext *ctx, JSObject *obj, uintN argc,jsval *argv, jsval *rval)
{
	HWND hwnd;
	jsobject *jsobj = NULL;

	jsobj = JS_GetPrivate(ctx, obj);
	if ( !jsobj || !(jsobj->htmlobj) ) {
		return JS_TRUE;
	}
	hwnd = ((DwMgWidget*)jsobj->htmlobj)->window;
	if ( !hwnd ) {
        return JS_TRUE;
	}

    if (IsWindowEnabled (hwnd))
        SetFocusChild(hwnd);
    return JS_TRUE;
}
static int get_checkbox_state(jsobject *jsobj)
{
	DwWidget *dw = NULL;
	DwMgWidget *mgdw = NULL;

	dw = (DwWidget*)jsobj->htmlobj;
	if ( !dw ) {
		return -1;
	}
	mgdw = (DwMgWidget*)dw;
	if ( !mgdw->window ) {
		return -1;
	}

    return SendMessage(mgdw->window, BM_GETCHECK, 0, 0);
}

static void set_checkbox_state(int boolean, jsobject *jsobj)
{
	DwWidget *dw = NULL;
	DwMgWidget *mgdw = NULL;

	dw = (DwWidget*)jsobj->htmlobj;
	if ( !dw ) {
		return;
	}
	mgdw = (DwMgWidget*)dw;
	if ( !mgdw->window ) {
		return;
	}

    SendMessage(mgdw->window, BM_SETCHECK, boolean , 0);
}

/*
 *	
 *	return value: 
 *
 *	-1:	error
 *	 0:	enabled
 *	 1:	disabled
 *
 */

const char* getcheckboxeventstr(jsobject *jsobj, int rc)
{
	int idx = -1;

	switch ( rc ) {
	case BN_CLICKED:
		idx = get_props_index(checkbox_propidx, checkbox_propidxlen, "onClick");
		break;
	case BN_PUSHED:
		idx = get_props_index(checkbox_propidx, checkbox_propidxlen, "onMouseDown");
		break;
	case BN_UNPUSHED:
		idx = get_props_index(checkbox_propidx, checkbox_propidxlen, "onMouseUp");
		break;
	case BN_SETFOCUS:
		idx = get_props_index(checkbox_propidx, checkbox_propidxlen, "onFocus");
		break;
	case BN_KILLFOCUS:
		idx = get_props_index(checkbox_propidx, checkbox_propidxlen, "onBlur");
		break;
	default:
		break;
	}

	if ( idx == -1 ) {
		return NULL;
	}

	return jsobj->jsprops[idx];
}

#endif
