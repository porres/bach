/**
 @file
 portal.c
 
 @name 
 bach.portal
 
 @realname 
 bach.portal
 
 @type
 object
 
 @module
 bach
 
 @author
 bachproject
 
 @digest 
 Let lllls through
 
 @description
 Performs no operation upon the incoming llll, except optional format (text/native) conversions.
 
 @discussion
 As usual, the formats of the output lllls are controlled by the <m>out</m> attribute.
 In <o>bach.portal</o>, though, the latter can have an additional value, <m>p</m>,
 which allows controlling the output format for subpatchers and abstractions.
 If <o>bach.portal</o>'s <m>out</m> attribute is set to <m>p</m>,
 its outlets are not immediately initialized as soon as the patch is loaded.
 For this reason, a message coming from a <o>loadbang</o> or <o>loadmess</o> object might get lost, or be output in the wrong format.
 When <o>bach.portal</o> is ready, it outputs a <m>bang</m> from its rightmost outlet
 which can be used to trigger any message to output automatically when the patch is loaded.
 <o>bach.portal</o> has a communication mechanism with <o>bach.args</o>,
 ensuring that for each subpatcher <o>bach.portal</o> is ready when <o>bach.args</o> outputs its arguments.
 In this way, it is safe for the arguments of the patch to trigger messages to the outside of the patch itself.
 
 @category
 bach, bach objects, bach llll
 
 @keywords
 portal, text, native, disabled, outlet, convert

 @seealso
 bach.reg
 
 @owner
 Andrea Agostini
 */

#include "llllobj.h"
#include "ext_common.h"
#include "ext_globalsymbol.h"
#include "ext_maxtypes.h"

typedef struct _portal
{
	struct llllobj_object 	n_ob;
	long				n_proxies;	
	void				**n_proxy;
	long				n_clone;
	long				n_in;	
	long				n_loadbanged;
	long				n_processed;
	long				n_ready;
	long				n_retrying;
    t_object            *n_patcher;
} t_portal;

void portal_assist(t_portal *x, void *b, long m, long a, char *s);

t_portal *portal_new(t_symbol *s, short ac, t_atom *av);
void portal_free(t_portal *x);

void portal_bang(t_portal *x);
void portal_int(t_portal *x, t_atom_long v);
void portal_float(t_portal *x, double v);
void portal_anything(t_portal *x, t_symbol *msg, long ac, t_atom *av);
void portal_loadbang(t_portal *x);

void portal_dopargs(t_portal *x, t_symbol *msg, long argc, t_atom *argv);
void portal_out_arg_process(t_portal *x, long ac, t_atom *av);
void portal_add_to_portalpatchers(t_portal *x, t_object *patcher);
void portal_remove_from_portalpatchers(t_portal *x);

t_class *portal_class;

int T_EXPORT main()
{
	t_class *c;
	
	common_symbols_init();
	llllobj_common_symbols_init();
	
	if (llllobj_check_version(BACH_LLLL_VERSION) || llllobj_test()) {
		error("bach: bad installation");
		return 1;
	}
	
	c = class_new("bach.portal", (method)portal_new, (method)portal_free, (short)sizeof(t_portal), 0L, A_GIMME, 0);
	
	
	// @method llll @digest Output the llll unchanged
	// @description An llll in any inlet is immediately output from the current outlet unchanged,
	// except for the optional format conversion according to the <m>out</m> attribute.
	class_addmethod(c, (method)portal_anything,		"anything",		A_GIMME,	0);
	
	class_addmethod(c, (method)portal_int,			"int",			A_LONG,		0);
	class_addmethod(c, (method)portal_float,		"float",		A_FLOAT,	0);
	class_addmethod(c, (method)portal_anything,		"list",			A_GIMME,	0);
	class_addmethod(c, (method)portal_bang,			"bang",			0);
	class_addmethod(c, (method)portal_loadbang,		"loadbang",		0);
	class_addmethod(c, (method)portal_loadbang,		"dblclick",		A_CANT,		0);
	class_addmethod(c, (method)portal_assist,		"assist",		A_CANT,		0);
	class_addmethod(c, (method)portal_dopargs,		"dopargs",		A_GIMME,	0);

	llllobj_class_add_out_attr(c, LLLL_OBJ_VANILLA);
	
	class_register(CLASS_BOX, c);
	portal_class = c;
	
	if (!bach->b_initpargs)
		bach->b_initpargs = (t_object *) object_new_typed(CLASS_NOBOX, gensym("bach.initpargs"), 0, NULL);
		
	CLASS_ATTR_LONG(c, "clone", 0, t_portal, n_clone);
	CLASS_ATTR_FILTER_CLIP(c, "clone", 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, "clone", 0, "onoff", "Clone");

	dev_post("bach.portal compiled %s %s", __DATE__, __TIME__);
	
	return 0;
}

