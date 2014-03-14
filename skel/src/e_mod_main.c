#include <e.h>
#include "e_mod_main.h"

typedef struct _Instance Instance;

struct _Instance 
{
  E_Gadcon_Client *gcc;
  Evas_Object * widget;
};

/* Functions Proto Requirement for Gadcon*/
static E_Gadcon_Client *_gc_init(E_Gadcon *gc, const char *name, const char *id, const char *style);
static void _gc_shutdown(E_Gadcon_Client *gcc);
static void _gc_orient(E_Gadcon_Client *gcc, E_Gadcon_Orient orient);
static const char *_gc_label(const E_Gadcon_Client_Class *client_class);
static Evas_Object *_gc_icon(const E_Gadcon_Client_Class *client_class, Evas *evas);
static const char *_gc_id_new(const E_Gadcon_Client_Class *client_class);

/* Define the class and gadcon function this module provides*/
static const E_Gadcon_Client_Class _gc_class = 
{
   GADCON_CLIENT_CLASS_VERSION, "skel", 
   {
      _gc_init, _gc_shutdown, _gc_orient, _gc_label, _gc_icon, _gc_id_new, NULL, NULL
   },
   E_GADCON_CLIENT_STYLE_PLAIN
};

static Config_Item *_config_item_get(const char *id);

static E_Config_DD *conf_edd = NULL;
static E_Config_DD *conf_item_edd = NULL;

Config *skel_conf = NULL;

static E_Gadcon_Client *
_gc_init(E_Gadcon *gc, const char *name, const char *id, const char *style) 
{
  Instance *inst;
  E_Gadcon_Client *gcc;
  char buf[4096]; //PATH_MAX
  const char *mod_dir;

  inst = E_NEW(Instance, 1);   
  snprintf(buf, sizeof(buf), "%s/skel.edj",e_module_dir_get(skel_conf->module));

  inst->widget = edje_object_add(gc->evas);
  if (!e_theme_edje_object_set(inst->widget, 
        "base/theme/modules/skel", "modules/skel/main"))
  edje_object_file_set(inst->widget, buf, "modules/skel/main");
  evas_object_show(inst->widget);

  gcc = e_gadcon_client_new(gc, name, id, style, inst->widget);
  gcc->data = inst;
  inst->gcc = gcc;

  skel_conf->instances = eina_list_append(skel_conf->instances, inst);

    /* Handlers for mouse events*/
   /*evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_DOWN,
				                            _button_cb_mouse_down, inst);
   evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_IN,
                                    _tclock_cb_mouse_in, inst);
   evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_OUT,
                                    _tclock_cb_mouse_out, inst);
  */
  return gcc;
}

static void
_gc_shutdown(E_Gadcon_Client *gcc) 
{
   Instance *inst;
  
   if( !(inst = gcc->data)) return;
   
   /* Remove mouse handlers 
    evas_object_event_callback_del(inst->tclock, EVAS_CALLBACK_MOUSE_DOWN,
                                  _tclock_cb_mouse_down);
    evas_object_event_callback_del(inst->tclock, EVAS_CALLBACK_MOUSE_IN,
                                  _tclock_cb_mouse_in);
    evas_object_event_callback_del(inst->tclock, EVAS_CALLBACK_MOUSE_OUT,
                                  _tclock_cb_mouse_out);
    */
   
  if (inst->widget) evas_object_del(inst->widget);

   skel_conf->instances = eina_list_remove(skel_conf->instances, inst);
   E_FREE(inst);
}

static void
_gc_orient(E_Gadcon_Client *gcc, E_Gadcon_Orient orient) 
{
  Instance *inst;
  Evas_Coord mw, mh;

  inst = gcc->data;
  mw = 0, mh = 0;
  edje_object_size_min_get(inst->widget, &mw, &mh);
  if ((mw < 1) || (mh < 1))
    edje_object_size_min_calc(inst->widget, &mw, &mh);
  if (mw < 4) mw = 4;
  if (mh < 4) mh = 4;
  e_gadcon_client_min_size_set(gcc, mw, mh);
}

static const char *
_gc_label(const E_Gadcon_Client_Class *client_class) 
{
   return "skel";
}