void portal_loadbang(t_portal *x)
{
	// this calls dopargs for all the objects that have been registered so far
	// that is, all the objects that have been instantiated so far with @out p
	if (x->n_processed)
		return;
	x->n_processed = 0;
	object_method_typed(bach->b_initpargs, gensym("run"), 0, NULL, NULL);
	if (x->n_ready)
		llllobj_outlet_bang((t_object *) x, LLLL_OBJ_VANILLA, x->n_proxies + 1);
	else
		x->n_loadbanged = 1;
}

void portal_bang(t_portal *x)
{	
	portal_anything(x, _sym_bang, 0, NULL);
}

void portal_int(t_portal *x, t_atom_long v)
{
	t_atom outatom;
	atom_setlong(&outatom, v);
	portal_anything(x, _sym_int, 1, &outatom);
}

void portal_float(t_portal *x, double v)
{
	t_atom outatom;
	atom_setfloat(&outatom, v);
	portal_anything(x, _sym_float, 1, &outatom);
}

void portal_anything(t_portal *x, t_symbol *msg, long ac, t_atom *av)
{
	t_llll *inlist = NULL;
	
	long inlet = proxy_getinlet((t_object *) x);
	if (msg == LLLL_NATIVE_MSG)
		inlist = llllobj_get_retained_native_llll_from_args(ac, av); // non-NULL if it's a valid native list

	if (inlist && (x->n_ob.l_out[inlet].b_type == LLLL_O_NATIVE)) { // native -> native
		if (!x->n_clone) {
			outlet_anything(x->n_ob.l_out[inlet].b_outlet, msg, ac, av);
			llll_release(inlist);
		} else {
			t_llll *cloned = llll_clone(inlist);
			llll_release(inlist);
			llllobj_outlet_llll((t_object *) x, LLLL_OBJ_VANILLA, inlet, cloned);
			llll_free(cloned);
		}
	} else if (!inlist && (x->n_ob.l_out[inlet].b_type == LLLL_O_TEXT)) { // text -> text
		if (msg != _sym_list)
			outlet_anything(x->n_ob.l_out[inlet].b_outlet, msg, ac, av);
		else
			outlet_list(x->n_ob.l_out[inlet].b_outlet, NULL, ac, av);
	} else if (inlist) { // native -> text
		llllobj_outlet_llll((t_object *) x, LLLL_OBJ_VANILLA, inlet, inlist);
		llll_release(inlist);
	} else { // text -> native
		inlist = llllobj_parse_llll((t_object *) x, LLLL_OBJ_VANILLA, msg, ac, av, LLLL_PARSE_DONT); // LLLL_PARSE_DONT is ignored
		if (inlist) {
			llllobj_outlet_llll((t_object *) x, LLLL_OBJ_VANILLA, inlet, inlist);
			llll_free(inlist);
		}
	}
}

void portal_assist(t_portal *x, void *b, long m, long a, char *s)
{	
	if (m == ASSIST_INLET) {
		sprintf(s, "llll %ld", a + 1); // @in 0 @loop 1 @type llll @digest llll to be output unchanged
	} else if (a > x->n_proxies)
		sprintf(s, "Readybang"); // @out 1 @type llll @digest Bang when ready
		// @description If <o>bach.portal</o>'s <m>out</m> attribute is set to <m>p</m>,
		// its outlets are not immediately initialized as soon as the patch is loaded.
		// For this reason, a message coming from a <o>loadbang</o> or <o>loadmess</o> object
		// might get lost, or be output in the wrong format.
		// When <o>bach.portal</o> is ready, it outputs a <m>bang</m> from its rightmost outlet
		// which can be used to trigger any message to output automatically when the patch is loaded.
	else {
		char *type = NULL;
		llllobj_get_llll_outlet_type_as_string((t_object *) x, LLLL_OBJ_VANILLA, a, &type);
		sprintf(s, "llll %ld (%s)", a + 1, type); // @out 0 @loop 1 @type llll @digest Output llll
		// @description The llll coming from the corresponding inlet is immediately output unchanged.
	}
}

void portal_free(t_portal *x)
{
	long i;
	t_atom init_atom[1];
	atom_setobj(init_atom, x);
	object_method_typed(bach->b_initpargs, _sym_remove, 1, init_atom, NULL);
    portal_remove_from_portalpatchers(x);
	for (i = x->n_proxies; i > 0; i--)
		object_free_debug(x->n_proxy[i]);
	bach_freeptr(x->n_proxy);
	llllobj_obj_free((t_llllobj_object *) x);
}

t_portal *portal_new(t_symbol *s, short ac, t_atom *av)
{
	t_portal *x = NULL;
	long i, proxies = 0;
	long true_ac;
	char outlets[LLLL_MAX_OUTLETS], *this_outlets;
	t_max_err err = MAX_ERR_NONE;
	
	true_ac = attr_args_offset(ac, av);
	
	if ((x = (t_portal *) object_alloc_debug(portal_class))) {
		// @arg 0 @name inlets @optional 1 @type int @digest Number of llll inlets/outlets
		proxies = (true_ac && atom_gettype(av) == A_LONG) ? atom_getlong(av) : 1; // one for each list (plus one standard input for ctl inlet)
		x->n_proxies = CLAMP(proxies, 1, LLLL_MAX_INLETS) - 1;
		
		x->n_proxy = (void **) bach_newptr((x->n_proxies + 1) * sizeof(void *));
		for (i = x->n_proxies; i > 0; i--)
			x->n_proxy[i] = proxy_new_debug((t_object *) x, i, &x->n_in);

		attr_args_process(x, ac, av);
		
//		if (!bach_is_loadtime()) {
			//dev_object_post((t_object *) x, "typed in!");
			defer_low(x, (method) portal_loadbang, NULL, 0, NULL);
//		}
		
		if (x->n_ob.l_outtypes && (x->n_ob.l_outtypes->s_name[0] == 'p' || x->n_ob.l_outtypes->s_name[0] == 'P')) { // @out p
			x->n_ob.l_out = (t_llllobj_out *) bach_newptrclear((x->n_proxies + 2) * sizeof(t_llllobj_out));
			for (i = x->n_proxies + 1; i >= 0; i--)
				x->n_ob.l_out[i].b_outlet = outlet_new(x, NULL);

			t_atom init_atom[1];
			atom_setobj(init_atom, x);
			object_method_typed(bach->b_initpargs, _sym_add, 1, init_atom, NULL);
		} else {
			this_outlets = outlets;
			*this_outlets++ = 'b';
			for (i = x->n_proxies; i >= 0; i--, this_outlets++)
				*this_outlets = '4';
			*this_outlets = 0;
			llllobj_obj_setup((t_llllobj_object *) x, 0, outlets);
			x->n_ready = 1;
			//dev_object_post((t_object *) x, "portal %p open", x);
		}
	} else 
		error(BACH_CANT_INSTANTIATE);

	if (x && err == MAX_ERR_NONE)
		return x;

	object_free_debug(x); // unlike freeobject(), this works even if the argument is NULL
	return NULL;
}