static Evas_Object *
_gc_icon(const E_Gadcon_Client_Class *client_class, Evas *evas) 
{
   Evas_Object *o;
   char buf[4096];

   if (!skel_conf->module) return NULL;

   snprintf(buf, sizeof(buf), "%s/e-module-skel.edj", 
	    e_module_dir_get(skel_conf->module));

   o = edje_object_add(evas);
   edje_object_file_set(o, buf, "icon");
   return o;
}

static const char *
_gc_id_new(const E_Gadcon_Client_Class *client_class)
{
   Config_Item *ci;

   ci = _config_item_get(NULL);
   return ci->id;
}

static Config_Item *
_config_item_get(const char *id) 
{
  Eina_List *l = NULL;
  Config_Item *ci;


  if (!id)
  {
	  /* Create id */
	  char buf[128];
    int num = 0;
    if (skel_conf->items)
	  {
	    ci = eina_list_last(skel_conf->items)->data;
      const char *p;
	    p = strrchr(ci->id, '.');
	    if (p) num = atoi(p + 1) + 1;
	  }
	  snprintf(buf, sizeof(buf), "%s.%d", _gc_class.name, num);
	  id = buf;
  }
  else
  {
	  for (l = skel_conf->items; l; l = l->next) 
	  {
	    ci = l->data;
	    if (!ci->id) continue;
	    if (!strcmp(ci->id, id))
	    {
		    return ci;
	    }
	  }
  }

  ci = E_NEW(Config_Item, 1);
  ci->id = eina_stringshare_add(id);

  skel_conf->items = eina_list_append(skel_conf->items, ci);
  return ci;
}

EAPI E_Module_Api e_modapi = { E_MODULE_API_VERSION, "Skel" };

EAPI void *
e_modapi_init(E_Module *m) 
{
  char buf[4096];
  #if HAVE_LOCALE_H
    setlocale(LC_ALL, "");
  #endif
  snprintf(buf, sizeof(buf), "%s/locale", e_module_dir_get(m));
  bindtextdomain(PACKAGE, buf);
  bind_textdomain_codeset(PACKAGE, "UTF-8");

  conf_item_edd = E_CONFIG_DD_NEW("Skel_Config_Item", Config_Item);
  conf_edd = E_CONFIG_DD_NEW("Skel_Config", Config);
   
  #undef T
  #define T Config_Item
  #undef D
  #define D conf_item_edd
  E_CONFIG_VAL(D, T, id, STR);
   
  #undef T
  #define T Config
  #undef D
  #define D conf_edd
  E_CONFIG_LIST(D, T, items, conf_item_edd);
   
  skel_conf = e_config_domain_load("module.skel", conf_edd);
  if (!skel_conf) 
  {
	  Config_Item *ci;
	
	  skel_conf = E_NEW(Config, 1);
	  ci = E_NEW(Config_Item, 1);
	  ci->id = eina_stringshare_add("0");
	
	  skel_conf->items = eina_list_append(skel_conf->items, ci);
  }
   
  skel_conf->module = m;
  e_gadcon_provider_register(&_gc_class);
  return m;
}

EAPI int
e_modapi_shutdown(E_Module *m) 
{
  skel_conf->module = NULL;
  e_gadcon_provider_unregister(&_gc_class);
  /*Delete dialog for configuration if needed*/
  /*if (skel_conf->config_dialog)
     e_object_del(E_OBJECT(skel_conf->config_dialog));
  */
  /*delete main menu of the widget if needed*/
  /*if (skel_conf->menu) 
  {
	  e_menu_post_deactivate_callback_set(skel_conf->menu, NULL, NULL);
	  e_object_del(E_OBJECT(skel_conf->menu));
	  skel_conf->menu = NULL;
  }*/
  /*Remove items*/
  while (skel_conf->items) 
  {
	  Config_Item *ci;
	
	  ci = skel_conf->items->data;
	  if (ci->id) eina_stringshare_del(ci->id);
	  skel_conf->items = eina_list_remove_list(skel_conf->items, skel_conf->items);
	  E_FREE(ci);
  }

  E_FREE(skel_conf);
  E_CONFIG_DD_FREE(conf_item_edd);
  E_CONFIG_DD_FREE(conf_edd);
  return 1;
}

EAPI int
e_modapi_save(E_Module *m) 
{
   e_config_domain_save("module.skel", conf_edd, skel_conf);
   return 1;
}