void portal_dopargs(t_portal *x, t_symbol *msg, long argc, t_atom *argv)
{
	t_object *patcher; // the patcher containing the portal
	t_jbox *pfftbox; 
	t_object *box; // the box containing the patcher
	t_atombuf *patcherargs;
	char outlets[LLLL_MAX_OUTLETS], *this_outlets;
	long i;
	long ac = 0;
	t_atom *av = NULL;
	t_max_err err = MAX_ERR_NONE;
	t_symbol *grandma_classname;

//	object_obex_lookup(x, gensym("#B"), (t_object **) &box);
	object_obex_lookup(x, gensym("#P"), (t_object **) &patcher);

    portal_add_to_portalpatchers(x, patcher);
    
	if (!(box = object_attr_getobj(patcher, _sym_box)))
		object_method(patcher, _sym_getassoc, &box);

	if (box) {
		grandma_classname = object_classname(box);
		
		
		if (grandma_classname == gensym("bpatcher")) {	
			object_attr_getvalueof(box, _sym_args, &ac, &av);
			portal_out_arg_process(x, ac, av);
			bach_freeptr(av);
		}
		else {
			if (grandma_classname == gensym("pfft~") || 
				grandma_classname == gensym("poly~") || 
				grandma_classname == gensym("dynamicdsp~") ||
				grandma_classname == gensym("dynamicserial~")) {
				err = object_obex_lookup(box, gensym("#B"), (t_object **) &pfftbox);  // it's a poly
				patcherargs = (t_atombuf *) pfftbox->b_binbuf;
			} else
				patcherargs = (t_atombuf *) ((t_jbox *) box)->b_binbuf;
			portal_out_arg_process(x, patcherargs->a_argc, patcherargs->a_argv);
		}
	} else {
		if (!x->n_retrying) {
			x->n_retrying = 1;
			defer_low(x, (method) portal_dopargs, NULL, 0, NULL);
			return;
		} else {
			object_warn((t_object *) x, "out p has no effect in a top-level patcher: defaulting to native");
		}
	}
	x->n_retrying = 0;

	this_outlets = outlets;
	*this_outlets++ = 'b';
	for (i = x->n_proxies; i >= 0; i--, this_outlets++)
		*this_outlets = '4';
	*this_outlets = 0;
	llllobj_obj_setup((t_llllobj_object *) x, 0, outlets);
	x->n_ready = 1;
	if (x->n_loadbanged)
		portal_loadbang(x);
	//dev_object_post((t_object *) x, "portal %p open", x);
	return;
}

void portal_add_to_portalpatchers(t_portal *x, t_object *patcher)
{
    if (x->n_patcher || !patcher) // means that for some reason (initpargs...) it has already been called
        return;
    x->n_patcher = patcher;
    
    t_hashtab *portalpatchers = bach->b_portalpatchers;
    t_llll *portals = NULL;
    
    if (hashtab_lookup(portalpatchers, (t_symbol *) patcher, (t_object **) &portals) != MAX_ERR_NONE) {
        portals = llll_get();
        hashtab_store(portalpatchers, (t_symbol *) patcher, (t_object *) portals);
    }

    llll_appendobj(portals, x);
}

void portal_remove_from_portalpatchers(t_portal *x)
{
    t_object *patcher = x->n_patcher;
    if (!patcher)
        return;
    t_hashtab *portalpatchers = bach->b_portalpatchers;
    t_llll *portals = NULL;

    hashtab_lookup(portalpatchers, (t_symbol *) patcher, (t_object **) &portals);
    t_llllelem *portal_elem;
    for (portal_elem = portals->l_head; portal_elem; portal_elem = portal_elem->l_next) {
        if (hatom_getobj(&portal_elem->l_hatom) == (t_object *) x)
            break;
    }
    if (!portal_elem) {
        object_bug((t_object *) x, "can't remove from portalpatchers");
        return;
    }
    llll_destroyelem(portal_elem);
    if (portals->l_size == 0) {
        llll_free(portals);
        hashtab_delete(portalpatchers, (t_symbol *) patcher);
    }
    return;
}

void portal_out_arg_process(t_portal *x, long ac, t_atom *av)
{
	long i;
	long true_ac = attr_args_offset(ac, av);
	for (i = true_ac; i < ac; i++) {
		if (atom_getsym(av + i) == gensym("@out") && i + 1 < ac && atom_gettype(av + i + 1) == A_SYM && *(atom_getsym(av + i + 1)->s_name) != '@')
			attr_args_process(x, 2, av + i);
	}
}