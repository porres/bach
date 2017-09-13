/**
	@file
	score.c
	
	@name 
	bach.score
	
	@realname 
	bach.score

	@type
	object
	
	@module
	bach

	@author
	bachproject
	
	@digest 
	Display and edit a score in classical notation
	
	@description
	Displays a score in classical notation, and provides the interface to interact with it.
	 
	@discussion
	
	@category
	bach, bach objects, bach notation, bach interface, U/I
	
 	@keywords
	display, edit, measured, classical, score, measure, note, chord, voice, slot, marker, 
	write, open, MusicXML, MIDI, import, export, microtone, duration, cent, midicent, 
	velocity, extra, pitch breakpoint, glissando, pitch, select, enharmonicity, graphic,
	articulation

	@palette
	YES
	
	@palette category
	bach, bach objects, bach notation, bach interface

	@seealso
	bach.roll, bach.slot, bach.beatbox, bach.beatunbox, bach.quantize, bach.score2roll, bach.playkeys, bach.chordrev, bach.chordrot,
	bach.restrot, bach.tierev, bach.fromc&amp;r, bach.toc&amp;r, bach.slot2line, bach.slot2curve, 
	bach.slot2filtercoeff, bach.ezmidiplay, bach.sliceheader, Hello World, Brother John, Through The Looking Glass, 
	Slot Machines, The Gift Of Synthesis, Real Time Stories, The Minimal Way, The World Outside, Beams Through The Trees, A Mazing Grace, Edit Path
	
	@owner
	Daniele Ghisi
*/

#include "score_files.h"
#include "llll_modifiers.h"
#include "notation_attrs.h"
#include "notation_goto.h"
#include "ext_drag.h"

DEFINE_LLLL_ATTR_DEFAULT_GETTER(t_notation_obj, loop_region_as_llll, score_getattr_loop)


// global class pointer variable
t_class	*s_score_class = 0;


// functions
BEGIN_CHECK_LINKAGE

void score_assist(t_score *x, void *b, long m, long a, char *s);
void score_inletinfo(t_score *x, void *b, long a, char *t);
void score_free(t_score *x);
t_score* score_new(t_symbol *s, long argc, t_atom *argv);
void score_paint(t_score *x, t_object *view);
t_max_err score_notify(t_score *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void score_getmaxID(t_score *x);

void score_undo(t_score *x);
void score_redo(t_score *x);
void score_inhibit_undo(t_score *x, long val);
void score_prune_last_undo_step(t_score *x);

void score_resetslotinfo(t_score *x);
void score_resetnoteheadinfo(t_score *x);
void score_resetarticulationinfo(t_score *x);

void score_bang(t_score *x);
void score_anything(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_float(t_score *x, double num);
long score_acceptsdrag(t_score *x, t_object *drag, t_object *view);
void score_clock(t_score *x, t_symbol *s);

void score_lambda(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_join(t_score *x);
void score_deletegrace(t_score *x);
void score_mergegrace(t_score *x);

// interface functions
void score_fixvzoom(t_score *x);
void score_ceilmeasures(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_subscore(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_collapse(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_merge(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_overtype(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_snap_pitch_to_grid(t_score *x, t_symbol *s, long argc, t_atom *argv);

t_llll* get_score_values_as_llll_for_pwgl(t_score *x);

void score_clearnotes(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_cleartempi(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_cleararticulations(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_dump(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_score2roll(t_score *x, t_symbol *s, long argc, t_atom *argv);
void select_all(t_score *x);

void score_send_current_chord(t_score *x);

// interface functions
void score_getdomain(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getdomainpixels(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getlength(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_testdomain(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_domain(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_setdomain(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_clearselection(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getnumvoices(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getnummeasures(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getnumchords(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getnumnotes(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getpixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_gettimeatpixel(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_pixeltotime(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_timetopixel(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_timepoint(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_dumpnotepixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_dumpmeasurepixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_dumpvoicepixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getvzoom(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getzoom(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_openslotwin(t_score *x, t_symbol *s, long argc, t_atom *argv);

// setters
t_max_err score_setattr_nonantialiasedstaff(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_tonedivision(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_accidentalsgraphic(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_accidentalspreferences(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_enharmonictable(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_showstems(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_zoom(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_vzoom(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_set_noteheads_font(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_set_accidentals_font(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_set_articulations_font(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_maxdots(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_centeredwholerests(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_autoparserhythm(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_outputtrees(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_maketreecompatiblewithts(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_allowbeaming(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_extendbeamsoverrests(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_restswithinbeaming(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_maxrestfloatingsteps(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_syncopationasymratio(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_spacingtype(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_spacingparameter(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_spacingwidth(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_spacingproportionality(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_minmeaswidth(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_minmeaswidthpersymunit(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_syncopationposdurratio(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_showaccidentalspreferences(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_showaccidentalstiepreferences(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_cautionaryaccidentals(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_cautionaryaccidentalsdecay(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_cautionaryaccidentalsremind(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_maxbeamslope(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_maxbeamdeltay(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_reversegracedirection(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_restswithinbeaming(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_constraintbeamsinspaces(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_tieassignspitch(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_autocompletemeasures(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_dontbeamtupletswithotherthings(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_admitdottedtuplets(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_showtimesignatures(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_hidevoices(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_voicespacing(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_showsynchronoustempionce(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_lyricsaffectspacing(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_dynamicsaffectspacing(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_showmeasurenumbers(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_treehandling(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_gracedur(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_gracemaxperc(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_hidetupletshapeonbeams(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_leveltobeam(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_breakrootlevelbeam(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_jointuplets(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_simplifytuplets(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_showtupletspreferences(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_beamgracesequences(t_score *x, t_object *attr, long ac, t_atom *av);
t_max_err score_setattr_loop(t_score *x, t_object *attr, long ac, t_atom *av);

// mute/lock/solo
void score_mute(t_score *x);
void score_unmute(t_score *x);
void score_solo(t_score *x);
void score_unsolo(t_score *x);
void score_lock(t_score *x);
void score_unlock(t_score *x);

void evaluate_selection(t_score *x, long modifiers, char alsosortselectionbyonset, t_llll *forced_routers = NULL);
void selection_send_command(t_score *x, long modifiers, long command_number, char alsosortselectionbyonset);

// conversions
void calculate_tuttipoints(t_score *x);
t_note *find_nearest_note(t_score *x, double xpos, double ypos);
t_measure *find_measure_containing_ux(t_score *x, long num_voice, double ux, char always_return_something);

void score_int(t_score *x, t_atom_long chordID);

// mouse functions
void score_mouseenter(t_score *x, t_object *patcherview, t_pt pt, long modifiers);
void score_mouseleave(t_score *x, t_object *patcherview, t_pt pt, long modifiers);
void score_mousemove(t_score *x, t_object *patcherview, t_pt pt, long modifiers);
void score_mousedrag(t_score *x, t_object *patcherview, t_pt pt, long modifiers);
void score_mousedown(t_score *x, t_object *patcherview, t_pt pt, long modifiers);
void score_mouseup(t_score *x, t_object *patcherview, t_pt pt, long modifiers);
void score_mousewheel(t_score *x, t_object *view, t_pt pt, long modifiers, double x_inc, double y_inc);
long score_oksize(t_score *x, t_rect *newrect);
long score_key(t_score *x, t_object *patcherview, long keycode, long modifiers, long textcharacter);
long score_keyfilter(t_score *x, t_object *patcherview, long *keycode, long *modifiers, long *textcharacter);
void score_enter(t_score *x);
void score_mousedoubleclick(t_score *x, t_object *patcherview, t_pt pt, long modifiers);
void score_focusgained(t_score *x, t_object *patcherview);
void score_focuslost(t_score *x, t_object *patcherview);
void score_edclose(t_score *x, char **ht, long size);
void score_okclose(t_score *x, char *s, short *result);
void score_jsave(t_score *x, t_dictionary *d);
void score_preset(t_score *x);
void score_begin_preset(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_restore_preset(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_end_preset(t_score *x);
void score_playselection(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_play(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_do_play(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_stop(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_do_stop(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_pause(t_score *x);
void score_play_offline(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_task(t_score *x);

void score_name(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_nameappend(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_slottoname(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_nametoslot(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_clearnames(t_score *x, t_symbol *s, long argc, t_atom *argv);

void score_copy_selected_measures(t_score *x, char cut);
void score_paste_replace_measures(t_score *x, char also_paste_tempi);
void score_paste_measures(t_score *x, long at_this_measure_1based, long from_this_voice_1based, char also_paste_tempi, char ignore_copied_voice_offset, long how_many_times);

void score_copy(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_cut(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_paste(t_score *x, t_symbol *s, long argc, t_atom *argv);


// auxiliary functions
t_note* ID_to_note(t_score *x, long ID);
t_chord* ID_to_chord(t_score *x, long ID);
void verbose_print(t_score *x);
t_scorevoice* nth_scorevoice(t_score *x, long n);

//t_llll* get_voice_extras_values_as_llll(t_score *x, t_scorevoice *voice);
void send_all_values_as_llll(t_score *x, long send_what_for_header);
void send_subscore_values_as_llll(t_score *x, t_llll* whichvoices, long start_meas, long end_meas, t_llll *what_to_dump);
t_llll* get_subscore_values_as_llll(t_score *x, t_llll* whichvoices, long start_meas, long end_meas, t_llll *what_to_dump);
t_llll* get_subvoice_values_as_llll(t_score *x, t_scorevoice *voice, long start_meas, long end_meas, char tree, char also_get_level_information);
void send_score_values_as_llll(t_score *x, long send_what);
void send_measuresinfo_values_as_llll(t_score *x);
void send_cents_values_as_llll(t_score *x, char tree);
void send_durations_values_as_llll(t_score *x, char tree);
void send_velocities_values_as_llll(t_score *x, char tree);
void send_ties_values_as_llll(t_score *x, char tree);
void send_extras_values_as_llll(t_score *x, char tree);
void set_voice_tstempo_values_from_llll(t_score *x, t_llll* onsets, t_scorevoice *voice);
void set_voice_durations_values_from_llll(t_score *x, t_llll* durations, t_scorevoice *voice);
void set_voice_velocities_values_from_llll(t_score *x, t_llll* velocities, t_scorevoice *voice);
void set_voice_ties_values_from_llll(t_score *x, t_llll* velocities, t_scorevoice *voice);
void set_voice_graphic_values_from_llll(t_score *x, t_llll* graphics, t_scorevoice *voice);
void set_voice_breakpoints_values_from_llll(t_score *x, t_llll* breakpoints, t_scorevoice *voice);
void set_voice_slots_values_from_llll(t_score *x, t_llll* slots, t_scorevoice *voice);
void set_voice_articulation_values_from_llll(t_score *x, t_llll* articulations, t_scorevoice *voice);
void set_all_extras_values_from_llll(t_score *x, t_llll* extras);
void set_all_graphic_values_from_llll(t_score *x, t_llll* graphics);
void set_all_breakpoints_values_from_llll(t_score *x, t_llll* breakpoints);
void set_all_slots_values_from_llll(t_score *x, t_llll* slots);
void set_all_articulations_values_from_llll(t_score *x, t_llll* articulations);
void addchord_from_llll(t_score *x, t_llll* chord, t_scorevoice *voice);
void set_measure_durations_values_from_llll(t_score *x, t_llll* durations, t_measure *measure);
void set_measure_velocities_values_from_llll(t_score *x, t_llll* measure_velocities, t_measure *measure);
void set_measure_ties_values_from_llll(t_score *x, t_llll* measure_ties, t_measure *measure);
void set_measure_graphic_values_from_llll(t_score *x, t_llll* graphic, t_measure *measure);
void set_measure_breakpoints_values_from_llll(t_score *x, t_llll* breakpoints, t_measure *measure);
void set_measure_slots_values_from_llll(t_score *x, t_llll* breakpoints, t_measure *measure);
void set_measure_articulations_values_from_llll(t_score *x, t_llll* articulations, t_measure *measure);

t_llll* get_timesignature_as_llll(t_timesignature *ts);
t_llll* get_voice_measures_values_as_llll(t_scorevoice *voice);
t_llll* measure_get_tempi_as_llll(t_measure *measure);

// xml communication
t_llll* get_score_values_for_xml_as_llll(t_score *x);

// ref-voice is 0-based, whichvoices are 1-based
t_llll* get_collapsed_score_as_llll(t_score *x, t_llll *whichvoices, long reference_voice, t_llll *what_to_dump);

// modifying selection
void score_sel_delete(t_score *x, t_symbol *s, long argc, t_atom *argv);
char score_sel_delete_item(t_score *x, t_notation_item *curr_it, char *need_check_scheduling, t_llll *slots_to_transfer_to_next_note_in_chord_1based = NULL, char transfer_slots_even_if_empty = false);
void score_sel_deletemeasures(t_score *x);
void score_clear_selection(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_rebeam(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_reparse(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_clearmarkers(t_score *x);
void score_select(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_selectmeasures(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_change_onset(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_change_velocity(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_change_cents(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_change_pitch(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_change_measureinfo(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_change_tie(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_add_breakpoint(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_erase_breakpoints(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_add_slot(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_erase_slot(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_change_slot_value(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_dumpselection(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_sendcommand(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_sel_retranscribe(t_score *x);
void score_sel_resetgraphic(t_score *x);
void score_sel_resetarticulations(t_score *x);
void score_inscreen(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_inscreenpos(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_inscreenmeas(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_legato(t_score *x);
void score_glissando(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_resetlocalwidthmultiplformeas(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_setmeasureparameters(t_score *x, t_symbol *s, long argc, t_atom *argv);
char score_sel_dilate_mc(t_score *x, double mc_factor, double fixed_mc_point);
void score_split(t_score *x, t_symbol *s, long argc, t_atom *argv);


void score_addmarker(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_deletemarker(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_clearmarkers(t_score *x);
void score_markername(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getmarker(t_score *x, t_symbol *s, long argc, t_atom *argv);

void score_getcursor(t_score *x);
void score_setcursor(t_score *x, t_symbol *s, long argc, t_atom *argv);
void score_getloop(t_score *x);

void changed_bang(t_score *x, int change_type);

long get_global_num_notes_voice(t_scorevoice *voice);
void recalculate_all_chord_parameters(t_score *x);
void recalculate_all_tuttipoints_spacing(t_score *x);
void check_all_measures_autocompletion(t_score *x);
void check_all_measures_ties(t_score *x);
void recalculate_all_beamings(t_score *x);
void recalculate_all_utf_measure_timesignatures(t_score *x);
char merge(t_score *x, t_rational threshold_sym, double threshold_cents, char gathering_policy_sym, char gathering_policy_cents, char selection_only);
void clear_voice(t_score *x, t_scorevoice *voice);
void delete_voice_tempi(t_score *x, t_scorevoice *voice);
void delete_measure_tempi(t_score *x, t_measure *measure);
void recompute_all_and_redraw(t_score *x);
void refresh_all_tuttipoints_offset_ux(t_score *x);
char delete_selected_measures(t_score *x);
char clear_selected_measures(t_score *x);
char duplicate_selected_measures(t_score *x);
void delete_all_chords_from_measure(t_score *x, t_measure *measure);
void recompute_all(t_score *x);
void overtype(t_score *x, t_timepoint *from_here, t_timepoint *to_here, t_llll *new_content);

char change_pitch_for_selection(t_score *x, double delta, char mode, char allow_voice_change, char snap_pitch_to_grid);
t_chord *shift_note_allow_voice_change(t_score *x, t_note *note, double delta, char mode, char *old_chord_deleted, char allow_voice_change);
void snap_pitch_to_grid_voice(t_score *x, t_scorevoice *voice);
char quick_merge_selection(t_score *x);
char split_selection(t_score *x, long how_many, char merge_alltied_chords_first);
char gather_all_selected_chords_with_merge_flag(t_score *x, t_chord *chord, char only_tied_ones, t_chord **reference, t_notation_item **next_sel_pointer);
char chord_merge_mc(t_score *x, t_chord *chord, double threshold_cents, char gathering_policy_cents);
void refresh_measure_numbers(t_score *x, t_scorevoice *voice);
void turn_chord_into_rest(t_score *x, t_chord *chord);
void calculate_all_tempi_remaining_onsets(t_score *x);
t_measure *create_and_insert_new_measure(t_score *x, t_scorevoice *voice, t_measure *refmeasure, char direction, unsigned long force_ID, t_measure *refmeasure_for_measureinfo, char clone_tempi);
void insert_new_measure_in_all_voices(t_score *x, t_scorevoice *reference_voice, long ref_meas_ID, char direction, 
										t_measure **added_measures, long *num_added_meas, char also_add_undo_ticks, e_undo_operations *undo_op);
void insert_measures_from_message(t_score *x, long start_voice_num_one_based, long end_voice_num_one_based, long ref_meas_num_one_based, t_llll *meas_ll, char allow_multiple_measures_per_voice);
char delete_selected_tempi(t_score *x);
char switch_interpolation_to_selected_tempi(t_score *x);

char is_note_in_selected_region(t_score *x, t_chord *chord, t_note *note);
char is_chord_in_selected_region(t_score *x, t_chord *chord);
char change_selection_tempo(t_score *x, double delta_tempo, char also_synchronous_tempi);
char move_selection_breakpoint(t_score *x, double delta_x_pos, double delta_y_pos, char tail_only);
void insert_measure(t_score *x, t_scorevoice *voice, t_measure *measure_to_insert, t_measure *after_this_measure, unsigned long force_ID);
void insert_tuttipoint(t_score *x, t_tuttipoint *tuttipoint_to_insert, t_tuttipoint *after_this_tuttipoint);
void select_all_measure_in_selected_measures_range(t_score *x);	
void exit_linear_edit(t_score *x);
void end_editing_measure_in_linear_edit(t_score *x, t_measure *measure);
char splatter_selected_tree_nodes(t_score *x, char also_lock_general_mutex, char lock_measure_rhythmic_trees, char create_undo_tick);
char destroy_selected_tree_nodes(t_score *x, char also_lock_general_mutex, char also_lock_measure_rhythmic_tree, char also_add_undo_tick);

void clear_notes_flag(t_score *x, long flag);
void clear_measures_and_chords_flag(t_score *x, long flag);

void score_slice(t_score *x, t_symbol *s, long argc, t_atom *argv);

// new play
void set_everything_unplayed(t_score *x);

// slurs
t_slur *add_slur(t_score *x, t_note *start_note, t_note *end_note);
char delete_slurs_in_selection(t_score *x);
void change_slur_starting_note(t_score *x, t_slur *slur, t_note *newnote);
void change_slur_ending_note(t_score *x, t_slur *slur, t_note *newnote);

// selections
void preselect_elements_in_region_for_mouse_selection(t_score *x, double ms1, double ms2, double mc1, double mc2, long v1, long v2);

// quantization
void score_quantize(t_score *x, t_symbol *s, long argc, t_atom *argv);

// exotic
void score_copy_slots_to_tied_noted_sequences(t_score *x);

void score_set_spacingtype(t_score *x, long l);

DEFINE_NOTATIONOBJ_CHARPTR_GETTER(show_measure_numbers, num_voices)
DEFINE_NOTATIONOBJ_SYMPTR_GETTER(clefs_as_symlist, num_voices)
DEFINE_NOTATIONOBJ_SYMPTR_GETTER(keys_as_symlist, num_voices)
DEFINE_NOTATIONOBJ_SYMPTR_GETTER(full_acc_repr, num_voices)
DEFINE_NOTATIONOBJ_CHARPTR_GETTER(hidevoices_as_charlist, num_voices)
DEFINE_NOTATIONOBJ_DBLPTR_GETTER(voiceuspacing_as_floatlist, num_voices_plus_one)

// clipboard
t_clipboard clipboard = {k_NONE, k_NOTATION_OBJECT_SCORE, NULL, NULL, 0.};

END_CHECK_LINKAGE

t_max_err score_notify(t_score *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    notationobj_handle_attr_modified_notify((t_notation_obj *)x, s, msg, sender, data);
    jbox_notify((t_jbox *)x, s, msg, sender, data);
	return MAX_ERR_NONE;
}



long score_acceptsdrag(t_score *x, t_object *drag, t_object *view)
{
    return notationobj_acceptsdrag((t_notation_obj *)x, drag, view);
}

void check_all_notes_order(t_score *x){
	t_scorevoice *voice = x->firstvoice;
	t_measure *meas;
	t_chord *ch;
	for (voice = x->firstvoice; voice && (voice->v_ob.number < x->r_ob.num_voices); voice = voice->next)
		for (meas = voice->firstmeasure; meas; meas = meas->next)
			for (ch = meas->firstchord; ch; ch = ch->next)
				check_notes_order(ch);
}


void score_bang(t_score *x){
// reconstruct score!

	long i;
	t_llll *inlist[7];
	t_llll *measuresinfo, *cents, *durations, *velocities, *ties, *extras;
	t_scorevoice *voice; t_measure *measure; t_chord *chord; t_note *note;
	char more_than_one_extra_defined = false;
	
	char debug = false;

	for (i = 0; i < 7; i++) 
		inlist[i] = llllobj_get_store_contents((t_object *) x, LLLL_OBJ_UI, i, true);
	
	if (inlist[6] && inlist[6]->l_head && hatom_gettype(&inlist[6]->l_head->l_hatom) == H_LLLL)
		more_than_one_extra_defined = true;

	if (!debug)
		create_whole_score_undo_tick(x); 

	lock_general_mutex((t_notation_obj *)x);

	x->r_ob.nullify_incorrect_ties = false;
	if (x->must_append_measures) {	// mode "addmeasures": we add some nils in each voice for each skip of existing measure!
		long j;
		for (i = 1; i <= 6; i++) {
			if (inlist[i]) {
				t_llllelem *elem; t_scorevoice *voice = x->firstvoice;
				t_llllelem *startelem = (i == 6 && !more_than_one_extra_defined && inlist[i]->l_head) ? inlist[i]->l_head->l_next : inlist[i]->l_head;
				
				for (elem = startelem; elem && voice; elem = elem->l_next) {
					if (hatom_gettype(&elem->l_hatom) == H_LLLL) {
						if (i == 6 && more_than_one_extra_defined){ // specific case when more than 1 extra is defined
							t_llll *this_extra = hatom_getllll(&elem->l_hatom);
							if (this_extra && this_extra->l_head && hatom_gettype(&this_extra->l_head->l_hatom) == H_SYM && this_extra->l_head->l_next) {
								t_llllelem *elem2;
								voice = x->firstvoice;
								for (elem2 = this_extra->l_head->l_next; elem2 && voice; elem2 = elem2->l_next) {
									if (hatom_gettype(&elem2->l_hatom) == H_LLLL) {
										for (j = 0; j < voice->num_measures; j++)
											llll_prependllll(hatom_getllll(&elem2->l_hatom), llll_get(), 0, WHITENULL_llll);
										voice = voice->next;
									}
								}
							}
						} else {
							for (j = 0; j < voice->num_measures; j++)
								llll_prependllll(hatom_getllll(&elem->l_hatom), llll_get(), 0, WHITENULL_llll);
							voice = voice->next;
						}
					}
				}
			}
		}
	} else {
		if (x->r_ob.autoclear)
			score_clear_all(x);
	}
		
	measuresinfo = inlist[1];
	cents = inlist[2];
	durations = inlist[3];
	velocities = inlist[4];
	ties = inlist[5];
	extras = inlist[6];
	
    // detecting number of voices and auto-sizing if needed
    long num_introduced_voices = -1; // = don't change
    if (measuresinfo->l_depth > 2)
        num_introduced_voices = MAX(num_introduced_voices, (long)measuresinfo->l_size);
    num_introduced_voices = MAX(num_introduced_voices, (long)cents->l_size);
    num_introduced_voices = MAX(num_introduced_voices, (long)durations->l_size);
    num_introduced_voices = MAX(num_introduced_voices, (long)velocities->l_size);
    num_introduced_voices = MAX(num_introduced_voices, (long)ties->l_size);
    if (extras && extras->l_head) {
        if (hatom_gettype(&extras->l_head->l_hatom) == H_SYM) // just 1 extra defined
            num_introduced_voices = MAX(num_introduced_voices, (long)extras->l_size - 1);
        else {
            t_llllelem *elem;
            for (elem = extras->l_head; elem; elem = elem->l_next) {
                if (hatom_gettype(&elem->l_hatom) == H_LLLL) {
                    t_llll *ll = hatom_getllll(&elem->l_hatom);
                    if (ll->l_head && hatom_gettype(&ll->l_head->l_hatom) == H_SYM)
                        num_introduced_voices = MAX(num_introduced_voices, (long)ll->l_size - 1);
                }
            }
        }
    }
    
    // we change the number of voices if there was no "addmeasures" message and the number of voice inserted is different from the existing one,
    // or if there was "addmeasures" message, and the number of voices inserted is greater than the existing ones.
    if (x->r_ob.autosize) {
        if (num_introduced_voices > 0 &&
            ((!x->must_append_measures && x->r_ob.num_voices != num_introduced_voices) ||
             (x->must_append_measures && x->r_ob.num_voices < num_introduced_voices)))
            set_numvoices((t_notation_obj *)x, num_introduced_voices);
	}

	
/*	// DEBUG!!!
	t_llll *debug = score_separate2gathered_syntax(measuresinfo, cents, durations, velocities, ties, extras);
	llllobj_outlet_llll((t_object *)x, LLLL_OBJ_UI, 0, debug);
	llll_free(debug);
	unlock_general_mutex((t_notation_obj *)x);
	return;
*/
	
	// reconstruct score from separate parameters
	x->durations_given = false;
	
	if (measuresinfo && (measuresinfo->l_size > 0)) {
		set_all_tstempo_values_from_llll(x, measuresinfo, num_introduced_voices);
		llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), 1);
	}
	if (cents && (cents->l_size > 0)) { 
		set_all_cents_values_from_llll(x, cents, true, num_introduced_voices);
		llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), 2); 
	}
	if (durations && (durations->l_size > 0)) {
		x->durations_given = true;
		set_all_durations_values_from_llll(x, durations, num_introduced_voices);
		llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), 3);
	}
	if (velocities && (velocities->l_size > 0)) { 
		set_all_velocities_values_from_llll(x, velocities, num_introduced_voices);
		llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), 4);
	}
	if (ties && (ties->l_size > 0)) {
		set_all_ties_values_from_llll(x, ties, num_introduced_voices);
		llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), 5);
		x->r_ob.nullify_incorrect_ties = true; // is it needed?
	}
	if (extras && (extras->l_size > 0)) {
		set_all_extras_values_from_llll(x, extras);
		llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), 6);
	}

	if (!x->must_append_measures) {
		// compute all the approximations for all the notes
		for (voice = x->firstvoice; voice && (voice->v_ob.number < x->r_ob.num_voices); voice = voice->next)
			for (measure = voice->firstmeasure; measure; measure = measure->next)
				for (chord = measure->firstchord; chord; chord = chord->next)
					for (note = chord->firstnote; note; note = note->next)
						note_compute_approximation((t_notation_obj *) x, note); 
		recompute_all(x); 
	} else {
		for (voice = x->firstvoice; voice && (voice->v_ob.number < x->r_ob.num_voices); voice = voice->next)
			for (measure = voice->firstmeasure; measure; measure = measure->next)
				if (measure->need_recompute_beamings) 
					recompute_all_for_measure((t_notation_obj *)x, measure, true);
		x->r_ob.need_recompute_tuttipoints = true;
		x->r_ob.need_reassign_local_spacing = true;
	}

	check_all_notes_order(x);
		
	// TOGLIERE
	set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
	perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);

	x->must_append_measures = false;
	x->r_ob.nullify_incorrect_ties = true;

	unlock_general_mutex((t_notation_obj *)x);
	
	for (i = 0; i < 7; i++)
		llll_free(inlist[i]);

	if (!debug) {
		handle_rebuild_done((t_notation_obj *) x);
		
		x->r_ob.hscrollbar_pos = 0.; // needed here, to initialize r_ob->screen_ux_start
		update_hscrollbar((t_notation_obj *)x, 1);
		
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_SCORE);
	}
}


void score_begin_preset(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	x->r_ob.preset_ac = atom_getlong(argv) ;
	x->r_ob.preset_av = (t_atom *)bach_newptr(x->r_ob.preset_ac * sizeof(t_atom));
	
	// could allocate memory here etc.
//	post("begin_preset %ld values will be arriving",x->r_ob.preset_ac);
}

void score_restore_preset(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	long start, size;
	
	start = atom_getlong(argv);
	size = atom_getlong(argv + 1);
//	post("restore_preset start %ld size %ld", start,size);
	
	sysmem_copyptr(argv+2, x->r_ob.preset_av+start, size*sizeof(t_atom));
}

void score_end_preset(t_score *x)
{
//	post("end_preset received");
	score_anything(x, NULL, x->r_ob.preset_ac,x->r_ob.preset_av);
	bach_freeptr(x->r_ob.preset_av);
}


void score_preset(t_score *x){
	t_atom temp[256];
	void *buf;
	long i, index, size;
	char wrote = false, begin = true, done = false;
	t_llll *whole_info = get_score_values_as_llll(x, k_CONSIDER_FOR_SAVING, k_HEADER_ALL, true, x->r_ob.output_and_save_level_types, true, false); // we store in the preset the clefs changes

	// 1. we deparse the list
	t_atom *av = NULL, *this_av = NULL;
	long ac = llll_deparse(whole_info, &av, 0, /* LLLL_D_FLOAT64 | */  LLLL_D_QUOTE); // 0 is the offset
	this_av = av;

	// 2. We fill the binbuf
	buf = gensym("_preset")->s_thing; 
	if (!buf)	// no preset object
		return;
		
	index = 0;
	while (!done) {
		i = index;
		size = ac - index;
		if (size > 250)
			size = 250;
		else	// you are at the end
			done = true;
		
		sysmem_copyptr(this_av, temp+5, 250*sizeof(t_atom));

		if (size) {
			atom_setobj(temp, x);
			atom_setsym(temp + 1, ob_sym(x));
			if (begin) {
				atom_setsym(temp + 2, gensym("begin_preset"));
				atom_setlong(temp + 3, ac);
				binbuf_insert(buf, NULL, 4, temp);
				begin = false;
			}
			atom_setsym(temp + 2, gensym("restore_preset"));
			atom_setlong(temp + 3, index);
			atom_setlong(temp + 4, size);
			binbuf_insert(buf, NULL, size + 5, temp);
			wrote = true;
		}
		index += size;
		this_av += 250;
	}
	if (wrote) {
		atom_setobj(temp, x);
		atom_setsym(temp + 1, ob_sym(x));
		atom_setsym(temp + 2, gensym("end_preset"));
		binbuf_insert(buf, NULL, 3, temp);
	}
		
	if (av) bach_freeptr(av);
}

// *******************************************************************************************
// communication with bach.quantize
// *******************************************************************************************

void score_quantize(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// we send a message like
	// quantize <cents> <durations> <velocities> <ties> <extras>
	// formatted for quantization
	
	t_llll *out_llll = llll_get();
	
	t_llll *out_cents = llll_get();
	t_llll *out_durations = llll_get();
	t_llll *out_velocities = llll_get();
	t_llll *out_ties = llll_get();
	t_llll *out_graphic = llll_get();
	t_llll *out_breakpoints = llll_get();
	t_llll *out_slots = llll_get();
	t_llll *out_extras = llll_get();

	t_llll *what_to_dump_llll;
	long what_to_dump = k_HEADER_ALL;

	t_chord *chord;
	t_scorevoice *voice;

	llll_appendsym(out_graphic, _llllobj_sym_graphic, 0, WHITENULL_llll);
	llll_appendsym(out_breakpoints, _llllobj_sym_breakpoints, 0, WHITENULL_llll);
	llll_appendsym(out_slots, _llllobj_sym_slots, 0, WHITENULL_llll);

	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice;	voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
		t_llll *out_voice_cents = llll_get();
		t_llll *out_voice_durations = llll_get();
		t_llll *out_voice_velocities = llll_get();
		t_llll *out_voice_ties = llll_get();
		t_llll *out_voice_graphic = llll_get();
		t_llll *out_voice_breakpoints = llll_get();
		t_llll *out_voice_slots = llll_get();

		for (chord = voice->firstmeasure ? voice->firstmeasure->firstchord : NULL; chord; chord = get_next_chord(chord)) {
			if (chord->r_sym_duration.r_num > 0) { // real chord
				t_note *note;
				t_llll *this_event_cents = llll_get();
				t_llll *this_event_velocities = llll_get();
				t_llll *this_event_ties = llll_get();
				t_llll *this_event_graphic = llll_get();
				t_llll *this_event_breakpoints = llll_get();
				t_llll *this_event_slots = llll_get();
				
				for (note = chord->firstnote; note; note = note->next){
					llll_appenddouble(this_event_cents, note->midicents, 0, WHITENULL_llll);
					llll_appendlong(this_event_velocities, note->velocity, 0, WHITENULL_llll);
					llll_appendllll(this_event_graphic, note_get_graphic_values_no_router_as_llll((t_notation_obj *) x, note), 0, WHITENULL_llll);
					llll_appendllll(this_event_breakpoints, note_get_breakpoints_values_no_router_as_llll((t_notation_obj *) x, note), 0, WHITENULL_llll);
					llll_appendllll(this_event_slots, note_get_slots_values_no_header_as_llll((t_notation_obj *) x, note, false), 0, WHITENULL_llll);
					llll_appendlong(this_event_ties, note->tie_to ? 1 : 0, 0, WHITENULL_llll);
				}
				
/*				char debug1[1000], debug2[1000], debug3[1000], debug4[1000], debug5[1000], debug6[1000];
				llll_to_char_array(this_event_cents, debug1, 999);
				llll_to_char_array(this_event_velocities, debug2, 999); 
				llll_to_char_array(this_event_ties, debug3, 999); 
				llll_to_char_array(this_event_graphic, debug4, 999); 
				llll_to_char_array(this_event_breakpoints, debug5, 999); 
				llll_to_char_array(this_event_slots, debug6, 999); */
				
				llll_appendllll(out_voice_cents, this_event_cents, 0, WHITENULL_llll); 
				llll_appenddouble(out_voice_durations, chord->duration_ms, 0, WHITENULL_llll); 
				llll_appendllll(out_voice_velocities, this_event_velocities, 0, WHITENULL_llll); 
				llll_appendllll(out_voice_ties, this_event_ties, 0, WHITENULL_llll); 
				llll_appendllll(out_voice_graphic, this_event_graphic, 0, WHITENULL_llll); 
				llll_appendllll(out_voice_breakpoints, this_event_breakpoints, 0, WHITENULL_llll); 
				llll_appendllll(out_voice_slots, this_event_slots, 0, WHITENULL_llll); 
			} else {
				// add a pause
				llll_appendllll(out_voice_cents, llll_get(), 0, WHITENULL_llll); 
				llll_appenddouble(out_voice_durations, -chord->duration_ms, 0, WHITENULL_llll); // pause: negative
				llll_appendllll(out_voice_velocities, llll_get(), 0, WHITENULL_llll); 
				llll_appendllll(out_voice_ties, llll_get(), 0, WHITENULL_llll); 
				llll_appendllll(out_voice_graphic, llll_get(), 0, WHITENULL_llll); 
				llll_appendllll(out_voice_breakpoints, llll_get(), 0, WHITENULL_llll); 
				llll_appendllll(out_voice_slots, llll_get(), 0, WHITENULL_llll); 
			}
		}
		
		llll_appendllll(out_cents, out_voice_cents, 0, WHITENULL_llll);
		llll_appendllll(out_durations, out_voice_durations, 0, WHITENULL_llll);
		llll_appendllll(out_velocities, out_voice_velocities, 0, WHITENULL_llll);
		llll_appendllll(out_ties, out_voice_ties, 0, WHITENULL_llll);
		llll_appendllll(out_graphic, out_voice_graphic, 0, WHITENULL_llll);
		llll_appendllll(out_breakpoints, out_voice_breakpoints, 0, WHITENULL_llll);
		llll_appendllll(out_slots, out_voice_slots, 0, WHITENULL_llll);
	}
	
	// building extras
	llll_appendllll(out_extras, out_graphic, 0, WHITENULL_llll);	
	llll_appendllll(out_extras, out_breakpoints, 0, WHITENULL_llll);	
	llll_appendllll(out_extras, out_slots, 0, WHITENULL_llll);	
	
	what_to_dump_llll = llllobj_parse_llll((t_object *)x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_RETAIN);
	what_to_dump = header_objects_to_long(what_to_dump_llll);
	if (what_to_dump == 0)
		what_to_dump = k_HEADER_ALL;
	llll_free(what_to_dump_llll);
	
	

	llll_appendsym(out_llll, gensym("quantize"), 0, WHITENULL_llll);
	llll_chain(out_llll, get_notation_obj_header_as_llll((t_notation_obj *)x, what_to_dump, false, false, true, k_CONSIDER_FOR_DUMPING));
	llll_appendllll(out_llll, out_cents, 0, WHITENULL_llll);
	llll_appendllll(out_llll, out_durations, 0, WHITENULL_llll);
	llll_appendllll(out_llll, out_velocities, 0, WHITENULL_llll);
	llll_appendllll(out_llll, out_ties, 0, WHITENULL_llll);
	llll_appendllll(out_llll, out_extras, 0, WHITENULL_llll);
	
	unlock_general_mutex((t_notation_obj *)x);

	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 0, out_llll);
	llll_free(out_llll);
}


void score_send_current_chord(t_score *x){
	t_scorevoice *voice;
	t_measure *meas;
	t_chord *chord;
	t_note *note;
	t_llll *out_llll = llll_get();
	t_llll *out_cents = llll_get();
	t_llll *out_vels = llll_get();
	
	lock_general_mutex((t_notation_obj *)x);
	double curr_pos_ux = x->r_ob.playing ? x->r_ob.play_head_ux : x->r_ob.play_head_start_ux;
	double curr_pos_ms = x->r_ob.playing ? x->r_ob.play_head_ms : x->r_ob.play_head_start_ms;
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
		for (meas = voice->firstmeasure; meas; meas = meas->next){
			if (meas->tuttipoint_reference->offset_ux + meas->start_barline_offset_ux > curr_pos_ux)
				break;
			if (meas->tuttipoint_reference->offset_ux + meas->start_barline_offset_ux + meas->width_ux < curr_pos_ux)
				continue;
			for (chord = meas->firstchord; chord; chord = chord->next){
/*
				double this_chord_offset_ux = chord->prev ? chord->stem_offset_ux : meas->start_barline_offset_ux;
				double next_chord_offset_ux = chord->next ? chord->next->stem_offset_ux : meas->start_barline_offset_ux + meas->width_ux;
				if (meas->tuttipoint_reference->offset_ux + this_chord_offset_ux > curr_pos_ux)
					break;
				if (meas->tuttipoint_reference->offset_ux + this_chord_offset_ux <= curr_pos_ux && 
					meas->tuttipoint_reference->offset_ux + next_chord_offset_ux >= curr_pos_ux) {
*/
                double this_chord_ms = chord_get_onset_ms(chord);
                t_chord *next_chord = get_next_chord(chord);
                double next_chord_ms = next_chord ? chord_get_onset_ms(get_next_chord(chord)) : x->r_ob.length_ms_till_last_note;
                if (this_chord_ms > curr_pos_ms)
                    break;

                if (this_chord_ms <= curr_pos_ms && next_chord_ms > curr_pos_ms) {
					for (note = chord->firstnote; note; note = note->next){
						
						// breakpoints
						t_bpt *prev_bpt = note->firstbreakpoint;
						while (prev_bpt && prev_bpt->next && get_breakpoint_absolute_onset(prev_bpt->next) <= curr_pos_ms)
							prev_bpt = prev_bpt->next;
						
						if (!prev_bpt || !prev_bpt->next) {
							llll_appenddouble(out_cents, note->midicents + note->lastbreakpoint->delta_mc, 0, WHITENULL_llll);
							llll_appendlong(out_vels, x->r_ob.breakpoints_have_velocity ? note->lastbreakpoint->velocity : note->velocity, 0, WHITENULL_llll);
						} else {
							double cents = rescale_with_slope(curr_pos_ms, get_breakpoint_absolute_onset(prev_bpt), get_breakpoint_absolute_onset(prev_bpt->next), 
															  note->midicents + prev_bpt->delta_mc, note->midicents + prev_bpt->next->delta_mc, prev_bpt->next->slope, false);
							double velocity;
							if (x->r_ob.breakpoints_have_velocity)
								velocity = rescale(curr_pos_ms, get_breakpoint_absolute_onset(prev_bpt), get_breakpoint_absolute_onset(prev_bpt->next), 
												   prev_bpt->velocity, prev_bpt->next->velocity);
							else 
								velocity = note->velocity;
							llll_appenddouble(out_cents, cents, 0, WHITENULL_llll);
							llll_appendlong(out_vels, velocity, 0, WHITENULL_llll);
						}
						
					}
				}
			}
		}
	}
	llll_appendsym(out_llll, _llllobj_sym_currentchord, 0, WHITENULL_llll);
	llll_appendllll(out_llll, out_cents, 0, WHITENULL_llll);
	llll_appendllll(out_llll, out_vels, 0, WHITENULL_llll);
	unlock_general_mutex((t_notation_obj *)x);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, out_llll);
	llll_free(out_llll);
}


// *******************************************************************************************
// cursor
// *******************************************************************************************




void score_setcursor(t_score *x, t_symbol *s, long argc, t_atom *argv){
    t_llll *args = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_RETAIN);
    
	if (args->l_size == 1 && is_hatom_number(&args->l_head->l_hatom)) {
		scoreapi_setcursor_from_double(x, atom_getfloat(argv));
    } else if (args->l_size == 1 && hatom_gettype(&args->l_head->l_hatom) == H_SYM) {
        t_notation_item *it;
        lock_general_mutex((t_notation_obj *)x);
        if ((it = names_to_single_notation_item((t_notation_obj *) x, args)))
            scoreapi_setcursor_from_double(x, notation_item_get_onset_ms((t_notation_obj *)x, it));
        unlock_general_mutex((t_notation_obj *)x);
	} else {
		// measure point in measure, voice
		scoreapi_setcursor_from_llll(x, args);
	}
    if (x->r_ob.notify_also_upon_messages) {
        send_moved_playhead_position((t_notation_obj *) x, 7);
    }
    llll_free(args);
}

void score_getcursor(t_score *x){
	send_playhead_position((t_notation_obj *) x, 7);
}


void score_getloop(t_score *x){
	send_loop_region((t_notation_obj *) x, 7);
}


// *******************************************************************************************
// modifying selected objects
// *******************************************************************************************


char score_sel_delete_item(t_score *x, t_notation_item *curr_it, char *need_check_scheduling, t_llll *slots_to_transfer_to_next_note_in_chord_1based, char transfer_slots_even_if_empty)
{
	char changed = 0;
	if (curr_it->type == k_NOTE) {
		t_note *nt = (t_note *) curr_it;
		notation_item_delete_from_selection((t_notation_obj *) x, curr_it);
		if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)){
			create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nt, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
            transfer_note_slots((t_notation_obj *)x, nt, slots_to_transfer_to_next_note_in_chord_1based, transfer_slots_even_if_empty);
            note_delete((t_notation_obj *)x, nt, false);
			changed = 1;
		}
	} else if (curr_it->type == k_CHORD) {
		t_chord *ch = (t_chord *) curr_it;
		notation_item_delete_from_selection((t_notation_obj *) x, curr_it);
		if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)ch)){
			create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			turn_chord_into_rest(x, ch);
			changed = 1;
		}
	} else if (curr_it->type == k_MEASURE) {
		t_measure *meas = (t_measure *) curr_it;
		notation_item_delete_from_selection((t_notation_obj *) x, curr_it);
		if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)meas)){
            char need_update_solos = false;
            if (has_measure_attached_markers(x, meas))
                create_header_undo_tick((t_notation_obj *) x, k_HEADER_MARKERS);
            if (meas->prev)
                create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas->prev, k_UNDO_MODIFICATION_CHANGE); // 'coz of ties
			create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_UNDO_MODIFICATION_ADD);
            if (delete_measure(x, meas, meas->firstchord ? get_prev_chord(meas->firstchord) : NULL, &need_update_solos)) {
				if (need_check_scheduling) *need_check_scheduling = true;
            }
            if (need_update_solos) update_solos((t_notation_obj *)x);
			recompute_all_except_for_beamings_and_autocompletion(x);
			changed = 1;
		}
	} else if (curr_it->type == k_PITCH_BREAKPOINT) {
		t_bpt *bpt = (t_bpt *) curr_it;
		notation_item_delete_from_selection((t_notation_obj *) x, curr_it);
		if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)bpt->owner)){
			create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)bpt, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			delete_breakpoint((t_notation_obj *) x, bpt);
			changed = 1;
		}
	}	
	return changed;
}

void score_sel_deletemeasures(t_score *x)
{
    delete_selected_measures(x);
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_DELETE_SELECTED_MEASURES);
}

void score_sel_delete(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_notation_item *curr_it = NULL;
	t_notation_item *curr_it2;
//	char changed = 0;
	char need_check_scheduling = false;
    
    t_llll *transfer_slots = NULL;
    long even_if_empty = false;
    t_llll *ll = llllobj_parse_llll((t_object *)x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
    llll_parseargs_and_attrs((t_object *) x, ll, "li", gensym("transferslots"), &transfer_slots, gensym("empty"), &even_if_empty);
    llll_free(ll);
    
	t_notation_item *lambda_it = x->r_ob.lambda_selected_item_ID > 0 ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : NULL;
	
	// this must be here at the beginning, because it changes the selected items!
	turn_selection_into_rests(x, true, true, true, transfer_slots, even_if_empty);

	lock_general_mutex((t_notation_obj *)x);

	for (curr_it = x->r_ob.firstselecteditem; curr_it; curr_it = curr_it2) {
		curr_it2 = curr_it->next_selected;
		
		if (lambda_it && (lambda_it == curr_it || // lambda item is exactly the item we're deleting..
						  notation_item_is_ancestor_of((t_notation_obj *)x, lambda_it, curr_it) || // or one of its ancestors...
						  notation_item_is_ancestor_of((t_notation_obj *)x, curr_it, lambda_it))) { // or one of its progeny...
//			cpost("Trying to delete item %p (type %ld). Can't.", curr_it, curr_it->type);
			object_error((t_object *)x, "Can't delete item, it's being output from the playout!");
			curr_it->flags = k_FLAG_TO_BE_DELETED;
			continue;
		}
		
		score_sel_delete_item(x, curr_it, &need_check_scheduling, transfer_slots, even_if_empty);
	}
	
    if (transfer_slots)
        llll_free(transfer_slots);

	if (need_check_scheduling)
		check_correct_scheduling((t_notation_obj *)x, false);
		
	perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
	close_slot_window((t_notation_obj *)x); // if we were in slot view...
	unlock_general_mutex((t_notation_obj *)x);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_DELETE_SELECTION);
    
}

void score_sel_snap_pitch_to_grid(t_score *x){
	char changed;
	t_llll *garbage;
	changed = snap_pitch_to_grid_for_selection((t_notation_obj *) x);

	garbage = llll_get();
	lock_general_mutex((t_notation_obj *)x);
	perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
	close_slot_window((t_notation_obj *)x); // if we were in slot view...
	unlock_general_mutex((t_notation_obj *)x);

	llll_free(garbage);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_SNAP_PITCH_TO_GRID_FOR_SELECTION);
}

void score_sel_resetarticulations(t_score *x)
{
	delete_articulations_in_selection((t_notation_obj *) x); 
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_DELETE_ARTICULATIONS_FOR_SELECTION);
}

void score_resetgraphic(t_score *x)
{
	t_scorevoice *voice;
	char changed = false;
	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
		t_measure *meas;
		for (meas = voice->firstmeasure; meas; meas = meas->next){
			t_chord *chord;
			for (chord = meas->firstchord; chord; chord = chord->next){
				t_note *note;
				create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)chord, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
				for (note = chord->firstnote; note; note = note->next){
					changed |= reset_note_graphic((t_notation_obj *) x, note);
				}
			}
		}
	}
	unlock_general_mutex((t_notation_obj *)x);
	
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_RESET_ALL_ENHARMONICITIES);
}


void score_sel_resetgraphic(t_score *x){
	char changed;
	t_llll *garbage;
	changed = reset_selection_graphic((t_notation_obj *) x);

	garbage = llll_get();
	lock_general_mutex((t_notation_obj *)x);
	perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
	close_slot_window((t_notation_obj *)x); // if we were in slot view...
	unlock_general_mutex((t_notation_obj *)x);

	llll_free(garbage);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_RESET_ENHARMONICITY_FOR_SELECTION);
}


void score_clear_selection(t_score *x, t_symbol *s, long argc, t_atom *argv){
	if (argc == 0) {
		lock_general_mutex((t_notation_obj *)x);	
		clear_preselection((t_notation_obj *) x);
		clear_selection((t_notation_obj *) x);
		unlock_general_mutex((t_notation_obj *)x);	
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	} else if (argc == 1 && atom_gettype(argv) == A_LONG) {
		lock_general_mutex((t_notation_obj *)x);	
		clear_preselection((t_notation_obj *) x);
		clear_voice_selection((t_notation_obj *) x, atom_getlong(argv) - 1);
		unlock_general_mutex((t_notation_obj *)x);	
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	}
	handle_change_selection((t_notation_obj *)x);
}

void score_clearmarkers(t_score *x){
	create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
	lock_general_mutex((t_notation_obj *)x);
	lock_markers_mutex((t_notation_obj *)x);;
	clear_all_markers((t_notation_obj *) x);
	unlock_markers_mutex((t_notation_obj *)x);;
	unlock_general_mutex((t_notation_obj *)x);
	
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CLEAR_MARKERS);
}


void score_do_selectmeasures(t_score *x, e_selection_modes mode, t_llll *selectllll){
	if (selectllll && (selectllll->l_size >= 1)) {
		long id_meas1 = -1, id_meas2 = -1;
		t_scorevoice *voice;
		t_measure *meas1, *meas2, *temp;
		t_llll *voicenums = llll_get(); // 1-based numbers of measures to select
		
		lock_general_mutex((t_notation_obj *)x);
		delete_item_type_from_selection((t_notation_obj *) x, -k_MEASURE);
		
		if ((selectllll->l_size >= 2) && (hatom_gettype(&selectllll->l_head->l_next->l_hatom) == H_LONG))
			llll_appendlong(voicenums, hatom_getlong(&selectllll->l_tail->l_hatom), 0, WHITENULL_llll);
		else if ((selectllll->l_size >= 2) && (hatom_gettype(&selectllll->l_head->l_next->l_hatom) == H_LLLL) && hatom_getllll(&selectllll->l_head->l_next->l_hatom)->l_size > 0)
			llll_chain_clone(voicenums, hatom_getllll(&selectllll->l_head->l_next->l_hatom));
		else { // all voices
			long i;
			for (i = 1; i <= x->r_ob.num_voices; i++) 
				llll_appendlong(voicenums, i, 0, WHITENULL_llll);
		}
		
		if (hatom_gettype(&selectllll->l_head->l_hatom) == H_LONG) {
			id_meas1 = hatom_getlong(&selectllll->l_head->l_hatom);
			id_meas2 = id_meas1;
		} if (hatom_gettype(&selectllll->l_head->l_hatom) == H_LLLL) {
			t_llll *thisllll = hatom_getllll(&selectllll->l_head->l_hatom);
			if ((thisllll->l_size >= 1) && (hatom_gettype(&thisllll->l_head->l_hatom) == H_LONG)){
				id_meas1 = hatom_getlong(&thisllll->l_head->l_hatom);
				id_meas2 = id_meas1;
			}
			if ((thisllll->l_size >= 2) && (hatom_gettype(&thisllll->l_head->l_next->l_hatom) == H_LONG)){
				id_meas2 = hatom_getlong(&thisllll->l_head->l_next->l_hatom);
			}
		}
		
		t_llllelem *voiceelem;
		for (voiceelem = voicenums->l_head; voiceelem; voiceelem = voiceelem->l_next) {
			if (hatom_gettype(&voiceelem->l_hatom) == H_LONG) {
				voice = nth_scorevoice(x, hatom_getlong(&voiceelem->l_hatom) - 1);
				if (voice) {
					long zerobased_id_meas1 = id_meas1 >= 0 ? id_meas1 - 1: voice->num_measures + id_meas1; // 0-based
					long zerobased_id_meas2 = id_meas2 >= 0 ? id_meas2 - 1: voice->num_measures + id_meas2; // 0-based					
					
                    // THESE TWO LINES are dirty and awful. Most of the time users might actually want not to select things, e.g. sel measure range (5 -1) in a score having 3 measures
/*					if (zerobased_id_meas1 >= voice->num_measures)
						zerobased_id_meas1 = voice->num_measures - 1;
					if (zerobased_id_meas2 >= voice->num_measures) 
						zerobased_id_meas2 = voice->num_measures - 1;
 */
                    
					if (zerobased_id_meas1 < 0 || zerobased_id_meas2 < 0 || zerobased_id_meas1 > zerobased_id_meas2) 
						continue;
					
					meas1 = nth_measure_of_scorevoice(voice, zerobased_id_meas1);
					meas2 = nth_measure_of_scorevoice(voice, zerobased_id_meas2);
					if (!meas1 || !meas2) 
						continue;
					
					temp = meas1;
					while (temp && (temp->prev != meas2)) {
						if (notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)temp)) {
							if (mode == k_SELECTION_MODE_INVOLUTIVE || mode == k_SELECTION_MODE_FORCE_UNSELECT)
								notation_item_delete_from_selection((t_notation_obj *) x, (t_notation_item *)temp);
						} else {
							if (mode == k_SELECTION_MODE_FORCE_SELECT || mode == k_SELECTION_MODE_INVOLUTIVE)
								notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)temp);
						}
						temp = temp->next;
					}
				}
			}
		}
        
		unlock_general_mutex((t_notation_obj *)x);
        x->r_ob.selection_type = get_selected_items_common_type((t_notation_obj *)x);
		handle_change_selection((t_notation_obj *)x);
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
		llll_free(voicenums);
	}
	
	
}


void score_selectmeasures(t_score *x, t_symbol *s, long argc, t_atom *argv){
// expected syntax : (meas1 meas2) voice   OR    (meas1 meas2)   OR     meas1 voice    OR   meas1
    e_selection_modes mode = symbol_to_mode((t_notation_obj *)x, s);
	t_llll *selectllll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE); // We clone it: we operate destructively
	score_do_selectmeasures(x, mode, selectllll);
	llll_free(selectllll);
}

void select_notes_with_lexpr(t_score *x, e_selection_modes mode)
{
	t_scorevoice *voice;
	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
		t_measure *measure;
		for (measure = voice->firstmeasure; measure; measure = measure->next){
			t_chord *chord;
			for (chord = measure->firstchord; chord; chord = chord->next){
				t_note *note;
				for (note = chord->firstnote; note; note = note->next){
					t_hatom *res = lexpr_eval_for_notation_item((t_notation_obj *)x, (t_notation_item *)note, x->r_ob.n_lexpr);
					if (hatom_gettype(res) == H_LONG && hatom_getlong(res) != 0) 
						notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)note);
					bach_freeptr(res);
				}
			}
		}
	}
	move_preselecteditems_to_selection((t_notation_obj *)x, mode, false, false);
	unlock_general_mutex((t_notation_obj *)x);
}


void select_rests_with_lexpr(t_score *x, e_selection_modes mode)
{
    t_scorevoice *voice;
    lock_general_mutex((t_notation_obj *)x);
    for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
        t_measure *measure;
        for (measure = voice->firstmeasure; measure; measure = measure->next){
            t_chord *chord;
            for (chord = measure->firstchord; chord; chord = chord->next) {
                if (chord->firstnote)
                    continue;
                t_hatom *res = lexpr_eval_for_notation_item((t_notation_obj *)x, (t_notation_item *)chord, x->r_ob.n_lexpr);
                if (hatom_gettype(res) == H_LONG && hatom_getlong(res) != 0)
                    notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)chord);
                bach_freeptr(res);
            }
        }
    }
    move_preselecteditems_to_selection((t_notation_obj *)x, mode, false, false);
    unlock_general_mutex((t_notation_obj *)x);
}

void score_select(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
// select all the elements within a given region
// expected syntax: ms1 ms2 cents1 cents2 voice     OR     (meas1 pim1) (meas2 pim2) cents1 cents2

    e_selection_modes mode = symbol_to_mode((t_notation_obj *)x, s);
	t_llll *selectllll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE); // We clone it: we operate destructively

	if (selectllll) { 
		double ux1, ux2, mc1, mc2;
		
// This was here to fake the behavior upon clicks. But it is only confusing!
//		lock_general_mutex((t_notation_obj *)x);
//		delete_item_type_from_selection((t_notation_obj *) x, k_MEASURE);
//		unlock_general_mutex((t_notation_obj *)x);

		if (selectllll && selectllll->l_head) {
			long head_type = hatom_gettype(&selectllll->l_head->l_hatom);
			
			// (un)sel(ect) all
			if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _sym_all) {
				if (mode == k_SELECTION_MODE_FORCE_UNSELECT)
					clear_selection((t_notation_obj *)x);
				else
					select_all(x);
				
                
            /// sel note if
			} else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_note 
						&& selectllll->l_head->l_next && 
					   hatom_gettype(&selectllll->l_head->l_next->l_hatom) == H_SYM &&
					   hatom_getsym(&selectllll->l_head->l_next->l_hatom) == _llllobj_sym_if) {
				
				t_atom *new_av = NULL; 
				long new_ac = 0;
				
				if (x->r_ob.n_lexpr) 
					lexpr_free(x->r_ob.n_lexpr);
				
				llll_behead(selectllll);
				llll_behead(selectllll);
				
				new_ac = llll_deparse(selectllll, &new_av, 0, 0);
				x->r_ob.n_lexpr = notation_obj_lexpr_new(new_ac, new_av);

				if (new_av) 
					bach_freeptr(new_av);	
				
				if (x->r_ob.n_lexpr) {
					select_notes_with_lexpr(x, mode);
				} else {
					object_error((t_object *) x, "Bad expression!");
				}
                

            /// sel rest if
            } else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_rest
                       && selectllll->l_head->l_next &&
                       hatom_gettype(&selectllll->l_head->l_next->l_hatom) == H_SYM &&
                       hatom_getsym(&selectllll->l_head->l_next->l_hatom) == _llllobj_sym_if) {
                
                t_atom *new_av = NULL;
                long new_ac = 0;
                
                if (x->r_ob.n_lexpr)
                    lexpr_free(x->r_ob.n_lexpr);
                
                llll_behead(selectllll);
                llll_behead(selectllll);
                
                new_ac = llll_deparse(selectllll, &new_av, 0, 0);
                x->r_ob.n_lexpr = notation_obj_lexpr_new(new_ac, new_av);
                
                if (new_av) 
                    bach_freeptr(new_av);	
                
                if (x->r_ob.n_lexpr) {
                    select_rests_with_lexpr(x, mode);
                } else {
                    object_error((t_object *) x, "Bad expression!");
                }

			
			// (un)sel(ect) measure range
			} else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_measure && selectllll->l_head->l_next
					   && hatom_gettype(&selectllll->l_head->l_next->l_hatom) == H_SYM && hatom_getsym(&selectllll->l_head->l_next->l_hatom) == _llllobj_sym_range) {
				
				t_llll *temp = llll_clone(selectllll);
				llll_behead(temp);
				llll_behead(temp);
				score_do_selectmeasures(x, mode, temp);
				llll_free(temp);
				
			// (un)sel(ect) chord
			} else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_chord && selectllll->l_head->l_next) {
				t_chord *to_select;
				lock_general_mutex((t_notation_obj *)x);
				if (selectllll->l_depth == 1) {
                    if ((to_select = chord_get_from_path_as_llllelem_range((t_notation_obj *)x, selectllll->l_head->l_next))) {
                        if (to_select->firstnote)
                            add_all_chord_notes_to_preselection((t_notation_obj *)x, to_select);
                        else
                            notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
                    }
				} else {
					t_llllelem *elem;
					for (elem = selectllll->l_head->l_next; elem; elem = elem->l_next) 
						if (hatom_gettype(&elem->l_hatom) == H_LLLL)
                            if ((to_select = chord_get_from_path_as_llllelem_range((t_notation_obj *)x, hatom_getllll(&elem->l_hatom)->l_head))) {
                                if (to_select->firstnote)
                                    add_all_chord_notes_to_preselection((t_notation_obj *)x, to_select);
                                else
                                    notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
                            }
				}
				move_preselecteditems_to_selection((t_notation_obj *) x, mode, false, false);
				unlock_general_mutex((t_notation_obj *)x);
				
			// (un)sel(ect) note
			} else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_note && selectllll->l_head->l_next) {
				t_note *to_select;
                
				lock_general_mutex((t_notation_obj *)x);
				if (selectllll->l_depth == 1) {
					if ((to_select = note_get_from_path_as_llllelem_range((t_notation_obj *)x, selectllll->l_head->l_next)))
						notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
				} else {
					t_llllelem *elem;
					for (elem = selectllll->l_head->l_next; elem; elem = elem->l_next) 
						if (hatom_gettype(&elem->l_hatom) == H_LLLL)
							if ((to_select = note_get_from_path_as_llllelem_range((t_notation_obj *)x, hatom_getllll(&elem->l_hatom)->l_head)))
								notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
				}
				move_preselecteditems_to_selection((t_notation_obj *) x, mode, false, false);
				unlock_general_mutex((t_notation_obj *)x);

				
			// (un)sel(ect) measure
			} else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_measure && selectllll->l_head->l_next) {
				t_measure *to_select;
				lock_general_mutex((t_notation_obj *)x);
				if (selectllll->l_depth == 1) {
					if ((to_select = measure_get_from_path_as_llllelem_range((t_notation_obj *)x, selectllll->l_head->l_next)))
						notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
				} else {
					t_llllelem *elem;
					for (elem = selectllll->l_head->l_next; elem; elem = elem->l_next) 
						if (hatom_gettype(&elem->l_hatom) == H_LLLL)
							if ((to_select = measure_get_from_path_as_llllelem_range((t_notation_obj *)x, hatom_getllll(&elem->l_hatom)->l_head)))
								notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
				}
				move_preselecteditems_to_selection((t_notation_obj *) x, mode, false, false);
				unlock_general_mutex((t_notation_obj *)x);
				
            // (un)sel(ect) breakpoints/tail if
            } else if (head_type == H_SYM && (hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_breakpoint || hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_tail) && selectllll->l_head->l_next &&
                       hatom_gettype(&selectllll->l_head->l_next->l_hatom) == H_SYM &&
                       hatom_getsym(&selectllll->l_head->l_next->l_hatom) == _llllobj_sym_if) {
                
                char tail_only = (hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_tail);
                t_atom *new_av = NULL;
                long new_ac = 0;
                
                if (x->r_ob.n_lexpr)
                    lexpr_free(x->r_ob.n_lexpr);
                
                llll_behead(selectllll);
                llll_behead(selectllll);
                
                new_ac = llll_deparse(selectllll, &new_av, 0, 0);
                x->r_ob.n_lexpr = notation_obj_lexpr_new(new_ac, new_av);
                
                if (new_av)
                    bach_freeptr(new_av);
                
                if (x->r_ob.n_lexpr) {
                    select_breakpoints_with_lexpr((t_notation_obj *)x, mode, tail_only);
                } else {
                    object_error((t_object *) x, "Bad expression!");
                }
                
                
                
            // (un)sel(ect) marker by index
            } else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_marker && selectllll->l_head->l_next) {
                t_marker *to_select;
                lock_general_mutex((t_notation_obj *)x);
                if (selectllll->l_depth == 1) {
                    if ((to_select = get_marker_from_path_as_llllelem_range((t_notation_obj *)x, selectllll->l_head->l_next)))
                        notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
                } else {
                    t_llllelem *elem;
                    for (elem = selectllll->l_head->l_next; elem; elem = elem->l_next)
                        if (hatom_gettype(&elem->l_hatom) == H_LLLL)
                            if ((to_select = get_marker_from_path_as_llllelem_range((t_notation_obj *)x, hatom_getllll(&elem->l_hatom)->l_head)))
                                notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)to_select);
                }
                move_preselecteditems_to_selection((t_notation_obj *) x, mode, false, false);
                unlock_general_mutex((t_notation_obj *)x);
                
                
                
            // (un)sel(ect) marker if
            } else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_marker && selectllll->l_head->l_next &&
                       hatom_gettype(&selectllll->l_head->l_next->l_hatom) == H_SYM &&
                       hatom_getsym(&selectllll->l_head->l_next->l_hatom) == _llllobj_sym_if) {
                
                t_atom *new_av = NULL;
                long new_ac = 0;
                
                if (x->r_ob.n_lexpr)
                    lexpr_free(x->r_ob.n_lexpr);
                
                llll_behead(selectllll);
                llll_behead(selectllll);
                
                new_ac = llll_deparse(selectllll, &new_av, 0, 0);
                x->r_ob.n_lexpr = notation_obj_lexpr_new(new_ac, new_av);
                
                if (new_av)
                    bach_freeptr(new_av);
                
                if (x->r_ob.n_lexpr) {
                    select_markers_with_lexpr((t_notation_obj *)x, mode);
                } else {
                    object_error((t_object *) x, "Bad expression!");
                }

                
            // (un)sel(ect) all markers
			} else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_markers) {
				select_all_markers((t_notation_obj *) x, mode);

            // (un)sel(ect) notes
            } else if (head_type == H_SYM && (hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_notes)) {
                select_all_notes((t_notation_obj *) x, mode);
                
            // (un)sel(ect) all all chords
            } else if (head_type == H_SYM && (hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_chords)) {
                select_all_chords((t_notation_obj *) x, mode);

            // (un)sel(ect) all rests
            } else if (head_type == H_SYM && (hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_rests)) {
                select_all_rests((t_notation_obj *) x, mode);

            // (un)sel(ect) all breakpoints
            } else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_breakpoints) {
                select_all_breakpoints((t_notation_obj *)x, mode, false);

                // (un)sel(ect) all tails
            } else if (head_type == H_SYM && hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_tails) {
                select_all_breakpoints((t_notation_obj *)x, mode, true);

            // (un)sel(ect) all measures
            } else if (head_type == H_SYM && (hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_measures)) {
                select_all_measures((t_notation_obj *) x, mode);

            // warn: maybe they meant goto!
            } else if (head_type == H_SYM && (hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_next || hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_prev || hatom_getsym(&selectllll->l_head->l_hatom) == _llllobj_sym_time)) {
                object_warn((t_object *)x, "Unknown selection mode '%s'", hatom_getsym(&selectllll->l_head->l_hatom)->s_name);
                object_warn((t_object *)x, "    Maybe you meant to use it as command for the 'goto' message?");

            // (un)sel(ect) by name
			} else if (head_type == H_SYM || selectllll->l_size == 1){
				
				lock_general_mutex((t_notation_obj *)x);
				preselect_notation_items_by_name((t_notation_obj *)x, selectllll);
				move_preselecteditems_to_selection((t_notation_obj *) x, mode, false, false);
				unlock_general_mutex((t_notation_obj *)x);
				
			// (un)sel(ect) by rectangle
			} else {
				double ms1, ms2;
				
				t_llll *voice_numbers = NULL; // will be 0-based
				if (selectllll->l_size >= 5) {
					t_llllelem *voiceelem = selectllll->l_head->l_next->l_next->l_next->l_next; 
					if (hatom_gettype(&voiceelem->l_hatom) == H_LLLL) {
						voice_numbers = llll_clone(hatom_getllll(&voiceelem->l_hatom));
						llll_develop_ranges_and_parse_negative_indices_inplace(&voice_numbers, x->r_ob.num_voices, true);
					} else if (hatom_gettype(&voiceelem->l_hatom) == H_LONG) {
						voice_numbers = llll_get();
						llll_appendlong(voice_numbers, hatom_getlong(&voiceelem->l_hatom) - 1, 0, WHITENULL_llll);
					}
				}
				
				ux1 = -1; 
				ux2 = (x->lasttuttipoint) ? x->lasttuttipoint->offset_ux + x->lasttuttipoint->width_ux : -1;

				if (selectllll->l_size >= 1)
					parse_open_timepoint_syntax_from_llllelem((t_notation_obj *)x, selectllll->l_head, &ux1, NULL, NULL, false);
				if (selectllll->l_size >= 2)
					parse_open_timepoint_syntax_from_llllelem((t_notation_obj *)x, selectllll->l_head->l_next, &ux2, NULL, NULL, false);
				
                // BEWARE: this is different with respect to bach.roll: selecting from measure (1) to measure (2) will only select elements
                // in measure 1 (measure 2 is excluded).
				ux1 -= CONST_EPSILON_SELECT;
				ux2 -= CONST_EPSILON_SELECT;
				if (ux1 > ux2) {
					llll_free(selectllll);
					return;
				}
				
				mc1 = -16000, mc2 = 32000;
				if ((selectllll->l_size >= 3) && 
					((hatom_gettype(&selectllll->l_head->l_next->l_next->l_hatom) == H_LONG) || (hatom_gettype(&selectllll->l_head->l_next->l_next->l_hatom) == H_DOUBLE) || 
					 (hatom_gettype(&selectllll->l_head->l_next->l_next->l_hatom) == H_RAT)))
					mc1 = hatom_getdouble(&selectllll->l_head->l_next->l_next->l_hatom);
				if ((selectllll->l_size >= 4) && 
					((hatom_gettype(&selectllll->l_head->l_next->l_next->l_next->l_hatom) == H_LONG) || (hatom_gettype(&selectllll->l_head->l_next->l_next->l_next->l_hatom) == H_DOUBLE) || 
					 (hatom_gettype(&selectllll->l_head->l_next->l_next->l_next->l_hatom) == H_RAT)))
					mc2 = hatom_getdouble(&selectllll->l_head->l_next->l_next->l_next->l_hatom);
				mc1 -= CONST_EPSILON_SELECT;
				mc2 += CONST_EPSILON_SELECT;
				if (mc1 > mc2) {
					llll_free(selectllll);
					return;
				}
				
				lock_general_mutex((t_notation_obj *)x);
				t_scorevoice *voice;
				clear_preselection((t_notation_obj *)x);
				if (!voice_numbers || voice_numbers->l_size == 0) {
					for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next)
						preselect_elements_in_region_for_mouse_selection(x, ux1, ux2, mc1, mc2, voice->v_ob.number, voice->v_ob.number);
				} else {
					t_llllelem *elem;
					for (elem = voice_numbers->l_head; elem; elem = elem->l_next)
						if (hatom_gettype(&elem->l_hatom) == H_LONG) {
							long this_voice_num = hatom_getlong(&elem->l_hatom);
							preselect_elements_in_region_for_mouse_selection(x, ux1, ux2, mc1, mc2, this_voice_num, this_voice_num);
						}
				}
				ms1 = unscaled_xposition_to_ms((t_notation_obj *)x, ux1, 1);
				ms2 = unscaled_xposition_to_ms((t_notation_obj *)x, ux2, 1);
				preselect_markers_in_region((t_notation_obj *)x, ms1, ms2); 
				move_preselecteditems_to_selection((t_notation_obj *) x, mode, false, false);
				unlock_general_mutex((t_notation_obj *)x);
			}
		}

		handle_change_selection((t_notation_obj *)x);

		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	}
	llll_free(selectllll);
}

void score_sel_change_cents(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_lexpr *lexpr = NULL;
	t_llll *new_cents = NULL;
	char changed = 0;
	char lambda = (s == _llllobj_sym_lambda);
	t_notation_item *curr_it;
	
	if (argc <= 0) 
		return;
	
	if (atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("="))
		lexpr = notation_obj_lexpr_new(argc - 1, argv + 1);
	else
		new_cents = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	
	lock_general_mutex((t_notation_obj *)x);
	curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
	changed = 0;
	while (curr_it) {
        switch (curr_it->type) {
            case k_NOTE:
                changed |= change_note_cents_from_lexpr_or_llll((t_notation_obj *)x, (t_note *) curr_it, lexpr, new_cents);
                break;
            case k_PITCH_BREAKPOINT:
                changed |= change_breakpoint_cents_from_lexpr_or_llll((t_notation_obj *)x, (t_bpt *) curr_it, lexpr, new_cents);
                break;
            case k_CHORD:
                changed |= change_chord_cents_from_lexpr_or_llll((t_notation_obj *)x, (t_chord *) curr_it, lexpr, new_cents);
                break;
            case k_MEASURE:
            {
                t_measure *meas = (t_measure *) curr_it;
                t_chord *ch = meas->firstchord;
                while (ch) {
                    t_note *nt;
                    t_llllelem *thiselem = (new_cents) ? new_cents->l_head : NULL;
                    for (nt=ch->firstnote; nt; nt = nt->next) {
                        if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt) && thiselem) {
                            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, curr_it, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
                            change_double((t_notation_obj *)x, &nt->midicents, lexpr, thiselem, 0, (t_notation_item *)nt);
                            note_set_auto_enharmonicity(nt);
                            if (thiselem && thiselem->l_next)
                                thiselem = thiselem->l_next;
                            changed = 1;
                        }
                    }
                    ch->need_recompute_parameters = true;
                    ch = ch->next;
                }
                recompute_all_for_measure((t_notation_obj *)x, meas, false);
            }
                break;
            default:
                break;
		}
		curr_it = lambda ? NULL : curr_it->next_selected;
	}

	set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
	perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
	
	close_slot_window((t_notation_obj *)x); // if we were in slot view...
	unlock_general_mutex((t_notation_obj *)x);

	if (new_cents)
		llll_free(new_cents);
	if (lexpr)
		lexpr_free(lexpr);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_CENTS_FOR_SELECTION);
}

void score_sel_change_pitch(t_score *x, t_symbol *s, long argc, t_atom *argv){
    t_lexpr *lexpr = NULL;
    t_llll *new_pitch = NULL;
    char changed = 0;
    char lambda = (s == _llllobj_sym_lambda);
    t_notation_item *curr_it;
    
    if (argc <= 0)
        return;
    
    if (atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("="))
        lexpr = notation_obj_lexpr_new(argc - 1, argv + 1);
    else
        new_pitch = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
    
    lock_general_mutex((t_notation_obj *)x);
    curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
    changed = 0;
    while (curr_it) {
        switch (curr_it->type) {
            case k_NOTE:
                changed |= change_note_pitch_from_lexpr_or_llll((t_notation_obj *)x, (t_note *) curr_it, lexpr, new_pitch);
                break;
//            case k_PITCH_BREAKPOINT:
//                changed |= change_breakpoint_cents_from_lexpr_or_llll((t_notation_obj *)x, (t_bpt *) curr_it, lexpr, new_pitch);
//                break;
            case k_CHORD:
                changed |= change_chord_pitch_from_lexpr_or_llll((t_notation_obj *)x, (t_chord *) curr_it, lexpr, new_pitch);
                break;
            case k_MEASURE:
            {
                t_measure *meas = (t_measure *) curr_it;
                t_chord *ch = meas->firstchord;
                while (ch) {
                    t_note *nt;
                    t_llllelem *thiselem = (new_pitch) ? new_pitch->l_head : NULL;
                    for (nt=ch->firstnote; nt; nt = nt->next) {
                        if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt) && thiselem) {
                            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, curr_it, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
                            t_pitch pitch = note_get_pitch((t_notation_obj *)x, nt);
                            change_pitch((t_notation_obj *)x, &pitch, lexpr, thiselem, (t_notation_item *)nt);
                            note_set_user_enharmonicity(nt, pitch);
                            if (thiselem && thiselem->l_next)
                                thiselem = thiselem->l_next;
                            changed = 1;
                        }
                    }
                    ch->need_recompute_parameters = true;
                    ch = ch->next;
                }
                recompute_all_for_measure((t_notation_obj *)x, meas, false);
            }
                break;
            default:
                break;
        }
        curr_it = lambda ? NULL : curr_it->next_selected;
    }
    
    set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
    perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
    
    close_slot_window((t_notation_obj *)x); // if we were in slot view...
    unlock_general_mutex((t_notation_obj *)x);
    
    if (new_pitch)
        llll_free(new_pitch);
    if (lexpr)
        lexpr_free(lexpr);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_PITCH_FOR_SELECTION);
}



void set_measure_measureinfo(t_score *x, t_measure *measure, t_llll *measureinfo)
{
    // are there specification for the ts/tempo?
    if (measureinfo) { // must be a llll
        t_llll *tsllll = NULL, *tempollll = NULL;
        long measurebarline = 0;
        if ((measureinfo->l_size >= 1) && (hatom_gettype(&measureinfo->l_head->l_hatom) == H_LLLL)) { // there are ts/tempo specifications
            tsllll = hatom_getllll(&measureinfo->l_head->l_hatom);
            if ((measureinfo->l_size >= 2) && (hatom_gettype(&measureinfo->l_head->l_next->l_hatom) == H_LLLL)) {
                tempollll = hatom_getllll(&measureinfo->l_head->l_next->l_hatom);
                if ((measureinfo->l_size >= 3) && (hatom_gettype(&measureinfo->l_head->l_next->l_next->l_hatom) == H_SYM)) { // measure barline
                    measurebarline = hatom_getlong(&measureinfo->l_head->l_next->l_next->l_hatom);
                }
            }
        }
        
        set_measure_ts_and_tempo_from_llll((t_notation_obj *) x, measure, tsllll, tempollll, measurebarline, measureinfo, false);
        x->r_ob.need_reassign_local_spacing = true;
        
    }
}


void score_sel_change_measureinfo(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_lexpr *lexpr = NULL;
    t_llll *new_measureinfo = NULL;
    char changed = 0;
    char lambda = (s == _llllobj_sym_lambda);
    t_notation_item *curr_it;
    
    if (argc <= 0)
        return;
    
    if (atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("="))
        lexpr = notation_obj_lexpr_new(argc - 1, argv + 1);
    else
        new_measureinfo = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
    
    lock_general_mutex((t_notation_obj *)x);
    curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
    changed = 0;
    while (curr_it) {
        if (curr_it->type == k_MEASURE) {
            t_measure *meas = (t_measure *) curr_it;

            if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)meas)) {
                create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                if (meas->next) // because of ties...
                    create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas->next, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                
                if (new_measureinfo)
                    set_measure_measureinfo(x, meas, new_measureinfo);
            }
//            recompute_all_for_measure((t_notation_obj *)x, meas, true);
//            if (meas->next)
//                recompute_all_for_measure((t_notation_obj *)x, meas->next, true);
        }
        curr_it = lambda ? NULL : curr_it->next_selected;
    }
    
    recompute_all(x);
    set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
    perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
    
    close_slot_window((t_notation_obj *)x); // if we were in slot view...
    unlock_general_mutex((t_notation_obj *)x);
    
    if (new_measureinfo)
        llll_free(new_measureinfo);
    if (lexpr)
        lexpr_free(lexpr);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_MEASUREINFO_FOR_SELECTION);
}


void score_sel_change_onset(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_lexpr *lexpr = NULL;
	t_llll *new_onset = NULL;
	t_notation_item *curr_it;
	char changed = 0;
	char lambda = (s == _llllobj_sym_lambda);

	if (argc <= 0) 
		return;

	if (atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("="))
		lexpr = notation_obj_lexpr_new(argc - 1, argv + 1);
	else
		new_onset = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	
	lock_general_mutex((t_notation_obj *)x);
	curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
	while (curr_it) {
        switch (curr_it->type) {
            case k_PITCH_BREAKPOINT:
                changed |= change_breakpoint_onset_from_lexpr_or_llll((t_notation_obj *)x, (t_bpt *) curr_it, lexpr, new_onset);
                break;
                
            case k_MARKER:
            {
                t_marker *marker = (t_marker *) curr_it;
                
                if (!(x->r_ob.header_undo_flags & k_HEADER_MARKERS)) {
                    create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
                    x->r_ob.header_undo_flags |= k_HEADER_MARKERS;
                }
                
                change_marker_onset_from_lexpr_or_llllelem((t_notation_obj *) x, marker, lexpr, new_onset ? new_onset->l_head : NULL);
                sync_marker_absolute_ms_onset(x, marker);
                changed = 1;
            }
                break;
                
            default:
                break;
        }

        curr_it = lambda ? NULL : curr_it->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);

	if (new_onset) 
		llll_free(new_onset);
	if (lexpr)
		lexpr_free(lexpr);

	if (changed) {
		lock_general_mutex((t_notation_obj *)x);
        sync_all_markers_absolute_ms_onset(x);
		check_markers_order((t_notation_obj *) x);
		unlock_general_mutex((t_notation_obj *)x);
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_ONSET_FOR_SELECTION);
	}
}



void score_sel_change_velocity(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_lexpr *lexpr = NULL;
	t_llll *new_velocity = NULL;
	char changed = 0;
	char lambda = (s == _llllobj_sym_lambda);
	t_notation_item *curr_it;
	
	if (argc <= 0) 
		return;
	
	if (atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("="))
		lexpr = notation_obj_lexpr_new(argc - 1, argv + 1);
	else
		new_velocity = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	
	changed = 0;
	lock_general_mutex((t_notation_obj *)x);
	curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
	while (curr_it) {
        switch (curr_it->type) {
            case k_NOTE:
                changed |= change_note_velocity_from_lexpr_or_llll((t_notation_obj *)x, (t_note *) curr_it, lexpr, new_velocity);
                break;
                
            case k_PITCH_BREAKPOINT:
                changed |= change_breakpoint_velocity_from_lexpr_or_llll((t_notation_obj *)x, (t_bpt *) curr_it, lexpr, new_velocity);
                break;
                
            case k_CHORD:
                changed |= change_chord_velocity_from_lexpr_or_llll((t_notation_obj *)x, (t_chord *) curr_it, lexpr, new_velocity);
                break;
                
            case k_MEASURE:
            {
                t_measure *meas = (t_measure *) curr_it;
                t_chord *ch = meas->firstchord;
                while (ch) {
                    t_note *nt;
                    t_llllelem *thiselem = (new_velocity) ? new_velocity->l_head : NULL;
                    for (nt=ch->firstnote; nt; nt = nt->next) {
                        if ((!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) && thiselem) {
                            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, curr_it, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
                            change_long((t_notation_obj *)x, &nt->velocity, lexpr, thiselem, 0, (t_notation_item *)nt);
                            clip_long(&nt->velocity, CONST_MIN_VELOCITY, CONST_MAX_VELOCITY);
                            if (thiselem && thiselem->l_next)
                                thiselem = thiselem->l_next;
                            changed = 1;
                        }
                    }
                    ch = ch->next;
                }
            }
                break;
                
            default:
                break;
        }
        
		curr_it = lambda ? NULL : curr_it->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);

	if (new_velocity) 
		llll_free(new_velocity);
	if (lexpr)
		lexpr_free(lexpr);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_VELOCITY_FOR_SELECTION);
}



void score_sel_change_tie(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_lexpr *lexpr = NULL;
    t_llll *new_tie = NULL;
    char changed = 0;
    char lambda = (s == _llllobj_sym_lambda);
    t_notation_item *curr_it;
    
    if (argc <= 0)
        return;
    
    if (atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("="))
        lexpr = notation_obj_lexpr_new(argc - 1, argv + 1);
    else
        new_tie = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
    
    changed = 0;
    lock_general_mutex((t_notation_obj *)x);
    curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
    while (curr_it) {
        if (curr_it->type == k_NOTE) {
            changed |= change_note_tie_from_lexpr_or_llll((t_notation_obj *)x, (t_note *) curr_it, lexpr, new_tie);
            check_ties_around_measure(((t_note *)curr_it)->parent->parent);
            recompute_all_for_measure_and_neighbor_measures((t_notation_obj *)x, ((t_note *)curr_it)->parent->parent, false);
        } else if (curr_it->type == k_CHORD) {
            changed |= change_chord_tie_from_lexpr_or_llll((t_notation_obj *)x, (t_chord *) curr_it, lexpr, new_tie);
            check_ties_around_measure(((t_chord *)curr_it)->parent);
            recompute_all_for_measure_and_neighbor_measures((t_notation_obj *)x, ((t_chord *)curr_it)->parent, false);
        } else if (curr_it->type == k_MEASURE) {
            t_measure *meas = (t_measure *) curr_it;
            t_chord *ch = meas->firstchord;
            while (ch) {
                t_note *nt;
                t_llllelem *thiselem = (new_tie ? new_tie->l_head : NULL);
                for (nt=ch->firstnote; nt; nt = nt->next) {
                    if ((!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) && thiselem) {
                        long curr_tie = tie_to_long((t_notation_obj *)x, nt);

                        // adding undo tick
                        if (meas->prev) // ties might affect previous and following measure!
                            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas->prev, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                        create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                        if (meas->next)
                            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas->next, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                        

                        change_long((t_notation_obj *)x, &curr_tie, lexpr, thiselem, 0, (t_notation_item *)nt);
                        curr_tie = CLAMP(curr_tie, 0, 3);
                        assign_tie_from_long((t_notation_obj *)x, nt, curr_tie);
                        if (thiselem && thiselem->l_next)
                            thiselem = thiselem->l_next;
                        changed = 1;
                    }
                }
                ch = ch->next;
            }
            check_ties_around_measure(meas);
            recompute_all_for_measure_and_neighbor_measures((t_notation_obj *)x, meas, false);
        }
        curr_it = lambda ? NULL : curr_it->next_selected;
    }
    unlock_general_mutex((t_notation_obj *)x);
    
    
    if (new_tie)
        llll_free(new_tie);
    if (lexpr)
        lexpr_free(lexpr);
    
    if (changed)
        set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_TIE_FOR_SELECTION);
}



void score_sel_erase_breakpoints(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_notation_item *curr_it;
	char lambda = (s == _llllobj_sym_lambda);
	char changed = 0;
	
	lock_general_mutex((t_notation_obj *)x);
	curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
	while (curr_it) {
		if (curr_it->type == k_NOTE) {
			t_note *nt = (t_note *) curr_it;
			if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
				create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nt->parent, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
				note_delete_breakpoints((t_notation_obj *) x, nt);
				changed = 1;
			}
		} else if (curr_it->type == k_CHORD) {
			t_chord *ch = (t_chord *) curr_it;
			t_note *nt; 
			for (nt=ch->firstnote; nt; nt = nt->next) {
				if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
					create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
					note_delete_breakpoints((t_notation_obj *) x, nt);
					changed = 1;
				}
			}
		} else if (curr_it->type == k_MEASURE) {
			t_measure *meas = (t_measure *) curr_it;
			t_chord *ch = meas->firstchord;
			while (ch) {
				t_note *nt; 
				for (nt=ch->firstnote; nt; nt = nt->next) {
					if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
						create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
						note_delete_breakpoints((t_notation_obj *) x, nt);
						changed = 1;
					}
				}
				ch = ch->next;
			}
		}
		curr_it = lambda ? NULL : curr_it->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_ERASE_BREAKPOINTS_FOR_SELECTION);
}


void score_sel_add_breakpoint(t_score *x, t_symbol *s, long argc, t_atom *argv){
	double rel_x_pos, y_pos, slope;
	long vel = 0; char auto_vel;
	t_notation_item *curr_it;
	char lambda = (s == _llllobj_sym_lambda);
	char changed = 0;
	
	if (argc < 2) 
		return;
	
	rel_x_pos = atom_getfloat(argv);
	y_pos = atom_getfloat(argv+1);
	slope = (argc >= 3) ? atom_getfloat(argv+2) : 0.;
	if (slope < -1.) 
		slope = -1.; 
	if (slope > 1.) 
		slope = 1.;
	if (argc >= 4) {
		auto_vel = false;
		vel = atom_getlong(argv+3);
	} else 
		auto_vel = true;
	
	lock_general_mutex((t_notation_obj *)x);
	curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
	while (curr_it) {
		if (curr_it->type == k_NOTE) {
			t_note *nt = (t_note *) curr_it;
			if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
				create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nt->parent, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
				add_breakpoint((t_notation_obj *) x, nt, rel_x_pos, y_pos, slope, 0, vel, auto_vel);
				changed = 1;
			}
		} else if (curr_it->type == k_CHORD) {
			t_chord *ch = (t_chord *) curr_it;
			t_note *nt; 
			for (nt=ch->firstnote; nt; nt = nt->next) {
				if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
					create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
					add_breakpoint((t_notation_obj *) x, nt, rel_x_pos, y_pos, slope, 0, vel, auto_vel);
					changed = 1;
				}
			}
		} else if (curr_it->type == k_MEASURE) {
			t_measure *meas = (t_measure *) curr_it;
			t_chord *ch = meas->firstchord;
			while (ch) {
				t_note *nt; 
				for (nt=ch->firstnote; nt; nt = nt->next) {
					if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
						create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
						add_breakpoint((t_notation_obj *) x, nt, rel_x_pos, y_pos, slope, 0, vel, auto_vel);
						changed = 1;
					}
				}
				ch = ch->next;
			}
		}
		curr_it = lambda ? NULL : curr_it->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_ADD_BREAKPOINTS_TO_SELECTION);
}

void score_sel_add_slot(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *slot_as_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE); // We clone it: we operate destructively
	char lambda = (s == _llllobj_sym_lambda);
	char changed = 0;
	
	if (slot_as_llll) {
		t_notation_item *curr_it;
		lock_general_mutex((t_notation_obj *)x);
		curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
		while (curr_it) {
			if (curr_it->type == k_NOTE) {
				t_note *nt = (t_note *) curr_it;
				if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
					t_llll *llllcopy;	
					create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, curr_it, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
					llllcopy = llll_clone(slot_as_llll);
					set_slots_values_to_note_from_llll((t_notation_obj *) x, nt, slot_as_llll);
					llll_free(llllcopy);
					changed = 1;
				}
			} else if (curr_it->type == k_CHORD) {
				t_chord *ch = (t_chord *) curr_it;
                if (!ch->firstnote) { // rest
#ifdef BACH_CHORDS_HAVE_SLOTS
                    if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)ch)) {
                        t_llll *llllcopy;
                        create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
                        llllcopy = llll_clone(slot_as_llll);
                        set_slots_values_to_notationitem_from_llll((t_notation_obj *) x, (t_notation_item *)ch, slot_as_llll);
                        llll_free(llllcopy);
                        changed = 1;
                    }
#endif
                } else {
                    t_note *nt;
                    for (nt=ch->firstnote; nt; nt = nt->next) {
                        if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
                            t_llll *llllcopy;
                            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nt->parent, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
                            llllcopy = llll_clone(slot_as_llll);
                            set_slots_values_to_note_from_llll((t_notation_obj *) x, nt, slot_as_llll);
                            llll_free(llllcopy);
                            changed = 1;
                        }
                    }
                }
			} else if (curr_it->type == k_MEASURE) {
				t_measure *meas = (t_measure *) curr_it;
				t_chord *ch = meas->firstchord;
				while (ch) {
					t_note *nt; 
					for (nt=ch->firstnote; nt; nt = nt->next) {
						if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
							t_llll *llllcopy;	
							create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
							llllcopy = llll_clone(slot_as_llll);
							set_slots_values_to_note_from_llll((t_notation_obj *) x, nt, slot_as_llll);
							llll_free(llllcopy);
							changed = 1;
						}
					}
					ch = ch->next;
				}
			}
			curr_it = lambda ? NULL : curr_it->next_selected;
		}
		unlock_general_mutex((t_notation_obj *)x);
	}
	
	llll_free(slot_as_llll);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_ADD_SLOTS_TO_SELECTION);
}

void score_sel_dumpselection(t_score *x, t_symbol *s, long argc, t_atom *argv){
    
    t_llll *router_ll = NULL;
    t_llll *args = llllobj_parse_llll((t_object *)x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE);
    llll_parseargs_and_attrs((t_object *)x, args, "l", gensym("router"), &router_ll);
    evaluate_selection(x, 0, true, router_ll);
    llll_free(args);
    llll_free(router_ll);
}

void score_sel_sendcommand(t_score *x, t_symbol *s, long argc, t_atom *argv){
	if (argc > 0) {
		long command_num = atom_getlong(argv) - 1;
		if ((command_num >= 0) && (command_num < CONST_MAX_COMMANDS))
			selection_send_command(x, 0, command_num, true);
	}
}

// arguments are: slot#, position, new value.
void score_sel_erase_slot(t_score *x, t_symbol *s, long argc, t_atom *argv){
	long slotnum;
	char lambda = (s == _llllobj_sym_lambda);

	if (argc < 1) 
		return;

	slotnum = atom_to_slotnum((t_notation_obj *)x, argv, true);
	if (slotnum < 0)
		return;

    notationobj_sel_erase_slot((t_notation_obj *)x, slotnum, lambda);
	
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_ERASE_SLOTS_FOR_SELECTION);
}


void score_sel_move_slot(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    char lambda = (s == _llllobj_sym_lambda);
    
    if (argc < 2) {
        object_error((t_object *)x, "Not enough arguments!");
        return;
    }
    
    // arguments are: slot#, position, new value.
    
    long from = (atom_gettype(argv) == A_SYM ? slotname_to_slotnum((t_notation_obj *) x, atom_getsym(argv)) : atom_getlong(argv)-1);
    long to = (atom_gettype(argv + 1) == A_SYM ? slotname_to_slotnum((t_notation_obj *) x, atom_getsym(argv + 1)) : atom_getlong(argv + 1)-1);
    if (from < 0 || to < 0 || from >= CONST_MAX_SLOTS || to >= CONST_MAX_SLOTS) {
        object_error((t_object *)x, "Wrong slot numbers!");
        return;
    }
    
    notationobj_sel_move_slot((t_notation_obj *)x, from, to, false, lambda);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_MOVE_SLOTS_FOR_SELECTION);
}


void score_sel_copy_slot(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    char lambda = (s == _llllobj_sym_lambda);
    
    if (argc < 2) {
        object_error((t_object *)x, "Not enough arguments!");
        return;
    }
    
    // arguments are: slot#, position, new value.
    
    long from = (atom_gettype(argv) == A_SYM ? slotname_to_slotnum((t_notation_obj *) x, atom_getsym(argv)) : atom_getlong(argv)-1);
    long to = (atom_gettype(argv + 1) == A_SYM ? slotname_to_slotnum((t_notation_obj *) x, atom_getsym(argv + 1)) : atom_getlong(argv + 1)-1);
    if (from < 0 || to < 0 || from >= CONST_MAX_SLOTS || to >= CONST_MAX_SLOTS) {
        object_error((t_object *)x, "Wrong slot numbers!");
        return;
    }
    
    notationobj_sel_move_slot((t_notation_obj *)x, from, to, true, lambda);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_COPY_SLOTS_FOR_SELECTION);
}






// arguments are: slot#, position, new value (as llll).
void score_sel_change_slot_value(t_score *x, t_symbol *s, long argc, t_atom *argv){
	long slotnum, position;
	t_llll *new_values_as_llll;
	char lambda = (s == _llllobj_sym_lambda);
	char changed = 0;

	if (argc < 3) 
		return;

	slotnum = atom_to_slotnum((t_notation_obj *)x, argv, true);
	if (slotnum < 0)
		return;
	
	if (atom_gettype(argv+1) == A_SYM && atom_getsym(argv+1) == _llllobj_sym_all)
        position = -1;
    else
        position = atom_getlong(argv+1);
    
	new_values_as_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc-2, argv+2, LLLL_PARSE_CLONE);

	if (new_values_as_llll) {
		t_notation_item *curr_it;
		lock_general_mutex((t_notation_obj *)x);
		curr_it = lambda ? (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, x->r_ob.lambda_selected_item_ID) : x->r_ob.firstselecteditem;
		while (curr_it) {
			if (curr_it->type == k_NOTE) {
				t_note *nt = (t_note *) curr_it;
				if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
					create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nt->parent, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
					change_note_slot_value((t_notation_obj *) x, nt, slotnum, position, new_values_as_llll);
					changed = 1;
				}
			} else if (curr_it->type == k_CHORD) {
				t_chord *ch = (t_chord *) curr_it;
                if (!ch->firstnote) {
#ifdef BACH_CHORDS_HAVE_SLOTS
                    create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
                    change_notation_item_slot_value((t_notation_obj *) x, (t_notation_item *)ch, slotnum, position, new_values_as_llll);
                    changed = 1;
#endif
                } else {
                    t_note *nt;
                    for (nt=ch->firstnote; nt; nt = nt->next) {
                        if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
                            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
                            change_note_slot_value((t_notation_obj *) x, nt, slotnum, position, new_values_as_llll);
                            changed = 1;
                        }
                    }
                }
			} else if (curr_it->type == k_MEASURE) {
				t_measure *meas = (t_measure *) curr_it;
				t_chord *ch = meas->firstchord;
				while (ch) {
					t_note *nt; 
					for (nt=ch->firstnote; nt; nt = nt->next) {
						if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
							create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
							change_note_slot_value((t_notation_obj *) x, nt, slotnum, position, new_values_as_llll);
							changed = 1;
						}
					}
					ch = ch->next;
				}
			}
			curr_it = lambda ? NULL : curr_it->next_selected;
		}
		unlock_general_mutex((t_notation_obj *)x);
	}
	
	llll_free(new_values_as_llll);
	
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_SLOTS_FOR_SELECTION);
}



void score_addmarker(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *params = llllobj_parse_llll((t_object *)x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	if (params->l_size >= 2) { // position, name
		double pos_ms = 0;
		e_marker_roles markerrole = k_MARKER_ROLE_NONE;
		t_timepoint tp = build_timepoint_with_voice(0, long2rat(0), 0);
		char attach_to = k_MARKER_ATTACH_TO_MS;
		t_llll *content = NULL;
		t_marker *newmarker;

		if (hatom_gettype(&params->l_head->l_hatom) == H_SYM && hatom_getsym(&params->l_head->l_hatom) == _llllobj_sym_cursor) 
			pos_ms = (!x->r_ob.playing ? x->r_ob.play_head_start_ms : x->r_ob.play_head_ms);
		else if (hatom_gettype(&params->l_head->l_hatom) == H_LLLL) {
			tp = llll_to_timepoint((t_notation_obj *) x, hatom_getllll(&params->l_head->l_hatom), NULL, true);
			attach_to = k_MARKER_ATTACH_TO_MEASURE;
		} else if (is_hatom_number(&params->l_head->l_hatom)){
			pos_ms = hatom_getdouble(&params->l_head->l_hatom);
		}

		create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
		
		if (attach_to == k_MARKER_ATTACH_TO_MEASURE)
			pos_ms = unscaled_xposition_to_ms((t_notation_obj *)x, timepoint_to_unscaled_xposition((t_notation_obj *)x, tp, false, CONST_MARKERS_ON_FIRST_MEASURE_CHORDS), 1);
		
		if (params->l_size >= 3 && hatom_gettype(&params->l_head->l_next->l_next->l_hatom) == H_SYM)
			markerrole = sym_to_marker_role(hatom_getsym(&params->l_head->l_next->l_next->l_hatom));

		t_llll *names = get_names_from_llllelem((t_notation_obj *)x, params->l_head->l_next);

		if (params->l_size >= 4 && hatom_gettype(&params->l_head->l_next->l_next->l_next->l_hatom) == H_LLLL) {
			content = llll_clone(hatom_getllll(&params->l_head->l_next->l_next->l_next->l_hatom));
		}
		
		lock_markers_mutex((t_notation_obj *)x);;
		newmarker = add_marker((t_notation_obj *) x, names, pos_ms, tp, attach_to, markerrole, content, 0);
		sync_marker_absolute_ms_onset(x, newmarker);
		unlock_markers_mutex((t_notation_obj *)x);;

		recompute_total_length((t_notation_obj *)x);
		update_hscrollbar((t_notation_obj *)x, 0);
		
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_ADD_MARKER);
		llll_free(names);
	
	}
	llll_free(params);
}


void score_deletemarker(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *args = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_RETAIN);
	if (argc >= 1 && atom_gettype(argv) == A_SYM) { // position, name
		char res;
		create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
		lock_markers_mutex((t_notation_obj *)x);;
		res = delete_marker_by_name((t_notation_obj *) x, args);
		unlock_markers_mutex((t_notation_obj *)x);;
		if (res) 
			handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_DELETE_MARKER);
		else
			remove_all_free_undo_ticks((t_notation_obj *)x, true);
	}
	llll_free(args);
}


void score_markername(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *args = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	if (argc >= 1 && atom_gettype(argv) == A_SYM) { // position, name
		char incremental = find_long_arg_attr_key(args, gensym("incremental"), 0, true);
		create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
		if (change_selected_markers_name((t_notation_obj *) x, args, incremental))
			handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_MARKER_NAME);
		else
			remove_all_free_undo_ticks((t_notation_obj *)x, true);
	}
	llll_free(args);
}


void score_getmarker(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *args = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
    char namefirst = find_long_arg_attr_key(args, gensym("namefirst"), 0, true);
	if (args->l_size - get_num_llll_in_llll_first_level(args) >= 1) { // position, name
		t_marker *marker;
		t_llll *marker_llll = NULL;
		lock_markers_mutex((t_notation_obj *)x);
		marker = markername2marker((t_notation_obj *) x, args);
		if (marker)
			marker_llll = get_single_marker_as_llll((t_notation_obj *) x, marker, namefirst);
		unlock_markers_mutex((t_notation_obj *)x);
		if (marker_llll) {
			llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, marker_llll);
			llll_free(marker_llll);
		}
	} else {
		send_marker((t_notation_obj *) x, NULL, namefirst, 7);
	}
	llll_free(args);
}


/// *** VOICE COLLAPSING *** ///
void score_collapse(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	if (proxy_getinlet((t_object *) x) == 0) {
		t_llll *inputlist = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE); // We clone it: we operate destructively
		if (inputlist) {
			t_llll *which_voices = llll_get();
			t_llll *what_to_dump = NULL;
			t_llll *out_llll;
			long ref_voice = 0;
			
			if (inputlist->l_head && hatom_gettype(&inputlist->l_head->l_hatom) == H_LLLL) {
				llll_clone_upon(hatom_getllll(&inputlist->l_head->l_hatom), which_voices);
				if (inputlist->l_head->l_next && hatom_gettype(&inputlist->l_head->l_next->l_hatom) == H_LLLL)
					what_to_dump = hatom_getllll(&inputlist->l_head->l_next->l_hatom);
			}
			
			ref_voice = which_voices->l_head ? hatom_getlong(&which_voices->l_head->l_hatom) - 1 : 0;
			
			out_llll = get_collapsed_score_as_llll(x, which_voices, ref_voice, what_to_dump);
			llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 0, out_llll);
			llll_free(out_llll);
			llll_free(which_voices);
			llll_free(inputlist);
		}
	}
}

t_llll* get_collapsed_score_as_llll(t_score *x, t_llll *whichvoices, long reference_voice, t_llll *what_to_dump) {
	
	t_llll *active_chords, *active_until;
	t_rational curr_nonref_onset_sec_next;
	t_chord *curr_nonref_chord;
	t_llllelem *active_chords_elem, *active_until_elem, *temp1, *temp2;
	long k = 0; long i;

	t_scorevoice *refvoice = nth_scorevoice(x, reference_voice);

	t_llll *out_llll = llll_get();
	t_llll *out_voice = llll_get();
//	t_llll *midichannels = llll_get();
//	t_llll *clefs = llll_get();
	
	t_chord *chord;
	t_measure *measure;
	t_scorevoice *voice;

	char we_take_it[CONST_MAX_VOICES];
	t_chord *cur_ch[CONST_MAX_VOICES];
	t_chord *these_ch[CONST_MAX_VOICES];
	char what_to_dump_is_empty = (what_to_dump && has_llll_symbols_in_first_level(what_to_dump)) ? false : true;

	// preamble
	llll_appendsym(out_llll, _llllobj_sym_score, 0, WHITENULL_llll); // "score" message
	
	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_slotinfo))
		llll_appendllll(out_llll, get_slotinfo_values_as_llll((t_notation_obj *) x, false, false, false), 0, WHITENULL_llll); // slotinfo

	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_commands))
		llll_appendllll(out_llll, get_commands_values_as_llll((t_notation_obj *) x, false), 0, WHITENULL_llll); // command

	lock_general_mutex((t_notation_obj *)x);

	voice = x->firstvoice;
	while (voice && (voice->v_ob.number < x->r_ob.num_voices)) { // append chord lllls
		// check if we have to take the voice
		long voice_num = CLAMP(voice->v_ob.number, 0, CONST_MAX_VOICES - 1);
		we_take_it[voice_num] = 0;
		if (whichvoices->l_size == 0)
			we_take_it[voice_num] = 1;
		else {
			t_llllelem* elem;
			for (elem = whichvoices->l_head; elem; elem = elem->l_next) {
				if (hatom_gettype(&elem->l_hatom) == H_LONG) {
					if (hatom_getlong(&elem->l_hatom) == voice->v_ob.number + 1) {
						we_take_it[voice_num] = 1;
						break;
					}
				}
			}
		}
		
//		if (we_take_it[voice->v_ob.number]) {
//			llll_appendlong(midichannels, voice->v_ob.midichannel, 0, WHITENULL_llll);
//			llll_appendsym(clefs, x->r_ob.clefs_as_symlist[voice->v_ob.number], 0, WHITENULL_llll);
//		}
		voice = voice->next;
	}

    if (!is_long_in_llll_first_level(whichvoices, reference_voice + 1)) {
		if (whichvoices && whichvoices->l_size > 0) 
			refvoice = nth_scorevoice(x, hatom_getlong(&whichvoices->l_head->l_hatom) - 1);
		else
			refvoice = x->firstvoice;
    }

//	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_clefs))
//		llll_appendllll(out_llll, clefs, 0, WHITENULL_llll);
//	else
//		llll_free(clefs);
		
//	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_midichannels))
//		llll_appendllll(out_llll, midichannels, 0, WHITENULL_llll);
//	else
//		llll_free(midichannels);
	
    if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_articulationinfo))
        llll_appendllll(out_llll, get_articulationinfo_as_llll((t_notation_obj *)x));
    
    if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_noteheadinfo))
        llll_appendllll(out_llll, get_noteheadinfo_as_llll((t_notation_obj *)x));

    
	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_body)) {
		
		// initializing multicursor
		for (voice = x->firstvoice;	voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
			long voice_num = CLAMP(voice->v_ob.number, 0, CONST_MAX_VOICES - 1);
			cur_ch[voice_num] = (voice->firstmeasure) ? voice->firstmeasure->firstchord : NULL;
			while (cur_ch[voice_num] && cur_ch[voice_num]->r_sym_duration.r_num < 0) {
				cur_ch[voice_num] = get_next_chord(cur_ch[voice_num]);
			}
		}
		
		// setting flags
		for (voice = x->firstvoice;	voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next)
			for (measure = voice->firstmeasure; measure; measure = measure->next)
				for (chord = measure->firstchord; chord; chord = chord->next)
					chord->r_it.flags = (e_bach_internal_notation_flags) (chord->r_it.flags & ~k_FLAG_COLLAPSE);
		
		curr_nonref_onset_sec_next = long2rat(-1);
		active_chords = llll_get();
		active_until = llll_get();
		
		
		for (measure = refvoice->firstmeasure; measure; measure = measure->next) {
			t_llll *out_meas = llll_get();
			
			// measure info: ts and tempi
			llll_appendllll(out_meas, measure_get_measureinfo_as_llll((t_notation_obj *) x, measure), 0, WHITENULL_llll);	
			
			for (chord = measure->firstchord; chord; chord = chord->next) {
				t_rational ref_chord_r_onset_sec = chord_get_overall_rat_onset_sec(chord);
				t_rational ref_nextchord_r_onset_sec = chord_get_overall_rat_onset_sec_plus_duration(chord);
				t_rational cur;
				t_llll *out_ch = llll_get();
				t_rational curr_nonref_onset_sec_this;
				t_note *nt;
				t_timepoint ref_nextchord_timepoint, curr_nonref_onset_this_timepoint;
				t_rational sym_dur;
				
				// finding all chords syncronous with *chord 
				k = 0;
				for (i = 0; i < x->r_ob.num_voices; i++)
					these_ch[i] = NULL;
				for (i = 0; i < x->r_ob.num_voices; i++)
					if (we_take_it[i] && i != reference_voice) {
						while (cur_ch[i] && cur_ch[i]->r_sym_duration.r_num < 0)
							cur_ch[i] = get_next_chord(cur_ch[i]);
						if (cur_ch[i] && rat_rat_cmp(chord_get_overall_rat_onset_sec(cur_ch[i]), ref_chord_r_onset_sec) == 0) {
							if (k < CONST_MAX_VOICES)
								these_ch[k++] = cur_ch[i];
							// increasing chord
							cur_ch[i] = get_next_chord(cur_ch[i]);
							while (cur_ch[i] && cur_ch[i]->r_sym_duration.r_num < 0)
								cur_ch[i] = get_next_chord(cur_ch[i]);
						}
					}
				if (k < CONST_MAX_VOICES)
					these_ch[k++] = chord;
				
				// finding curr nonref chord (which starts AFTER *chord)
				cur = long2rat(-1);
				curr_nonref_onset_sec_next = long2rat(-1);
				for (i = 0; i < x->r_ob.num_voices; i++)
					if (we_take_it[i] && i != reference_voice)
						if (cur_ch[i] && (cur.r_num < 0 || rat_rat_cmp(chord_get_overall_rat_onset_sec(cur_ch[i]), cur) < 0)) {
							curr_nonref_chord = cur_ch[i];
							cur = chord_get_overall_rat_onset_sec(cur_ch[i]);
						}
				for (i = 0; i < k - 1; i++) {
					t_rational end = chord_get_overall_rat_onset_sec_plus_duration(these_ch[i]);
					if (cur.r_num < 0  || rat_rat_cmp(end, cur) < 0) {
						curr_nonref_chord = NULL;
						cur = end;
					}
				}
				for (active_until_elem = active_until->l_head; active_until_elem; active_until_elem = active_until_elem->l_next) {
					t_rational until = hatom_getrational(&active_until_elem->l_hatom);
					if (cur.r_num < 0  || rat_rat_cmp(until, cur) < 0) {
						curr_nonref_chord = NULL;
						cur = until;
					}
				} 
				curr_nonref_onset_sec_next = cur;
				
				curr_nonref_onset_sec_this = ref_chord_r_onset_sec;
				while (curr_nonref_onset_sec_next.r_num > 0 && rat_rat_cmp(curr_nonref_onset_sec_next, ref_nextchord_r_onset_sec) < 0) { 
					t_timepoint curr_nonref_onset_next_timepoint = rat_sec_to_timepoint((t_notation_obj *)x, curr_nonref_onset_sec_next, reference_voice);
					t_timepoint curr_nonref_onset_this_timepoint = rat_sec_to_timepoint((t_notation_obj *)x, curr_nonref_onset_sec_this, reference_voice);
					t_rational sym_dur = get_sym_durations_between_timepoints(refvoice, curr_nonref_onset_this_timepoint, curr_nonref_onset_next_timepoint);
					t_llll *out_ch_cloned;
					// setting chord values
					llll_appendrat(out_ch, sym_dur, 0, WHITENULL_llll); // rational_duration
					
					active_chords_elem = active_chords->l_head;
					active_until_elem = active_until->l_head; 
					while (active_chords_elem && active_until_elem) { // chords which remain to be completed
						t_chord *ch_to_complete = (t_chord *) hatom_getobj(&active_chords_elem->l_hatom);
						t_rational until = hatom_getrational(&active_until_elem->l_hatom);
						char last_one = rat_rat_cmp(until, curr_nonref_onset_sec_next) <= 0 ? 1 : 0;
						
						for (nt = ch_to_complete->firstnote; nt; nt = nt->next)
							llll_appendllll(out_ch, get_scorenote_values_as_llll((t_notation_obj *) x, nt, last_one ? k_CONSIDER_FOR_COLLAPSING_AS_NOTE_END : k_CONSIDER_FOR_COLLAPSING_AS_NOTE_MIDDLE), 0, WHITENULL_llll);	
						
						if (last_one) {
							temp1 = active_chords_elem->l_next;
							temp2 = active_until_elem->l_next;
							llll_destroyelem(active_chords_elem);
							llll_destroyelem(active_until_elem);
							active_chords_elem = temp1;
							active_until_elem = temp2;
						} else {
							active_chords_elem = active_chords_elem->l_next;
							active_until_elem = active_until_elem->l_next;
						}
					}
					
					for (i = 0; i < k; i++) { // syncronous chords
						t_rational ch_end = chord_get_overall_rat_onset_sec_plus_duration(these_ch[i]);
						char ended = rat_rat_cmp(ch_end, curr_nonref_onset_sec_next) <= 0 ? 1 : 0;
						
						for (nt = these_ch[i]->firstnote; nt; nt = nt->next){
							these_ch[i]->r_it.flags = (e_bach_internal_notation_flags) (these_ch[i]->r_it.flags | k_FLAG_COLLAPSE);
							llll_appendllll(out_ch, get_scorenote_values_as_llll((t_notation_obj *) x, nt, ended ? k_CONSIDER_FOR_DUMPING : k_CONSIDER_FOR_COLLAPSING_AS_NOTE_BEGINNING), 0, WHITENULL_llll);	
						}
						
						if (!ended) {
							llll_appendobj(active_chords, these_ch[i], 0, WHITENULL_llll);
							llll_appendrat(active_until, ch_end, 0, WHITENULL_llll);
						}
					}
					
					llll_appendlong(out_ch, 0, 0, WHITENULL_llll); // locked / muted
					out_ch_cloned = llll_clone(out_ch);
					llll_appendllll(out_meas, out_ch_cloned, 0, WHITENULL_llll);
					llll_free(out_ch);
					out_ch = llll_get();
					
					// preparing things for next cycle:
					// finding all syncronous chords for next cycle
					k = 0;
					for (i = 0; i < x->r_ob.num_voices; i++)
						these_ch[i] = NULL;
					for (i = 0; i < x->r_ob.num_voices; i++)
						if (we_take_it[i] && i != reference_voice) {
							while (cur_ch[i] && cur_ch[i]->r_sym_duration.r_num < 0)
								cur_ch[i] = get_next_chord(cur_ch[i]);
							if (cur_ch[i] && rat_rat_cmp(chord_get_overall_rat_onset_sec(cur_ch[i]), curr_nonref_onset_sec_next) == 0) {
								if (k < CONST_MAX_VOICES)
									these_ch[k++] = cur_ch[i];
								// increasing chord
								cur_ch[i] = get_next_chord(cur_ch[i]);
								while (cur_ch[i] && cur_ch[i]->r_sym_duration.r_num < 0)
									cur_ch[i] = get_next_chord(cur_ch[i]);
							}
						}
					
					// finding next nonref chord (which starts AFTER *chord)
					curr_nonref_onset_sec_this = curr_nonref_onset_sec_next;
					cur = long2rat(-1);
					for (i = 0; i < x->r_ob.num_voices; i++)
						if (we_take_it[i] && i != reference_voice)
							if (cur_ch[i] && (cur.r_num < 0 || rat_rat_cmp(chord_get_overall_rat_onset_sec(cur_ch[i]), cur) < 0)) {
								curr_nonref_chord = cur_ch[i];
								cur = chord_get_overall_rat_onset_sec(cur_ch[i]);
							}
					for (i = 0; i < k; i++) {
						t_rational end = chord_get_overall_rat_onset_sec_plus_duration(these_ch[i]);
						if (cur.r_num < 0  || (rat_rat_cmp(end, cur) < 0 && rat_rat_cmp(end, curr_nonref_onset_sec_next) > 0)) {
							curr_nonref_chord = NULL;
							cur = end;
						}
					}
					for (active_until_elem = active_until->l_head; active_until_elem; active_until_elem = active_until_elem->l_next) {
						t_rational until = hatom_getrational(&active_until_elem->l_hatom);
						if (cur.r_num < 0  || (rat_rat_cmp(until, cur) < 0  && rat_rat_cmp(until, curr_nonref_onset_sec_next) > 0)) {
							curr_nonref_chord = NULL;
							cur = until;
						}
					} 
					curr_nonref_onset_sec_next = cur;
					
					if (curr_nonref_onset_sec_next.r_num < 0) 
						break;
					
				}
				
				// ** last subchord **
				ref_nextchord_timepoint = rat_sec_to_timepoint((t_notation_obj *)x, ref_nextchord_r_onset_sec, reference_voice);
				curr_nonref_onset_this_timepoint = rat_sec_to_timepoint((t_notation_obj *)x, curr_nonref_onset_sec_this, reference_voice);
				sym_dur = get_sym_durations_between_timepoints(refvoice, curr_nonref_onset_this_timepoint, ref_nextchord_timepoint);
				llll_appendrat(out_ch, sym_dur, 0, WHITENULL_llll); // rational_duration
				
				active_chords_elem = active_chords->l_head;
				active_until_elem = active_until->l_head; 
				while (active_chords_elem && active_until_elem) { // chords which remain to be completed
					t_chord *ch_to_complete = (t_chord *) hatom_getobj(&active_chords_elem->l_hatom);
					t_rational until = hatom_getrational(&active_until_elem->l_hatom);
					char last_one = rat_rat_cmp(until, ref_nextchord_r_onset_sec) <= 0 ? 1 : 0;
					
					for (nt = ch_to_complete->firstnote; nt; nt = nt->next){
						llll_appendllll(out_ch, get_scorenote_values_as_llll((t_notation_obj *) x, nt, last_one ? k_CONSIDER_FOR_COLLAPSING_AS_NOTE_END : k_CONSIDER_FOR_COLLAPSING_AS_NOTE_MIDDLE), 0, WHITENULL_llll);	
					}
					
					if (last_one) {
						temp1 = active_chords_elem->l_next;
						temp2 = active_until_elem->l_next;
						llll_destroyelem(active_chords_elem);
						llll_destroyelem(active_until_elem);
						active_chords_elem = temp1;
						active_until_elem = temp2;
					} else {
						active_chords_elem = active_chords_elem->l_next;
						active_until_elem = active_until_elem->l_next;
					}
				}
				
				for (i = 0; i < k; i++) { // syncronous chords
					t_rational ch_end = chord_get_overall_rat_onset_sec_plus_duration(these_ch[i]);
					char ended = rat_rat_cmp(ch_end, ref_nextchord_r_onset_sec) <= 0 ? 1 : 0;
					
					for (nt = these_ch[i]->firstnote; nt; nt = nt->next){
						these_ch[i]->r_it.flags = (e_bach_internal_notation_flags) (these_ch[i]->r_it.flags | k_FLAG_COLLAPSE);
						llll_appendllll(out_ch, get_scorenote_values_as_llll((t_notation_obj *) x, nt, ended ? k_CONSIDER_FOR_DUMPING : k_CONSIDER_FOR_COLLAPSING_AS_NOTE_BEGINNING), 0, WHITENULL_llll);	
					}
					
					if (!ended) {
						llll_appendobj(active_chords, these_ch[i], 0, WHITENULL_llll);
						llll_appendrat(active_until, ch_end, 0, WHITENULL_llll);
					}
				}
				
				llll_appendlong(out_ch, 0, 0, WHITENULL_llll); // locked / muted
				llll_appendllll(out_meas, out_ch, 0, WHITENULL_llll);
				
			}
			
            llll_append_notationitem_flag((t_notation_obj *)x, out_meas, (t_notation_item *)measure);
            
			llll_appendllll(out_voice, out_meas, 0, WHITENULL_llll);
		}
		llll_appendllll(out_llll, out_voice, 0, WHITENULL_llll);
		
		llll_free(active_chords);
		llll_free(active_until);
		
	}
	
	unlock_general_mutex((t_notation_obj *)x);

	return out_llll;
}


/// *** LEGATO *** ///
// legato on selection
void score_legato(t_score *x) {
	t_scorevoice *voice;
	create_whole_score_undo_tick(x); 
	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
		t_measure *meas;
		t_llll *last_nonrest_chord_llll = NULL;
		for (meas = voice->firstmeasure; meas; meas = meas->next){
			t_chord *chord;
			for (chord = meas->firstchord; chord; chord = chord->next){
				if (chord->r_sym_duration.r_num < 0) { // rest
					if (notation_item_is_globally_selected((t_notation_obj *) x, (t_notation_item *)chord) && last_nonrest_chord_llll) {
						t_chord *prevchord; t_note *prevnt, *nt;
						prevchord = get_prev_chord(chord);
						if (prevchord->r_sym_duration.r_num >= 0) {
							t_llll *localcopy;
							t_rational duration_to_set = rat_abs(chord->r_sym_duration);
							localcopy = llll_clone(last_nonrest_chord_llll);
							set_scorechord_values_from_llll((t_notation_obj *) x, chord, localcopy, false, false);
							llll_free(localcopy);
							
							for (nt = chord->firstnote; nt; nt = nt->next)	
								note_compute_approximation((t_notation_obj *) x, nt); 
							
							chord->r_sym_duration = duration_to_set;
							for (prevnt = prevchord->firstnote; prevnt; prevnt = prevnt->next)
								prevnt->tie_to = (t_note *) WHITENULL;
						}
					}
				} else {
					if (last_nonrest_chord_llll)
						llll_free(last_nonrest_chord_llll);
					last_nonrest_chord_llll = get_scorechord_values_as_llll((t_notation_obj *) x, chord, k_CONSIDER_FOR_DUMPING, false);
				}
			}
		}
		if (last_nonrest_chord_llll)
			llll_free(last_nonrest_chord_llll);
	}
	unlock_general_mutex((t_notation_obj *)x);
	recompute_all_and_redraw(x);
	perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
	handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_LEGATO_FOR_SELECTION);
}

/// *** GLISSANDO *** ///
// glissando on selection

void score_glissando_fn(t_notation_obj *r_ob, t_note *nt, void *slope)
{
    note_change_tail_for_glissando_till_next(r_ob, nt, *((double *)slope));
}


void score_glissando(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    double slope = 0;
    if (argc && argv && is_atom_number(argv))
        slope = atom_getfloat(argv);
    
    iterate_notewise_changes_on_selection((t_notation_obj *)x, (notation_obj_note_fn)score_glissando_fn, &slope, true, k_CHORD, false);
    handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_GLISSANDO_FOR_SELECTION);
}


/// *** INSCREEN AND PLAYING *** ///

void score_inscreen(t_score *x, t_symbol *s, long argc, t_atom *argv){
// force a given point to be within the screen (changing the scrollbar
// admitted syntax: inscreen <position_in_mms>
//				    inscreen (measure position_in_measure voice)
	t_llll *inscreen;
	char need_redraw;

	if (argc <= 0) 
		return;
	
	inscreen = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	need_redraw = scoreapi_inscreen(x, inscreen);
	llll_free(inscreen);
	if (need_redraw) {
		if (x->r_ob.notify_also_upon_messages)
			send_domain(x, 7, NULL);
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	}
}

void score_inscreenpos(t_score *x, t_symbol *s, long argc, t_atom *argv){
// force a given point to be at a given position of the screen (changing the scrollbar
// admitted syntax: inscreen <position_in_screen_0_to_1> <position_in_mms>
//				    inscreen <position_in_screen_0_to_1> (measure position_in_measure voice)
	t_llll *inscreen;
	char need_redraw;

	if (argc <= 0) return;
	inscreen = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	need_redraw = scoreapi_inscreenpos(x, inscreen);
	llll_free(inscreen);
	
	if (need_redraw) {
		if (x->r_ob.notify_also_upon_messages)
			send_domain(x, 7, NULL);
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	}
}


void score_split(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	if (argc && argv && is_atom_number(argv)) 
		split_selection(x, atom_getlong(argv), argc > 1 ? false : true);
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_SPLIT_SELECTION); 
}

void score_join(t_score *x)
{
	quick_merge_selection(x);
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_JOIN_SELECTION); 
}


void score_mergegrace(t_score *x)
{
    t_notation_item *curr_it;
    char changed = false;
    
    lock_general_mutex((t_notation_obj *)x);
    
    curr_it = x->r_ob.firstselecteditem;
    clear_preselection((t_notation_obj *) x);
    
    for (curr_it = x->r_ob.firstselecteditem; curr_it; curr_it = curr_it->next_selected) {
        t_chord *ch = notation_item_chord_get_parent((t_notation_obj *)x, curr_it);
        if (ch && !ch->is_grace_chord) {
            char this_changed = false;
            // deleting grace chords RIGHT befor ch.
            t_chord *temp;
            
            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);

            while (true) {
                temp = ch->prev;
                if (!temp || !temp->is_grace_chord)
                    break;
                changed = this_changed = true;
                merge_chords(x, ch, temp, 0, false, false);
            }
            
            if (this_changed)
                recompute_all_for_measure((t_notation_obj *)x, ch->parent, true);
        }
    }
    
    if (changed)
        perform_analysis_and_change(x, NULL, NULL, x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_IGNORE ? k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE : k_BEAMING_CALCULATION_DONT_DELETE_LEVELS + k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
    
    unlock_general_mutex((t_notation_obj *)x);
    
    if (changed) {
        invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_MERGE_GRACE);
    }
}


void score_deletegrace(t_score *x)
{
    t_notation_item *curr_it;
    char changed = false;
    
    lock_general_mutex((t_notation_obj *)x);
    
    curr_it = x->r_ob.firstselecteditem;
    clear_preselection((t_notation_obj *) x);
    
    for (curr_it = x->r_ob.firstselecteditem; curr_it; curr_it = curr_it->next_selected) {
        t_chord *ch = notation_item_chord_get_parent((t_notation_obj *)x, curr_it);
        if (ch && !ch->is_grace_chord) {
            char this_changed = false;
            // deleting grace chords RIGHT befor ch.
            t_chord *temp;

            create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);

            while (true) {
                temp = ch->prev;
                if (!temp || !temp->is_grace_chord)
                    break;
                changed = this_changed = true;
                chord_delete_from_measure((t_notation_obj *)x, temp, true);
            }
            
            if (this_changed)
                recompute_all_for_measure((t_notation_obj *)x, ch->parent, true);
        }
    }
    
    if (changed)
        perform_analysis_and_change(x, NULL, NULL, x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_IGNORE ? k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE : k_BEAMING_CALCULATION_DONT_DELETE_LEVELS + k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
    
    unlock_general_mutex((t_notation_obj *)x);
    
    if (changed) {
        invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_GRACE);
    }
}


void score_inscreenmeas(t_score *x, t_symbol *s, long argc, t_atom *argv){
	// force some measures to be exactly in the screen, by changing their local_spacing_width_multiplier
	t_llll *inscreen;
	char need_redraw;
	
	if (argc <= 0) return;
	inscreen = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	need_redraw = scoreapi_inscreenmeas(x, inscreen);
	llll_free(inscreen);
	if (need_redraw) {
		if (x->r_ob.notify_also_upon_messages)
			send_domain(x, 7, NULL);
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	}
}

void score_resetlocalwidthmultiplformeas(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *arguments;
	char changed = false;
	t_tuttipoint *start = x->firsttuttipoint, *end = x->lasttuttipoint, *temp;
	t_llll *garbage;
	
	if (!x->firstvoice)
		return;
	
	lock_general_mutex((t_notation_obj *)x);
	arguments = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	if (arguments && arguments->l_head) {
		if (hatom_gettype(&arguments->l_head->l_hatom) == H_LONG) {
			long start_meas_ID = hatom_getlong(&arguments->l_head->l_hatom);
			t_measure *meas = nth_measure_of_scorevoice(x->firstvoice, start_meas_ID - 1);
			if (meas)
				start = meas->tuttipoint_reference;
		}
		if (arguments->l_head->l_next && hatom_gettype(&arguments->l_head->l_next->l_hatom) == H_LONG){
			long end_meas_ID = hatom_getlong(&arguments->l_head->l_next->l_hatom);
			t_measure *meas = nth_measure_of_scorevoice(x->firstvoice, end_meas_ID - 1);
			if (meas)
				end = meas->tuttipoint_reference;
		}
	}
	
	for (temp = start; temp; temp = temp->next){
		t_measure *meas;
		long i;
		changed = true;
		for (i = 0; i < x->r_ob.num_voices; i++) // cycle on the voices
			for (meas = temp->measure[i]; (temp->next && meas != temp->next->measure[i]) || (!temp->next && meas); meas = meas->next) // cycle on the measures within the tuttipoin
				create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_UNDO_MODIFICATION_CHANGE);
		assign_local_spacing_width_multiplier((t_notation_obj *)x, temp, 1.);
		recompute_all_for_tuttipoint_region((t_notation_obj *) x, temp, false);
		if (temp == end) 
			break;
	}

	garbage = llll_get();
	set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
	perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
	unlock_general_mutex((t_notation_obj *)x);
	
	llll_free(garbage);
	llll_free(arguments);

	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_RESET_LOCAL_SPACING);
}

void score_setmeasureparameters(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *garbage;
	t_llll *arguments = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	
	if (arguments && arguments->l_size > 1) {
		long meas_num = 1, voice_num = -1; // -1 = all
		t_measure *meas = NULL; 
		
		if (hatom_gettype(&arguments->l_head->l_hatom) == H_LONG) 
			meas_num = hatom_getlong(&arguments->l_head->l_hatom);
		else if (hatom_gettype(&arguments->l_head->l_hatom) == H_LLLL) {
			t_llll *subllll = hatom_getllll(&arguments->l_head->l_hatom);
			if (subllll && subllll->l_head) {
                if (subllll->l_size == 1) {
                    if (hatom_gettype(&subllll->l_head->l_hatom) == H_LONG)
                        meas_num = hatom_getlong(&subllll->l_head->l_hatom);
                } else if (subllll->l_size > 1) {
                    if (hatom_gettype(&subllll->l_head->l_hatom) == H_LONG)
                        voice_num = hatom_getlong(&subllll->l_head->l_hatom) - 1;
                    if (subllll->l_head->l_next && hatom_gettype(&subllll->l_head->l_next->l_hatom) == H_LONG) {
                        meas_num = hatom_getlong(&subllll->l_head->l_next->l_hatom);
                    }
                }
            }
		}
		
		llll_behead(arguments);
		
		if (voice_num >= 0) {
			lock_general_mutex((t_notation_obj *)x);
			meas = nth_measure_of_scorevoice(nth_scorevoice(x, voice_num), meas_num-1);
			if (meas){
				create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
				set_measure_parameters((t_notation_obj *) x, meas, arguments);
				recompute_all_for_measure((t_notation_obj *) x, meas, false);
			}
 		} else {
			long i;
			create_whole_score_undo_tick(x);
			lock_general_mutex((t_notation_obj *)x);
			for (i = 0; i < x->r_ob.num_voices; i++) {
				meas = nth_measure_of_scorevoice(nth_scorevoice(x, i), meas_num-1);
				if (meas) {
					set_measure_parameters((t_notation_obj *) x, meas, arguments);
					recompute_all_for_measure((t_notation_obj *) x, meas, false);
				}
 			}
		}
		
		garbage = llll_get();
		set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
		x->r_ob.need_reassign_local_spacing = true;
		perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
		unlock_general_mutex((t_notation_obj *)x);
		
		llll_free(garbage);

		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_SET_MEASURE_PARAMETERS);
	}
	
	llll_free(arguments);
}

void score_clock(t_score *x, t_symbol *s) 
{
	void *old = x->r_ob.setclock->s_thing; 
	t_object *c = 0; 
	
	// the line below can be restated as: 
	// if s is the empty symbol 
	// or s->s_thing is zero 
	// or s->s_thing is non-zero and a setclock object  
	if (s && (s == gensym("") || ((c = s->s_thing) && zgetfn(c, gensym("unset"))))) 
	{ 
		if (old) 
			score_stop(x, NULL, 0, NULL); 
		x->r_ob.setclock = s; 
		/*		if (x->m_running) 
		 setclock_delay(c, x->r_clock, 0L); */
	} 
}

void score_playselection(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// we set all the SELECTED chords as NON-played, the other ones as PLAYED
	double start_ms = -1;
	t_notation_item *selitem;
	char offline = (argc >= 1 && atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("offline"));
	t_atom av[2];
	
	// find selected chords and ms_boundaries
	lock_general_mutex((t_notation_obj *)x);
	for (selitem = x->r_ob.firstselecteditem; selitem; selitem = selitem->next_selected) {
		if (selitem->type == k_CHORD) {
			t_chord *chord = (t_chord *) selitem;
			double this_onset = chord_get_onset_ms(chord);
			if (start_ms < 0 || this_onset < start_ms)
				start_ms = this_onset;
		} else if (selitem->type == k_NOTE) {
			t_chord *chord = ((t_note *) selitem)->parent;
			double this_onset = chord_get_onset_ms(chord);
			if (start_ms < 0 || this_onset < start_ms)
				start_ms = this_onset;
		} else if (selitem->type == k_MEASURE) {
			t_measure *measure = (t_measure *) selitem;
			double this_onset = measure->tuttipoint_reference->onset_ms + measure->tuttipoint_onset_ms;
			if (start_ms < 0 || this_onset < start_ms)
				start_ms = this_onset;
		} else if (selitem->type == k_VOICE) {
			start_ms = 0;
			break;
		} 
	}
	unlock_general_mutex((t_notation_obj *)x);
	
	start_ms -= CONST_EPSILON2; // we remove an "epsilon" from the start_ms
	if (start_ms < 0.) 
		start_ms = 0.;
	
	x->r_ob.only_play_selection = true;
	
	if (offline) {
		atom_setsym(av, gensym("offline"));
		atom_setfloat(av + 1, start_ms);
	} else 
		atom_setfloat(av, start_ms);
	score_play(x, NULL, offline ? 2 : 1, av);
}

void score_pause(t_score *x)
{
	x->r_ob.show_playhead = true;
	x->r_ob.play_head_start_ms = x->r_ob.play_head_ms;
	x->r_ob.play_head_start_ux = x->r_ob.play_head_ux;
	score_stop(x, _llllobj_sym_pause, 0, NULL);
}

void score_play(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	char offline = (argc >= 1 && atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("offline"));
	if (offline) {
		if (bach_atomic_trylock(&x->r_ob.c_atomic_lock)) {
			object_warn((t_object *) x, "Already playing offline!");
			return;
		}
		score_play_offline(x, s, argc - 1, argv + 1);
		bach_atomic_unlock(&x->r_ob.c_atomic_lock);
		return;
	}

	if (x->r_ob.playing_offline) {
		object_warn((t_object *)x, "Can't play: already playing offline");
	} else {
		schedule_delay(x, (method) score_do_play, 0, s, argc, argv);
	}
}

void score_play_offline(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	if (x->r_ob.playing) {
		object_warn((t_object *)x, "Can't play offline: already playing");
	} else {
		x->r_ob.playing_offline = true;
		score_do_play(x, s, argc, argv);
		while (x->r_ob.playing_offline) {
			x->r_ob.play_step_count = x->r_ob.play_num_steps;
			score_task(x);
		}
	}
}

void score_do_play(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// let's find the first chord to play
	long i = 0;
	t_notation_item *firstplayeditem = NULL; 
	double start_ms = 0, end_ms = 0, firstplayeditem_onset = 0;
	t_measure *temp_meas = NULL;
	t_chord *temp_ch = NULL;
	t_scorevoice *voice;
	
	x->r_ob.are_there_solos = are_there_solos((t_notation_obj *) x);
	
	// setting automatic end
	end_ms = x->r_ob.length_ms_till_last_note;
	x->r_ob.play_head_fixed_end_ms = -1;

	// detecting start_ms
	if (argc > 0) {
		char err;
		t_llll *start_timepoint_syntax = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
		if (start_timepoint_syntax->l_size == 2) {
			char err2;
			t_llll *end_timepoint_syntax = llll_clone(start_timepoint_syntax);
			llll_behead(end_timepoint_syntax);
			llll_betail(start_timepoint_syntax);
			err2 = parse_open_timepoint_syntax((t_notation_obj *)x, end_timepoint_syntax, NULL, &end_ms, NULL, false);
			llll_free(end_timepoint_syntax);
			if (err2) {
				object_error((t_object *) x, "Error in defining play end position");
				return;
			}
			x->r_ob.play_head_fixed_end_ms = end_ms;
		}
		err = parse_open_timepoint_syntax((t_notation_obj *)x, start_timepoint_syntax, NULL, &start_ms, NULL, false);
//		if (start_timepoint_syntax->l_depth > 1)
//			start_ms -= CONST_EPSILON2;
		llll_free(start_timepoint_syntax);
		if (err) {
			object_error((t_object *) x, "Error in defining play start position");
			return;
		}
		x->r_ob.play_head_start_ms = start_ms;
		x->r_ob.play_head_start_ux = ms_to_unscaled_xposition((t_notation_obj *)x, start_ms, 1);
		
	} else if (argc == 0 && x->r_ob.show_playhead) {
		start_ms = x->r_ob.play_head_start_ms;
	} else {
		x->r_ob.play_head_start_ms = start_ms;
		if (start_ms == 0) 
			start_ms = get_first_onset_ms_for_grace_notes(x);
		x->r_ob.play_head_start_ux = ms_to_unscaled_xposition((t_notation_obj *)x, start_ms, 1);
	}
	update_playhead_cant_trespass_loop_end((t_notation_obj *)x);
	

	// first we send the playhead starting position
	send_playhead_position((t_notation_obj *) x, 7);

	// This line is no more needed, since we do it constantly at the stop method:
	// set_everything_unplayed(x)


	// then we send partial notes, if needed
	// i.e. the chords whose onset is < start_ms but whose duration continue at start_ms
	if (x->r_ob.play_partial_notes) {
		t_llll *to_send = llll_get();
		t_llll *to_send_references = llll_get();
		char is_notewise = true;

		lock_general_mutex((t_notation_obj *)x);
		
		for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
			for (temp_meas = voice->firstmeasure; temp_meas; temp_meas = temp_meas->next) {
				double measure_start_ms = temp_meas->tuttipoint_reference->onset_ms + temp_meas->tuttipoint_onset_ms;
				double measure_end_ms = measure_start_ms + temp_meas->total_duration_ms;

				if (measure_start_ms > start_ms)
					break;
				else if (measure_end_ms < start_ms)
					continue;
				
				for (temp_ch = temp_meas->firstchord; temp_ch; temp_ch = temp_ch->next) {
					if (temp_ch->onset > start_ms || (temp_ch->onset == start_ms && !is_all_chord_tied_from(temp_ch, false))) {
						break;
					} else if (should_element_be_played((t_notation_obj *) x, (t_notation_item *)temp_ch) &&
							   temp_ch->onset + temp_ch->duration_ms - CONST_EPSILON1 > start_ms) {
						t_llll *references, *this_llll;

						// we play the partial notes
						t_note *temp_nt;
						t_chord *next;
						
						temp_ch->played = true;
						for (temp_nt = temp_ch->firstnote; temp_nt; temp_nt = temp_nt->next) {
							temp_nt->played = true;
							llll_appendobj(x->r_ob.notes_being_played, temp_nt, 0, WHITENULL_llll);
						}
						
						// and we set as played all the rest sequences (if any)
						if (temp_ch->r_sym_duration.r_num < 0 && !x->r_ob.play_rests_separately) {
							t_chord *tmp = temp_ch, *end = get_last_rest_in_sequence(temp_ch, false);
							while (tmp) {
								tmp->played = true;
								tmp = get_next_chord(tmp);
								if (!tmp || tmp == end || tmp->r_sym_duration.r_num >= 0)
									break;
							}
						}
						
						// and we set as played all the tied notes
						if (!x->r_ob.play_tied_elements_separately) {
							t_chord *tmp = temp_ch;
							while (tmp && (next = get_next_chord(tmp)) && is_all_chord_tied_to((t_notation_obj *) x, tmp, 0, NULL)) {
								next->played = true;
								tmp = next;
							}
						}
						for (temp_nt = temp_ch->firstnote; temp_nt; temp_nt = temp_nt->next) {
							t_note *tmp_tied_note = temp_nt;
							while (tmp_tied_note && tmp_tied_note->tie_to && tmp_tied_note->tie_to != WHITENULL) {
								tmp_tied_note = tmp_tied_note->tie_to;
								tmp_tied_note->played = true;
//								llll_appendobj(x->r_ob.notes_being_played, tmp_tied_note, 0, WHITENULL_llll); // NO NEED TO STORE TIED NOTES
							}
						}
						
						this_llll = chord_get_as_llll_for_sending((t_notation_obj *) x, temp_ch, x->r_ob.play_partial_notes > 1 ? k_CONSIDER_FOR_PLAYING_AS_PARTIAL_NOTE_VERBOSE : k_CONSIDER_FOR_PLAYING_AS_PARTIAL_NOTE, -1, NULL, &references, &is_notewise);
						llll_appendllll(to_send, this_llll, 0, WHITENULL_llll);
						llll_appendllll(to_send_references, references, 0, WHITENULL_llll);
					}
				}
			}
		}

		llll_flatten(to_send, 1, 0);
		llll_flatten(to_send_references, 0, 0);
		
		unlock_general_mutex((t_notation_obj *)x);
		
		send_sublists_through_playout_and_free((t_notation_obj *) x, 7, to_send, to_send_references, is_notewise);
	}
	
	// setting the chord_play_cursor to NULL for every voice (why for every voice and not just the used ones??? because if one changes the number
    // of voices during playback this could be an issue...)
    for (i = 0; i < CONST_MAX_VOICES; i++) {
		x->r_ob.chord_play_cursor[i] = NULL;
        x->r_ob.tempo_play_cursor[i] = NULL;
        x->r_ob.measure_play_cursor[i] = NULL;
    }
	x->r_ob.marker_play_cursor = NULL;

	lock_general_mutex((t_notation_obj *)x);

	x->r_ob.dont_schedule_loop_end = x->r_ob.dont_schedule_loop_start = false;

	firstplayeditem = get_next_item_to_play((t_notation_obj *)x, start_ms);
	if (firstplayeditem)
		firstplayeditem_onset = notation_item_get_onset_ms((t_notation_obj *)x, firstplayeditem);
	
	// if the first chord is beyond the end limit, we stop
	if (firstplayeditem && firstplayeditem_onset >= end_ms) 
		firstplayeditem = NULL; 

	if (firstplayeditem || start_ms <= end_ms) {
		
		x->r_ob.scheduled_item = firstplayeditem;
		x->r_ob.scheduled_ms = firstplayeditem ? firstplayeditem_onset : end_ms;

		// if we're scheduling loop start or end, we don't want it to be rescheduled for next event
		x->r_ob.dont_schedule_loop_start = (x->r_ob.scheduled_item && x->r_ob.scheduled_item->type == k_LOOP_START) ? true : false;
		x->r_ob.dont_schedule_loop_end = (x->r_ob.scheduled_item && x->r_ob.scheduled_item->type == k_LOOP_END) ? true : false;

		unlock_general_mutex((t_notation_obj *)x);

		x->r_ob.play_head_ms = start_ms;
		x->r_ob.play_head_ux = ms_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.play_head_ms, 1);

		if (x->r_ob.catch_playhead && force_inscreen_ux_rolling(x, x->r_ob.play_head_ux, 0, true, false))
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);

		x->r_ob.playing = true;
		llllobj_outlet_symbol_as_llll((t_object *)x, LLLL_OBJ_UI, 7, _llllobj_sym_play);
		
		if (x->r_ob.theoretical_play_step_ms <= 0) {
			// step at each scheduled event
			x->r_ob.play_num_steps = 1;
			x->r_ob.play_step_ms = (x->r_ob.scheduled_ms - start_ms);
		} else {
			// fluid scheduling for redraw
			x->r_ob.play_num_steps = MAX(1, round((x->r_ob.scheduled_ms - start_ms) / x->r_ob.theoretical_play_step_ms));
			x->r_ob.play_step_ms = (x->r_ob.scheduled_ms - start_ms)/x->r_ob.play_num_steps;
		}
		x->r_ob.play_step_count = 0;
		
		if (!x->r_ob.playing_offline) {
			setclock_getftime(x->r_ob.setclock->s_thing, &x->r_ob.start_play_time);
			setclock_fdelay(x->r_ob.setclock->s_thing, x->r_ob.m_clock, x->r_ob.play_step_ms);
			if (x->r_ob.highlight_played_notes)
				invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			else
                notationobj_redraw((t_notation_obj *) x);
		}

	} else 
		unlock_general_mutex((t_notation_obj *)x);
}

void score_stop(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	schedule_delay(x, (method) score_do_stop, 0, s, argc, argv);
}

void score_do_stop(t_score *x, t_symbol *s, long argc, t_atom *argv){
	lock_general_mutex((t_notation_obj *)x);
	x->r_ob.playing = false;
	set_everything_unplayed(x);
	x->r_ob.play_head_ms = -1;
	x->r_ob.dont_schedule_loop_end = x->r_ob.dont_schedule_loop_start = false;
	setclock_unset(x->r_ob.setclock->s_thing, x->r_ob.m_clock);
	x->r_ob.only_play_selection = false;
	x->r_ob.play_step_count = 0;
	unlock_general_mutex((t_notation_obj *)x);

    llllobj_outlet_symbol_as_llll((t_object *)x, LLLL_OBJ_UI, 7, s ? s : _llllobj_sym_stop);
	invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
}

void set_everything_unplayed(t_score *x){
	t_scorevoice *voice; t_measure *meas; t_chord *chord; t_note *note;
	for (voice = x->firstvoice; voice && (voice->v_ob.number < x->r_ob.num_voices); voice = voice->next)
		for (meas = voice->firstmeasure; meas; meas = meas->next) {
			for (chord = meas->firstchord; chord; chord = chord->next) {
				chord->played = false;
				for (note = chord->firstnote; note; note = note->next) 
					note->played = false;
			}
		}
	llll_clear(x->r_ob.notes_being_played);
}

void score_task(t_score *x)
{
// clock activation.
// Time is not spanned uniformously from beginning to end, cause we want to have great precision on each note!
// So, each interval (from note to note) is divided into x->r_ob.play_num_steps steps, when we get to the last one, we compute the next note values.

	x->r_ob.play_head_ms += x->r_ob.play_step_ms;
	x->r_ob.play_step_count++;

	if (x->r_ob.highlight_played_notes)
		check_unplayed_notes((t_notation_obj *) x, x->r_ob.play_head_ms);

	if (x->r_ob.play_step_count < x->r_ob.play_num_steps) {

		// we haven't reached the next event: we just redraw the playline
		
		lock_general_mutex((t_notation_obj *)x);
		x->r_ob.play_head_ux = ms_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.play_head_ms, 1);
		unlock_general_mutex((t_notation_obj *)x);
		
		setclock_fdelay(x->r_ob.setclock->s_thing, x->r_ob.m_clock, x->r_ob.play_step_ms);

		if (x->r_ob.theoretical_play_step_ms > 0) {
			if (x->r_ob.catch_playhead && force_inscreen_ux_rolling(x, x->r_ob.play_head_ux, 0, true, false))
				invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
            notationobj_redraw((t_notation_obj *) x);
		}
	
	} else {
		
		// we have reached the next scheduled event
		
		lock_general_mutex((t_notation_obj *)x);

		if (x->r_ob.scheduled_item) {
			
			char scheduled_item_type = x->r_ob.scheduled_item->type;
			t_scorevoice *voice;
			t_notation_item *nextitemtoplay = NULL; 
			t_chord *temp_ch;
			t_marker *marker;
            t_tempo *tempo;
            t_measure *measure;
			long count = 0, i;
			double last_scheduled_ms, nextitemtoplay_onset = 0;
			long max_chord_per_scheduler_tick = MAX(x->r_ob.max_num_chord_per_scheduler_event, 1);
			t_notation_item **items_to_send;

			t_llll *to_send = llll_get();
			t_llll *to_send_references = llll_get();
			char is_notewise = true;

			items_to_send = (t_notation_item **) bach_newptr(max_chord_per_scheduler_tick * sizeof(t_notation_item *));

			// finding synchronous items
			if (scheduled_item_type == k_CHORD || scheduled_item_type == k_MARKER || scheduled_item_type == k_MEASURE || scheduled_item_type == k_TEMPO) {
				items_to_send[0] = x->r_ob.scheduled_item;
                
				if (items_to_send[0]->type == k_CHORD) {
					long voice_num = CLAMP(((t_chord *)items_to_send[0])->parent->voiceparent->v_ob.number, 0, CONST_MAX_VOICES - 1);
					x->r_ob.chord_play_cursor[voice_num] = (t_chord *)items_to_send[0];
                    
                    if (x->r_ob.play_mode == k_PLAYMODE_CHORDWISE && !x->r_ob.play_tied_elements_separately && is_all_chord_tied_to((t_notation_obj *)x, (t_chord *)items_to_send[0], false, NULL))
                        x->r_ob.chord_play_cursor[voice_num] = get_last_tied_chord((t_chord *)items_to_send[0]);
                    
                } else if (items_to_send[0]->type == k_TEMPO) {
                    long voice_num = CLAMP(((t_tempo *)items_to_send[0])->owner->voiceparent->v_ob.number, 0, CONST_MAX_VOICES - 1);
                    x->r_ob.tempo_play_cursor[voice_num] = (t_tempo *)items_to_send[0];
                } else if (items_to_send[0]->type == k_MEASURE) {
                    long voice_num = CLAMP(((t_measure *)items_to_send[0])->voiceparent->v_ob.number, 0, CONST_MAX_VOICES - 1);
                    x->r_ob.measure_play_cursor[voice_num] = (t_measure *)items_to_send[0];
				} else if (items_to_send[0]->type == k_MARKER)
					x->r_ob.marker_play_cursor = (t_marker *)items_to_send[0];

				count = 1; 

				if (x->r_ob.play_markers) {
					for (marker = x->r_ob.marker_play_cursor ? x->r_ob.marker_play_cursor->next : x->r_ob.firstmarker;
						 marker && count < x->r_ob.max_num_chord_per_scheduler_event; marker = marker->next) {
						if (should_element_be_played((t_notation_obj *) x, (t_notation_item *)marker)) {
							if (marker->position_ms == x->r_ob.scheduled_ms) {
								if (count < max_chord_per_scheduler_tick)
									items_to_send[count++] = (t_notation_item *)marker;
								x->r_ob.marker_play_cursor = marker;
							} else if (marker->position_ms > x->r_ob.scheduled_ms) {
								break;
							}
						}
					}
				}
                
                if (x->r_ob.play_measures) {
                    for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices && count < x->r_ob.max_num_chord_per_scheduler_event; voice = voice->next){
                        for (measure = x->r_ob.measure_play_cursor[voice->v_ob.number] ? get_next_measure(x->r_ob.measure_play_cursor[voice->v_ob.number]) : voice->firstmeasure;
                             measure && count < x->r_ob.max_num_chord_per_scheduler_event; measure = get_next_measure(measure)){
                            
                            if (should_element_be_played((t_notation_obj *) x, (t_notation_item *)measure)){
                                double measure_onset = notation_item_get_onset_ms((t_notation_obj *)x, (t_notation_item *)measure);
                                if (measure_onset == x->r_ob.scheduled_ms) {
                                    if (count < max_chord_per_scheduler_tick)
                                        items_to_send[count++] = (t_notation_item *)measure;
                                    x->r_ob.measure_play_cursor[voice->v_ob.number] = measure;
                                } else if (measure_onset > x->r_ob.scheduled_ms) {
                                    break;
                                }
                            }
                        }
                    }
                }

                if (x->r_ob.play_tempi) {
                    for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices && count < x->r_ob.max_num_chord_per_scheduler_event; voice = voice->next){
                        for (tempo = x->r_ob.tempo_play_cursor[voice->v_ob.number] ? get_next_tempo(x->r_ob.tempo_play_cursor[voice->v_ob.number]) : (voice->firstmeasure ? voice->firstmeasure->firsttempo : NULL);
                             tempo && count < x->r_ob.max_num_chord_per_scheduler_event; tempo = get_next_tempo(tempo)){
                            
                            if (should_element_be_played((t_notation_obj *) x, (t_notation_item *)tempo)){
                                if (tempo->onset == x->r_ob.scheduled_ms) {
                                    if (count < max_chord_per_scheduler_tick)
                                        items_to_send[count++] = (t_notation_item *)tempo;
                                    x->r_ob.tempo_play_cursor[voice->v_ob.number] = tempo;
                                } else if (tempo->onset > x->r_ob.scheduled_ms) {
                                    break;
                                }
                            }
                        }
                    }
                }

				for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices && count < x->r_ob.max_num_chord_per_scheduler_event; voice = voice->next){
					for (temp_ch = x->r_ob.chord_play_cursor[voice->v_ob.number] ? get_next_chord(x->r_ob.chord_play_cursor[voice->v_ob.number]) : (voice->firstmeasure ? voice->firstmeasure->firstchord : NULL);
                         temp_ch && count < x->r_ob.max_num_chord_per_scheduler_event; temp_ch = get_next_chord(temp_ch)){
                        
                        if (should_element_be_played((t_notation_obj *) x, (t_notation_item *)temp_ch)){
                            if (temp_ch->onset == x->r_ob.scheduled_ms) {
                                if (count < max_chord_per_scheduler_tick)
                                    items_to_send[count++] = (t_notation_item *)temp_ch;
                                x->r_ob.chord_play_cursor[voice->v_ob.number] = temp_ch;
                            } else if (temp_ch->onset > x->r_ob.scheduled_ms) {
                                break;
                            }
                        }
                    }
                }
            }
			
			// we now have to find the next item to schedule
			if (!x->r_ob.playing_offline && scheduled_item_type == k_LOOP_END) {
				// looping: setting the chord_play_cursor, measure_play_cursor and tempo_play_cursor to NULL for every voice
                for (i = 0; i < x->r_ob.num_voices; i++) {
					x->r_ob.chord_play_cursor[i] = NULL;
                    x->r_ob.tempo_play_cursor[i] = NULL;
                    x->r_ob.measure_play_cursor[i] = NULL;
                }
				x->r_ob.marker_play_cursor = NULL;
				
				// we reset the start play time, and we set the starting playhead position to the loop start position 
				setclock_getftime(x->r_ob.setclock->s_thing, &x->r_ob.start_play_time);
				if (x->r_ob.play_head_start_ms != x->r_ob.loop_region.start.position_ms) {
					x->r_ob.play_head_start_ms = x->r_ob.loop_region.start.position_ms; 
					x->r_ob.play_head_start_ux = ms_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.loop_region.start.position_ms, 1);
				}
				
				last_scheduled_ms = x->r_ob.loop_region.start.position_ms;
				x->r_ob.play_head_ms = last_scheduled_ms;
				x->r_ob.dont_schedule_loop_start = false;
			} else {
				last_scheduled_ms = x->r_ob.scheduled_ms;
				if (scheduled_item_type == k_LOOP_START)
					x->r_ob.dont_schedule_loop_end = false;
			}
			
			nextitemtoplay = get_next_item_to_play((t_notation_obj *)x, last_scheduled_ms);
			if (nextitemtoplay)
				nextitemtoplay_onset = notation_item_get_onset_ms((t_notation_obj *)x, nextitemtoplay);
				
			#ifdef BACH_PLAY_DEBUG
				if (nextitemtoplay)
					post("Next item to play has onset: %.2f ms", nextitemtoplay_onset);
			#endif
			
			if (x->r_ob.play_head_fixed_end_ms > 0 && nextitemtoplay && nextitemtoplay_onset >= x->r_ob.play_head_fixed_end_ms)
				nextitemtoplay = NULL;			
			
			// loop start must be scheduled once: if we schedule loop start, then we schedule a chord falling ON the loop start, we don't want next item to be the loop start again
			// we check if we can resume scheduling the loop start or end
			if (x->r_ob.dont_schedule_loop_start && x->r_ob.loop_region.start.position_ms != nextitemtoplay_onset)
				x->r_ob.dont_schedule_loop_start = false;
			if (x->r_ob.dont_schedule_loop_start && x->r_ob.loop_region.start.position_ms != nextitemtoplay_onset)
				x->r_ob.dont_schedule_loop_end = false;

			// we schedule the next item
			x->r_ob.scheduled_ms = nextitemtoplay ? nextitemtoplay_onset : (x->r_ob.play_head_fixed_end_ms > 0 ? x->r_ob.play_head_fixed_end_ms : x->r_ob.length_ms);
			if (x->r_ob.theoretical_play_step_ms <= 0){
				// just one step per scheduled event
				x->r_ob.play_num_steps = 1;
				x->r_ob.play_step_ms = (x->r_ob.scheduled_ms - last_scheduled_ms);
			} else {
				// fluid steps for redraw
				x->r_ob.play_num_steps = MAX(1, round((x->r_ob.scheduled_ms - last_scheduled_ms) / x->r_ob.theoretical_play_step_ms));
				x->r_ob.play_step_ms = (x->r_ob.scheduled_ms - last_scheduled_ms)/x->r_ob.play_num_steps;
			}

			if (nextitemtoplay) { // we check if we should or should not prevent loops from being scheduled
				if (nextitemtoplay->type == k_LOOP_START)
					x->r_ob.dont_schedule_loop_start = true;
				else if (nextitemtoplay->type == k_LOOP_END)
					x->r_ob.dont_schedule_loop_end = true;
			}

			x->r_ob.play_step_count = 0;
			x->r_ob.scheduled_item = nextitemtoplay; // this has to be within the mutex!


			// gathering chord values as llll
			for (i = 0; i < count; i++) {
				t_llll *this_llll = NULL, *references = NULL;
				
				if (items_to_send[i]->type == k_CHORD) {
					
					// chord is played
					((t_chord *)items_to_send[i])->played = true;
					// and we set as played all the rest sequences (if any)
					if (((t_chord *)items_to_send[i])->r_sym_duration.r_num < 0 && !x->r_ob.play_rests_separately) {
						t_chord *tmp = ((t_chord *)items_to_send[i]), *end = get_last_rest_in_sequence(((t_chord *)items_to_send[i]), false);
						while (tmp) {
							tmp->played = true;
							if (tmp == end || tmp->r_sym_duration.r_num >= 0)
								break;
							tmp = get_next_chord(tmp);
						}
					}
					
					if (((t_chord *)items_to_send[i])->r_sym_duration.r_num >= 0 && !x->r_ob.play_tied_elements_separately) {
						t_chord *tmp = (t_chord *)items_to_send[i];
						t_note *temp_nt;
						t_chord *next;
						for (temp_nt = tmp->firstnote; temp_nt; temp_nt = temp_nt->next) {
							temp_nt->played = true;
							if (!temp_nt->tie_from) // we don't keep track of notes continuing ties in the "notes_being_played" llll
								llll_appendobj(x->r_ob.notes_being_played, temp_nt, 0, WHITENULL_llll);
						}
						while (tmp && (next = get_next_chord(tmp)) && is_all_chord_tied_to((t_notation_obj *) x, tmp, 0, NULL)) {
							if (next->num_notes == tmp->num_notes)
								next->played = true;
							tmp = next;
						}
                        if (x->r_ob.play_mode == k_PLAYMODE_NOTEWISE) {
                            for (temp_nt = tmp->firstnote; temp_nt; temp_nt = temp_nt->next) {
                                t_note *tmp_tied_note = temp_nt;
                                while (tmp_tied_note && tmp_tied_note->tie_to && tmp_tied_note->tie_to != WHITENULL) {
                                    tmp_tied_note = tmp_tied_note->tie_to;
                                    tmp_tied_note->played = true;
                                    //								llll_appendobj(x->r_ob.notes_being_played, tmp_tied_note, 0, WHITENULL_llll); // we don't keep track of notes continuing ties in the "notes_being_played" llll
                                }
                            }
                        }
					}
					this_llll = chord_get_as_llll_for_sending((t_notation_obj *) x, (t_chord *)items_to_send[i], k_CONSIDER_FOR_PLAYING, -1, NULL, &references, &is_notewise);
                    
                } else if (items_to_send[i]->type == k_MEASURE) {
                    t_llll *temp = measure_get_as_llll_for_sending((t_notation_obj *) x, (t_measure *)items_to_send[i], k_CONSIDER_FOR_PLAYING);
                    this_llll = llll_get();
                    references = llll_get();
                    llll_appendobj(this_llll, temp, 0, WHITENULL_llll);
                    llll_appendobj(references, (t_measure *)items_to_send[i], 0, WHITENULL_llll);
                } else if (items_to_send[i]->type == k_TEMPO) {
                    t_llll *temp = get_tempo_as_llll_for_sending((t_notation_obj *) x, (t_tempo *)items_to_send[i], k_CONSIDER_FOR_PLAYING);
                    this_llll = llll_get();
                    references = llll_get();
                    llll_appendobj(this_llll, temp, 0, WHITENULL_llll);
                    llll_appendobj(references, items_to_send[i], 0, WHITENULL_llll);
				} else if (items_to_send[i]->type == k_MARKER) {
					t_llll *temp = get_single_marker_as_llll((t_notation_obj *) x, (t_marker *)items_to_send[i], true);
					this_llll = llll_get();
					references = llll_get();
					llll_appendobj(this_llll, temp, 0, WHITENULL_llll);
					llll_appendobj(references, items_to_send[i], 0, WHITENULL_llll);
				}
				if (this_llll && references) {
					llll_appendllll(to_send, this_llll, 0, WHITENULL_llll);
					llll_appendllll(to_send_references, references, 0, WHITENULL_llll);
				}
			}
			
			llll_flatten(to_send, 1, 0);
			llll_flatten(to_send_references, 0, 0);
			
			unlock_general_mutex((t_notation_obj *)x);
			
			if (!x->r_ob.playing_offline) 
				setclock_fdelay(x->r_ob.setclock->s_thing, x->r_ob.m_clock, x->r_ob.play_step_ms);
			
			x->r_ob.play_head_ms = last_scheduled_ms;
			x->r_ob.play_head_ux = ms_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.play_head_ms, 1);
			
			if (!x->r_ob.playing_offline)
				if (x->r_ob.catch_playhead && force_inscreen_ux_rolling(x, x->r_ob.play_head_ux, 0, true, false))
					invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			
			// outputting chord values
			if (count > 0)
				send_sublists_through_playout_and_free((t_notation_obj *) x, 7, to_send, to_send_references, is_notewise);
			else if (scheduled_item_type == k_LOOP_START || scheduled_item_type == k_LOOP_END) {
				llllobj_outlet_symbol_couple_as_llll((t_object *)x, LLLL_OBJ_UI, 7, _llllobj_sym_loop, scheduled_item_type == k_LOOP_START ? _llllobj_sym_start : _llllobj_sym_end);
				llll_free(to_send);
				llll_free(to_send_references);
			}
			
			if (!x->r_ob.playing_offline) {
				if (x->r_ob.highlight_played_notes)
					invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
				else
                    notationobj_redraw((t_notation_obj *) x);
			}

			bach_freeptr(items_to_send);
			
		} else {

			// next event is the end of the score
			char need_repaint = (x->r_ob.playing_offline == 0); 
			t_llll *end_llll;
			set_everything_unplayed(x);
			x->r_ob.playing = x->r_ob.playing_offline = false;
			x->r_ob.play_head_ms = -1;
			x->r_ob.play_head_ux = -1;
			x->r_ob.scheduled_item = NULL;
			x->r_ob.only_play_selection = false;
			x->r_ob.play_step_count = 0;
			unlock_general_mutex((t_notation_obj *)x);
			
			// send "end" message 
			end_llll = llll_get();
			llll_appendsym(end_llll, _llllobj_sym_end, 0, WHITENULL_llll);
			llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, end_llll);
			llll_free(end_llll);
			
			if (need_repaint)
				invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
		}
	}
}



int T_EXPORT main(void){
	t_class *c;
	
	common_symbols_init();
	llllobj_common_symbols_init();
	
	bach_inspector_ui_classinit();

	if (llllobj_check_version(BACH_LLLL_VERSION) || llllobj_test()) {
		error("bach: bad installation");
		return 1;
	}
	
	CLASS_NEW_CHECK_SIZE(c, "bach.score", 
							(method)score_new,
							(method)score_free,
							sizeof(t_score),
							(method)NULL,
							A_GIMME,
							0L);
	
	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;

//	jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH | JBOX_FONTATTR);
	jbox_initclass(c, JBOX_TEXTFIELD);
	
	class_addmethod(c, (method) score_paint, "paint", A_CANT, 0);


	// @method subscore @digest Output a portion of <o>bach.score</o>
	// @description The <m>subscore</m> message outputs the gathered syntax information of a portion of the <o>bach.score</o>,
	// namely it outputs only certain voices and within a certain time interval.
	// The syntax for the <m>subscore</m> message is:
	// <b>subscore <m>VOICES</m> <m>MEASURE_RANGE</m> <m>optional:SELECTIVE_OPTIONS</m></b>,
	// <m>VOICES</m> is an llll of the kind <b>(<m>voice_number1</m> <m>voice_number2</m>...)</b>
	// containing the number of the voices to be output; leave <b>nil</b> or <b>()</b> if you want to output all voices. <br />
	// <m>MEASURE_RANGE</m> is an llll of the kind <b>(<m>start_measure_number</m> <m>end_measure_number</m>)</b> representing the
	// range of measures that has to be output (the defined one are included). 
	// Leave such list as <b>nil</b> or <b>()</b> if you want this range to be all the measures of the <o>bach.score</o>.
	// Otherwise <m>start_measure_number</m> is the number of the starting measure in the range, and
	// <m>end_measure_number</m> is the number of the ending measure in the range; leave any negative value
	// for <m>end_ms</m> if you want the portion of <o>bach.score</o> to be output to go till the end of the notation object. <br />
	// The third llll, <m>optional:SELECTIVE_OPTIONS</m>, is optional, and if given might contain a symbol or list of symbols
	// which handles what part of the header should be dumped. By default all header is output. Options for these symbols are exactly
	// the same as for the <m>dump</m> message (see its documentation to know more). For instance <b>subscore (4 5) (4 9) (clefs markers body)</b>
	// output voices 4 and 5 in the portion of the <o>bach.score</o> going from measure 4 to measure 9, and outputs in addition to the musical content (the body)
	// the information about clefs and the markers. 
	// Leave <b>(body)</b> as third parameter if you only want to dump the music content, and no header information
	// @marg 0 @name voices @optional 0 @type llll
	// @marg 1 @name measure_range @optional 0 @type llll
	// @marg 2 @name selective_options @optional 1 @type llll
    // @example subscore (1 2 4) (3 7) @caption extract voices 1, 2 and 4, measure 3 through 7
    // @example subscore () (3 7) @caption extract all voices, from measure 3 to measure 7
    // @example subscore (1 2) () @caption extract voices 1 and 2 for the whole duration
    // @example subscore (1 3) (4 -1) @caption extract voices 1 and 3 from measure 4 to the end
    // @example subroll (1 3) (4 -1) (body) @caption only dump the body
    // @example subroll (1 3) (4 -1) (clefs markers body) @caption only dump clefs, markers and body
    // @seealso dump
	class_addmethod(c, (method) score_subscore, "subscore", A_GIMME, 0);


	// @method merge @digest Merge notes or chords
	// @description You can use the <m>merge</m> message to perform a merging of chords whose duration is too small, 
	// and/or of notes too near (with respect to the pitch). The two tasks are separate: the first implies that small chords will be deleted, 
	// and their duration will increase other chords' duration; the second implies that notes whose pitches differ by less then a pitch 
	// threshold will be merged to a single one. <br />
	// Two arguments are needed for the "merge" command: the time threshold (in symbolic units, e.g.: <b>1/32</b> merges all the
	// chords whose duration is less or equal to 1/32) and the pitch threshold (in cents).
	// If any of the two threshold is set as negative, this means that there will be no corresponding merging: leave the time threshold as negative
	// if you only want to merge in the pitch direction, leave the pitch threshold as negative if you only want to merge chords in the time coordinate. <br />
	// Two additional optional arguments can be given specifying, respectively: the time merging policy (-1 = merge towards the left chord, 
	// 0 = split small chord's duration between left and right chords, 1 = merge towards the right chord), the pitch merging policy (-1 = set pitch to the bottommost 
	// pitch, 0 = set pitch to the average of pitches, 1 = set pitch to the topmost pitch). 
	// The merging policies are optional parameters: if you don't specify them, by default they are 0 (= merge to the average). <br />
	// If an optional "selection" symbol is put in front of the time threshold argument, the merging is performend only on the current selection,
	// otherwise it is performed on the whole score.
	// @marg 0 @name selection @optional 1 @type sym
	// @marg 1 @name threshold_time @optional 0 @type rational
	// @marg 2 @name threshold_cents @optional 0 @type float
	// @marg 3 @name time_merging_policy @optional 1 @type int
	// @marg 4 @name pitch_merging_policy @optional 1 @type int
    // @example merge 1/32 10 @caption merge chords <= 1/32 and notes whose distance is <= 10cents
    // @example merge -1 10 @caption just merge notes whose distance is <= 10cents
    // @example merge selection 1/32 10 @caption just merge elements if they are selected
    // @example merge 1/32 10 -1 @caption as the first example, but merge to the leftmost chord
    // @example merge 1/32 10 1 @caption as the first example, but merge to the rightmost chord
    // @example merge 1/32 10 1 1 @caption the same, but merge notes to the highest one
    // @example merge 1/32 10 1 -1 @caption the same, but merge notes to the lowest one
    // @example merge 1/32 10 0 -1 @caption merge chords to the average and notes to the lowest one
    // @seealso split
	class_addmethod(c, (method) score_merge, "merge", A_GIMME, 0);

    
    // @method overtype @digest Replace score content at given points
    // @description You can use the <m>overtype</m> message to replace the score content at given points in time for given voices, independently
    // from the measure barline boxing. Somehow, this operation corresponds to putting a cursor somewhere in the score, and start "typing"
    // some content. <br />
    // The first argument for the <m>overtype</m> message is the overtyping region, in the form
    // <b>(<m>START_TIMEPOINT</m> <m>optional_END_TIMEPOINT</m>)</b>, where each one of the two elements must be in the timepoint syntax
    // (see below). If the second timepoint is not given, then the duration of the overtyping region will be determined by the duration
    // of the overtype content. Notice that the timepoints also include the voices in which the overtype must occurr. <br />
    // The second argument is indeed the content to be overtyped. This is assumed to be in the score gathered-syntax, but without any
    // measure llll levels. Somehow, it is assumed to be an "unboxed" score content in gathered syntax, although rhythmic tree levels might
    // also appear in such content. <br /> <br />
    // @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
    // @marg 0 @name region @optional 0 @type llll
    // @marg 1 @name content @optional 0 @type llll
    // @example overtype ((1) (1 2/4)) (((1/2 (C4 100 0)))) @caption replace first two quarters of measure 1 with a middle C note
    // @example overtype ((1) (1 1/2)) (((-1/2))) @caption replace first two quarters of measure 1 with rests
    // @example overtype ((1) (1 1/2)) ((())) @caption exactly the same
    // @example overtype ((2 3)) (((1/2 (C4 100 0)) (1/2 (E4 100 0)) (1/2 (G4 100 0)))) @caption put a C major arpeggio of half notes in voice 2 starting measure 3
    // @example overtype ((2 3 1/2)) (((1/4 (C4 100 0)) (1/4 (E4 100 0)) (1/4 (G4 100 0)))) @caption put a C major arpeggio of quarter notes in voice 2, starting from 1/2 after beginning of measure 3
    // @example overtype ((1 1 3/8) (1 2 1/4)) (((1/8 (C4 100 0)) (1/8 (E4 100 0)) (1/4 (G4 100 0)) (1/8 (C5 100 0)) (1/4 (E5 100 0)) (1/4 (E5 100 0)))) @caption replace content of voice 1, from measure 1 (after 3/4) to measure 2 (after 1/4), with the defined gathered-syntax content (if content overflows boundaries, it is trimmed)
    // @example overtype ((1 1 3/8) (2 2 1/4)) (((1/8 (C4 100 0)) (1/8 (E4 100 0)) (1/8 (G4 100 0))) (((leveltype 25) (1/12 (7200. 100 0)) (1/12 (7400 100 0)) (1/12 (7600. 100 0))) ((1/8 (7700 100 0)) (1/8 (7900 100 0))) ((-1/8) (1/8 (7700 100 0))))) @caption replace portion of score spanning multiple voices, with content containing rhythmic tree specification
    // @seealso llll, autobeam
    class_addmethod(c, (method) score_anything, "overtype", A_GIMME, 0);


	// @method collapse @digest Collapse score to a single voice
	// @description Use the <m>collapse</m> message to reduce the score to a single voice, containing the .
	// Polyphonies are collapsed to sequence of chords whose notes are properly tied (exactly as the original content).
	// A simple <m>collapse</m> message will collapse the content of all voices. You can add up to two llll optional arguments, 
	// to obtain the general syntax:
	// <b>collapse <m>VOICES</m> <m>SELECTIVE_OPTIONS</m></b>,
	// <m>VOICES</m> is an llll of the kind <b>(<m>voice_number1</m> <m>voice_number2</m>...)</b>
	// containing the number of the voices to be output; leave <b>nil</b> or <b>()</b> if you want to output all voices. <br />
	// <m>SELECTIVE_OPTIONS</m>, if given, might contain a symbol or list of symbols
	// which handle what part of the header should be dumped. By default all header is output. Options for these symbols are exactly
	// the same as for the <m>dump</m> message (see its documentation to know more). For instance <b>collapse (4 5 8) (markers body)</b>
	// will collapse voices 4, 5 and 8 into a single voice, preserving also markers. <br />
	// By default the measureinfo (see <m>llll</m> message) of the first voice is kept. If a <m>VOICES</m> llll is specified, the measureinfo 
	// of the first entered voice number is kept as reference. If you want to change such reference, just change the order of the numbers inside
	// <m>VOICES</m>. For instance <b>collapse (5 4 8) (markers body)</b> will act as before, but keeping measureinfo of voice 5.
	// @marg 0 @name voices @optional 0 @type llll
	// @marg 1 @name selective_options @optional 1 @type llll
    // @example collapse @caption collapse everything onto a single voice
    // @example collapse (2 3 6) @caption collapse only voices 2, 3 and 6
    // @example collapse (2 3 6) (body markers) @caption the same, but only dump body and markers
    // @example collapse () (body) @caption collapse all voices, but only send out the body
    // @seealso quantize
	class_addmethod(c, (method) score_collapse, "collapse", A_GIMME, 0);


	// @method ceilmeasures @digest Pad measures
	// @description The message <m>ceilmeasures</m> forces all voices have the same number of measures,
	// by possibly appending blank measures at the end of some voice. 
	// If the voice have the same tempi and time signatures, this message force all voices to end together
	// (otherwise this is not necessarily the case). <br />
    // If an integer argument is provided, then all voices are padded so to have that number of measures.
    // @marg 0 @name num_measures @optional 1 @type int
    // @example ceilmeasures @caption Make all voice have the same number of measures
    // @example ceilmeasures 100 @caption Pad all voices to have 100 measures
	class_addmethod(c, (method) score_ceilmeasures, "ceilmeasures", A_GIMME, 0);

    
	// @method filltiedsequenceswithslots @digest Fill tie sequences with slots
	// @description Fills each note in a sequence of ties with the slots of the first tied note, provided that the <m>singleslotfortiednotes</m> specification
	// is set in the slotinfo (otherwise the slot is not copied). This might be useful for instance while reversing, scrambling or modifying sequences of tied notes
	// while preserving the correct slot information.
    // @seealso addslot
	class_addmethod(c, (method) score_copy_slots_to_tied_noted_sequences, "filltiesequenceswithslots", 0);

    
	// @method clear @digest Clear all the measures or a specific parameter inlet
	// @description A <m>clear</m> message sent in the first inlet will delete all the measures the <o>bach.score</o>, and all the markers.
	// If an integer argument is given, the message will only clear the content of a specific voice (the one corresponding to the input integer number). <br />
	// A <m>clear</m> message sent in any of the separate parameters inlets (all inlets but the first one) will clear the content which was
	// possibly stored in such inlet. This is equivalent to sending a <b>nil</b> or <b>()</b> message in that inlet.
	// @marg 0 @name voice_number @optional 1 @type int
    // @example clear @caption delete all measures
    // @example clear 3 @caption the same, only for voice 3
    // @seealso clearall, clearbreakpoints, delete
	class_addmethod(c, (method) score_anything, "clear", A_GIMME, 0);


	// @method clearall @digest Clear all separate parameters inlets
	// @description @copy BACH_DOC_MESSAGE_CLEARALL
    // @seealso clear
	class_addmethod(c, (method) score_anything, "clearall", A_GIMME, 0);

	// @method clearmarkers @digest Delete all markers
	// @description @copy BACH_DOC_MESSAGE_CLEARMARKERS
    // @seealso addmarker, deletemarker
	class_addmethod(c, (method) score_clearmarkers, "clearmarkers", 0);


	// @method appendmeasures @digest Append several measures
	// @description An <m>appendmeasures</m> message will append one or more measures to the existing ones.
	// If the message has no arguments, the separate parameters are supposed to have been inserted through the separate parameter inlets,
	// and the <m>addmeasures</m> message will essentially act like a <m>bang</m> which in turns will preserve the existing content, 
	// and append the newly introduced measures. Refer to the <m>llll</m> message to know more about separate parameter syntax of the inlets.<br />
    // If the message has a single integer argument <m>N</m>, <m>N</m> empty measures will be appended to all voices. <br />
    // If the message has a two arguments <m>voice_num</m> and <m>N</m>, <m>N</m> measures will be appended to the specified voice; <m>voice_num</m>
    // can also be an llll in the form <b>(<m>start_voice_num</m> <m>end_voice_num</m>)</b>, in which case measures are appended to all voices
    // from <m>start_voice_num</m> to <m>end_voice_num</m>; finally <m>voice_num</m> can also be the "all" symbol, in which case
    // measures are appended to all voices. <br />
	// If the message has more complex llll arguments, these are supposed to be in llll form, exactly in in the same syntax as the whole <o>bach.score</o>
	// gathered syntax (without header specification): one llll for each voice,
	// containing one llll for each measure to add (in measure gathered syntax). If for a given voice
	// you don't need to add any chord, just set a <b>()</b> llll.
    // In this case the message syntax becomes:
    // <b>appendmeasures <m>VOICE1</m> <m>VOICE2</m>...</b>,
    // where each voice is an llll in voice gathered syntax.
	// For instance, a valid message would be <b>appendmeasures ((((3 4)()) (1/4 (7185. 100 0)) (1/4 (6450. 100 0)) (1/4 (7185 100 0)))
	// (((4 4) ()) (-1/4) (1/12 (5850. 100 0 1)) (1/6 (6300. 100 0 0) 2) (1/6 (6600. 100 0)) (1/12 (6600. 100 1)) (1/4 (6600. 100 0))))</b>
	// <br /> <br />
	// @copy BACH_DOC_VOICE_GATHERED_SYNTAX_SCORE
    // @marg 0 @name voice_numbers @optional 1 @type int/symbol/llll
    // @marg 1 @name how_many @optional 1 @type int
	// @marg 2 @name voices @optional 1 @type llll
    // @example appendmeasures 5 @caption append 5 empty measures to all voices
    // @example appendmeasures all 5 @caption exactly the same
    // @example appendmeasures 2 5 @caption append 5 empty measures to voice 2
    // @example appendmeasures (2 4) 5 @caption append 5 empty measures to voices 2 through 4
    // @example appendmeasures ((((3 4) ()) (1/4 (C4 100 0)) (2/4 (E4 100 0))) (((2 4) ()) (2/4 (G4 100 0)))) ((((5 4) ()) (-1) (1/4 ( C4 100 0)))) @caption add some measures in gathered syntax (on element for each voice)
    // @seealso llll, appendmeasure, insertmeasure, insertmeasures, deletemeasures
	class_addmethod(c, (method) score_anything, "appendmeasures", A_GIMME, 0);
    class_addmethod(c, (method) score_anything, "addmeasures", A_GIMME, 0);

    
    // @method appendmeasure @digest Append a single measure
    // @description An <m>appendmeasure</m> message will append a single measure to the existing ones.
    // The message accepts a voice number as first argument, and the gathered syntax of the measure to add as second argument. <br />
    // The voice number can also be an llll in the form <b>(<m>start_voice_num</m> <m>end_voice_num</m>)</b>, in which case the measure is
    // appended to all voices from <m>start_voice_num</m> to <m>end_voice_num</m>; <m>voice_num</m> can also be the "all" symbol, in which case
    // measure is appended to all voices. <br />
    // The measure (second argument) is expected to be in gathered syntax (see below).
    // If more than one voice is concerned, and if additional measures (in gathered syntax) are added after this second argument,
    // measure are assigned in voice-wise fashion. If no measure gathered syntax is specified, an empty measure will be appended. <br />
    // For instance, a valid message would be <b>appendmeasure (2 3) ((((3 2) 8) ()) (3/8 (6000 100)) (-1/4)) (((5 8) ()) (1/4 (5900 100)) (-3/4))</b>
    // <br /> <br />
    // @copy BACH_DOC_MEASURE_GATHERED_SYNTAX_SCORE
    // @marg 0 @name voice_numbers @optional 1 @type int/symbol/llll
    // @marg 1 @name measures @optional 1 @type llll
    // @example appendmeasure @caption append 1 empty measure to all voices
    // @example appendmeasure 2 (((8 8)) ()) @caption append a blank 8/8 measure to voice 2
    // @example appendmeasure all (((8 8)) ()) @caption append a blank 8/8 measure to all voices
    // @example appendmeasure (2 5) ((((3 2) 8) ()) (3/8 (6000 100)) (-1/4)) @caption append a measure to voices 2 through 5
    // @example appendmeasure (2 3) ((((3 2) 8) ()) (3/8 (6000 100)) (-1/4)) (((5 8) ()) (1/4 (5900 100)) (-3/4)) @caption append first measure to voice 2, second measure to voice 3
    // @seealso appendmeasures, insertmeasure, insertmeasures, deletemeasures
    class_addmethod(c, (method) score_anything, "appendmeasure", A_GIMME, 0);

    
    // @method insertmeasures @digest Insert several measures
    // @description An <m>insertmeasures</m> message will insert one or more measures at a given score position.
    // The message expects a voice number as first argument, the measure number for the insertion as second argument,
    // and either the number of (empty) measures to insert, or the gathered syntax of the measures to insert as third argument. <br />
    // The voice number can also be an llll in the form <b>(<m>start_voice_num</m> <m>end_voice_num</m>)</b>, in which case the measure is
    // inserted in all voices from <m>start_voice_num</m> to <m>end_voice_num</m>; <m>voice_num</m> can also be the "all" symbol, in which case
    // the measure is inserted in all voices (default). <br />
    // The measure number for insertion is the measure index the measure will have once inserted. This can be either positive or negative:
    // negative numbers mean counting backwards from the score end (-1 = last measure, -2 = one-but-last measure, and so on). <br />
    // If the third argument is an integer number <m>N</m>, <m>N</m> empty measures will be inserted at the given position.
    // If it is an llll, it is supposed to be in the standard score gathered syntax form (one element for each voice). In this case the
    // message syntax would become:
    // <b>insertmeasures <m>voice_numbers</m> <m>measure_number_for_insertion</m> <m>VOICE1</m> <m>VOICE2</m>...</b>,
    // where each voice is an llll in voice gathered syntax.
    // For instance, a valid message would be <b>insertmeasures 2 3 ((((3 4) ()) (1/4 (7185 100)) (1/4 (6450 100)) (1/4 (7185 100))) (((4 4) ())
    // (-1/4 0) (1/12 (5850 100)) (1/6 (6300 100)) (1/6 (6600 100)) (1/12 (6600 100)) (1/4 (6600 100))))</b>
    // <br /> <br />
    // @copy BACH_DOC_VOICE_GATHERED_SYNTAX_SCORE
    // @marg 0 @name voice_numbers @optional 1 @type int/symbol/llll
    // @marg 1 @name measure_position @optional 1 @type int
    // @marg 2 @name voices @optional 1 @type llll
    // @example insertmeasures 2 3 5 @caption insert 5 empty measures at measure 3, voice 2 only
    // @example insertmeasures (2 4) 1 5 @caption insert 5 empty measures at the beginning, voice 2 through 4
    // @example insertmeasures all -1 3 @caption insert 3 empty measures at the end, for all voices
    // @example insertmeasures all -2 3 @caption insert 3 empty measures before last measure, for all voices
    // @example insertmeasures 2 3 ((((3 4) ()) (1/4 (7185 100)) (1/4 (6450 100)) (1/4 (7185 100))) (((4 4) ()) (-1/4 0) (1/12 (5850 100)) (1/6 (6300 100)) (1/6 (6600 100)) (1/12 (6600 100)) (1/4 (6600 100)))) @caption insert given measures in voice 2, at measure 3
    // @seealso appendmeasure, appendmeasures, insertmeasure, deletemeasures
    class_addmethod(c, (method) score_anything, "insertmeasures", A_GIMME, 0);
    
    
    // @method insertmeasure @digest Insert a single measure
    // @description An <m>insertmeasure</m> message will insert a single measure at a given score position.
    // The message expects a voice number as first argument, the measure number for the insertion as second argument,
    // and the gathered syntax of the measure to insert as third argument. <br />
    // The voice number can also be an llll in the form <b>(<m>start_voice_num</m> <m>end_voice_num</m>)</b>, in which case the measure is
    // inserted in all voices from <m>start_voice_num</m> to <m>end_voice_num</m>; <m>voice_num</m> can also be the "all" symbol, in which case
    // the measure is inserted in all voices (default). <br />
    // The measure number for insertion is the measure index the measure will have once inserted. This can be either positive or negative:
    // negative numbers mean counting backwards from the score end (-1 = last measure, -2 = one-but-last measure, and so on). <br />
    // The gathered syntax of the measure is the standard score gathered syntax for measures (see below).
    // If more than one voice is concerned, and if additional measures (in gathered syntax) are added after this third argument,
    // measure are assigned in voice-wise fashion. If no measure gathered syntax is specified, an empty measure will be isnerted.
    // @copy BACH_DOC_MEASURE_GATHERED_SYNTAX_SCORE
    // @marg 0 @name voice_numbers @optional 1 @type int/symbol/llll
    // @marg 0 @name measure_position @optional 0 @type int
    // @marg 1 @name measures @optional 1 @type llll
    // @example insertmeasure all 1 @caption insert a blank measure in all voices, at the beginning
    // @example insertmeasure 2 3 @caption insert a blank measure in voice 2, at measure number 1
    // @example insertmeasure 2 3 (((8 8)) ()) @caption insert a blank 8/8 measure in voice 2, at measure number 3
    // @example insertmeasure 2 -2 (((8 8)) ()) @caption the same, before the last measure
    // @example insertmeasure insert -3 (((8 8)) ()) @caption insert a blank 8/8 measure to all voices, before the one-but-last measure
    // @example insertmeasure all 2 ((((3 2) 8) ()) (3/8 (6000 100)) (-1/4)) @caption insert a given measure in all voices at measure 2
    // @example insertmeasure (6 7) 2 ((((3 2) 8) ()) (3/8 (6000 100)) (-1/4)) @caption the same, for voices 6 through 7
    // @example insertmeasure (6 7) 2 ((((3 2) 8) ()) (3/8 (6000 100)) (-1/4)) (((5 8) ()) (1/4 (5900 100)) (-3/4)) @caption insert first measure in voice 6, second measure in voice 7
    // @seealso appendmeasure, appendmeasures, insertmeasures, deletemeasures
    class_addmethod(c, (method) score_anything, "insertmeasure", A_GIMME, 0);
    
    
    // @method deletevoice @digest Delete a voice
    // @description A <m>deletevoice</m> message, followed by an integer number <m>N</m>, will delete the <m>N</m>-th voice from the score.
    // @marg 0 @name voice_number @optional 0 @type int
    // @example deletevoice 4 @caption delete 4th voice
    // @seealso insertvoice
    class_addmethod(c, (method) score_anything, "deletevoice", A_GIMME, 0);
    
    
    // @method insertvoice @digest Insert a voice
    // @description An <m>insertvoice</m> message, followed by an integer number <m>N</m>, will create a new voice and insert it as the <m>N</m>-th voice
    // of the current score. If a second llll argument is set, this can be either an integer, specifying the number of reference voice from where
    // the measureinfo should be copied (hence the new voice will be filled with empty measures, according to the specified reference voice),
    // or an llll, containing the gathered syntax of the whole new voice, so that the new voice comes also
    // filled with musical content. <br />
    // @copy BACH_DOC_VOICE_GATHERED_SYNTAX_SCORE
    // @marg 0 @name voice_number @optional 0 @type int
    // @marg 1 @name voice_or_ref @optional 1 @type int/llll
    // @example insertvoice 2 @caption insert a empty voice as 2nd voice
    // @example insertvoice 2 4 @caption also initialize it with the properties and measureinfo of 4th voice
    // @example insertvoice 2 ((((3 4) ()) (1/4 (C4 100 0)) (2/4 (E4 100 0))) (((2 4) ()) (2/4 (G4 100 0)))) @caption also fill it with some content
    // @seealso deletevoice
    class_addmethod(c, (method) score_anything, "insertvoice", A_GIMME, 0);
    

	// @method clearnotes @digest Clear all the chords
	// @description A <m>clearnotes</m> message sent in the first inlet will delete all the chords the <o>bach.score</o>, but will keep the existing measures
	// as they are (only they will be voided).
	// If an integer argument is given, the message will only clear the content of a specific voice (the one corresponding to the input integer number). <br />
	// @marg 0 @name voice_number @optional 1 @type int
    // @example clearnotes @caption clear all the chords
    // @example clearnotes 3 @caption clear all the chords in voice 3
    // @seealso cleartempi, clearbreakpoints, cleararticulations, delete, clear
	class_addmethod(c, (method) score_clearnotes, "clearnotes", A_GIMME, 0);
	
	
	// @method cleartempi @digest Clear all the tempi
	// @description A <m>cleartempi</m> message sent in the first inlet will delete all the tempi inside the <o>bach.score</o>.
	// If an integer argument is given, the message will only clear the content of a specific voice (the one corresponding to the input integer number). <br />
	// @marg 0 @name voice_number @optional 1 @type int
    // @example cleartempi @caption clear all the tempi
    // @example cleartempi 4 @caption clear all the tempi in voice 4
    // @seealso clearchords, clearbreakpoints, cleararticulations, delete, clear
	class_addmethod(c, (method) score_cleartempi, "cleartempi", A_GIMME, 0);
	

    // @method clearbreakpoints @digest Clear all the pitch breakpoints
    // @description A <m>clearbreakpoints</m> message sent in the first inlet will delete all the pitch breakpoints inside the <o>bach.score</o>, and
    // reset the note tails.
    // If an integer argument is given, the message will only clear the content of a specific voice (the one corresponding to the input integer number). <br />
    // @marg 0 @name voice_number @optional 1 @type int
    // @example clearbreakpoints @caption clear all the breakpoints
    // @example clearbreakpoints 2 @caption clear all the clearbreakpoints in voice 2
    // @seealso clearchords, cleartempi, cleararticulations, delete, clear
    class_addmethod(c, (method) score_anything, "clearbreakpoints", A_GIMME, 0);

    
	// @method cleararticulations @digest Clear all the articulations
	// @description A <m>cleararticulations</m> message sent in the first inlet will delete all the articulations inside the <o>bach.score</o>.
	// If an integer argument is given, the message will only clear the content of a specific voice (the one corresponding to the input integer number). <br />
	// @marg 0 @name voice_number @optional 1 @type int
    // @example cleararticulations @caption clear all the articulations
    // @example cleararticulations 2 @caption clear all the articulations in voice 2
    // @seealso clearchords, cleartempi, clearbreakpoints, delete, clear
	class_addmethod(c, (method) score_cleararticulations, "cleararticulations", A_GIMME, 0);

	
	// @method interp @digest Obtain active notes data at a given instant
	// @description The <m>interp</m> message, followed by a time value (in milliseconds) retrieves
	// the instantaneous data of all the notes which are active at the given time instant.
	// The answer is sent through the playout in the following form: <b>interp <m>VOICE1</m> <m>VOICE2</m>...</b>
	// where each <m>VOICE</m> is in the form <b>(<m>CHORD</m>)</b>, being the chord active at the 
	// given time instant (if any, or null if none), with <m>CHORD</m> being in the form <b>(<m>NOTE1</m> <m>NOTE2</m>...)</b>, 
	// being the chord notes active at
	// the given time instant, each in the standard note gathered syntax, with two important variations:
	// there is no tie element, and for each slot marked as temporal only the slot element at the given time instant
	// is output (e.g. the interpolated function point of a function slot). <br /> <br />
	// @copy BACH_DOC_NOTE_GATHERED_SYNTAX_SCORE
	// @marg 0 @name time @optional 0 @type float/llll
    // @example interp 1000 @caption get info on chords being played at 1s
    // @example interp (3) @caption get info on chords being played at beginning of measure 3
    // @example interp (3.5) @caption get info on chords being played at half of measure 3
    // @example interp (3 1/4) @caption get info on chords being played after 1/4 of measure 3
    // @example interp (2 3 1/4) @caption get info on chords being played after 1/4 of measure 3 of voice 2 (but retrieve all voices!)
    // @seealso getcurrentchord, sample
	class_addmethod(c, (method) score_anything, "interp", A_GIMME, 0);
	
	
	// @method sample @digest Sample score data
	// @description The <m>sample</m> message, followed by a integer (the number of sampling points), 
	// samples the note data (exactly as <m>interp</m> does) throughout the score, at the (uniformly taken) sampling point. 
	// The answer is sent through the playout in the following form: <b>sample (<m>t1</m> <m>t2</m>...) (<m>RES1</m> <m>RES2</m>...)...</b>
	// where each <m>t</m> is an instant in milliseconds, and each <m>RES</m> is the result of the <m>interp</m> message performed
	// on such instant (see <m>interp</m> to know more). <br />
    // If the <b>ms</b> symbol is given as second argument, the first numeric argument (which can also be non-integer, in
    // this case) is considered to be the distance between samples (in milliseconds), and not the number of samples. <br />
    // If the <b>timepoint</b> symbol is given as second argument, the first argument is expected to be an interval expressed in the
    // timepoint syntax; the score will be sampled at multiples of such timepoint. For instance, a timepoint of <b>(1)</b> will sample
    // the score at the beginning of each measure, while a timepoint of <b>(0 1/4)</b> will sample the score at each quarter note;
    // a timepoint of <b>(4 0 1/4)</b> will sample the score at each quarter note, taking the fourth voice as "master" to consider
    // the timepoints (which might be crucial if voices have different tempi or time signatures). If no voice number is specified, the
    // voice having the most measures is considered. Extended timepoint syntax (with floating point numbers) is not supported. <br /> <br />
    // @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
    // @marg 0 @name num_samples_or_delta @optional 0 @type number/llll
    // @marg 1 @name ms_or_timepoint @optional 1 @type symbol
    // @example sample 10 @caption sample score in 10 equally spaced points
    // @example sample 1000 ms @caption sample score each second
    // @example sample (1) timepoint @caption sample score at the beginning of each measure
    // @example sample (0 1/4) timepoint @caption sample score at each quarter note
    // @example sample (2 0 1/2) timepoint @caption sample score at each half note, having as reference the second voice
    // @seealso interp, getcurrentchord
	class_addmethod(c, (method) score_anything, "sample", A_GIMME, 0);
	

	// @method dump @digest Dump information
	// @description A <m>dump</m> message will simply output 
	// the content of all separate parameters from all separate outlets (in separate syntax, right-to-left), and 
	// all the content of <o>bach.score</o> in gathered syntax from the first outlet (header information included). See below to know more
	// about gathered and separate syntaxes. <br />
	// The <m>dump</m> message also accepts as argument one of the following symbols, which will only dump a portion of the global information: <br />
	// - <b>measureinfo</b>: only dump the onsets (in separate syntax) from the second outlet. <br />
	// - <b>cents</b>: only dump the cents (in separate syntax) from the third outlet. <br />
	// - <b>durations</b>: only dump the durations (in separate syntax) from the fourth outlet. <br />
	// - <b>velocities</b>: only dump the velocities (in separate syntax) from the fifth outlet. <br />
	// - <b>ties</b>: only dump the velocities (in separate syntax) from the fifth outlet. <br />
	// - <b>extras</b>: only dump the extras (in separate syntax) from the sixth outlet. <br />
	// - <b>score</b>: only dump the whole <o>bach.score</o> gathered syntax from the first outlet (and nothing from the separate parameters outlets). <br />
	// - <b>body</b>: only dump the whole <o>bach.score</o> gathered syntax from the first outlet, and drop all the header specifications: only dump the 
	// body (content) of the <o>bach.score</o>. <br />
	// - <b>header</b>: only dump the whole <o>bach.score</o> from the first outlet, dropping all the content (body) specification, and only 
	// outputting all the header specification. <br />
	// Moreover, if one needs to only dump specific header elements, the <m>dump</m> message may also accept a list of symbols referring to them.
	// Namely, the possibilities are the following ones: "keys", "clefs", "voicenames", "midichannels", "markers", "slotinfo", "command".
	// Also the "body" symbol can always be added, in order to also dump the music content.
	// For instance, <b>dump keys slotinfo voicenames</b> will dump the key signatures, the slotinfo and the voicenames, 
	// while, <b>dump keys slotinfo voicenames body</b> will do the same but will also output the music content (body). 
	// The order of the dumped elements does not correspond in general to the order of the symbols in the list.
	// <br /> <br />
	// @copy BACH_DOC_SCORE_GATHERED_SYNTAX 
	// @copy BACH_DOC_SCORE_SEPARATE_SYNTAX
	// @copy BACH_DOC_SEPARATE_SYNTAX_EXTRAS
    // @marg 0 @name selective_dump_options @optional 1 @type list
    // @example dump @caption dump whole information from all outlets
    // @example dump separate @caption dump separate-syntax outlets only
    // @example dump roll @caption dump first outlet only (gathered syntax)
    // @example dump measureinfo @caption dump measureinfo only
    // @example dump velocities @caption dump velocities only
    // @example dump body @caption dump first outlet only, but without dumping the header
    // @example dump header @caption the same, but only dumping the header
    // @example dump keys clefs body @caption dump keys, clefs and body from first outlet
    // @seealso getmarker, llll
	class_addmethod(c, (method) score_dump, "dump", A_GIMME, 0);
	
	
	// @method score2roll @digest Send data for converting content into a <o>bach.roll</o>
	// @description A <m>score2roll</m> message will trigger the output from the first outlet of information which should be given to the
	// <o>bach.score2roll</o> first inlet, in order to have it convert such information into a <o>bach.roll</o> object. <br />
    // Message attributes might also trigger the conversion of measure barlines, time signatures, tempi or measure divisions into markers.
    // If all measureinfo are synchronous throughout the whole score, such measure information is only set once for all voices;
    // otherwise markers are set for each measure of each voice.
    // @mattr markmeasures @type int @default 0 @digest If non-zero, converts measure barlines to markers
    // @mattr marktimesigs @type int @default 0 @digest If non-zero, converts time signatures to markers
    // @mattr marktempi @type int @default 0 @digest If non-zero, converts tempi to markers
    // @mattr markdivisions @type int @default 0 @digest If non-zero, converts measure divisions to markers
    // @example score2roll @markmeasures 1 @caption convert score to roll and put a marker at each measure
	class_addmethod(c, (method) score_score2roll, "score2roll", A_GIMME, 0);


	// @method openslotwin @digest Open a slot window
	// @description @copy BACH_DOC_MESSAGE_OPENSLOTWINS
	// @marg 0 @name slot_number_or_name @optional 0 @type int/symbol
    // @example openslotwin 3 @caption open 3rd slot window for selected note
    // @example openslotwin amplienv @caption open slot window for slot named 'amplienv'
    // @seealso addslot
	class_addmethod(c, (method) score_openslotwin, "openslotwin", A_GIMME, 0);


	// @method getdomain @digest Get the current domain
	// @description The <m>getdomain</m> message forces <o>bach.score</o> to output from the playout the current domain,
	// i.e. the portion of <o>bach.score</o> currently displayed inside the screen. The output information is both in milliseconds
	// and in timepoints.
	// The syntax of the output answer is: <b>domain <m>start_ms</m> <m>end_ms</m> <m>VOICE1</m> <m>VOICE2</m>...</b>, where the 
	// two elements following the "domain" symbol are the starting and ending point of the domain in milliseconds,
	// and each <m>VOICE</m> is an llll containing the information about the timepoints of each voice (i.e. at which measure and point in measure,
	// for each voice, the domain starts and ends): its syntax is: <b>((<m>start_measure_number</m> <m>start_sym_onset</m>) 
	// (<m>end_measure_number</m> <m>end_sym_onset</m>) <m>total_sym_duration</m>)</b>, where <m>start_measure_number</m> is the measure number of the
	// measure where the domain starts, and <m>start_sym_onset</m> is the symbolic onset (point inside the measure) at which the domain start happens;
	// <m>end_measure_number</m> is the measure number of the
	// measure where the domain end, and <m>end_sym_onset</m> is the symbolic onset (point inside the measure) at which the domain end happens;
	// finally <m>total_sym_duration</m> is the global duration of the domain in symbolic units. <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
	class_addmethod(c, (method) score_getdomain, "getdomain", A_GIMME, 0);


	// @method getdomainpixels @digest Get the pixel position of the domain
	// @description The <m>getdomainpixels</m> message forces <o>bach.score</o> to output from the playout the current position
	// (in pixels) of the domain, i.e. the starting (left) and ending (right) pixels of the currently displayed portion of music.
	// These pixels are referred to the notation object box.
	// The syntax of the output answer is: <b>domainpixels <m>start_pixel</m> <m>end_pixel</m></b>, where the 
	// two elements following the "domainpixels" symbol are the leftmost and rightmost horizontal points (in pixels) of the domain. <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso getdomainpixels, domain, getlength, testdomain
	class_addmethod(c, (method) score_getdomainpixels, "getdomainpixels", A_GIMME, 0);


	// @method testdomain @digest Test hypotetical domain 
	// @description The <m>testdomain</m> message forces <o>bach.score</o> to output from the playout a hypotetical domain, 
	// given a fixed domain starting point. The idea is that the user asks <o>bach.score</o> which would be the complete domain if such domain
	// started at a given point. 
	// The hypotetical starting point of the domain is given as argument, and can be expressed either as absolute millisecond position (a single floating point
	// number) or as a timepoint (an llll).
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// The syntax of the output answer is: <b>testdomain <m>start_ms</m> <m>end_ms</m> <m>VOICE1</m> <m>VOICE2</m>...</b>, where the 
	// two elements following the "testdomain" symbol are the starting and ending point of the hypotetical domain in milliseconds,
	// and each <m>VOICE</m> is an llll containing the information about the timepoints of each voice (i.e. at which measure and point in measure,
	// for each voice, the domain would start and end): its syntax is: <b>((<m>start_measure_number</m> <m>start_sym_onset</m>) 
	// (<m>end_measure_number</m> <m>end_sym_onset</m>) <m>total_sym_duration</m>)</b>, where <m>start_measure_number</m> is the measure number of the
	// measure where the domain would start, and <m>start_sym_onset</m> is the symbolic onset (point inside the measure) at which the domain start would happen;
	// <m>end_measure_number</m> is the measure number of the
	// measure where the domain would end, and <m>end_sym_onset</m> is the symbolic onset (point inside the measure) at which the domain end would happen;
	// finally <m>total_sym_duration</m> is the global duration the domain would have, in symbolic units. <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
	// @marg 1 @name domain_start @optional 0 @type llll
    // @example testdomain 200 @caption send domain information if the domain started at 200ms
    // @example testdomain (2) @caption the same, if it started at measure 2
    // @example testdomain (1 1/8) @caption the same, if it started at measure 1, after 1/8
    // @example testdomain (2 1 1/8) @caption the same, if it started at measure 1, after 1/8, in voice 2
    // @seealso domain, getdomainpixels, getlength
	class_addmethod(c, (method) score_testdomain, "testdomain", A_GIMME, 0);
	
	
	// @method domain @digest Change horizontal zoom to set the displayed domain
	// @description If the <m>domain</m> message is followed by a single number, this is considered as the size in milliseconds
	// of the domain (i.e. the displayed portion of <o>bach.roll</o>), and such message will change the <m>zoom</m> so that the displayed
	// portion of music has indeed that global duration. <br />
	// If the <m>domain</m> message is followed by a two numbers, these are considered as the starting and ending domain points in milliseconds,
	// and such message will change both the <m>zoom</m> and the scrollbar position so that the displayed
	// portion of music starts from the input starting point and ends at the input ending point. <br />
	// If a third argument is given, this is considered to be an ending pad in pixels (scaled with respect to the <m>vzoom</m>), so that the input ending point actually happens
	// a certain number of pixels before (if pad is positive) or after (is pad is negative) the end of the displayed portion of music. <br />
	// Beware: the <m>domain</m> message will not act on the actual measure spacing. It is a quick-and-dirty message which will act on the global horizontal zoom.
	// @marg 0 @name duration_or_starting_point_ms @optional 0 @type float
	// @marg 1 @name ending_point_ms @optional 1 @type float
	// @marg 2 @name ending_pad_pixels @optional 1 @type float
    // @example domain 4000 @caption force the domain to contain 4s
    // @example domain 2000 3000 @caption display the portion between 2s and 3s
    // @example domain 2000 3000 10 @caption ...and leave 10 pixels of ending pad
    // @seealso inscreenmeas, getdomain, getdomainpixels, testdomain, inscreen, inscreenpos
	class_addmethod(c, (method) score_domain, "domain", A_GIMME, 0);
	
	
	// @method timetopixel @digest Convert a time position into a pixel position
	// @description The <m>timetopixel</m> message converts a position given (as argument) in milliseconds or in the timepoint syntax,
    // into a position given in pixels, representing
	// the pixel at which such time position happens, in the current domain.
	// The output answer is sent through the playout, and its syntax is: <b>pixel <m>pixel_position</m></b>, where the last element
	// is indeed the position in pixels corresponding to the inserted position in milliseconds.
	// Such element can be replaced by a <o>bach.score</o> timepoint. <br />
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
	// @marg 1 @name position @optional 0 @type llll
    // @example timetopixel 4000 @caption get the pixel position corresponding to 4s
    // @example timetopixel (2) @caption get the pixel position corresponding to beginning of measure 2
    // @example timetopixel (1 1/8) @caption the same, for measure 1 after 1/8
    // @example timetopixel (3 1 1/8) @caption the same, for measure 1 after 1/8 of voice 3
    // @seealso pixeltotime, timepoint
    class_addmethod(c, (method) score_timetopixel, "timetopixel", A_GIMME, 0);
	class_addmethod(c, (method) score_getpixelpos, "getpixelpos", A_GIMME, 0); // old, deprecated

    
    // @method timepoint @digest Perform conversions between timepoint and millisecond syntaxes
    // @description The <m>timepoint</m> message, either followed by a single number, considered as milliseconds position,
    // or by an llll representing the timepoint syntax, will convert the inserted time position into milliseconds and timepoint for
    // all voices of the current score (different voices might end up having different timepoints only if tempi or timesignatures are not synchronous).
    // The output answer is sent through the playout, and its syntax is: <b>timepoint <m>ms</m> <m>TP_voice1</m> <m>TP_voice2</m>...</b>, where the
    // <m>TP_voice</m> are lllls in timepoint syntax. <br /> <br />
    // @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
    // @copy BACH_DOC_QUERY_LABELS
    // @marg 0 @name query_label @optional 1 @type llll
    // @marg 1 @name position @optional 0 @type number/llll
    // @example timepoint 1000 @caption get the timepoints (measure, point in measure) for all voices, for the point corresponding to 1s
    // @example timepoint 2000 @caption get the timepoints at 2s, for all voices
    // @example timepoint (2) @caption get the millisecond position and the timepoints at beginning of measure 2, first voice
    // @example timepoint (2 3/8) @caption the same for position at measure 2, after 3/8
    // @example timepoint (4 2 3/8) @caption the same for position at measure 2, after 3/8, for voice 4
    // @seealso timetopixel, pixeltotime
    class_addmethod(c, (method) score_timepoint, "timepoint", A_GIMME, 0);

    
	// @method pixeltotime @digest Convert a pixel position into a time position
	// @description The <m>pixeltotime</m> message converts a position given in pixels (as argument) into a temporal position, output both in milliseconds and
	// as timepoints, representing the time corresponding to the inserted pixel position.
	// The output answer is sent through the playout, and its syntax is: <b>time <m>time_position_ms</m> <m>VOICE1</m> <m>VOICE2</m>...</b>, where the
	// two elements following the "timeatpixel" symbol are the starting and ending point of the hypotetical domain in milliseconds,
	// and each <m>VOICE</m> is an llll containing the information about the timepoints of each voice (i.e. at which measure and point in measure,
	// for each voice, fall on the introduced pixel): its syntax is: <b>(<m>measure_number</m> <m>sym_onset</m>)</b>, where <m>measure_number</m> is the measure number of the
	// measure where the pixel lies, and <m>sym_onset</m> is the symbolic onset (point inside the measure) to which the pixel correspond.
	// If the pixel does not fall exactly on a chord, by default, the timepoints are output by properly interpolating the timepoints of the chords whose onset are
	// immediately at left and at right of the pixels. On the other hand, one can modify this behavior by introducing a final symbol argument (the "snap" type), which can be one 
	// of the followings: <br />
	// - <b>left</b>: always return the timepoint of the chord whose onset falls immediately at left of the inserted pixel. <br />
	// - <b>right</b>: always return the timepoint of the chord whose onset falls immediately at right of the inserted pixel. <br />
	// - <b>nearest</b>: return the timepoint of the chord whose onset falls immediately either at left or at right of the inserted pixel, depending on which onset is nearest to the pixel position. <br />
	// - <b>interp</b> (default): interpolate between left and right chord (as explained above). <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
	// @marg 1 @name pixel_position @optional 0 @type float
	// @marg 2 @name snap_type @optional 1 @type symbol
    // @example pixeltotime 300 @caption get the timepoint corresponding to 300pixels
    // @example pixeltotime 300 interp @caption the same thing
    // @example pixeltotime 300 nearest @caption get the timepoint of the chord nearest to 300pixels
    // @example pixeltotime 300 left @caption the same, only for chords with position <300pixels
    // @example pixeltotime 300 right @caption the same, only for chords with position >300pixels
    // @seealso timetopixel, timepoint
    class_addmethod(c, (method) score_pixeltotime, "pixeltotime", A_GIMME, 0);
	class_addmethod(c, (method) score_gettimeatpixel, "gettimeatpixel", A_GIMME, 0); // old deprecated

    
    // @method refresh @digest Force recomputation and redraw
    // @description The <m>refresh</m> message forces the recomputation of all the features and forces the object box to be redrawn.
    class_addmethod(c, (method) score_anything, "refresh", A_GIMME, 0);


    // @method realtime @digest Toggle real-time mode
    // @description The <m>realtime 1</m> message will toggle the real-time mode, i.e. will change some attribute values in order to
    // disable the play highlight, the undo system and the legend. This will increase the object performance for real-time tasks.
    // A subsequent <m>realtime 0</m> message will disable real-time mode, reverting the value of all attribute to the previous ones.
    // @marg 0 @name realtime_mode @optional 0 @type int
    // @example realtime 1 @caption turn real-time mode on
    // @example realtime 0 @caption turn real-time mode off
    class_addmethod(c, (method) score_anything, "realtime", A_GIMME, 0);

    
	// @method dumpnotepixelpos @digest Retrieve pixel position of every chord and note
	// @description The <m>dumpnotepixelpos</m> message retrieves the pixel position of any chord and note, and outputs it from the playout.
	// The output answer has the syntax: 
	// <b>notepixelpos <m>VOICE1</m> <m>VOICE2</m>...</b>, where each <m>VOICE</m> is an llll of the form:
	// <b>(<m>MEASURE1</m> <m>MEASURE2</m>...)</b>, and where each <m>MEASURE</m> is in turn an llll of the form 
	// <b>(<m>CHORD1</m> <m>CHORD2</m>...)</b>, and where each <m>CHORD</m> is an llll accounting for the pixel position of the chord
	// and all its notes, in the form: 
	// <b>(<m>x-pixel_onset</m> (<m>duration_in_horizontal_pixels_note1</m>  <m>duration_in_horizontal_pixels_note2</m> ...)
	// (<m>y_pixel_noteheadcenter_note1</m>   <m>y_pixel_noteheadcenter_note2</m> ...)
	// (<m>x_pixel_accidental_left_position_note1</m> <m>x_pixel_accidental_left_position_note2</m> ...))</b>.
	// The <m>x-pixel_onset</m> is the pixel corresponding to the chord onset; each <m>duration_in_horizontal_pixels_note</m> is the
	// length of eachnote duration line in pixels; each <m>y_pixel_noteheadcenter_note</m> is the vertical position (in pixels) of the center
	// of the noteheads, and each <m>x_pixel_accidental_left_position_note</m> is the pixel position of the left boundary of the leftmost accidental
	// for each note (or, if a note has no accidentals, the leftmost point of the notehead). <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll	
    // @seealso dumpvoicepixelpos, dumpmeasurepixelpos
    class_addmethod(c, (method) score_dumpnotepixelpos, "dumpnotepixelpos", A_GIMME, 0);
    class_addmethod(c, (method) score_dumpnotepixelpos, "dumppixelpos", A_GIMME, 0); // old, deprecated


	
	// @method dumpvoicepixelpos @digest Retrieve pixel position of every voice
	// @description @copy BACH_DOC_MESSAGE_DUMPVOICEPIXELPOS
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso dumpnotepixelpos, dumpmeasurepixelpos
	class_addmethod(c, (method) score_dumpvoicepixelpos, "dumpvoicepixelpos", A_GIMME, 0);
    class_addmethod(c, (method) score_dumpvoicepixelpos, "getvoicepixelpos", A_GIMME, 0); // old, deprecated

    
    // @method dumpmeasurepixelpos @digest Retrieve pixel position of every measure
    // @description The <m>dumpmeasurepixelpos</m> message retrieves the pixel position of any measure, and outputs it from the playout.
    // The output answer has the syntax:
    // <b>measurepixelpos <m>VOICE1</m> <m>VOICE2</m>...</b>, where each <m>VOICE</m> is an llll of the form:
    // <b>(<m>MEASURE1</m> <m>MEASURE2</m>...)</b>, and where each <m>MEASURE</m> is in turn an llll of the form
    // <b>((<m>x-start_pixel</m> <m>x-end_pixel</m>) (<m>y-start_pixel</m> <m>y-end_pixel</m>))</b>,
    // i.e. determining a pixel rectangle representing the measure. The y values are the staff topmost and bottommost pixel positions. <br />
    // @copy BACH_DOC_QUERY_LABELS
    // @marg 0 @name query_label @optional 1 @type llll
    // @seealso dumpvoicepixelpos, dumpnotepixelpos
    class_addmethod(c, (method) score_dumpmeasurepixelpos, "dumpmeasurepixelpos", A_GIMME, 0);

    

	// @method getvzoom @digest Retrieve the current vertical zoom
	// @description @copy BACH_DOC_MESSAGE_GETVZOOM
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso getzoom, fixvzoom
	class_addmethod(c, (method) score_getvzoom, "getvzoom", A_GIMME, 0);


	// @method getzoom @digest Retrieve the current horizontal zoom
	// @description @copy BACH_DOC_MESSAGE_GETZOOM
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso getvzoom
	class_addmethod(c, (method) score_getzoom, "getzoom", A_GIMME, 0);


	// @method getlength @digest Get the total length
	// @description @copy BACH_DOC_MESSAGE_GETLENGTH
	// @marg 0 @name query_label @optional 1 @type llll
	class_addmethod(c, (method) score_getlength, "getlength", A_GIMME, 0);


	// @method getnumvoices @digest Get the number of voices
	// @description The <m>getnumvoices</m> message forces <o>bach.score</o> to output from the playout the current number of voices.
	// The syntax of the output answer is: <b>numvoices <m>num_voices</m></b>, where the last element is the number of voices. <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso getnummeasures, getnumchords, getnumnotes
	class_addmethod(c, (method) score_getnumvoices, "getnumvoices", A_GIMME, 0);


	// @method getnummeasures @digest Get the number of measures
	// @description The <m>getnummeasures</m> message forces <o>bach.score</o> to output from the playout the current number of measures, for each voice.
	// The syntax of the output answer is: <b>nummeasures <m>num_measures_voice1</m> <m>num_measures_voice2</m>...</b>, 
	// i.e. after the "nummeasures" symbol, a list of integers is given, representing the number of measures for each one of the voices. <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso getnumvoices, getnumchords, getnumnotes
	class_addmethod(c, (method) score_getnummeasures, "getnummeasures", A_GIMME, 0);


	// @method getnumchords @digest Get the number of chords
	// @description The <m>getnumchords</m> message forces <o>bach.score</o> to output from the playout the number of chord, for each measure and for each voice.
	// The syntax of the output answer is: <b>numchords <m>VOICE1</m> <m>VOICE2</m>...</b>, where each <m>VOICE</m> is an llll of the form
	// <b>(<m>num_chords_measure1</m> <m>num_chords_measure1</m>...)</b>, 
	// i.e. after the "numchords" symbol, a list of lists is given: each outer list represents a voice, and innerly contains an integer for each one
	// of its measures: such integer is the number of chords of the measure. <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso getnumvoices, getnummeasures, getnumnotes
	class_addmethod(c, (method) score_getnumchords, "getnumchords", A_GIMME, 0);


	// @method getnumnotes @digest Get the number of notes
	// @description The <m>getnumnotes</m> message forces <o>bach.score</o> to output from the playout the number of notes, for each chord, for each measure and for each voice.
	// The syntax of the output answer is: <b>numnotes <m>VOICE1</m> <m>VOICE2</m>...</b>, where each <m>VOICE</m> is an llll of the form
	// <b>(<m>MEASURE1</m> <m>MEASURE2</m>...)</b>, and each <m>MEASURE</m> is in turn an llll of the form <b>(<m>num_notes_chord1</m> <m>num_notes_chord2</m>...)</b>,
	// i.e. after the "numnotes" symbol, a list of lists of lists is given: each outer list represents a voice, then one has a level for each measure, 
	// containing contains an integer for each one of its chords: such integer is the number of notes of the chord. <br />
	// @copy BACH_DOC_QUERY_LABELS
	// @marg 0 @name query_label @optional 1 @type llll
    // @seealso getnumvoices, getnummeasures, getnumchords
	class_addmethod(c, (method) score_getnumnotes, "getnumnotes", A_GIMME, 0);


	// @method fixvzoom @digest Fix the vertical zoom value 
	// @description @copy BACH_DOC_MESSAGE_FIXVZOOM
    // @seealso getvzoom
	class_addmethod(c, (method) score_fixvzoom, "fixvzoom", 0);

    class_addmethod(c, (method) score_preset, "preset", 0);
    class_addmethod(c, (method) score_begin_preset, "begin_preset", A_GIMME, 0);
    class_addmethod(c, (method) score_restore_preset, "restore_preset", A_GIMME, 0);
    class_addmethod(c, (method) score_end_preset, "end_preset", 0);
	CLASS_METHOD_ATTR_PARSE(c, "begin_preset", "undocumented", gensym("long"), 0L, "1");
	CLASS_METHOD_ATTR_PARSE(c, "restore_preset", "undocumented", gensym("long"), 0L, "1");
	CLASS_METHOD_ATTR_PARSE(c, "end_preset", "undocumented", gensym("long"), 0L, "1");



	// @method llll @digest Function depends on inlet
	// @description In the first inlet, an llll is expected to be a gathered syntax content of the entire <o>bach.score</o> (building
	// the new <o>bach.score</o> content from scratch), or an llll containing just some header specification 
	// (thus not affecting the body of the object). See below for more information on the gathered syntax. <br />
	// In any of the other inlets, an llll is expected to contain the separate syntax of some parameter. Namely: <br />
	// - second inlet: Measureinfo <br />
	// - third inlet: Pitches or cents <br />
	// - fourth inlet: Durations <br />
	// - fifth inlet: Velocities <br />
	// - sixth inlet: Ties <br />
	// - seventh inlet: Extras <br />
	// See below for more information about the separate syntax. Also see the <m>bang</m> method to rebuild content from these separate
	// parameters.<br /> <br />
	// @copy BACH_DOC_SCORE_GATHERED_SYNTAX
	// @copy BACH_DOC_SCORE_SEPARATE_SYNTAX
	// @copy BACH_DOC_SEPARATE_SYNTAX_EXTRAS
    // @seealso dump
	class_addmethod(c, (method) score_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method) score_int, "int", A_LONG, 0);
	class_addmethod(c, (method) score_float, "float", A_FLOAT, 0);


	// @method bang @digest Build content from separate parameters
	// @description A <m>bang</m> in any of the inlets will first rebuild new <o>bach.score</o> content starting from
	// the parameters given as input in the separate inlets (see <m>llll</m> method). 
	// If the <m>autoclear</m> attribute is set to 1, 
	// first of all the <m>bang</m> will clear the content of the <o>bach.score</o>; if on the other hand
	// the <m>autoclear</m> attribute is set to 0, the new parameter will modify the existing content.
	// Not all the separate parameters need to be given to build the new content: if some are not given, default values
	// will be used instead. Default measureinfo is a 4/4 time signature, with quarter = 60 tempo. 
	// Default durations are given by the equal division of the measure duration by the number of chords (i.e. if eight chords are needed in a 4/4 measure,
	// default duration will be 1/8 each). Default pitch in cents is 6000 (middle C).
	// Default velocity is 100, but the last one is padded if more chords than the input velocities are
	// to be created. Default extras are no extras. 
	class_addmethod(c, (method) score_bang, "bang", 0);


	// @method clock @digest Select a clock source
	// @description The word <m>clock</m>, followed by the name of an existing <m>setclock</m> objects, sets the <o>bach.roll</o> 
	// object to be controlled by that <m>setclock</m> object rather than by Max's internal millisecond clock. 
	// The word <m>clock</m> by itself sets the <o>bach.roll</o> object back to using Max's regular millisecond clock.
    // @marg 0 @name setclock_name @optional 1 @type symbol
    // @example clock ticker @caption link to 'ticker' setclock
    // @example clock @caption link to default clock
	class_addmethod(c, (method) score_clock, "clock", A_DEFSYM, 0);


	// @method play @digest Play
	// @description The <m>play</m> message plays a portion or all the <o>bach.score</o>.
	// "Playing" means that <o>bach.score</o> sends from the "playout" outlet, at the timing corresponding to each chord onset,
	// the information about all the chord, or about all the notes of the chord (depending on the <m>playmode</m> attribute, to which you
	// should refer to know more). Rests are sequenced if the <m>playrests</m> attribute is set to 1.
    // Markers are sequenced if the <m>playmarkers</m> attribute is set to 1. Tempi are sequenced
    // if the <m>playtempi</m> attribute is set to 1. Measure starting barlines (along with the corresponding measureinfo) are sequenced if
    // the <m>playmeasures</m> attribute is set to 1. <br />
	// The sequencing takes into account the solo and mute state of each element (see <m>solo</m>, <m>unsolo</m>, <m>mute</m> and <m>unmute</m> messages):
	// if elements are muted, they will not be sequences; if some elements are set as "solo", just those elements will be sequenced.
	// If multiple chords or notes are simultaneous, the order is from topmost voice to bottommost voice, then from lowest note to highest note.
	// Sequencing can be controlled with a variable speed via the <m>clock</m> message and the <o>setclock</o> object.
	// The <m>play</m> message, without any further argument, plays the <o>bach.score</o> from the current playhead cursor position 
	// (by default: the beginning of the <o>bach.score</o>) to the end. <br />
	// If you put as first argument the "offset" symbol, all the playing will be done in non-real-time mode, i.e. with no sequencing involved; playing messages
	// will be still output from the playout, but one after another, "immediately". <br />
	// If you give a single numeric argument, it will be the starting point in milliseconds
	// of the region to be played: <o>bach.roll</o> will play from that point to the end. If you give two numeric arguments, they will be the starting and
	// ending point in milliseconds of the region to be played.
	// Each one of such numbers can be replaced by a <o>bach.score</o> timepoint. <br />
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// @copy BACH_DOC_PLAYOUT_SYNTAX_SCORE
	// @marg 0 @name offline_mode @optional 1 @type symbol
	// @marg 1 @name start @optional 1 @type float/llll
	// @marg 2 @name end @optional 2 @type float/llll
    // @example play @caption play from current playhead position
    // @example play offline @caption play in non-realtime mode ("uzi-like")
    // @example play 2000 @caption play starting from 2s till the end
    // @example play 2000 4000 @caption play starting from 2s, stop at 4s
    // @example play (4) @caption play starting from measure 4
    // @example play (4 1/4) @caption play starting from measure 4, after 1/4
    // @example play (4.5) @caption play starting from half of measure 4
    // @example play (2 4 1/4) @caption play starting from measure 4, after 1/4, for voice 2
    // @example play (2 4 1/4) (5 10 1/12) @caption the same, but only till measure 10 of voice 5, after 1/12
    // @example play offline (2 4 1/4) (5 10 1/12) @caption the same, in non-realtime mode
    // @example play (3.5) (10) @caption play from half of measure 3 to beginning of measure 10 (i.e. end of measure 9)
    // @seealso stop, pause, setcursor, playselection
	class_addmethod(c, (method) score_play, "play", A_GIMME, 0);


	// @method playselection @digest Only play selected items
	// @description The <m>playselection</m> message only plays the selected content. It works exactly like <m>play</m>, but it starts playing
	// at the beginning of the selection, and ends playing at the end of the last selected item. Only selected items are sequenced.
	// Mute and solo status are also taken into account (see <m>play</m>). <br />
	// If you put as first argument the "offset" symbol, all the playing will be done in non-real-time mode, i.e. with no sequencing involved; playing messages
	// will be still output from the playout, but one after another, "immediately", in the low-priority queue. <br />
	// @marg 0 @name offline_mode @optional 1 @type symbol
    // @example playselection @caption play selected items only
    // @example playselection offline @caption the same, in non-realtime mode ("uzi-like")
    // @seealso stop, pause, play
	class_addmethod(c, (method) score_playselection, "playselection", A_GIMME, 0);


	// @method stop @digest Stop
	// @description The <m>stop</m> message stops any ongoing playing, if <o>bach.score</o> was in play mode (nothing happens otherwise).
    // @seealso pause, play, playselection
	class_addmethod(c, (method) score_stop, "stop", A_GIMME, 0);


	// @method pause @digest Pause
	// @description The <m>pause</m> message pause any ongoing playing, if <o>bach.score</o> was in play mode (nothing happens otherwise).
	// When paused, the play head cursor is updated to its last reached position while playing, so that a new play will begin by default
    // @seealso play, playselection, stop
	// at that position. 
    // @seealso play, playselection, stop
	class_addmethod(c, (method) score_pause, "pause", A_GIMME, 0);


	// @method resetslotinfo @digest Reset the slotinfo to the default one
	// @description @copy BACH_DOC_RESET_SLOTINFO
    // @seealso eraseslot
	class_addmethod(c, (method) score_resetslotinfo, "resetslotinfo", 0);
	
    
    // @method resetslotinfo @digest Reset the custom articulations definition to the default one
    // @description @copy BACH_DOC_RESET_ARTICULATIONINFO
    class_addmethod(c, (method) score_resetarticulationinfo, "resetarticulationinfo", 0);
    
    
    // @method resetslotinfo @digest Reset the custom notehead definition to the default one
    // @description @copy BACH_DOC_RESET_NOTEHEADINFO
    class_addmethod(c, (method) score_resetnoteheadinfo, "resetnoteheadinfo", 0);
    



	// @method quantize @digest Send quantization data 
	// @description The <m>quantize</m> message outputs all the content of <o>bach.score</o> from the first outlet,
	// in a form which is understandable by <o>bach.quantize</o>, for a score-to-score quantization.
	// You should just connect the first outlet of <o>bach.score</o> to the second inlet of <o>bach.quantize</o>, to have this latter
	// properly get all the information (refer to the <o>bach.quantize</o> help and documentation; also see #quantize in the help center). <br />
	// By default, the <m>quantize</m> message also outputs all the header information, but as for the <m>dump</m> message, 
	// if one needs to only dump specific header elements, the <m>quantize</m> message may also accept a list of symbols referring to them.
	// Namely, the possibilities are the following ones: "keys", "clefs", "voicenames", "midichannels", "markers", "slotinfo", "command".
	// Also the "body" symbol should always be added, in order to also dump the music content (otherwise no real quantization can be performed).
	// For instance, <b>quantize keys slotinfo body</b> will dump the key signatures, the slotinfo and will send the quantization data of the 
	// music content.
	// @marg 0 @name selective_quantize_options @optional 1 @type list
    // @example quantize @content quantize whole content
    // @example quantize body @content quantize but keep no header elements
    // @example quantize body keys markers @content as previously, but also keep keys and markers
	class_addmethod(c, (method) score_quantize, "quantize", A_GIMME, 0);


	// @method getcurrentchord @digest Get notes at cursor position
	// @description @copy BACH_DOC_MESSAGE_GETCURRENTCHORD 
    // @seealso interp, sample
	class_addmethod(c, (method) score_send_current_chord, "getcurrentchord", 0);
	
	
	// @method getloop @digest Get current loop region position 
	// @description The <m>getloop</m> message retrieves the current loop region position, and outputs it from the playout in the form
	// <b>loop <m>loop_start_ms</m> <m>loop_end_ms</m> <m>LOOP_START_TIMEPOINT</m> <m>LOOP_END_TIMEPOINT</m></b>, where the two parameters 
	// after the "loop" symbol are the beginning and end position of the loop, in milliseconds (also see the <m>loop</m> attribute), while 
	// the two last parameters are lllls representing the same milliseconds position but as timepoints, in the form 
	// <b>(<m>measure_number</m> <m>sym_onset_in_measure</m> <m>voice_number</m>)</b>. The two timepoints might not be linked to the same voice:
	// the two voice numbers are in general different (depending on the voice where the loop extreme aligns best to a chord).
	class_addmethod(c, (method) score_getloop, "getloop", 0);


	// @method mute @digest Mute selected items
	// @description @copy BACH_DOC_MESSAGE_MUTE 
    // @seealso unmute, solo, unsolo
	class_addmethod(c, (method) score_mute, "mute", 0);


	// @method unmute @digest Unmute selected items
	// @description @copy BACH_DOC_MESSAGE_UNMUTE
    // @seealso mute, solo, unsolo
	class_addmethod(c, (method) score_unmute, "unmute", 0);


	// @method solo @digest Set selected items as solo
	// @description @copy BACH_DOC_MESSAGE_SOLO
    // @seealso unsolo, mute, unmute
	class_addmethod(c, (method) score_solo, "solo", 0);


	// @method unsolo @digest Remove solo status from selected items
	// @description @copy BACH_DOC_MESSAGE_UNSOLO
    // @seealso solo, mute, unmute
	class_addmethod(c, (method) score_unsolo, "unsolo", 0);


	// @method lock @digest Lock selected items
	// @description @copy BACH_DOC_MESSAGE_LOCK
    // @seealso unlock
	class_addmethod(c, (method) score_lock, "lock", 0);


	// @method unlock @digest Unlock selected items
	// @description @copy BACH_DOC_MESSAGE_UNLOCK
    // @seealso lock
	class_addmethod(c, (method) score_unlock, "unlock", 0);


	// @method copy @digest Copy
	// @description Copies into the global clipboard selected musical content or slot data.
	// If no argument is given, the current selected measures are copied.
    // If a "durationline" symbol is given as argument, the duration line is copied.
    // If a "slot" symbol is given as argument,
	// the content of the open slot window (if any) is copied; if in addition to the "slot" symbol an integer
	// is given, such integer is the number of the slot whose content is copied (this will work even if no slot window is open);
    // finally, this integer can be substituted by the "all" symbol to copy the content of all slots.
	// @marg 0 @name slot @optional 1 @type symbol
	// @marg 1 @name slot_number @optional 1 @type int/symbol
    // @example copy @caption copy selected measures
    // @example copy slot 1 @caption copy first slot
    // @example copy slot active @caption copy currently open slot
    // @example copy slot all @caption copy all slots
    // @example copy durationline @caption copy the duration line
    // @seealso cut, paste
	class_addmethod(c, (method) score_copy, "copy", A_GIMME, 0);
	
	
	// @method cut @digest Cut
	// @description Cuts, and puts global roll clipboard, selected musical content or slot data.
	// The behavior and arguments of <m>cut</m> are exaclty as <m>copy</m>, with the addition that the copied 
	// content is subsequently deleted (see <m>copy</m> to know more).
	// @marg 0 @name slot @optional 1 @type symbol
	// @marg 1 @name slot_number @optional 1 @type int/symbol
    // @example cut @caption cut selected measures
    // @example cut slot 1 @caption cut first slot
    // @example cut slot active @caption cut currently open slot
    // @example cut slot all @caption cut all slot
    // @example cut durationline @caption cut the duration line
    // @seealso copy, paste
	class_addmethod(c, (method) score_cut, "cut", A_GIMME, 0);
	
	
	// @method paste @digest Paste
	// @description Pastes the content contained in the global clipboard. <br />
	// If the clipboard contains some measures and the first argument is the "replace" symbol, the measure are pasted over the selected ones.
    // If the clipboard contains some measures and the first argument is not the "replace" symbol, the copied measure are inserted, by default
    // at the end of the score. You can change this behavior by setting two optional arguments: an integer specifying the measure number for the
    // insertion (default: -1 = end), and an integer specifying the voice offset for the insertion (default: 1 = no offset, i.e. start
    // with first voice). <br />
    // If the clipboard contains duration line (breakpoints) content, and the "durationline" symbol is used as first argument,
    // the duration line is applied to all selected notes. <br />
	// If the clipboard contains slot content, and the "slot" symbol is used as argument,
    // the slot content is applied to the selected notes. If an integer argument is given, and a single slot
	// was copied, the slot content is applied only to the specified slot number. This number can be replaced with the "active" symbol, and the 
	// slot content will be applied to the open slot window.
    // @marg 0 @name replace @optional 1 @type symbol
	// @marg 1 @name slot @optional 1 @type symbol
	// @marg 2 @name slot_number @optional 1 @type int/symbol
    // @mattr tempi @type int @default 0 @digest If non-zero, also pastes tempi for copied measures
    // @mattr repeat @type int @default 1 @digest Number of times pasting must be performed
    // @example paste replace @caption paste measures over selected ones
    // @example paste @caption append copied measures at the end
    // @example paste @repeat 200 @caption append copied measures 200 times
    // @example paste 10 @caption insert copied measures at measure 10
    // @example paste 10 2 @caption insert copied measures at measure 10 starting at voice 2
    // @example paste slot @caption paste copied slot(s) to selection
    // @example paste slot all @caption the same
    // @example paste slot 3 @caption paste copied slot to slot 3 of selection
    // @example paste slot ampenv @caption paste copied slot to "ampenv" slot of selection
    // @example paste slot active @caption paste copied slot to currently open slot
    // @seealso cut, copy
    //	class_addmethod(c, (method) score_paste, "paste", A_GIMME, 0);  // THIS MUST BE COMMENTED!!!!!
                                                                        // Paste must be indeed handled inside the anything method,
                                                                        // otherwise it treads over the external editor paste method
	
	
	// @method delete @digest Delete current selected items
	// @description @copy BACH_DOC_MESSAGE_DELETE
    // If measures are selected, only their notes and chords are deleted, and measure are preserved.
    // Use <m>deletemeasures</m> if you need to delete whole measures. <br />
    // When deleting notes from a chord, use the "@transferslots" specification to define a set of slots to be transfered to a neighbour note in the same chord,
    // whenever possible. Use also "all" to transfer all slots, and "auto" to transfer a standard set of slots (dynamics, lyrics, articulations, annotations);
    // use "dynamics", "lyrics", "noteheads", "articulations", "annotation" instead of slot numbers.
    // Use the additional "@empty" message argument in order to tell with a 0/1 integer whether empty slots should also be transfered (by default: 0 = false).
    // @mattr transferslots @type llll @default null @digest If non-null, when deleting notes from a chord, these slots will be transfered to another chord note whenever possible
    // @mattr empty @type int @default 0 @digest Also transfer empty slots
    // @example delete @caption delete current selection
    // @example delete @transferslots 20 21 @caption delete current selection, and transfer content of slot 20 and 21 to another chord note, if possible
    // @example delete @transferslots all @caption delete current selection, and transfer all slots
    // @example delete @transferslots all @empty 1 @caption delete current selection, and transfer all slots, even the empty ones
    // @seealso eraseslot, deletemeasures
	class_addmethod(c, (method) score_sel_delete, "delete", A_GIMME, 0);

    
    // @method deletemeasures @digest Delete current selected measures
    // @description The <m>deletemeasures</m> message simply deletes all the selected measures.
    // Differently from the <m>delete</m> command, all the measures will be deleted, not just their content.
    // @seealso delete
    class_addmethod(c, (method) score_sel_deletemeasures, "deletemeasures", 0);


	// @method clearselection @digest Clear current selection
	// @description @copy BACH_DOC_MESSAGE_CLEARSELECTION
	// @marg 0 @name voice_number @optional 1 @type int
    // @example clearselection @caption make no elements selected
    // @example clearselection 3 @caption make no elements selected in voice 3
    // @seealso sel, unsel, select
	class_addmethod(c, (method) score_clear_selection, "clearselection", A_GIMME, 0);


	// @method snappitchtogrid @digest Snap selected notes' pitches to the current microtonal grid
	// @description @copy BACH_DOC_MESSAGE_SNAPPITCHTOGRID
    // @seealso respell, snaponsettogrid, snaptailtogrid
	class_addmethod(c, (method) score_sel_snap_pitch_to_grid, "snappitchtogrid", 0);
	
	
	// @method respell @digest Respell selected notes automatically
	// @description @copy BACH_DOC_MESSAGE_RESPELL
    // @seealso snappitchtogrid
	class_addmethod(c, (method) score_sel_resetgraphic, "respell", 0);
	
	
	// @method clearselectionarticulations @digest Clear articulations for selected items
	// @description The message <m>clearselectionarticulations</m> deletes all the articulations of all selected notes.
	// @seealso clear
    class_addmethod(c, (method) score_sel_resetarticulations, "clearselectionarticulations", 0);


	// @method sel @digest Select items
	// @description The word <m>sel</m> add some notation items to the current selection. In the basic behavior,
	// the word must be followed by four elements, specifying: <br />
	// - the starting temporal point of the selection, in milliseconds (leave <b>nil</b> or <b>()</b> if you want to select from the beginning); <br />
	// - the ending temporal point of the selection, in milliseconds (leave <b>nil</b> or <b>()</b> if you want to select until the end); <br />
	// - the minimum pitch of the selection, in cents (leave <b>nil</b> or <b>()</b> if you don't want to put a lower bound on pitches); <br />
	// - the maximum pitch of the selection, in cents (leave <b>nil</b> or <b>()</b> if you don't want to put an upper bound on pitches). <br />
	// Both the starting and the ending temporal point of the selection can be substituted by timepoints. <br />
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// Other selection modes are possible: <br />
	// - If the word <m>sel</m> is followed by the symbol <b>all</b>, all notes, chords and markers are selected. <br />
	// - If the word <m>sel</m> is followed by a category plural symbol, all the corresponding elements are selected.
    // Category plural symbols are: <b>markers</b>, <b>notes</b>, <b>chords</b>, <b>rests</b>,
    // <b>measures</b>, <b>breakpoints</b>, <b>tails</b>. <br />
	// - If the word <m>sel</m> is followed by the symbol <b>measure</b> followed by one or two integers (representing an address), a certain measure is selected.
	// The full syntax for the integers is: <m>voice_number</m> <m>measure_number</m>. If just one integer is given, the voice number is considered
	// to be by default 1. 
	// For instance, <b>sel measure 3</b> selects the third measure (of first voice), while <b>sel measure 2 3</b> does the same with the second voice. 
	// Negative positions are also allowed, counting backwards. Multiple measures can be selected at once, provided that instead of a list integers one gives
	// a sequence of wrapped lists of integers, for instance <b>sel measure (2 3) (2 4) (2 7) (5 1)</b>.<br />
	// - If the word <m>sel</m> is followed by the symbols <b>measure range</b>, a certain measure range is selected.
	// If a single argument (after the <b>measure range</b> symbols) is given, this is either a single number, representing the measure number to be selected, 
	// in all voices, or an llll in the form <b>(<m>start_meas_num</m> <m>end_meas_num</m>)</b>, representing the range of measures (from the one
	// whose number is <m>start_meas_num</m> to the one whose number is <m>end_meas_num</m>) to be selected, in all voices.
	// If a second argument is given, this is expected to be an llll containing the numbers of the voices for the selection 
	// (default being <b>()</b>, meaning: all voices). For instance <b>selmeasures (4 12) (1 2 5)</b> selects the range of measures
	// from 4 to 12 only in voices 1, 2 and 5. If a single number is given instead of the voices llll, the selection is performed only
	// for the voice having such number.
	// - If the word <m>sel</m> is followed by the symbol <b>chord</b> followed by one, two or three integers (representing an address), a certain chord is selected.
	// The full syntax for the integers is: <m>voice_number</m> <m>measure_number</m> <m>chord_index</m>. If less elements are given, the first ones are considered
	// to be by default 1's. The chord index is the counting index of the chord inside the measure.
	// For instance, <b>sel chord 2 3</b> selects the third chord of second measure (of first voice), while <b>sel chord 4 2 3</b> does the same with the fourth voice. 
	// Negative positions are also allowed, counting backwards. Multiple chords can be selected at once, provided that instead of a l ofist integers one gives
	// a sequence of wrapped lists of integers, for instance <b>sel chord (2 3) (4 1 2) (5 6 1)</b>.<br />
	// - If the word <m>sel</m> is followed by the symbol <b>note</b> followed by one, two, three or four integers (representing an address), a certain note is selected.
	// The full syntax for the integers is: <m>voice_number</m> <m>measure_number</m> <m>chord_index</m> <m>note_index</m>. If less elements are given, the first ones are considered
	// to be by default 1's. The chord index is the counting index of the chord inside the measure; the note index is taken from the lowest to the highest.
	// For instance, <b>sel note 4 2 3</b> selects the third note of second chord of fourth measure (of first voice), while <b>sel note 5 4 2 3</b> does the same with the fifth voice. 
	// Negative positions are also allowed, counting backwards. Multiple notes can be selected at once, provided that instead of a list integers one gives
	// a sequence of wrapped lists of integers, for instance <b>sel note (5 2 4 3) (1 1 1 -1)</b>.<br />
    // - If the word <m>sel</m> is followed by the symbols <b>note if</b>, <b>rest if</b>, <b>marker if</b>, <b>breakpoint if</b>
    // or <b>tail if</b> followed by a
    // condition to be verified, a conditional selection on notes, rests, markers or pitch breakpoints (respectively) is performed.
    // and notes/markers/pitch breakpoints matching such condition are selected.
    // The condition must be an expr-like expression returning 1 if notes have to be selected, 0 otherwise.
	// You can use symbolic variables inside such expressions. <br />
	// @copy BACH_DOC_SYMBOLIC_VARIABLES
	// For instance <b>sel note if velocity == 100</b> selects all notes whose
	// velocity is exactly equal to 100, while <b>round(cents / 100.) % 12 == 0</b> select all the C's.<br />
	// - If the word <m>sel</m> is followed by any other symbol or sequence of symbols, these are interpreted as names, and the notation items
	// matching all these names (or a single name, if just one symbol is entered) are selected. <br />
	// @marg 0 @name arguments @optional 0 @type llll
    // @example sel all @caption select everything
    // @example sel notes @caption select all notes
    // @example sel chords @caption select all chords
    // @example sel rests @caption select all rests
    // @example sel markers @caption select all markers
    // @example sel breakpoints @caption select all breakpoints
    // @example sel tails @caption select all tails
    // @example sel measures @caption select all measures
    // @example sel chord 3 2 @caption select 3rd chord of 2nd measure (of 1st voice)
    // @example sel chord 2 5 4 @caption select 4th chord of 5th measure 2nd voice
    // @example sel chord -1 -2 -1 @caption select last chord of one-but-last measure of last voice
    // @example sel note 3 4 1 -1 @caption select last note of 1st chord of 4th measure of 3rd voice
    // @example sel measure 2 @caption select measure 2 in 1st voice
    // @example sel measure 2 3 @caption select measure 2 in 3rd voice
    // @example sel measure 2 -1 @caption select measure 2 in last voice
    // @example sel measure range 2 @caption select measure 2 in all voices
    // @example sel measure range (2) @caption the same
    // @example sel measure range (2 4) @caption select measures from 2 to 4 (included) in all voices
    // @example sel measure range (2 4) (1 4) @caption the same, but only from voices 1 through 4
    // @example sel measure range (2 4) 1 @caption the same, but only for voice 1
    // @example sel measure range (2 4) (1) @caption the same
    // @example sel marker 5 @caption select 5th marker
    // @example sel marker -2 @caption select one-but-last marker
    // @example sel marker (1) (-2) (5) @caption select multiple markers
    // @example sel chords (1 3 1) (2 2 2) (-2 5 4) @caption select multiple chord
    // @example sel notes (1 3 2 4) (1 3 3 2) (2 4 -5 2) @caption select multiple notes
    // @example sel John @caption select all items named 'John'
    // @example sel John Lennon @caption select all items named both 'John' and 'Lennon'
    // @example sel 1000 3000 6000 7200 @caption select notes between 1s and 3s, and between 6000 and 7200cents
    // @example sel 1000 3000 6000 7200 2 @caption same, for second voice only
    // @example sel 1000 3000 6000 7200 (1 3 4) @caption same, for voices 1, 3 and 4
    // @example sel 1000 3000 () () @caption select notes between 1s and 3s
    // @example sel () () 4800 6000 @caption select notes below middle C
    // @example sel () () () () -1 @caption select every note in last voice
    // @example sel (3) (9) 6000 7200 @caption select notes in central octave from measure 3 to measure 9 (excluded)
    // @example sel (3 1/4) (9.5) 6000 7200 @caption the same, from measure 3 after 1/4 to half of measure 9
    // @example sel (2 1/4 3) (1 5 1/4) 6000 7200 @caption play from measure 3 after 1/4 in 2nd voice to measure 5 after 1/4 in 1st voice
    // @example sel (2 1/4 3) (1 5 1/4) 6000 7200 4 @caption the same, but only select notes in voice 4
    // @example sel () (2 2 3/8) () () 2 @caption select all notes before measure 3/8 after beginning of measure 2, voice 2 only
    // @example sel note if cents == 6000 @caption select all middle C's
    // @example sel note if (cents % 1200) == 0 @caption select all C's
    // @example sel note if (cents < 6000) && (duration < 1000) @caption select all notes <1s below middle C
    // @example sel note if symduration > 1/4 @caption select all notes longer than 1/4
    // @example sel note if (symonset > 1/4) && (tie == 0) @caption select all notes whose onset in their measure is greater than 1/4 and have no starting or ending tie
    // @example sel note if tie == 1 @caption select notes that start a tie but do not end one
    // @example sel note if tie == 2 @caption select notes that end a tie but do not start one
    // @example sel note if tie == 3 @caption select notes that both start and end a tie
    // @example sel note if tie == 0 @caption select notes with no ties
    // @example sel note if (measure == 4) && (velocity > 100)  @caption select notes in measure 4 with velocity > 100
    // @example sel rest if symduration <= 1/8 @caption select all rests shorter than 1/8
    // @example sel marker if onset > 5000 @caption select all markers with onset >5s
    // @example sel breakpoint if (cents > 7200) && (velocity > 100) @caption select all pitch breakpoints >7200cents with velocity >100
    // @example sel tail if cents > 6000 @caption select all tails above middle C
    // @seealso unsel, select, subsel, clearselection
	class_addmethod(c, (method) score_select, "sel", A_GIMME, 0);


	// @method unsel @digest Deselect items
	// @description The message <m>unsel</m> works in the opposite way of the message <m>sel</m>, i.e. deselect
	// notation items (removes them from the current selection). Refer to the message <m>sel</m> to know more about the
	// syntax, since it is exactly the same.
	// @marg 0 @name arguments @optional 0 @type llll
    // @seealso sel, select, subsel, clearselection
	class_addmethod(c, (method) score_select, "unsel", A_GIMME, 0);


	// @method unsel @digest Involutively select items
	// @description The message <m>select</m> works as <m>sel</m> but in an involutive manner: i.e. it selects non-selected items
	// and deselect already selected items. This is essentially what happens when you draw a 
	// selection rectangle on the notation object, and then draw another overlapping one keeping the Shift key pressed.
	// Refer to the message <m>sel</m> to know more about the syntax, since it is exactly the same.
	// The only exception is for the <m>select all</m> message, which does not work involutively and always select all the notation items.
	// @marg 0 @name arguments @optional 0 @type llll
    // @seealso sel, unsel, subsel, clearselection
	class_addmethod(c, (method) score_select, "select", A_GIMME, 0);


    
    // @method subsel @digest Restrict selection
    // @description The message <m>subsel</m> works as the <m>sel</m> message, but restricting the existing selection.
    // More specifically, elements are only kept as selected if they verify the specified property.
    // In other words, the <m>subsel</m> message create an intersection between the specified selection and the existing one.
    // Refer to the message <m>sel</m> to know more about the syntax, since it is exactly the same.
    // @marg 0 @name arguments @optional 0 @type llll
    // @seealso sel, unsel, select, clearselection
    class_addmethod(c, (method) score_select, "subsel", A_GIMME, 0);
    
    
    // @method goto @digest Move selection
    // @description The <m>goto</m> message moves the selection to a new item, i.e. clears the selection and select the new item.
    // The first argument is the movement type: <br />
    // <b>prev</b>: select previous notation item, if any; <br />
    // <b>next</b>: select next notation item, if any. <br />
    // @mattr skiprests @type int @default 0 @digest If non-zero, skip rests when selecting previous chords
    // @example sel next @caption select next notation item
    // @example sel prev @caption select previous notation item
    // @example sel next @skiprests 1 @caption select next item, skip rests
    // @seealso sel, select, unsel
    class_addmethod(c, (method) score_anything, "goto", A_GIMME, 0);
    
    
    
	// (deprecated, undocumented: use "sel measure range" instead)
	class_addmethod(c, (method) score_selectmeasures, "selmeasures", A_GIMME, 0);
	class_addmethod(c, (method) score_selectmeasures, "unselmeasures", A_GIMME, 0);
	class_addmethod(c, (method) score_selectmeasures, "selectmeasures", A_GIMME, 0);


	// @method name @digest Assign name(s) to selected items
	// @description @copy BACH_DOC_MESSAGE_NAME
	// @marg 0 @name names @optional 0 @type llll
    // @mattr incremental @type int @default 0 @digest If non-zero, assigns incremental numbering to selected markers in addition to names
    // @mattr progeny @type int @default 0 @digest If non-zero, assigns names to both selected chords and all their notes
    // @example name George @caption name selected elements as 'George'
    // @example name (George Martin) (George Harrison) @caption assign complex llll name
    // @example name George @incremental 1 @caption also add unique incremental numbers to selected markers
    // @example name George @progeny 1 @caption if chords are selected also assign name to their notes
    // @seealso nameappend, clearnames, sel, unsel, select
	class_addmethod(c, (method) score_name, "name", A_GIMME, 0);

	
	// @method name @digest Append name(s) to selected items
	// @description Works like the <m>name</m> message, but appends the given names
	// to the already existing ones (see <m>name</m>).
	// @marg 0 @name names @optional 0 @type llll
    // @example name Ringo @caption append 'Ringo' to selected element's names
    // @example name (Ringo Starr) ((Low High Snare) (Sizzle Crash)) @caption append complex llll name
    // @seealso name, clearnames
	class_addmethod(c, (method) score_nameappend, "nameappend", A_GIMME, 0);

	
	// @method clearnames @digest Clear names of selected items
	// @description Clears all the names of all selected items.
    // If you want to only clear names for some category, add the category name(s) after the <m>clearnames</m> symbol. Names must be among the following ones:
    // "chords", "measures", "voices", "notes", "markers".
    // @marg 0 @name categories @optional 1 @type llll
    // @example clearnames @caption delete all names
    // @example clearnames marker @caption only delete marker names
    // @example clearnames measure chord note @caption only delete measures, chord and note names
    // @seealso name, nameappend
	class_addmethod(c, (method) score_clearnames, "clearnames", A_GIMME, 0);


	
    // @method slottoname @digest Assign name(s) to selected items from slot content
    // @description A <m>slottoname</m> message followed by an integer <m>N</m> overwrites the names of selected
    // notes and rests with the content of their slot <m>N</m>. Slot <m>N</m> is expected to be a text or llll slot.
    // N.B.: While setting notes' names, the message also clears the corresponding chords' names.
    // @marg 0 @name slot_number_or_name @optional 0 @type int/symbol
    // @seealso name, nameappend, clearnames, sel, unsel, select
    class_addmethod(c, (method) score_slottoname, "slottoname", A_GIMME, 0);
    
    
    // @method nametoslot @digest Copy name(s) to slot for selected items
    // @description A <m>nametoslot</m> message followed by an integer <m>N</m> copies the names of the selected
    // notes inside their slot <m>N</m>. <br />
    // A second argument sets the policy in case the chord containing the note also has a name. This argument
    // must be one of the following symbols: <br />
    // - "ignore" (default): chord name is ignored; <br />
    // - "replace": chord name is always used instead of note names; <br />
    // - "replacenull": chord name is used only for notes having no name (i.e. <b>null</b> name); <br />
    // - "prepend": chord name is prepended to existing note names; <br />
    // - "append": chord name is appended to existing note names; <br />
    // - "merge" or "mergeprepend" (default): only chord names which don't already belong to the note are prepended to existing note names; <br />
    // - "mergeappend": only chord names which don't already belong to note are appendend to existing note names.
    // @marg 0 @name slot_number_or_name @optional 0 @type int/symbol
    // @marg 1 @name policy @optional 1 @type symbol
    // @seealso name, nameappend, clearnames, sel, unsel, select
    class_addmethod(c, (method) score_nametoslot, "nametoslot", A_GIMME, 0);

    
    
	// @method autorhythm @digest Reparse automatically the rhythm of the selected portion of score
	// @description The <m>autorhythm</m> message retranscribes and rebeams the rhythm of the selected portion
	// of score. By default the reparsing and rebeaming will keep untouched the tuplet rhythmic levels. If you
	// want also an automatic complete reparsing of the tuplets, add a non-zero integer argument, for instance
	// <b>autorhythm 1</b>. (This message will overwrite the current value of the attribute <m>autoparserhythm</m>
    // by forcing the parsing to be always performed, even if this attribute is unset.)
	// @marg 0 @name reparse_tuplets @optional 1 @type int
    // @example autorhythm @caption reparse rhythm, but not tuplets
    // @example autorhythm 1 @caption reparse rhythm and tuplets
    // @seealso autobeam
	class_addmethod(c, (method) score_sel_reparse, "autorhythm", A_GIMME, 0);
    class_addmethod(c, (method) score_sel_reparse, "rebeam", A_GIMME, 0); // old name, kept for bw compatibility

    
    // @method autobeam @digest Rebeam automatically the rhythm of the selected portion of score
    // @description The <m>autobeam</m> message applies an automatic beaming to the selected portion
    // of score. (Differently from <m>autorhythm</m> message, this message will not overwrite the current value of the attribute <m>autoparserhythm</m>.)
    // @seealso autorhythm
    class_addmethod(c, (method) score_sel_rebeam, "autobeam", A_GIMME, 0);


	// @method onset @digest Modify the onset of selected markers
	// @description The word <m>onset</m>, followed by a number, sets the new onset, in milliseconds, for all 
	// the selected markers or pitch breakpoints. This absolute onset can be replaced by an llll containing a relative modification of the
	// existing onset, or by a generic equation.
	// @copy BACH_DOC_RELATIVE_MODIFICATIONS
	// @copy BACH_DOC_EQUATION_MODIFICATIONS 
	// @marg 0 @name onset @optional 0 @type number/llll/anything
    // @example onset 1000 @caption move selected markers to 1s
    // @example onset = 1000 @caption exactly the same
    // @example onset = onset + 1000 @caption shift selected markers by 1s forward
    // @example onset = (index - 1) * 1000 @caption space all markers by 1s
    // @example onset = "(pow(1.4, index) - 1)*1000" @caption the same, in rallentando
	class_addmethod(c, (method) score_sel_change_onset, "onset", A_GIMME, 0);
	
	
	// @method velocity @digest Modify the velocity of selected items
	// @description @copy BACH_DOC_MESSAGE_VELOCITY
	// @marg 0 @name velocity @optional 0 @type number/llll/anything
    // @example velocity 120 @caption set selected items' velocity to 120
    // @example velocity = 120 @caption exactly the same
    // @example velocity = velocity * 1.2 @caption increase velocity
    // @example velocity = symduration*127 @caption assign velocity depending on symbolic duration
    // @example velocity = "random(40, 121)" @caption assign velocity randomly
	class_addmethod(c, (method) score_sel_change_velocity, "velocity", A_GIMME, 0);

    
    // @method tie @digest Modify the ties of selected items
    // @description @copy BACH_DOC_MESSAGE_TIE
    // @marg 0 @name tie @optional 0 @type number/llll/anything
    // @example tie 0 @caption remove all ties for selection
    // @example tie = 0 @caption exactly the same
    // @example tie = 1 @caption selected notes will start ties (if possible)
    // @example tie = 2 @caption selected notes will end ties (if possible)
    // @example tie = 3 @caption selected notes will both start and end ties (if possible)
    // @example tie = 3 - tie @caption flip ties for selection
    class_addmethod(c, (method) score_sel_change_tie, "tie", A_GIMME, 0);


	// @method cents @digest Modify the pitch of selected items
	// @description @copy BACH_DOC_MESSAGE_CENTS
	// @marg 0 @name velocity @optional 0 @type number/llll/anything
    // @example cents 6000 @caption change selected notes to middle C's
    // @example cents = 6000 @caption exactly the same
    // @example cents = cents * 1.2 @caption stretch pitches
    // @example cents = "random(48, 73)*100" @caption assign pitch randomly
    // @example cents = 6000 + (cents % 1200) @caption collapse to middle octave
	class_addmethod(c, (method) score_sel_change_cents, "cents", A_GIMME, 0);

    
    // @method pitch @digest Modify the pitch of selected items
    // @description @copy BACH_DOC_MESSAGE_PITCH
    // @marg 0 @name pitch @optional 0 @type number/llll/anything
    // @example pitch C5 @caption change selected notes to middle C's
    // @example pitch = C5 @caption exactly the same
    // @example pitch = pitch + D0 @caption transpose by major second
    // @example pitch = pitch + Eb0 @caption transpose by minor third
    // @example pitch = pitch - C1 @caption transpose one octave down
    // @example pitch = (pitch % C1) + C5 @caption collapse to middle octave
    class_addmethod(c, (method) score_sel_change_pitch, "pitch", A_GIMME, 0);
    
    
    // @method cents @digest Modify the measureinfo of selected measures
    // @description Modifies the measureinfo of select measures. The expected syntax is the standard measureinfo syntax,
    // without the outermost level of parentheses (e.g. <b>measureinfo (4 4) (3/8 120)</b> is a valid message). <br />
    // @copy BACH_DOC_MEASUREINFO_SYNTAX
    // @marg 0 @name measureinfo @optional 0 @type llll
    // @example measureinfo (5 4) @caption change selected measures' time signature to 5/4
    // @example measureinfo (5 4) (1/4 120) @caption the same, but also add quarter = 120 tempo
    // @example measureinfo (5 4) ((1/4 120) (1/4 160 1/2)) @caption the same, but also define a quarter = 160 tempo after 1/2 from the measure beginning
    // @example measureinfo (5 4) ((1/4 120 0 1) (1/4 160 1/2)) @caption the same, with accelerando
    // @example measureinfo ((3 2 2) 8) @caption change time signature to 3+2+2/8
    // @example measureinfo ((3 2 2) 8) ((1/4 120) (1/4 180 1/2)) @caption the same, but also add two tempi inside the measure
    // @example measureinfo (11 8) (boxes 3/8 2/8 3/8 4/8) @caption change time signature to 11/8 and assign custom 3+2+3+4 boxes
    class_addmethod(c, (method) score_sel_change_measureinfo, "measureinfo", A_GIMME, 0);


	// @method addbreakpoint @digest Add a pitch breakpoint to each selected note
	// @description @copy BACH_DOC_MESSAGE_ADDBREAKPOINT
	// @marg 0 @name relative_x_position @optional 0 @type float
	// @marg 1 @name delta_midicents @optional 0 @type float
	// @marg 2 @name slope @optional 1 @type float
	// @marg 3 @name velocity @optional 1 @type int
    // @example addbreakpoint 0.5 200 @caption add a breakpoint at the middle of the note of +200cents
    // @example addbreakpoint 0.5 200 0.2 @caption the same, with slope 0.2
    // @example addbreakpoint 0.5 200 0.2 80 @caption the same, with velocity 80
    // @see erasebreakpoints
	class_addmethod(c, (method) score_sel_add_breakpoint, "addbreakpoint", A_GIMME, 0);


	// @method erasebreakpoints @digest Delete pitch breakpoints of selected notes
	// @description @copy BACH_DOC_MESSAGE_ERASEBREAKPOINTS
    // @see addbreakpoint
	class_addmethod(c, (method) score_sel_erase_breakpoints, "erasebreakpoints", A_GIMME, 0);


	// @method addslot @digest Set the content of one or more slots for selected notes
	// @description @copy BACH_DOC_MESSAGE_ADDSLOT
	// @marg 0 @name slots @optional 0 @type llll
    // @example addslot (6 0.512) @caption fill (float) slot 6 with number 0.512
    // @example addslot (5 42) @caption fill (int) slot 5 with number 42
    // @example addslot (7 "Lorem Ipsum" ) @caption fill (text) slot 7 with some text
    // @example addslot (10 (John George (Ringo) (Brian)) ) @caption fill (llll) slot 10 with an llll
    // @example addslot (3 10 20 30) @caption fill (intlist) slot 3 of selected notes with list of values 10, 20, 30
    // @example addslot (2 (0 0 0) (0.5 0 1) (1 1 0.2) @caption fill (function) slot 2 with a breakpoint function in (x y slope) form
    // @example addslot (amplienv (0 0 0) (0.5 0 1) (1 1 0.2)) @caption the same for slot named 'amplienv'
    // @example addslot (active (0 0 0) (0.5 0 1) (1 1 0.2)) @caption the same for currently open slot
    // @example addslot (3 10 20 30) (2 (0 0 0) (0.5 0 1) (1 1 0.2)) @caption set more slots at once
    // @seealso changeslotvalue, eraseslot, filltiesequenceswithslots
	class_addmethod(c, (method) score_sel_add_slot, "addslot", A_GIMME, 0);


	// @method eraseslot @digest Clear a specific slot for selected notes
	// @description @copy BACH_DOC_MESSAGE_ERASESLOT 
	// @marg 0 @name slot_number_or_name @optional 0 @type int/symbol
    // @example eraseslot active @caption clear currently open slot for selected notes
    // @example eraseslot 4 @caption clear 4th slot
    // @example eraseslot amplienv @caption clear slot named amplienv
    // @seealso addslot, changeslotvalue
	class_addmethod(c, (method) score_sel_erase_slot, "eraseslot", A_GIMME, 0);

    
    // @method moveslot @digest Move a slot to another one for selected items
    // @description @copy BACH_DOC_MESSAGE_MOVESLOT
    // @marg 0 @name slot_from @optional 0 @type int/symbol
    // @marg 1 @name slot_to @optional 0 @type int/symbol
    // @example moveslot 2 7 @caption move the content of slot 2 to slot 7 for selected items
    // @example moveslot 2 active @caption destination slot is the active slot
    // @example copyslot amplienv myfunction @caption copy the slot named amplienv to the slot named myfunction
    // @seealso copyslot, eraseslot, addslot, changeslotvalue, resetslotinfo
    class_addmethod(c, (method) score_sel_move_slot, "moveslot", A_GIMME, 0);
    
    
    // @method copyslot @digest Copy a slot to another one for selected items
    // @description @copy BACH_DOC_MESSAGE_COPYSLOT
    // @marg 0 @name slot_from @optional 0 @type int/symbol
    // @marg 1 @name slot_to @optional 0 @type int/symbol
    // @example copyslot 2 7 @caption copy the content of slot 2 to slot 7 for selected items
    // @example copyslot 2 active @caption destination slot is the active slot
    // @example copyslot amplienv myfunction @caption copy the 'amplienv' slot to the 'myfunction' slot
    // @seealso moveslot, eraseslot, addslot, changeslotvalue, resetslotinfo
    class_addmethod(c, (method) score_sel_copy_slot, "copyslot", A_GIMME, 0);
    

	// @method changeslotvalue @digest Change a specific value inside a slot for selected notes
	// @description @copy BACH_DOC_MESSAGE_CHANGESLOTVALUE
	// @marg 0 @name slot_number_or_name @optional 0 @type int/symbol
	// @marg 1 @name element_index @optional 0 @type int
	// @marg 2 @name slot_element @optional 0 @type llll	
    // @example changeslotvalue 3 2 13 @caption set the 2nd element of 3nd (int or float) slot to 13
    // @example changeslotvalue 3 0 13 @caption append 13 at the end of slot 3
    // @example changeslotvalue 1 2 0.5 10 0 @caption change the 2nd point of the 1st (function) slot to (0.5 10 0) in (x y slope) form
    // @example changeslotvalue active 2 0.5 10 0 @caption the same, for the currently open slot
    // @example changeslotvalue spectrenv 2 0.5 10 0 @caption the same, for the a slot named 'spectrenv'
    // @example changeslotvalue 9 1 highpass 400 0 2 @caption set the 1st element of 9nd (dynfilter) slot to "highpass 400 0 2"
    // @example changeslotvalue 8 0 Max.app 0 @caption append the Max.app file in the 8th (filelist) slot, and make it active
    // @example changeslotvalue 8 0 0 2 @caption Make 2nd file active in 8th (filelist) slot
    // @seealso addslot, eraseslot
    class_addmethod(c, (method) score_sel_change_slot_value, "changeslotvalue", A_GIMME, 0);


	// @method dumpselection @digest Play selected items off-line
	// @description The <m>dumpselection</m> message sends the content of each one of selected notation items from the 
	// playout, in playout syntax (off-line play).
    // You can safely rely on the fact that elements will be output ordered by onset. <br />
    // If a "router" message attribute is set, then the standard router ("note", "chord", "rest") is replaced by the specified one;
    // if the "router" attribute has length 3, the first symbol will be used for notes, the second one for chords, the third for rests. <br />
    // @copy BACH_DOC_PLAYOUT_SYNTAX_SCORE
    // @mattr router @type llll @default null @digest Sets a forced router to be used instead of the default one
    // @seealso sendcommand, play
	class_addmethod(c, (method) score_sel_dumpselection, "dumpselection", A_GIMME, 0);


	// @method sendcommand @digest Send a command to selected items
	// @description The <m>sendcommand</m> message sends a given command to the selected notation items,
	// whose content is then immediately output from the playout in playout syntax (see <m>dumpselection</m>)
	// but with a router different from the standard playout syntax router, i.e. the router defined by the given command.
	// It expects as argument the number of the command to be sent (1 to 5).
    // You can safely rely on the fact that elements will be output ordered by onset.
	// @copy BACH_DOC_COMMANDS_GENERAL
	// @copy BACH_DOC_PLAYOUT_SYNTAX_SCORE
	// @marg 0 @name command_number @optional 0 @type int
    // @example sendcommand 3 @caption send 3rd command
    // @seealso dumpselection
	class_addmethod(c, (method) score_sel_sendcommand, "sendcommand", A_GIMME, 0);
	
	
	// @method inscreen @digest Change scrollbar position to display a temporal point
	// @description The message <m>inscreen</m> followed by a temporal position specified in milliseconds forces such
	// position to be displayed inside the domain, by changing the scrollbar position.
	// The millisecond position can be replaced by a timepoint, or by the element name, or
    // by one of the following symbols: "cursor", "end". <br />
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// @marg 0 @name position_or_name @optional 0 @type llll
    // @example inscreen 5000 @caption display position at 5s in domain
    // @example inscreen john @caption display element named 'john' in domain
    // @example inscreen cursor @caption display playhead in domain
    // @example inscreen end @caption display end of score in domain
    // @example inscreen (4) @caption display measure 4 in domain
    // @example inscreen (4.5) @caption display position at half of measure 4 in domain
    // @example inscreen (4 1/4) @caption display position in measure 4 after 1/4 (of first voice) in domain
    // @example inscreen (3 4 1/4) @caption display position of 1/4 after beginning of measure 4 in 3rd voice in domain
    // @seealso inscreenpos, inscreenmeas
	class_addmethod(c, (method) score_inscreen, "inscreen", A_GIMME, 0);


	// @method inscreenpos @digest Change scrollbar position to precisely display a temporal point 
	// @description The message <m>inscreenpos</m> sets a given position in milliseconds (or a given timepoint) in a given point inside the notation object domain. 
	// It has to be followed by two argument: a value between 0 and 1, specifying the relative location inside the screen in which the temporal
	// point should be displayed (0 represents the screen beginning, 1 the screen end), and the position in milliseconds (as a single number), or the timepoint
	// (as an llll), or by the element name, or by one of the following symbols: "cursor", "end". <br />
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// @marg 0 @name domain_relative_location @optional 0 @type float
	// @marg 1 @name position_or_name @optional 0 @type llll
    // @example inscreenpos 0.5 5000 @caption display 5s position at the exact domain center
    // @example inscreenpos 0.9 john @caption display element named 'john' at 90% of the domain
    // @example inscreenpos 0.9 cursor @caption display playhead at 90% of the domain
    // @example inscreenpos 0.9 end @caption display end of score at 90% of the domain
    // @example inscreenpos 0.2 (4) @caption display beginning of measure 4 at 20% of the domain
    // @example inscreenpos 0.2 (4.5) @caption the same, for the position at half of measure 4
    // @example inscreenpos 0.2 (4 1/4) @caption the same, for the position in measure 4 after 1/4 (of first voice) in domain
    // @example inscreenpos 0.2 (3 4 1/4) @caption the same, for the position of 1/4 after beginning of measure 4 in 3rd voice in domain
    // @seealso inscreen, inscreenmeas
	class_addmethod(c, (method) score_inscreenpos, "inscreenpos", A_GIMME, 0);
	
	
	// @method inscreenmeas @digest Display measure range to fit the domain
	// @description The message <m>inscreenmeas</m> change the domain so that it exactly fits a given range of measures. To do so, not only the position 
	// of the scrollbar is changed, but also the width factors of the involved measures are precisely modified, so to fit precisely inside the screen.
	// The two arguments are the starting and ending numbers of the measures to be displayed in the screen. 
	// If a third argument is given, this is the voice number to which the measure range refers (by default: first voice).
	// For instance, <b>inscreenmeas 4 7 3</b> shows exactly in the screen measures 4 to 7 of voice 3.
	// @marg 0 @name start_measure_number @optional 0 @type int
	// @marg 1 @name end_measure_number @optional 0 @type int
	// @marg 2 @name voice_number @optional 1 @type int
    // @example inscreenmeas 5 10 @caption set domain to display measures 5 through 10
    // @example inscreenmeas 5 10 4 @caption the same, for voice 4
    // @example inscreenmeas 5 10 4 @caption the same, for voice 4
    // @seealso domain, inscreen, inscreenpos, resetwidthfactors
	class_addmethod(c, (method) score_inscreenmeas, "inscreenmeas", A_GIMME, 0);


	// @method split @digest Split selected chords
	// @description The <m>split</m> message splits each selected chord into a number of pieces given as argument.
	// By default, sequences of completely tied chords are split as a whole (e.g. splitting a 1/4 chord tied to an 1/8 chord into 3 pieces, will yield 
	// three 1/8 chords); you can individually split each one of the chords instead by adding the "separate" symbol as second argument.
	// @marg 0 @name num_parts @optional 0 @type int
	// @marg 1 @name separate @optional 1 @type symbol
    // @example split 3 @caption split selected chords into 3 pieces each
    // @example split 3 separate @caption the same, separately splitting each chord in a tied sequence (if any)
    // @seealso merge, join
	class_addmethod(c, (method) score_split, "split", A_GIMME, 0);
	
	// @method join @digest Join selected chords
	// @description The <m>join</m> message joins the selected chords when possible, by creating ties between them and by rhythmically re-writing the musical content.
    // @seealso split, merge
	class_addmethod(c, (method) score_join, "join", 0);	

    // @method deletegrace @digest Delete selected chords' grace notes
    // @description The <m>deletegrace</m> message deletes all the grace chords of all currently selected chords
    // @seealso mergegrace
    class_addmethod(c, (method) score_deletegrace, "deletegrace", 0);

    // @method mergegrace @digest Merge selected chords' grace notes
    // @description The <m>mergegrace</m> message merges all the grace notes into the main chords, for all currently selected chords
    // @seealso deletegrace
    class_addmethod(c, (method) score_mergegrace, "mergegrace", 0);

	// @method legato @digest Make selection legato
	// @description The <m>legato</m> message removes any rest inside the selected items, by extending the duration of the chords right before the rests.
    // @seealso glissando
	class_addmethod(c, (method) score_legato, "legato", 0);

    // @method glissando @digest Make selection glissando
    // @description The <m>glissando</m> message changes each note tail so that it matches the following note's pitch - in other words, turns the written musical
    // line into a continuous glissando. If a note is followed by a rest, nothing will happen.
    // You can set as optional argument the glissando slope, as floating point value, from -1. to 1. (0. being linear, default).
    // @marg 0 @name slope @optional 1 @type float
    // @example glissando 0.5 @caption make glissandos with slope = 0.5
    // @seealso legato
    class_addmethod(c, (method) score_glissando, "glissando", A_GIMME, 0);


	// @method resetwidthfactors @digest Reset measure width factors
	// @description The <m>resetwidthfactors</m> will revert all measure width factors to 1. 
	// Measure width factors are local width factors defined inside the measureinfo (see below) which alters the spacing width only for a given measure
	// (To be more precise: they alter measure width for all the region of measures, throughout all voices, where the given measure is, and extended 
	// at left and at right until a synchronous barline is found in all voices. In standard cases, where voices have the same tempi and time signatures, 
	// this correspond to the vertical range of measures having the same number, in all voices.)
	// A value of 1 corresponds to standard spacing, any lower values correspond
	// to a compression, any higher values correspond to an expansion. The <m>resetwidthfactors</m> message revert such values to the 1 (default). 
	// If no arguments are given, width factors are reset for the entire score.
	// If two arguments are given, they are expected to be integers, representing the measure numbers of the leftmost and rightmost
	// measure whose width factors should be reset (in any voice). 
	// If just one argument is given, it is expected this represents the leftmost measure number, and the rightmost boundary is considered to be
	// the end of the score. <br /> 
	// @copy BACH_DOC_MEASUREINFO_SYNTAX
	// @marg 0 @name start_measure_number @optional 1 @type int
	// @marg 1 @name end_measure_number @optional 1 @type int
    // @example resetwidthfactors @caption reset all width factors to 1
    // @example resetwidthfactors 10 @caption reset width factors for measures from 10 to end
    // @example resetwidthfactors 5 10 @caption reset width factors for measures from 5 through 10
    // @seealso inscreenmeas
	class_addmethod(c, (method) score_resetlocalwidthmultiplformeas, "resetwidthfactors", A_GIMME, 0);
	
	
	// @method setmeasureparameters @digest Set measure parameters
	// @description The <m>setmeasureparameters</m> will set specified measure parameters for a given measure.
	// The measure number is given as first argument: if it is a single integer, measure parameters are applied to all measure (throughout all voices)
	// having such measure number; if it is a wrapped list in the form <b>(<m>voice_number</m> <m>measure_number</m>)</b>, measure parameters are only applied
	// to the measure identified by the <m>measure_number</m> and for the voice identified by the <m>voice_number</m>.
	// Any further argument is an <m>SPECIFICATION</m> llll, specifying the measure parameter to be changed, yielding a complete syntax of
	// <b>setmeasureparameters <m>MEASURE</m> <m>SPECIFICATION1</m> <m>SPECIFICATION2</m>...</b>. <br />
	// @copy BACH_DOC_MEASUREINFO_SYNTAX_SINGLE_SPECIFICATION	
	// @marg 0 @name measure @optional 0 @type llll
    // @example setmeasureparameters 3 (width 400) (barline d) @caption set measure 3 to have width 400 pixel and dashed barline
    // @example setmeasureparameters (2 3) (width 400) (barline d) @caption the same, only for measure 3 of voice 2
    // @example setmeasureparameters 5 (shownumber -2) (barline h) @caption set measure 5 to have show measure number -2 and have hidden barline
    // @example setmeasureparameters 2 (shownumber off) @caption set measure 2 not to show any number
    // @seealso llll
	class_addmethod(c, (method) score_setmeasureparameters, "setmeasureparameters", A_GIMME, 0);


	// @method lambda @digest Perform operation on individual selected items 
	// @description @copy BACH_DOC_MESSAGE_LAMBDA
	// @marg 0 @name modification_message @optional 0 @type llll
    // @example lamdba cents $1 @caption assign the incoming value as note cents
    // @example lamdba changeslotvalue $1 $2 $3 @caption the same, for some slot value
    // @seealso cents, velocity, changeslotvalue, addslot, eraseslot, name
	class_addmethod(c, (method) score_lambda, "lambda", A_GIMME, 0);


	// @method setcursor @digest Move the playhead cursor 
	// @description The <m>setcursor</m> message moves the playhead cursor to a specific position, given as argument in milliseconds (if a single number is entered) 
	// or as a timepoint (if an llll is entered). <br />
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// The <m>setcursor</m> message can also directly move the playhead cursor to the position where a marker is. In this case, the syntax is
	// <b>setcursor <m>MARKER_NAME_OR_NAMES</m></b>, where <m>MARKER_NAME_OR_NAMES</m> is a single name or plain list of names. 
	// The playhead cursor is moved to the first marker matching all the names (if any).
	// @marg 0 @name position @optional 0 @type llll
    // @example setcursor 2000 @caption set the playhead at 2s
    // @example setcursor (4) @caption set the playhead at beginning of measure 4
    // @example setcursor (4.5) @caption set the playhead at half of measure 4
    // @example setcursor (4 1/4) @caption set the playhead after 1/4 of measure 4
    // @example setcursor (2 4 1/4) @caption set the playhead after 1/4 of measure 4, in second voice
    // @example setcursor John @caption set the cursor at the onset of the first item named 'John'
    // @example setcursor John Lennon @caption set the cursor at the onset of the first item named 'John' and 'Lennon'
    // @seealso getcursor, showcursor, hidecursor
	class_addmethod(c, (method) score_setcursor, "setcursor", A_GIMME, 0);


	// @method getcursor @digest Get current playhead cursor position 
	// @description The <m>getcursor</m> message retrieves the current playhead cursor position, and outputs it from the playout in the form
	// <b>cursor <m>cursor_position_ms</m></b>, where the last parameter is the current position of the playhead cursor in milliseconds. 
    // @seealso setcursor, showcursor, hidecursor
	class_addmethod(c, (method) score_getcursor, "getcursor", 0);
	

	// @method hidecursor @digest Hide the playhead cursor 
	// @description @copy BACH_DOC_MESSAGE_HIDECURSOR
    // @seealso showcursor, getcursor, setcursor
	class_addmethod(c, (method) score_hidecursor, "hidecursor", 0);


	// @method showcursor @digest Show the playhead cursor 
	// @description @copy BACH_DOC_MESSAGE_SHOWCURSOR
    // @seealso hidecursor, getcursor, setcursor
	class_addmethod(c, (method) score_showcursor, "showcursor", 0);


	// @method addmarker @digest Add a marker 
	// @description The <m>addmarker</m> adds a marker with a specified name at a specified position in the score.
	// At least two arguments are needed: the position of the marker and the marker name, as symbol.
	// The position of the marker is the first argument, and can be either a single floating point number, in which case it is taken as
	// the marker onset, in milliseconds, and the marker is assigned as attached to absolute millisecond position, or a timepoint llll, 
	// in which case the marker is assigned as measure-attached to the point defined by the timepoint. <br />
	// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE
	// For instance, <b>addmarker 1000 foo</b> adds a marker at 1000ms with the name "foo", while <b>addmarker (1 1/4 3) faa</b> adds a marker 
	// named "faa" at attached to the position: measure 1, onset 1/4, voice 3.
	// If several names need to be associated to the marker, the symbol can be replaced by a list of symbols,
	// for instance <b>addmarker 1000 (foo fee)</b>. If no names need to be associated to the marker, leave <b>()</b> as
	// name llll.
	// @marg 0 @name position_ms @optional 0 @type float
	// @marg 1 @name name_or_names @optional 0 @type llll
    // @example addmarker 2000 George @caption add an absolute-position marker named 'George' at 2s
    // @example addmarker 2000 (Foo Fee Faa) @caption the same, with a complex llll name
    // @example addmarker cursor John @caption add an absolute-position marker named 'John' at the current playhead position
    // @example addmarker (4) John @caption add a measure-attached marker named John at beginning of measure 4 (1st voice)
    // @example addmarker (4 1/2) John @caption the same, after 1/2 of measure 4 (1st voice)
    // @example addmarker (3 4 1/2) John @caption the same, after 1/2 of measure 4 (3rd voice)
    // @seealso deletemarker, clearmarkers
	class_addmethod(c, (method) score_addmarker, "addmarker", A_GIMME, 0);
	
	
	// @method deletemarker @digest Delete a marker 
	// @description @copy BACH_DOC_MESSAGE_DELETEMARKER
	// @marg 0 @name marker_names @optional 0 @type list
    // @example deletemarker Ringo @caption deletes first marker named Ringo
    // @example deletemarker Ringo Starr @caption deletes first marker with both Ringo and Starr as names
    // @seealso addmarker, clearmarkers, delete
	class_addmethod(c, (method) score_deletemarker, "deletemarker", A_GIMME, 0);


	// @method clearmarkers @digest Delete all markers
	// @description @copy BACH_DOC_MESSAGE_CLEARMARKERS
    // @seealso addmarker, deletemarker
	class_addmethod(c, (method) score_clearmarkers, "clearmarkers", 0);
	

	// @method getmarker @digest Retrieve marker information
	// @description The <m>getmarker</m>, without any further argument, will output all the markers from the playout in the form 
	// <b>markers <m>MARKER1</m> <m>MARKER2</m>...</b>, where each <m>MARKER</m> is an llll of the form
	// <b>(<m>POSITION</m> <m>name_or_names</m> none)</b>, where the <m>POSITION</m> is either a floating point number (if marker is attached to
	// an absolute position in milliseconds) or the marker timepoint (if the marker is attached to a given measure position).
	// The <m>name_or_names</m> parameter is either a single symbol or integer (if the marker has a single name), or an llll containing all the names
	// listed, in the form <b>(<m>name1</m> <m>name2</m> ...)</b>, where each <m>name</m> is a symbol or an integer. If a marker has no name, 
	// then <b>()</b> is used. Finally the <b>none</b> symbol represents the marker role (unsupported in <o>bach.score</o> but output for compatibility
	// with <o>bach.roll</o> and for a possible future usage). <br />
	// If you send a message <b>getmarker @namefirst 1</b>, all the markers will be output from the playout in the form
	// <b>markers <m>MARKER1</m> <m>MARKER2</m>...</b>, where each <m>MARKER</m> is an llll of the form
	// <b>(<m>name_or_names</m> <m>POSITION</m> none)</b>. <br />
	// You can retrieve the information about a specific marker by adding the marker name or names as arguments. In this case you'll get
	// from the playout an llll in the form <b>marker <m>name_or_names</m> <m>POSITION</m> none</b>, where all
	// the parameters are the same as above.
	// If more than one marker match the introduced name(s), only the information about the first marker (in temporal order) is output.
	// @marg 0 @name names @optional 1 @type list
    // @mattr namefirst @type int @default 0 @digest If non-zero outputs the marker name(s) as first element
    // @example getmarker @caption get all markers in standard (position name) form
    // @example getmarker @namefirst 1 @caption get all markers in (name position) form
    // @example getmarker Ringo @caption get the first marker named 'Ringo'
    // @example getmarker Ringo Starr @caption get the first marker named both 'Ringo' and 'Starr'
    // @seealso dump
	class_addmethod(c, (method) score_getmarker, "getmarker", A_GIMME, 0);

	class_addmethod(c, (method) score_markername, "markername", A_GIMME, 0); // deprecated: use "name"


	// @method read @digest Open file
	// @description The <m>read</m> message will open any file that <o>bach.score</o> is able to read. 
	// The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
	// allowing the choice of the file to open.
	// Supported file types are: <br />
	// - Previously saved <o>bach.score</o> content, either in native or in text format (see <m>write</m> and <m>writetxt</m> messages). 
	// These might include also files exported in bach format from OpenMusic or PWGL. <br />
	// - MusicXML files (see below for options). <br />
	// MIDI files are not imported directly into bach score. They should be imported into a <o>bach.roll</o>, possibly with a <b>read (markmeasures 1)</b> specification, 
	// and then quantized with <o>bach.quantize</o>. <br />
    // MusicXML files can have some import attributes: <br />
    // - <b>dynamics</b>: if non-zero, dynamics are imported into the currently linked slot (see <m>linkdynamicstoslot</m>). <br />
    // - <b>noteheads</b>: if non-zero, dynamics are imported into the currently linked slot (see <m>linknoteheadstoslot</m>). <br />
    // - <b>articulations</b>: if non-zero, articulations are imported into the currently linked slot (see <m>linkarticulationstoslot</m>). <br />
    // - <b>lyrics</b>: if non-zero, lyrics are imported into the currently linked slot (see <m>linklyricstoslot</m>). <br />
    //
	// @marg 0 @name filename @optional 1 @type symbol
    // @mattr dynamics @type int @default 1 @digest For MusicXML files, if non-zero, imports dynamics into the dynamics slot, if any
    // @mattr noteheads @type int @default 1 @digest For MusicXML files, if non-zero, imports noteheads into the noteheads slot, if any
    // @mattr articulations @type int @default 1 @digest For MusicXML files, if non-zero, imports articulations into the articulations slot, if any
    // @mattr lyrics @type int @default 1 @digest For MusicXML files, if non-zero, imports lyrics into the lyrics slot, if any
    // @example read @caption open file via dialog box
    // @example read rollexample.txt @caption open specific bach text file
    // @example read goldberg3.xml @caption open a MusicXML file
    // @example read Yester.xml @lyrics 0 @caption open a MusicXML file but don't import lyrics
    // @example read Yester.xml @noteheads 0 @dynamics 0 @caption open a MusicXML file but don't import noteheads nor dynamics
    // @seealso write, writetxt
	class_addmethod(c, (method) score_read, "read", A_GIMME, 0);
    class_addmethod(c, (method) score_read_singlesymbol, "readsinglesymbol", A_GIMME, 0); // only for drag'n'drop purposes

    
    // @method write @digest Save file in native format
	// @description The <m>write</m> message saves all the <o>bach.score</o> content in native format (with extension .llll).
	// The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
	// allowing the choice of the location and file name for saving.
	// @marg 0 @name filename @optional 1 @type symbol
    // @example write @caption save file in native format via dialog box
    // @example write myfile.llll @caption save bach file in native format
    // @seealso writetxt, exportmidi, exportxml, read
	class_addmethod(c, (method) score_write, "write", A_GIMME, 0);


	// @method writetxt @digest Save file in text format
	// @description The <m>writetxt</m> message saves all the <o>bach.score</o> content, as llll, in text format (with extension .txt).
	// This will result in a readable text file, but it might also lead to some very slight loss of precision. This is usually negligible, but
	// if it bothers you, use the <m>write</m> message instead. <br />
    // @copy BACH_DOC_WRITETXT_TEXT_FORMAT_AND_ARGUMENTS
    // @example writetxt @caption export as a text file, opening a dialog box for the file name
    // @example writetxt myfile.txt @caption export as a text file with the provided file name
    // @example writetxt myfile.txt @maxdecimals 3 @caption export with a floating-point precision of 3 decimal digits
    // @example writetxt myfile.txt @maxdecimals 3 @wrap 40 @caption as the above, limiting the length of each line to 40 characters
    // @example writetxt myfile.txt @indent 1 @caption export indenting each sublist by one space per depth level
    // @example writetxt myfile.txt @indent 1 @maxdepth 2 @caption as the above, but only first-level sublists are placed on new lines
    // @example writetxt myfile.txt @maxdepth 1 @caption no indentation is performed
    // @marg 0 @name filename @optional 1 @type symbol
    // @mattr indent @type atom @default tab @digest Number of spaces for indentation or "tab" symbol
    // @mattr maxdepth @type int @default -1 @digest Maximum depth for new lines
    // @mattr wrap @type int @default 0 @digest Maximum number of characters per line (0 means: no wrapping)
    // @seealso write, exportmidi, exportxml, read
	class_addmethod(c, (method) score_writetxt, "writetxt", A_GIMME, 0);


    
    // @method exportmidi @digest Export as MIDI file
    // @description The <m>exportmidi</m> message saves the content of the <o>bach.score</o> as a MIDI file.
    // The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
    // allowing the choice of the location and file name for saving.
    // Furthermore, some exporting specifications are available, and each has to be given as llll after the (optional) file name.
    // Such lllls will be in the form <b>(<m>specification_name</m> <m>specification_value</m>)</b>). Available specifications are: <br />
    // - <b>exportmarkers</b> (default: 1): if non-0, all the markers in the score will be exported. <br />
    // - <b>voices</b> (default: <b>()</b>): if a list of voices is provided, then only the specified voices will be exported.
    // If no list is provided, then all the voices of the score will be exported. Ranges can also be
    // expressed, as sublists. For example, <b>(voices 1 3 5)</b> will export the first, third and fifth voice,
    // while <b>(voices (1 5) 8)</b> will export all the voices from 1 to 5, and the 8th voice. <br />
    // - <b>format</b> (default: 0): the MIDI file format (0 = single track, 1 = multiple tracks). <br />
    // – <b>resolution</b> (default: 960): the number of MIDI ticks per beat. <br />
    // - <b>temporampsamplingrate</b> (default 240): is an option expressing the maximum duration, expressed in MIDI ticks,
    // for a single tempo step during tempo interpolations. With the default settings, this duration is one sixteenth note.
    // In bach.score, accelerandi and rallentandi vary the tempo continuously.
    // This can't be achieved within MIDI files: instead, an accelerando or a rallentando will be exported as a
    // sequence of discrete tempo changes, each representing a "step" of the accelerando or rallentando.
    // It is guaranteed that the overall duration of the tempo ramp will be consistent with the original.
    // @marg 0 @name filename @optional 1 @type symbol
    // @mattr exportmarkers @type int @default 1 @digest If non-zero, exports all the markers
    // @mattr voices @type llll @default null @digest Numbers of voices to be exported (<b>null</b> means: all voices)
    // @mattr format @type int @default 0 @digest MIDI file format (0 = single track, 1 = multiple tracks)
    // @mattr resolution @type int @default 960 @digest Number of MIDI ticks per beat
    // @mattr temporampsamplingrate @type int @default 240 @digest Maximum duration, in MIDI ticks, for a single tempo step during tempo interpolations
    // @example exportmidi @caption export MIDI file via dialog box
    // @example exportmidi mymidi.mid @caption export MIDI file
    // @example exportmidi mymidi.mid @exportmarkers 0 @caption the same, but don't export markers
    // @example exportmidi mymidi.mid @voices 1 3 @format 1 @caption exports voices 1 and 3 in format 1
    // @example exportmidi mymidi.mid @resolution 1920 @caption exports with a resolution of 1920 ticks per beat
    // @seealso write, writetxt, read, exportxml, exportom, exportpwgl
    class_addmethod(c, (method) score_exportmidi, "exportmidi", A_GIMME, 0);
    
    
    // @method exportlilypond @digest Export as LilyPond file
    // @description The <m>exportlilypond</m> message saves the content of the <o>bach.score</o> as a LilyPond file.
    // The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
    // allowing the choice of the location and file name for saving.
    // Furthermore, if a list of voices is provided as "voice" message argument, then only the specified voices will be exported.
    // If no list is provided, then all the voices of the score will be exported. Ranges can also be
    // expressed, as sublists. For example, <b>(voices 1 3 5)</b> will export the first, third and fifth voice,
    // while <b>(voices (1 5) 8)</b> will export all the voices from 1 to 5, and the 8th voice. <br />
    // @marg 0 @name filename @optional 1 @type symbol
    // @mattr voices @type llll @default null @digest Voices to be exported (<b>null</b> means all voices)
    // @example exportlilypond @caption export LilyPond file via dialog box
    // @example exportlilypond myfile.ly @caption export specific LilyPond file
    // @example exportlilypond myfile.ly @voices 1 3 @caption exports voices 1 and 3
    // @example exportlilypond myfile.ly @voices (1 3) @caption exports voices 1 through 3
    // @example exportlilypond myfile.ly @voices (1 3) 5 7 @caption exports voices 1 through 3, 5 and 7
    // @seealso write, writetxt, read, exportxml, exportom, exportpwgl, exportlilypondpdf
    class_addmethod(c, (method) score_exportlilypond, "exportlilypond", A_GIMME, 0);
    
    
    // @method exportlilypondpdf @digest Export PDF file via LilyPond
    // @description The <m>exportlilypondpdf</m> exports the content of the <o>bach.score</o> as a PDF file,
    // via a LilyPond conversion. LilyPond has to be installed properly in order for this to work.
    // The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
    // allowing the choice of the location and file name for saving.
    // @marg 0 @name filename @optional 1 @type symbol
    // @example exportlilypondpdf @caption export PDF file via LilyPond and a dialog box
    // @example exportlilypondpdf myfile.pdf @caption exports specific PDF file via LilyPond
    // @seealso exportlilypond, write, writetxt, read, exportxml, exportom, exportpwgl
    class_addmethod(c, (method) score_exportlilypond_pdf, "exportlilypondpdf", A_GIMME, 0);
    
    
    // @method exportxml @digest Export as MusicXML file
    // @description The <m>exportxml</m> message saves the content of the <o>bach.score</o> object in MusicXML format.
    // Such format is read by most of the currently available music engraving softwares.
    // The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
    // allowing the choice of the location and file name for saving.
    // Furthermore, some exporting specifications are available, and each has to be given as llll after the (optional) file name.
    // Such lllls will be in the form <b>(<m>specification_name</m> <m>specification_value</m>)</b>). Available specifications are: <br />
    // - <b>velocity</b> (default: 0): if non-0, the velocity for each note will be exported as a MusicXML direction.
    // This is not always desirable, as the direction itself will appear as an empty expression when
    // imported, for instance, into a software like Finale. <br />
    // - <b>directionslots</b> (default: <b>null</b>): if one or more slots numbers are specified, their contents will be converted
    // into text and exported as generic text directions. You can use range syntax to specify slot ranges, such as <b>(7 10)</b> meaning from 7th to 10th slot. <br />
    // - <b>dynamics</b>: if non-zero, dynamics are exported from the currently linked slot (see <m>linkdynamicstoslot</m>). <br />
    // - <b>noteheads</b>: if non-zero, dynamics are exported from the currently linked slot (see <m>linknoteheadstoslot</m>). <br />
    // - <b>articulations</b>: if non-zero, articulations are exported from the currently linked slot (see <m>linkarticulationstoslot</m>). <br />
    // - <b>lyrics</b>: if non-zero, lyrics are exported from the currently linked slot (see <m>linklyricstoslot</m>). <br />
    // - <b>parenthesizedquartertones</b> (default: <b>0</b>): export quarter tones as parenthesized versions of semitonal accidentals
    // which are a quartertone lower. For instance, if you set this specification to 1, a quarter sharp will be exported as a parenthesized
    // natural, a quarter flat will be exported as a parenthesized flat, and so on.
    // This specification should be considered as a workaround to ease quartertone export to Finale or similar pieces of software.
    // Please notice that music notation software with a good native support for quartertones
    // (e.g. Sibelius or MuseScore) will import them correctly from bach, without any workaround (no need to specify anything).
    // Things are more complex with software such Finale, whose quartertone support is more cumbersome.
    // With the default settings, what Finale will do is changing the character for single accidentals in order to show quartertones
    // accidentals when needed. This is something you might want to avoid, since editing your score from that point on becomes very
    // difficult. What might be handy to do, with Finale, is to export quartertone accidentals as if they were parenthesized versions
    // of the standard accidentals. You can easily map inside finale the parenthesized accidentals to the glyphs of
    // quartertone accidentals, via your favorite quartertone accidental font. A parenthesized natural will be displayed as
    // a quarter-sharp; a parenthesized sharp as a three-quarter-sharp, and so on. Everything, if parenthesized, will step up a
    // quarter tone. Once you have set up your new Finale document in this way, you can use the <b>(parenthesizedquartertones 1)</b>
    // specification in order to export quartertones in this way.
    // @marg 0 @name filename @optional 1 @type symbol
    // @mattr velocity @type int @default 0 @digest If non-zero, notes velocity i s exported as MusicXML direction
    // @mattr directionslots @type llll @default null @digest Export the content of these slots as generic text directions
    // @mattr dynamics @type int @default 1 @digest If non-zero, dynamics are exported from the dynamics slot, if any
    // @mattr noteheads @type int @default 1 @digest If non-zero, noteheads are exported from the noteheads slot, if any
    // @mattr articulations @type int @default 1 @digest If non-zero, articulations are exported from the articulations slot, if any
    // @mattr lyrics @type int @default 1 @digest If non-zero, lyrics are exported from the lyrics slot, if any
    // @mattr parenthesizedquartertones @type int @default 0 @digest If non-zero, exports quarter tones as parenthesized version of semitonal accidentals
    // @example exportxml @caption export MusicXML file via dialog box
    // @example exportxml myfile.xml (velocity 1) @caption also export velocity as MusicXML direction
    // @example exportxml myfile.xml (directionslots 7) @caption also export content of slot 7 as MusicXML directions
    // @example exportxml myfile.xml (directionslots 7 9) @caption also export content of slot 7 and 9 as MusicXML directions
    // @example exportxml myfile.xml (directionslots (7 11)) @caption also export content of slots 7 through 11 as MusicXML directions
    // @example exportxml myfile.xml (dynamics 0) @caption don't export dynamics
    // @example exportxml myfile.xml (lyrics 0) (articulations 0) @caption don't export lyrics, nor articulations
    // @example exportxml myfile.xml (noteheads 0) @caption don't export noteheads
    // @example exportxml myfile.xml (parenthesizedquartertones 1) @caption also export quartertones as parenthesized versions of the standard accidentals right below them (only useful with Finale)
    // @seealso write, writetxt, read, exportxml, exportom, exportpwgl, exportlilypond, exportlilypondpdf
    class_addmethod(c, (method) score_exportxml, "exportxml", A_GIMME, 0);
    
    
    // @method exportpwgl @digest Export to PWGL
    // @description The <m>exportpwgl</m> message saves the content of the <o>bach.score</o> object in a way that PWGL will be able to open
    // (by connecting the "import-bach" box, in the "pwgl2bach" library).
    // The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
    // allowing the choice of the location and file name for saving.
    // @marg 0 @name filename @optional 1 @type symbol
    // @example exportpwgl @caption export file in PWGL format via dialog box
    // @example exportpwgl forPWGL.txt @caption export file in PWGL format
    // @seealso write, writetxt, read, exportxml, exportom, exportxml, exportlilypond, exportlilypondpdf
    class_addmethod(c, (method) score_exportpwgl, "exportpwgl", A_GIMME, 0);
    
    
    // @method exportom @digest Export to OpenMusic
    // @description The <m>exportom</m> message saves the content of the <o>bach.score</o> object in a way that OpenMusic will be able to open
    // (by selecting "Import" and then "From bach" from the File menu). The content of a <o>bach.score</o> can be imported into an OpenMusic's "voice"
    // or "poly".
    // The file name (as symbol) can be given as optional first argument. If no such symbol is given, a dialog box will pop up
    // allowing the choice of the location and file name for saving.
    // @marg 0 @name filename @optional 1 @type symbol
    // @example exportom @caption export file in OpenMusic format via dialog box
    // @example exportom forOM.txt @caption export file in OpenMusic format
    // @seealso write, writetxt, read, exportxml, exportpwgl, exportxml, exportlilypond, exportlilypondpdf
    class_addmethod(c, (method) score_exportom, "exportom", A_GIMME, 0);
    
    

    
	// @method (mouse) @digest Edit content
	// @description @copy BACH_DOC_SCORE_EDIT_MOUSE_COMMANDS
	class_addmethod(c, (method) score_mouseenter, "mouseenter", A_CANT, 0);
	class_addmethod(c, (method) score_mouseleave, "mouseleave", A_CANT, 0);
	class_addmethod(c, (method) score_mousemove, "mousemove", A_CANT, 0);
	class_addmethod(c, (method) score_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method) score_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method) score_mouseup, "mouseup", A_CANT, 0);
	class_addmethod(c, (method) score_mousewheel, "mousewheel", A_CANT, 0);
    class_addmethod(c, (method) score_mousedoubleclick, "mousedoubleclick", A_CANT, 0);
	class_addmethod(c, (method) score_oksize, "oksize", A_CANT, 0);
    
    // @method (drag) @digest Open file
    // @description Dragging a file on the object will load its content, if a proper readable format is recognized.
    class_addmethod(c, (method) score_acceptsdrag,	"acceptsdrag_unlocked", A_CANT, 0);
    class_addmethod(c, (method) score_acceptsdrag,	"acceptsdrag_locked", A_CANT, 0);


	// @method (keyboard) @digest Edit content
	// @description @copy BACH_DOC_SCORE_EDIT_KEYBOARD_COMMANDS
  	class_addmethod(c, (method) score_key, "key", A_CANT, 0);
  	class_addmethod(c, (method) score_keyfilter, "keyfilter", A_CANT, 0);
	class_addmethod(c, (method) score_enter,	"enter", A_CANT, 0);
	class_addmethod(c, (method) score_focusgained, "focusgained", A_CANT, 0);
	class_addmethod(c, (method) score_focuslost, "focuslost", A_CANT, 0);
	class_addmethod(c, (method) score_edclose, "edclose", A_CANT, 0);
	class_addmethod(c, (method) score_okclose, "okclose", A_CANT, 0);
	class_addmethod(c, (method) score_jsave, "jsave", A_CANT, 0);

	class_addmethod(c, (method) score_inletinfo, "inletinfo", A_CANT, 0);
	class_addmethod(c, (method) score_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method) score_notify, "bachnotify", A_CANT, 0);

	// @method undo @digest Perform an undo step
	// @description An <m>undo</m> message will perform an undo step. 
    // @seealso redo, inhibitundo, droplastundo
	class_addmethod(c, (method) score_undo, "undo", 0);


	// @method redo @digest Perform a redo step
	// @description A <m>redo</m> message will perform a redo step. 
    // @seealso undo, inhibitundo, droplastundo
	class_addmethod(c, (method) score_redo, "redo", 0);

	
	// @method inhibitundo @digest Temporarily disable/enable undo step creation
	// @description An <m>inhibitundo</m> message followed by any non-zero number will
	// temporarily disable any undo step creation. Use the same message followed by a zero
	// to re-enable undo steps.
    // @seealso droplastundo, undo, redo
	class_addmethod(c, (method) score_inhibit_undo, "inhibitundo", A_LONG, 0);

	
	// @method droplastundo @digest Prune last undo step 
	// @description A <m>droplastundo</m> message will prune the last undo step, by merging
	// its information into the previous one.
    // @seealso inhibitundo, undo, redo
	class_addmethod(c, (method) score_prune_last_undo_step, "droplastundo", 0);

    
    // @method setnotationcolors @digest Change all notation colors at once
    // @description Set all the notation colors at once (accidentals, annotations, articulations, clefs, key signatures,
    // lyrics, main and auxiliary staff, notes, stems, tuplets, beams, flags, rests), in RGBA format.
    // @marg 0 @name red @optional 0 @type float
    // @marg 1 @name green @optional 0 @type float
    // @marg 2 @name blue @optional 0 @type float
    // @marg 3 @name alpha @optional 0 @type float
    // @example setallnotationcolors 1. 0. 0. 1. @caption turn all notation red
    class_addmethod(c, (method) score_anything, "setnotationcolors", A_GIMME, 0);

    
    
    // @method checkdynamics @digest Check dynamics for selection
    // @description A <m>checkdynamics</m> message will check the correctness of the dynamics sequence, for the whole score or - if
    // "selection" is set as first argument, for the selected items only. The dynamics sequence considered is the one contained in the slot
    // linked to dynamics, if any (see <m>linkdynamicstoslot</m>), or in another slot, if given as integer argument. <br />
    // Message attributes are:
    // • "inconsistent": value is expected to be 0 or 1, telling if the checking should happen on inconsistent dynamics
    // • "unnecessary": value is expected to be 0 or 1, telling if the checking should happen on unnecessary dynamics
    // @marg 0 @name selection @optional 1 @type symbol
    // @marg 1 @name slot_number @optional 1 @type int
    // @mattr inconsistent @type int @default 1 @digest If non-zero, checks inconsistent dynamics
    // @mattr unnecessary @type int @default 1 @digest If non-zero, checks unnecessary dynamics
    // @seealso fixdynamics
    // @example checkdynamics @caption check dynamics throughout the whole score
    // @example checkdynamics selection @caption check dynamics, for selected items only
    // @example checkdynamics @inconsistent 0 @caption check only wrong dynamics, don't check repeated ones
    class_addmethod(c, (method) score_anything, "checkdynamics", A_GIMME, 0);
    
    
    // @method fixdynamics @digest Fix dynamics for selection
    // @description A <m>fixdynamics</m> message will check the correctness of the dynamics sequence, for the whole score or - if
    // "selection" is set as first argument, for the selected items only. The dynamics sequence considered is the one contained in the slot
    // linked to dynamics, if any (see <m>linkdynamicstoslot</m>), or in another slot, if given as integer argument. <br />
    // If incorrect dynamics are found, they are also fixed.
    // Further message attributes are:
    // • "inconsistent": value is expected to be 0 or 1, telling if the fixing should happen on inconsistent dynamics
    // • "unnecessary": value is expected to be 0 or 1, telling if the fixing should happen on unnecessary dynamics
    // • "verbose": value is expected to be 0 or 1, telling if the fixing should log warnings in the max window for each found dynamics
    // @marg 0 @name selection @optional 1 @type symbol
    // @marg 1 @name slot_number @optional 1 @type int
    // @mattr inconsistent @type int @default 1 @digest If non-zero, fixes inconsistent dynamics
    // @mattr unnecessary @type int @default 1 @digest If non-zero, fixes unnecessary dynamics
    // @mattr verbose @type int @default 0 @digest If non-zero, logs warnings in the max window for each found dynamics
    // @seealso checkdynamics
    // @example fixdynamics @caption fix all dynamics issues
    // @example fixdynamics selection @caption same thing, for selected items only
    // @example fixdynamics @inconsistent 0 @caption fix only wrong dynamics, keep repeated ones
    // @example fixdynamics @unnecessary 0 @caption filter out dynamics repetitions
    class_addmethod(c, (method) score_anything, "fixdynamics", A_GIMME, 0);

    
    // @method dynamics2velocities @digest Assign velocities depending on dynamics
    // @description A <m>dynamics2velocities</m> message will assign all the note velocities (and pitch breakpoint velocity,
    // if <m>breakpointshavevelocity</m> is non-zero), depending on the current dynamics and hairpins, for the whole score or - if
    // "selection" is set as first argument, for the selected items only. The dynamics sequence considered is the one contained in the slot
    // linked to dynamics, if any (see <m>linkdynamicstoslot</m>), or in another slot, if given as integer argument. <br />
    // Unless explicit dynamic-velocity mapping is defined, a default mapping is used. Two message attributes handle such default mapping: <br />
    // • the "maxchars" attribute sets the width of the dynamics spectrum; <br />
    // • the "exp" attribute sets the exponent for the conversion. <br />
    // @copy BACH_DOC_DYNAMICS_SPECTRUM
    // Moreover, the "mapping" attribute defines a non-standard or more general mapping via an llll specification. Such parameter has syntax
    // <b>(<m>dynamics1</m> <m>velocity1</m>) (<m>dynamics2</m> <m>velocity2</m>)</b>.
    // You might also define a the dynamics association you want to drift from the default values; any other marking which does not show up in the
    // mapping will be converted via the default conversion equation.
    // @marg 0 @name selection @optional 1 @type symbol
    // @marg 1 @name slot_number @optional 1 @type int
    // @mattr maxchars @type int @default 4 @digest Width of the dynamics spectrum
    // @mattr exp @type float @default 0.8 @digest Exponent for the conversion
    // @mattr mapping @type llll @digest Custom dynamics-to-velocity mapping via <b>(<m>dynamics</m> <m>velocity</m>)</b> pairs
    // @seealso velocities2dynamics, checkdynamics, fixdynamics
    // @example dynamics2velocities @caption convert dynamics to velocities throughout the whole score
    // @example dynamics2velocities selection @caption same thing, for selected items only
    // @example dynamics2velocities @mapping (mp 70) (mf 80) @caption customly map "mp" to velocity = 70 and "mf" to velocity = 80
    // @example dynamics2velocities @maxchars 6 @caption use broader dynamics spectrum (from "pppppp" to "ffffff")
    // @example dynamics2velocities @maxchars 2 @caption use narrower dynamics spectrum (from "pp" to "ff")
    // @example dynamics2velocities @maxchars 2 @exp 0.5 @caption use narrower spectrum and a steeper mapping curve
    // @example dynamics2velocities @exp 1. @caption use a linear mapping curve
    class_addmethod(c, (method) score_anything, "dynamics2velocities", A_GIMME, 0);

    
    // @method velocities2dynamics @digest Infer dynamics depending on velocities
    // @description A <m>velocities2dynamics</m> message will infer and assign chord dynamics and hairpins, depending on the current velocities,
    // for the whole score or - if "selection" is set as first argument, for the selected items only.
    // The dynamics sequence considered is the one contained in the slot
    // linked to dynamics, if any (see <m>linkdynamicstoslot</m>), or in another slot, if given as integer argument. <br />
    // Unless explicit dynamic-velocity mapping is defined, a default mapping is used. Two message attributes handle such default mapping: <br />
    // • the "maxchars" attribute sets the width of the dynamics spectrum; <br />
    // • the "exp" attribute sets the exponent for the conversion. <br />
    // @copy BACH_DOC_DYNAMICS_SPECTRUM
    // Moreover, the "mapping" attribute defines a non-standard or more general mapping via an llll specification. Such parameter has syntax
    // <b>(mapping (<m>dynamics1</m> <m>velocity1</m>) (<m>dynamics2</m> <m>velocity2</m>)...)</b>.
    // Differently from <m>dynamics2velocities</m>, if you define a mapping, you need to define the velocity association for each of the dynamic marking
    // you want to use. <br />
    // An "unnecessary" attribute toggles whether unnecessary dynamic markings should by default be dropped (default is 1: yes, use 0 to turn this of). <br />
    // Finally, a "thresh" attribute sets a threshold for hairpin detection (default is 1., 0. meaning: no hairpin detection).
    // @marg 0 @name selection @optional 1 @type symbol
    // @marg 1 @name slot_number @optional 1 @type int
    // @mattr maxchars @type int @default 4 @digest Width of the dynamics spectrum
    // @mattr exp @type float @default 0.8 @digest Exponent for the conversion
    // @mattr mapping @type llll @digest Custom dynamics-to-velocity mapping via <b>(<m>dynamics</m> <m>velocity</m>)</b> pairs
    // @mattr unnecessary @type int @default 1 @digest If non-zero, drops unnecessary dynamic markings
    // @mattr thresh @type float @default 1. @digest Hairpin detection threshold
    // @seealso dynamics2velocities, checkdynamics, fixdynamics
    // @example velocities2dynamics @caption convert velocities to dynamics throughout the whole score
    // @example velocities2dynamics selection @caption same thing, for selected items only
    // @example velocities2dynamics @mapping (p 40) (mp 70) (mf 90) (f 110) @caption use only "p", "mp", "mf", "f" symbols, with the defined mappings
    // @example velocities2dynamics @maxchars 6 @caption use broader dynamics spectrum (from "pppppp" to "ffffff")
    // @example velocities2dynamics @maxchars 2 @caption use narrower dynamics spectrum (from "pp" to "ff")
    // @example velocities2dynamics @maxchars 2 @exp 0.5 @caption use narrower spectrum and a steeper mapping curve
    // @example velocities2dynamics @exp 1. @caption use a linear mapping curve
    // @example dynamics2velocities @thresh 10. @caption allow coarse hairpin detection
    // @example velocities2dynamics @thresh 0. @unnecessary 0 @caption assign a dynamic marking to each chord, no hairpins
    class_addmethod(c, (method) score_anything, "velocities2dynamics", A_GIMME, 0);
    
    
    
    // @method slice @digest Slice all the notes at a given timepoint
    // @description A <m>slice</m> message followed by one or more timepoints will split all the notes
    // inside the <b>bach.score</b> which exist at the given timepoint: a left- and a right-sided note. All breakpoints and temporal slots will be
    // properly split accordingly. If timepoints contain voice numbers, only specific voices will be split; otherwise all voices will be split
    // @marg 0 @name timepoints @optional 0 @type llll
    // @example slice (3 1/2) @caption slice all notes at measure 3 after 1/2
    // @example slice (4 3 1/2) @caption ...only in voice 4
    class_addmethod(c, (method) score_slice, "slice", A_GIMME, 0);
    
    
	class_addmethod(c, (method) score_getmaxID, "getmaxid", 0); // undocumented

	llllobj_class_add_out_attr(c, LLLL_OBJ_UI);

	
	notation_class_add_notation_attributes(c, k_NOTATION_OBJECT_SCORE);

	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0 0 526 120"); // new dimensions
	// @exclude bach.score

	CLASS_STICKY_ATTR(c,"category",0,"Appearance");

	CLASS_ATTR_CHAR(c,"slursshownchordwise",0, t_notation_obj, slurs_shown_chordwise);
	CLASS_ATTR_STYLE_LABEL(c,"slursshownchordwise",0,"onoff","Slurs Are Shown Chordswise");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"slursshownchordwise",0,"1");
	CLASS_ATTR_INVISIBLE(c, "slursshownchordwise", ATTR_GET_OPAQUE | ATTR_SET_OPAQUE); // just for now
	// @exclude bach.score
	
	CLASS_ATTR_CHAR(c,"slursavoidaccidentals",0, t_notation_obj, slurs_avoid_accidentals);
	CLASS_ATTR_STYLE_LABEL(c,"slursavoidaccidentals",0,"onoff","Slurs Avoid Accidentals");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"slursavoidaccidentals",0,"1");
	CLASS_ATTR_INVISIBLE(c, "slursavoidaccidentals", ATTR_GET_OPAQUE | ATTR_SET_OPAQUE); // just for now
	// @exclude bach.score
	
	CLASS_ATTR_CHAR(c,"slursalwayssymmetrical",0, t_notation_obj, slurs_always_symmetrical);
	CLASS_ATTR_STYLE_LABEL(c,"slursalwayssymmetrical",0,"onoff","Slurs Are Always Symmetrical");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"slursalwayssymmetrical",0,"1");
	CLASS_ATTR_INVISIBLE(c, "slursalwayssymmetrical", ATTR_GET_OPAQUE | ATTR_SET_OPAQUE); // just for now
	// @exclude bach.score
	
	CLASS_ATTR_CHAR(c,"align",0, t_notation_obj, align_chords_with_what);
	CLASS_ATTR_STYLE_LABEL(c,"align",0,"enumindex","Proportional Spacing Alignment Mode");
	CLASS_ATTR_ENUMINDEX(c,"align", 0, "Stems Notehead Center Notehead End");
	CLASS_ATTR_ACCESSORS(c, "align", (method)NULL, (method)score_setattr_align);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"align",0,"1");
	// @description Sets the alignment mode for chords when the Proportional spacing <m>spacingtype</m> is selected: <br />
	// - Stems: chords are aligned by their stem (whether or not it is displayed: see the <m>showstems</m> attribute). Two chords falling on the same time
	// instant will have their stems vertically aligned. <br />
	// - Notehead Center (default): means that chords are aligned by the center of the main notehead. This is similar to what happens in <o>bach.score</o>: 
	// two chords falling on the same time instant, one with stem up and one with stem down, will be aligned according to the position of their noteheads, and not 
	// the position of their stems. The noteheads will result aligned. <br />
	// - Notehead End: chords are aligned by the ending point of their noteheads. In this case, the duration line perfectly spans the time duration of the notes. 

    CLASS_ATTR_CHAR(c,"dlspansties",0, t_notation_obj, dl_spans_ties);
    CLASS_ATTR_STYLE_LABEL(c,"dlspansties",0,"enumindex","Duration Line Spans Ties");
    CLASS_ATTR_ENUMINDEX(c,"dlspansties", 0, "No Yes Yes With Hidden Notes");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"dlspansties",0,"0");
    // @description Toggles the ability for the duration line (if shown) to span sequences of
    // tied notes. Values are: 0 = Don't span ties (default); 1 = Span ties; 2 = Span ties,
    // and also hide notes and ties (i.e. only the duration line is kept to symbolize the duration of the
    // tied sequence).

    
	CLASS_STICKY_ATTR_CLEAR(c, "category");

    CLASS_ATTR_CHAR(c,"edittiedelementstogether", 0, t_notation_obj, edit_tied_notes_together);
    CLASS_ATTR_STYLE_LABEL(c, "edittiedelementstogether",0,"onoff","Edit Tied Notes Together");
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c,"edittiedelementstogether",0,"1");
    CLASS_ATTR_CATEGORY(c, "edittiedelementstogether", 0, "Behavior");
    // @description Toggles to ability to edit tied sequences of notes at once (namely: when a note is selected via the interface,
    // all the notes tied to and from it are automatically selected as well).

    

	CLASS_STICKY_ATTR(c,"category",0,"Settings");

    CLASS_ATTR_LONG(c, "tempodigits", 0, t_notation_obj, tempo_approx_digits);
    CLASS_ATTR_STYLE_LABEL(c,"tempodigits",0,"text","Number Of Digits For Tempo Approximation");
    CLASS_ATTR_FILTER_CLIP(c, "tempodigits", 0, BACH_MAX_TEMPO_DIGITS);
#ifdef C74_X64
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c,"tempodigits",0,"2");
#else
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c,"tempodigits",0,"1");
#endif
    // @description Sets the number of digits for tempo approximation. Defaults to 2 for 64-bit mode, to 1 for 32-bit mode.

    
    CLASS_ATTR_DOUBLE(c,"tempovadj",0, t_notation_obj, tempi_uy_pos);
    CLASS_ATTR_STYLE_LABEL(c,"tempovadj",0,"text","Tempi Vertical Adjustment");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"tempovadj",0,"24");
    // @exclude bach.slot
    // @description Sets the vertical shift (in pixels, rescaled depending on the <m>vzoom</m>) of the
    // tempi display. 0 will set the tempo top line coinciding with the topmost staff line.
    // The default is 24.
    
    
	CLASS_ATTR_NOTATIONOBJ_SYMPTR(c, "clefs", 0, clefs_as_symlist, CONST_MAX_VOICES, score_setattr_clefs);
	CLASS_ATTR_STYLE_LABEL(c,"clefs",0,"text","Clefs");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"clefs",0,"G");
	CLASS_ATTR_BASIC(c,"clefs",0);
	// @description @copy BACH_DOC_CLEFS

	CLASS_ATTR_NOTATIONOBJ_SYMPTR(c, "keys", 0, keys_as_symlist, CONST_MAX_VOICES, score_setattr_keys);
	CLASS_ATTR_STYLE_LABEL(c,"keys",0,"text","Keys");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"keys",0,"CM");
	CLASS_ATTR_BASIC(c,"keys",0);
	// @description @copy BACH_DOC_KEYS
	
	CLASS_ATTR_LONG(c, "tonedivision", 0, t_notation_obj, tone_division); 
	CLASS_ATTR_STYLE_LABEL(c,"tonedivision",0,"text","Microtonal Division");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"tonedivision",0,"2");
	CLASS_ATTR_ACCESSORS(c, "tonedivision", (method)NULL, (method)score_setattr_tonedivision);
	CLASS_ATTR_BASIC(c,"tonedivision",0);
	// @description @copy BACH_DOC_TONEDIVISION

	CLASS_ATTR_LLLL(c, "voicenames", 0, t_notation_obj, voicenames_as_llll, notation_obj_getattr_voicenames, notation_obj_setattr_voicenames);
	CLASS_ATTR_STYLE_LABEL(c,"voicenames",0,"text_large","Voice Names");
	CLASS_ATTR_SAVE(c, "voicenames", 0);
	CLASS_ATTR_PAINT(c, "voicenames", 0);
	// @description @copy BACH_DOC_VOICENAMES

	CLASS_ATTR_LLLL(c, "stafflines", 0, t_notation_obj, stafflines_as_llll, notation_obj_getattr_stafflines, notation_obj_setattr_stafflines);
	CLASS_ATTR_STYLE_LABEL(c,"stafflines",0,"text_large","Number Of Staff Lines");
	CLASS_ATTR_SAVE(c, "stafflines", 0);
	CLASS_ATTR_PAINT(c, "stafflines", 0);
	// @description @copy BACH_DOC_STAFFLINES
	
	CLASS_ATTR_CHAR(c, "accidentalsgraphic", 0, t_notation_obj, accidentals_display_type); 
	CLASS_ATTR_STYLE_LABEL(c,"accidentalsgraphic",0,"enumindex","Accidental Graphic");
	CLASS_ATTR_ENUMINDEX(c,"accidentalsgraphic", 0, "None Classical Fraction Unreduced Fraction Cents");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"accidentalsgraphic", 0,"1");
	CLASS_ATTR_ACCESSORS(c, "accidentalsgraphic", (method)NULL, (method)score_setattr_accidentalsgraphic);
	CLASS_ATTR_BASIC(c,"accidentalsgraphic",0);
	// @description @copy BACH_DOC_ACCIDENTALSGRAPHIC

	CLASS_ATTR_CHAR(c, "accidentalspreferences", 0, t_notation_obj, accidentals_preferences); 
	CLASS_ATTR_STYLE_LABEL(c,"accidentalspreferences",0,"enumindex","Accidental Preferences");
	CLASS_ATTR_ENUMINDEX(c,"accidentalspreferences", 0, "Auto Sharps Flats Custom");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"accidentalspreferences",0,"0");
	CLASS_ATTR_ACCESSORS(c, "accidentalspreferences", (method)NULL, (method)score_setattr_accidentalspreferences);
	// @description @copy BACH_DOC_ACCIDENTALSPREFERENCES

	CLASS_ATTR_NOTATIONOBJ_SYMPTR(c, "enharmonictable", 0, full_acc_repr, CONST_MAX_VOICES, score_setattr_enharmonictable);
	CLASS_ATTR_STYLE_LABEL(c,"enharmonictable",0,"text_large","Custom Enharmonic Table");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"enharmonictable",0,"default");
	// @description @copy BACH_DOC_ENHARMONICTABLE

	CLASS_ATTR_LONG(c,"measurenumberoffset", 0, t_notation_obj, measure_number_offset);
	CLASS_ATTR_STYLE_LABEL(c,"measurenumberoffset", 0, "text", "Measure Number Offset");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"measurenumberoffset", 0, "0");
	// @description Sets a global offset for the measure numbering (0 is the default, meaning: first measure start with 1).

	CLASS_ATTR_CHAR(c,"lyricsaffectspacing", 0, t_notation_obj, lyrics_affect_spacing);
	CLASS_ATTR_STYLE_LABEL(c,"lyricsaffectspacing", 0, "enum", "Lyrics Affect Spacing");
	CLASS_ATTR_ENUMINDEX(c,"lyricsaffectspacing", 0, "Never Only Inside Measures Also Measure Width When Overlapping Also Measure Width Always");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"lyricsaffectspacing", 0, "2");
	CLASS_ATTR_ACCESSORS(c, "lyricsaffectspacing", (method)NULL, (method)score_setattr_lyricsaffectspacing);
	// @description Sets the way in which lyrics affect the score spacing: <br />
	// - Never: lyrics never affect the spacing. <br />
	// - Only Inside Measures: They only affect the internal spacing of a measure, but they cannot increase the width of the measure. <br />
	// - Also Measure Width When Overlapping: They affect the internal spacing of the measure, and also can increase its width in case they risk to overlap.<br />
	// - Also Measure Width Always: They always affect both internal spacing and measure width increasing.
	
    
    CLASS_ATTR_CHAR(c,"dynamicsaffectspacing", 0, t_notation_obj, dynamics_affect_spacing);
    CLASS_ATTR_STYLE_LABEL(c,"dynamicsaffectspacing", 0, "enum", "Dynamics Affect Spacing");
    CLASS_ATTR_ENUMINDEX(c,"dynamicsaffectspacing", 0, "Never Only Inside Measures Also Measure Width When Overlapping Also Measure Width Always");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"dynamicsaffectspacing", 0, "2");
    CLASS_ATTR_ACCESSORS(c, "dynamicsaffectspacing", (method)NULL, (method)score_setattr_dynamicsaffectspacing);
    // @description Sets the way in which dynamics affect the score spacing: <br />
    // - Never: dynamics never affect the spacing. <br />
    // - Only Inside Measures: They only affect the internal spacing of a measure, but they cannot increase the width of the measure. <br />
    // - Also Measure Width When Overlapping: They affect the internal spacing of the measure, and also can increase its width in case they risk to overlap.<br />
    // - Also Measure Width Always: They always affect both internal spacing and measure width increasing.

    CLASS_ATTR_CHAR(c,"drawbarlinesacrossstaves", 0, t_notation_obj, draw_barlines_across_staves);
	CLASS_ATTR_STYLE_LABEL(c,"drawbarlinesacrossstaves", 0, "onoff", "Draw Barlines Across Staves");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"drawbarlinesacrossstaves", 0, "1");
	// @description Toggles the ability to draw barlines across all the staves. This is true only for barlines which perfectly
	// correspond in all voices.
	
	CLASS_STICKY_ATTR_CLEAR(c, "category");

	CLASS_STICKY_ATTR(c,"category",0,"Play");

    CLASS_ATTR_CHAR(c,"playtempi",0, t_notation_obj, play_tempi);
    CLASS_ATTR_STYLE_LABEL(c,"playtempi",0,"onoff","Play Tempi");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"playtempi", 0, "0");
    // @description Toggle the ability to also send the tempo through the playout during the playback. See the <m>play</m> message to know more
    // about the syntax. By default this is 0.

    CLASS_ATTR_CHAR(c,"playmeasures",0, t_notation_obj, play_measures);
    CLASS_ATTR_STYLE_LABEL(c,"playmeasures",0,"onoff","Play Measure Start Barlines");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"playmeasures", 0, "0");
    // @description Toggle the ability to also send the information about measure start at the beginning of playback. See the <m>play</m> message to know more
    // about the syntax. By default this is 0.


	CLASS_ATTR_SYM(c, "gracedur", 0, t_notation_obj, grace_note_equivalent_sym);
	CLASS_ATTR_STYLE_LABEL(c,"gracedur",0,"text","1/8 Grace Note Playback Duration");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"gracedur", 0,"1/64");
	CLASS_ATTR_ACCESSORS(c, "gracedur", (method)NULL, (method)score_setattr_gracedur);
	// @description Sets the playback equivalent duration of an eighth grace note, as a symbolic duration (i.e. fraction of the whole note).
	// By default, a 1/8 grace note is played as a 1/64 ordinary note. This attribute is in any case tempered by the <m>gracemaxperc</m> attribute.

	CLASS_ATTR_DOUBLE(c, "gracemaxperc", 0, t_notation_obj, max_percentage_of_chord_taken_by_grace_notes); 
	CLASS_ATTR_STYLE_LABEL(c,"gracemaxperc",0,"text","Maximum Playback Percentage Of Chord Taken By Grace Notes");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"gracemaxperc", 0,"90");
	CLASS_ATTR_ACCESSORS(c, "gracemaxperc", (method)NULL, (method)score_setattr_gracemaxperc);
	// @description Sets the maximum playback percentage of the duration of a note which can be taken by grace notes of the following chord.
	// If more than such duration is taken by following grace notes, the duration of each grace note is decreased (and will not be equal to <m>gracedur</m> any more) 
	// so that this percentage is always respected.

	CLASS_STICKY_ATTR_CLEAR(c, "category");

	
	CLASS_STICKY_ATTR(c,"category",0,"Rhythm");

	CLASS_ATTR_CHAR(c,"treehandling",0, t_notation_obj, tree_handling);
	CLASS_ATTR_STYLE_LABEL(c,"treehandling",0,"enum","Rhythmic Trees Handling");
	CLASS_ATTR_ENUMINDEX(c,"treehandling", 0, "Take For Granted Refine Only Graphically Refine Process Ignore");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"treehandling",0,"3");
	CLASS_ATTR_ACCESSORS(c, "treehandling", (method)NULL, (method)score_setattr_treehandling);
	// @description Sets the way in which rhythmic trees are interpreted. Some options are available, from the less invasive to the most invasive: <br />
	// - Take For Granted: trees are taken as they are, and essentially left unchanged (unless <m>maketreecompatiblewithts</m> is set). <br />
	// - Refine Only Graphically: see "Refine", but the refined levels are only displayed graphically and never output upon <m>dump</m>. <br />
	// - Refine: tree levels can be refined internally. This option tells bach.score that the tree the user inserts may not be the actual tree, 
	// but a precise suggestion, and score is allowed to create additional beaming or tuplet levels inside the already existing levels, without 
	// destroying or incorporating existing levels into new ones. <br />
	// - Process (default): tree levels can be refined internally and in some occasion wrapped or merged. This option tells bach.score that the tree 
	// the user inserts may not be the actual tree, but rather a quite loose suggestion: the algorithm is allowed to add levels if needed 
	// (even if they wrap existing levels), and in some particular cases to merge levels as well. <br />
	// - Ignore: all tree levels are ignored: the tree the user has inserted is completely by the algorithm, which is in charge to rebuild 
	// it automatically, from scratch.

	CLASS_ATTR_CHAR(c,"leveltobeam",0, t_notation_obj, tree_to_beaming_correspondence);
	CLASS_ATTR_STYLE_LABEL(c,"leveltobeam",0,"enum","Tree Levels To Beams Conversion");
	CLASS_ATTR_ENUMINDEX(c,"leveltobeam", 0, "Break Beams Around Levels Break Beams Around Tuplets Levels Can Share Beams");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"leveltobeam",0,"0");
	CLASS_ATTR_ACCESSORS(c, "leveltobeam", (method)NULL, (method)score_setattr_leveltobeam);
	// @description Sets the way in which levels in rhythmic trees are to be converted into beams. <br />
	// - Break Beams Around Levels (default): different levels will be identifiable by different beaming lines: at least one beaming line will be
	// broken from one level the following one. <br />
	// - Break Beams Arount Tuplet: as the previous one, but only valid for tuplet levels: tuplets will be thus left identifiable by one or more
	// specific beams, but non-tuplet levels can share the same beams. <br />
	// - Levels Can Share Beams: levels are never identified by specific beams: beams can start and end across levels, rather independently from them.

	CLASS_ATTR_CHAR(c,"breakrootlevelbeam",0, t_notation_obj, always_break_first_level_beams);
	CLASS_ATTR_STYLE_LABEL(c,"breakrootlevelbeam",0,"onoff","Always Break Root Level Beam");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"breakrootlevelbeam",0,"1");
	CLASS_ATTR_ACCESSORS(c, "breakrootlevelbeam", (method)NULL, (method)score_setattr_breakrootlevelbeam);
	// @description If this is set to 1 (default), no beams will be allowed to exist at the root level of the rhythmic tree of a measure.
	
	CLASS_ATTR_CHAR(c,"reducetreeidlelevels",0, t_notation_obj, always_reduce_tree_idle_levels);
	CLASS_ATTR_STYLE_LABEL(c,"reducetreeidlelevels",0,"onoff","Always Reduce Rhythmic Tree Idle Levels");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"reducetreeidlelevels",0,"1");
	// @description Toggles the ability to reduce automatically the idle, empty or unnecessary levels of rhythmic trees.
	
	CLASS_ATTR_CHAR(c,"maketreecompatiblewithts",0, t_notation_obj, make_tree_compatible_with_ts);
	CLASS_ATTR_STYLE_LABEL(c,"maketreecompatiblewithts",0,"onoff","Make Rhythmic Tree Compatible With Time Signature");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"maketreecompatiblewithts",0,"1");
	CLASS_ATTR_ACCESSORS(c, "maketreecompatiblewithts", (method)NULL, (method)score_setattr_maketreecompatiblewithts);
	// @description Toggles the ability to always make rhythmic trees compatible with time signatures. In other words,
	// it always applies a preliminar boxing of the rhythmic tree into a certain number of boxes, each corresponding to a beat.
	// For instance, in a 4/4 time signature, if this is set to 1, any tree will be first of all split into four branches,
	// one for each beat. If you use as <m>treehandling</m> the Take For Granted option, you might want to set this attribute to 0,
	// otherwise in any case trees will be parsed and modified according to the time signatures.   
	
	CLASS_ATTR_CHAR(c,"autoparserhythm",0, t_notation_obj, autoparse_rhythms);
	CLASS_ATTR_STYLE_LABEL(c,"autoparserhythm",0,"onoff","Parse Rhythm Automatically");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"autoparserhythm", 0, "1");
	CLASS_ATTR_ACCESSORS(c, "autoparserhythm", (method)NULL, (method)score_setattr_autoparserhythm);
	// @description Toggles the ability to automatically parse the rhythm inside each rhythmic tree level, e.g. rests and completely
	// tied chords are automatically tried to be merged.
	
	CLASS_ATTR_CHAR(c,"outputtrees",0, t_notation_obj, output_trees);
	CLASS_ATTR_STYLE_LABEL(c,"outputtrees",0,"enumindex","Output Rhythmic Trees Upon Dump");
	CLASS_ATTR_ENUMINDEX(c,"outputtrees", 0, "Never First Outlet Only All Outlets");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"outputtrees",0,"1");
	CLASS_ATTR_ACCESSORS(c, "outputtrees", (method)NULL, (method)score_setattr_outputtrees);
	// @description Sets the way in which rhythmic trees are output: <br />
	// - Never: rhythmic trees are never output from any of the outlets. <br />
	// - First Outlet Only (default): rhythmic trees are only output from the first outlet of <o>bach.score</o> (the one 
	// with the score gathered syntax), but never from any of the other outlets. <br />
	// - All Outlets: rhythmic trees are output from the first outlet and also from all the separate parameter outlets. <br />
	// Outputting rhythmic trees means essentially that the given llll containing the data (e.g. durations or velocities) is reshaped 
	// according to the shape of the rhythmic tree. Also see the <m>outputtiesindurationtree</m> attribute.

	CLASS_ATTR_CHAR(c,"outputtiesindurationtree",0, t_notation_obj, output_full_duration_tree);
	CLASS_ATTR_STYLE_LABEL(c,"outputtiesindurationtree",0,"onoff","Output Ties In Duration Tree");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"outputtiesindurationtree",0,"1");
	// @description Toggles the ability to output a symbol "t" to identify ties in the tree of durations.
	// This attribute is active only if <m>outputtrees</m> is set to "All Outlets": in this case the duration outlet will
	// output durations structured as the rhythmic tree. If this attribute is set to 1, after each note which starts a tie,
	// a "t" symbol will be put, identifying such tie. For instance: 
	// <b>( ( ( 1/8 t ( 1/24 1/24 1/24 ) ) ( ( ( -1/16 1/64 t ) ( 1/64 1/64 1/64 t ) ) ( 1/16 1/16 ) ) -1/2 ) )</b>.

	CLASS_ATTR_CHAR(c,"writetrees",0, t_notation_obj, write_trees);
	CLASS_ATTR_STYLE_LABEL(c,"writetrees",0,"onoff","Incorporate Tree Information Upon Write or Writetxt");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"writetrees",0,"1");
	// @description Toggles the saving of rhythmic tree levels inside the files created after a <m>write</m> or <m>writetxt</m>
	// message. By default this is active. 

	CLASS_ATTR_CHAR(c,"saveleveltypes",0, t_notation_obj, output_and_save_level_types);
	CLASS_ATTR_STYLE_LABEL(c,"saveleveltypes",0,"onoff","Output And Save Level Types");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"saveleveltypes",0,"1");
	// @description Toggles the saving and output of level type information. 
	// If this attribute is set to 1 (default), and <m>outputtrees</m> is not set on "Never", level type information of each level is output
	// within the level from the first outlet (gathered syntax). Also such information is stored inside the lllls created 
	// after a <m>write</m> or <m>writetxt</m> message. 
	// @copy BACH_DOC_SCORE_LEVELTYPE
	
	CLASS_ATTR_CHAR(c,"jointuplets",0, t_notation_obj, try_to_join_tuplets);
	CLASS_ATTR_STYLE_LABEL(c,"jointuplets",0,"onoff","Join Tuplets When Possible");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"jointuplets", 0, "1");
	CLASS_ATTR_FILTER_CLIP(c,"jointuplets", 0, 1);
	CLASS_ATTR_ACCESSORS(c, "jointuplets", (method)NULL, (method)score_setattr_jointuplets);
	// @description Toggles the ability to join tuplets automatically when possible. 
	// For instance, it will to gather two adjacent triplets into a single 6-plet.
	// This is handy in most situations; for instance if we had a rhythm whose duration tree looked like
	// <b>((1/24 1/24 1/24) (1/24 1/24 1/24))</b>, the algorithm would join the two triplets of sixteenth notes into
	// a single sextuplet <b>(1/24 1/24 1/24 1/24 1/24 1/24)</b>. 
	// Such joining is performed by taking care of delimiting beat levels (e.g. in a 4/4 time signature, eighth triplets are never 
	// joined into 6-plets).

	CLASS_ATTR_CHAR(c,"simplifytuplets",0, t_notation_obj, try_to_make_tuplets_simple);
	CLASS_ATTR_STYLE_LABEL(c,"simplifytuplets",0,"onoff","Make Tuplets Simpler When Possible");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"simplifytuplets", 0, "1");
	CLASS_ATTR_FILTER_CLIP(c,"simplifytuplets", 0, 1);
	CLASS_ATTR_ACCESSORS(c, "simplifytuplets", (method)NULL, (method)score_setattr_simplifytuplets);	
	// @description Toggles the ability to parse adjacent tuplets into simple ones, having bigger units. 
	// For instance, if we had a rhythm whose duration tree looked like <b>(1/6 1/12 t) (1/12 1/6)</b>,
	// where "t" represents a tie, the algorithm would merge the two tuplets of eighth notes into a bigger
	// tuplet of quarter notes: <b>(1/6 1/6 1/6)</b>.

	CLASS_ATTR_CHAR(c,"beamgracesequences",0, t_notation_obj, try_to_beam_grace_sequences);
	CLASS_ATTR_STYLE_LABEL(c,"beamgracesequences",0,"onoff","Beam Grace Sequences When Possible");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"beamgracesequences", 0, "1");
	CLASS_ATTR_FILTER_CLIP(c,"beamgracesequences", 0, 1);
	CLASS_ATTR_ACCESSORS(c, "beamgracesequences", (method)NULL, (method)score_setattr_beamgracesequences);	
	// @description Toggles the ability to beam sequences of grace chords, when possible.
	
	CLASS_ATTR_CHAR(c,"admitdottedtuplets",0, t_notation_obj, admit_dotted_tuplets);
	CLASS_ATTR_STYLE_LABEL(c,"admitdottedtuplets",0,"onoff","Admit Dotted Tuplets");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"admitdottedtuplets", 0, "1");
	CLASS_ATTR_ACCESSORS(c, "admitdottedtuplets", (method)NULL, (method)score_setattr_admitdottedtuplets);
	// @description Toggles the possibility to have dotted tuplets in the score.
	// The number of dots, in any case, has to respect the <m>maxdots</m> attribute.

	CLASS_ATTR_CHAR(c,"allowbeaming",0, t_notation_obj, allow_beaming);
	CLASS_ATTR_STYLE_LABEL(c,"allowbeaming",0,"onoff","Allow Beaming");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"allowbeaming", 0, "1");
	CLASS_ATTR_ACCESSORS(c, "allowbeaming", (method)NULL, (method)score_setattr_allowbeaming);
	// @description Toggles the possibility to beam chords. By default, of course, this is active.
	

	// OBSOLETE, no more supported, kept for legacy
	CLASS_ATTR_DOUBLE(c,"maxbeamslope", 0, t_notation_obj, max_beam_slope);
	CLASS_ATTR_STYLE_LABEL(c,"maxbeamslope", 0, "text", "Max Beam Slope");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"maxbeamslope", 0, "0.08");
	CLASS_ATTR_ACCESSORS(c, "maxbeamslope", (method)NULL, (method)score_setattr_maxbeamslope);
	CLASS_ATTR_INVISIBLE(c, "maxbeamslope", ATTR_GET_OPAQUE | ATTR_SET_OPAQUE); 
	// @exclude bach.score
	
	CLASS_ATTR_CHAR(c,"constraintbeamsinspaces",0, t_notation_obj, constraint_beam_start_end_in_spaces);
	CLASS_ATTR_STYLE_LABEL(c,"constraintbeamsinspaces",0,"onoff","Constrain Beams In Spaces");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"constraintbeamsinspaces", 0, "1");
	CLASS_ATTR_ACCESSORS(c, "constraintbeamsinspaces", (method)NULL, (method)score_setattr_constraintbeamsinspaces);
	// @description If this is set to 1, beams are constraint to lie on privileged position inside 
	// the staff lines. If this is 0, the beams position only depends on stems height. 
	// This attribute should be active for a better display result.

	CLASS_ATTR_CHAR(c,"restswithinbeaming",0, t_notation_obj, rests_within_beaming);
	CLASS_ATTR_STYLE_LABEL(c,"restswithinbeaming",0,"enumindex","Rests Can Be Within Beaming");
	CLASS_ATTR_ENUMINDEX(c,"restswithinbeaming", 0, "Never Only Internally Always");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"restswithinbeaming", 0, "1");
	CLASS_ATTR_ACCESSORS(c, "restswithinbeaming", (method)NULL, (method)score_setattr_restswithinbeaming);
	// @description Handles the possibility that rests be incorporated inside (under or over) beams: <br />
	// - Never: rests are never to be incorporated inside a beam; the beam should be rather broken. <br /> 
	// - Only Internally (default): rests can stay inside a beam only if they are not at the beginning or at the end of the beam. <br /> 
	// - Always: rests can alway stay inside beams. <br /> 

	CLASS_ATTR_CHAR(c,"extendbeamsoverrests",0, t_notation_obj, extend_beams_over_rests);
	CLASS_ATTR_STYLE_LABEL(c,"extendbeamsoverrests",0,"enumindex","Extend Beams Over Rests");
	CLASS_ATTR_ENUMINDEX(c,"extendbeamsoverrests", 0, "First Beam Only All Beams Up To Figure All Beams");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"extendbeamsoverrests", 0, "1");
	CLASS_ATTR_ACCESSORS(c, "extendbeamsoverrests", (method)NULL, (method)score_setattr_extendbeamsoverrests);
	// @description Handles the number of beams to be exended over rests when these are incorporated inside the beam (i.e.
	// the <m>restswithinbeaming</m> attribute must not be set on Never): <br />
	// - Only First Beam: only one beam has to be extended over the rests, all other possible beams will be broken. <br /> 
	// - All Beams Up To Figure: the number of beams which can be extended depends on the duration of the rest, namely from the
	// number of beams such rest had if it were a note. For instance, over a 1/16 rest we would be able to extend up to 2 beams, but not 3;
	// over a 1/64 rest up to 4 beams would be extended, but not 5, and so on. <br />
	// - All Beams: always extend all possible beams over rests.
	
	CLASS_ATTR_DOUBLE(c,"syncopationasymratio", 0, t_notation_obj, syncopation_asym_ratio);
	CLASS_ATTR_STYLE_LABEL(c,"syncopationasymratio", 0, "text", "Syncopation Max Asym-Ratio");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"syncopationasymratio", 0, "1.");
	CLASS_ATTR_ACCESSORS(c, "syncopationasymratio", (method)NULL, (method)score_setattr_syncopationasymratio);
	// @description Handles the display of syncopations depending on the asymmetricity of their sides. 
	// It is the maximum allowed ratio between the durations of the longest and the shortest side of the syncopation, 
	// in order for it to be a merged syncopation. 
	// By default this is 1., which means that only symmetric syncopation are admitted, i.e. the ones where both 
	// sides have the same duration. <br />
	// @copy BACH_DOC_SCORE_SYNCOPATION
	
	CLASS_ATTR_DOUBLE(c,"syncopationposdurratio", 0, t_notation_obj, syncopation_pos_dur_ratio);
	CLASS_ATTR_STYLE_LABEL(c,"syncopationposdurratio", 0, "text", "Syncopation Max Dur/Pos-Ratio");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"syncopationposdurratio", 0, "2.5");
	CLASS_ATTR_ACCESSORS(c, "syncopationposdurratio", (method)NULL, (method)score_setattr_syncopationposdurratio);
	// @description Handles the display of syncopation depending on their position and their left side duration. 
	// It is the maximum allowed ratio between the syncopation position, with numerator forcedly set to 1, and the 
	// duration of the syncopation left side, in order for a syncopation to be a merged syncopation. This might 
	// appear complicated: if so, just leave it to the default value of 2.5. However, the idea is that this ratio 
	// somehow tells "how simple" the syncopation is. <br />
	// @copy BACH_DOC_SCORE_SYNCOPATION
	
	CLASS_STICKY_ATTR_CLEAR(c, "category");
	
	
	CLASS_STICKY_ATTR(c,"category",0,"Edit");

	CLASS_ATTR_CHAR(c,"allowbarlinelock",0, t_notation_obj, allow_barline_lock);
	CLASS_ATTR_STYLE_LABEL(c,"allowbarlinelock",0,"onoff","Allow Barline Lock");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"allowbarlinelock", 0, "1");	
	// @exclude bach.slot
	// @description Toggles the ability to lock barlines (i.e. to fix measure widths).
	
	CLASS_STICKY_ATTR_CLEAR(c, "category");

	
	CLASS_STICKY_ATTR(c,"category",0,"Font");
	
	CLASS_ATTR_SYM(c,"notationfont", 0, t_notation_obj, noteheads_font);
	CLASS_ATTR_ALIAS(c,"notationfont", "ntfont");
	CLASS_ATTR_STYLE_LABEL(c, "notationfont",0,"font","Notation Font");
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c,"notationfont",0,"\"November for bach\"");
	CLASS_ATTR_ACCESSORS(c, "notationfont", (method)NULL, (method)score_set_noteheads_font);
	// @description @copy BACH_DOC_NOTATION_FONT

	CLASS_ATTR_SYM(c,"accidentalsfont", 0, t_notation_obj, accidentals_font);
	CLASS_ATTR_ALIAS(c,"accidentalsfont", "acfont");
	CLASS_ATTR_STYLE_LABEL(c, "accidentalsfont", 0, "font", "Accidentals Font"); 
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c,"accidentalsfont", 0, "\"November for bach\"");
	CLASS_ATTR_ACCESSORS(c, "accidentalsfont", (method)NULL, (method)score_set_accidentals_font);
	// @description @copy BACH_DOC_ACCIDENTALS_FONT

    CLASS_ATTR_SYM(c,"articulationsfont", 0, t_notation_obj, articulations_font);
    CLASS_ATTR_ALIAS(c,"articulationsfont", "acfont");
    CLASS_ATTR_STYLE_LABEL(c, "articulationsfont", 0, "font", "Articulations Font");
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c,"articulationsfont", 0, "\"November for bach\"");
    CLASS_ATTR_ACCESSORS(c, "articulationsfont", (method)NULL, (method)score_set_articulations_font);
    // @description @copy BACH_DOC_ARTICULATIONS_FONT

	CLASS_ATTR_DOUBLE(c,"measurenumberfontsize",0, t_notation_obj, measure_numbers_font_size);
	CLASS_ATTR_STYLE_LABEL(c,"measurenumberfontsize",0,"text","Measure Numbers Font Size");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"measurenumberfontsize", 0, "9");
	// @description Sets the font size with which measure numbers are displayed. 
	
	CLASS_STICKY_ATTR_CLEAR(c, "category");
	
	

	CLASS_STICKY_ATTR(c,"category",0,"Show");

    CLASS_ATTR_CHAR(c,"showtempointerpline",0, t_notation_obj, show_tempi_interp_line);
    CLASS_ATTR_STYLE_LABEL(c,"showtempointerpline",0,"onoff","Show Tempi Interpolation Dashed Line");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showtempointerpline", 0, "1");
    // @description Toggles the display of the dashed line for "rall." and "acc." specifications.

    CLASS_ATTR_CHAR(c,"showstems",0, t_notation_obj, show_stems);
    CLASS_ATTR_STYLE_LABEL(c,"showstems",0,"enumindex","Show Stems");
    CLASS_ATTR_ENUMINDEX(c,"showstems", 0, "Don't Main Main And Auxiliary");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showstems", 0, "2");
    CLASS_ATTR_ACCESSORS(c, "showstems", (method)NULL, (method)score_setattr_showstems);
    // @description Toggles the display of the chord stems. Values are: 0 = don't show,
    // 1 = show main stem, 2 = show both main and auxiliary unison stems (default).

    CLASS_ATTR_CHAR(c,"showstemsforbeamedrests",0, t_notation_obj, show_half_stem_for_beamed_rests);
    CLASS_ATTR_STYLE_LABEL(c,"showstemsforbeamedrests",0,"onoff","Show Stems For Beamed Rests");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showstemsforbeamedrests", 0, "1");
    CLASS_ATTR_BASIC(c,"showstemsforbeamedrests",0);
    // @description Toggles the display of the stems for rests under a beam.

    CLASS_ATTR_CHAR(c,"showflags",0, t_notation_obj, show_flags);
    CLASS_ATTR_STYLE_LABEL(c,"showflags",0,"onoff","Show Chord Flags");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showflags",0,"1");
    // @description Toggles the display of chord flags.

    CLASS_ATTR_CHAR(c,"showbeams",0, t_notation_obj, show_beams);
    CLASS_ATTR_STYLE_LABEL(c,"showbeams",0,"onoff","Show Beams");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showbeams",0,"1");
    // @description Toggles the display of beams.

    CLASS_ATTR_CHAR(c,"showties",0, t_notation_obj, show_ties);
    CLASS_ATTR_STYLE_LABEL(c,"showties",0,"onoff","Show Ties");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showties",0,"1");
    // @description Toggles the display of ties.

    CLASS_ATTR_CHAR(c,"showdots",0, t_notation_obj, show_dots);
    CLASS_ATTR_STYLE_LABEL(c,"showdots",0,"onoff","Show Augmentation Dots");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showdots",0,"1");
    // @description Toggles the display of augmentation dots.
    
    CLASS_ATTR_CHAR(c,"showrests",0, t_notation_obj, show_rests);
    CLASS_ATTR_STYLE_LABEL(c,"showrests",0,"enumindex","Show Rests");
    CLASS_ATTR_ENUMINDEX(c,"showrests", 0, "Never Not In Empty Measures Always");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showrests", 0, "2");
    // @description Toggles the display of the rests. Values are: 0 = never show,
    // 1 = show rests except when they are in empty measures, 2 = always show rests



	CLASS_ATTR_CHAR(c,"showslurs",0, t_notation_obj, show_slurs);
	CLASS_ATTR_STYLE_LABEL(c,"showslurs",0,"onoff","Show Slurs");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showslurs",0,"1");
	CLASS_ATTR_INVISIBLE(c, "showslurs", ATTR_GET_OPAQUE | ATTR_SET_OPAQUE); // just for now
	// @exclude bach.score
	// @description Toggles the display of slurs.
	
	CLASS_ATTR_CHAR(c,"showlyricswordextensions",0, t_notation_obj, show_lyrics_word_extensions);
	CLASS_ATTR_STYLE_LABEL(c,"showlyricswordextensions",0,"onoff","Show Lyrics Word Extensions");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showlyricswordextensions",0,"1");
	// @description Toggles the display of word extensions for lyrics. Word extensions are the horizontal
	// lines which extend a syllable over chords which don't have any defined syllable.
	
	CLASS_ATTR_CHAR(c,"showtempi",0, t_notation_obj, show_tempi);
	CLASS_ATTR_STYLE_LABEL(c,"showtempi",0,"onoff","Show Tempi");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showtempi", 0, "1");
	// @description Toggles the display of tempi.

	CLASS_ATTR_NOTATIONOBJ_CHARPTR(c, "showmeasurenumbers", 0, show_measure_numbers, CONST_MAX_VOICES, score_setattr_showmeasurenumbers);
	CLASS_ATTR_STYLE_LABEL(c,"showmeasurenumbers",0,"text","Show Measure Numbers");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showmeasurenumbers",0,"1");
	// @description Toggles the display of measure numbers. A 0/1 integer for each voice is expected: 0 means that the measure numbers
	// must be hidden, 1 means that the measure numbers will be shown for the given voice. If <m>drawbarlinesacrossstaves</m> is 1 and 
	// barlines are drawn across the staves, in any case the measure numbers can only be shown for the first voice.
	
	CLASS_ATTR_CHAR(c,"showbarlines", 0, t_notation_obj, show_barlines);
	CLASS_ATTR_STYLE_LABEL(c,"showbarlines", 0, "onoff", "Show Barlines");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showbarlines", 0, "1");
	// @description Toggles the display of barlines.
	
	CLASS_ATTR_CHAR(c,"showbarlinelocks", 0, t_notation_obj, show_barline_locks);
	CLASS_ATTR_STYLE_LABEL(c,"showbarlinelocks", 0, "onoff", "Show Barline Locks");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showbarlinelocks", 0, "1");
	// @description Toggles the display of barlines locks.

	CLASS_ATTR_CHAR(c,"showrhythmictreelocks", 0, t_notation_obj, show_rhythmic_tree_locks);
	CLASS_ATTR_STYLE_LABEL(c,"showrhythmictreelocks", 0, "onoff", "Show Rhythmic Tree Locks");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showrhythmictreelocks", 0, "1");
	// @description Toggles the display of rhythmic tree locks (over the barlines).
	
	CLASS_ATTR_CHAR(c,"showrhythmictree",0, t_notation_obj, show_rhythmic_tree);
	CLASS_ATTR_STYLE_LABEL(c,"showrhythmictree",0,"onoff","Show Rhythmic Trees");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showrhythmictree",0,"0");
	// @description Toggles the display of the rhythmic tree as lines ramifying over the staff.
	// Dashed lines correspond to tuplets, normal lines correspond to any other level type.
	// Red lines correspond to original levels, green lines correspond to beat levels (also called time-signature levels),
	// blue lines correspond to refined levels, violet lines correspond to display-only levels.
			
	CLASS_ATTR_CHAR(c,"showtimesignatures", 0, t_notation_obj, show_time_signatures);
	CLASS_ATTR_STYLE_LABEL(c,"showtimesignatures", 0, "onoff", "Show Time Signatures");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showtimesignatures", 0, "1");
	CLASS_ATTR_ACCESSORS(c, "showtimesignatures", (method)NULL, (method)score_setattr_showtimesignatures);
	// @description Toggles the display of time signatures.

	CLASS_STICKY_ATTR_CLEAR(c, "category");


	
	
	CLASS_STICKY_ATTR(c,"category",0,"Appearance");

	CLASS_ATTR_NOTATIONOBJ_DBLPTR(c, "voicespacing", 0, voiceuspacing_as_floatlist, CONST_MAX_VOICES + 1, score_setattr_voicespacing);
	CLASS_ATTR_STYLE_LABEL(c,"voicespacing",0,"text","Voice Spacing");
	CLASS_ATTR_SAVE(c,"voicespacing",0);
	CLASS_ATTR_PAINT(c,"voicespacing",0);
	// @description @copy BACH_DOC_VOICESPACING

	CLASS_ATTR_NOTATIONOBJ_CHARPTR(c, "hidevoices", 0, hidevoices_as_charlist, CONST_MAX_VOICES, score_setattr_hidevoices);
	CLASS_ATTR_STYLE_LABEL(c,"hidevoices",0,"text","Hide Voices");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"hidevoices",0,"0");
	// @description Decide which voices must be shown or hidden. A list of 0/1 integers is expected, one for each voice.
	// 0 means that the voice is visibile, 1 means that it is hidden.
	// If less symbols are entered, the other elements are considered to be 1.
	// Hiding voices does not affect the spacing of existing voices, which will continue to take into account the hidden voices.
	
	CLASS_ATTR_DOUBLE(c,"zoom",0, t_notation_obj, horizontal_zoom);
	CLASS_ATTR_STYLE_LABEL(c,"zoom",0,"text","Horizontal Zoom %");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"zoom",0,"100.");
	CLASS_ATTR_ACCESSORS(c, "zoom", (method)NULL, (method)score_setattr_zoom);
	CLASS_ATTR_BASIC(c,"zoom",0);
	// @description @copy BACH_DOC_ZOOM

	CLASS_ATTR_ATOM(c,"vzoom",0, t_notation_obj, vertical_zoom);
	CLASS_ATTR_STYLE_LABEL(c,"vzoom",0,"text","Vertical Zoom %");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"vzoom",0,"Auto");
	CLASS_ATTR_ACCESSORS(c, "vzoom", (method)NULL, (method)score_setattr_vzoom);
	CLASS_ATTR_BASIC(c,"vzoom",0);
	// @description @copy BACH_DOC_VZOOM

	CLASS_ATTR_CHAR(c,"showsynchronoustempionce",0, t_notation_obj, hide_tempi_when_equal_on_all_voices);
	CLASS_ATTR_STYLE_LABEL(c,"showsynchronoustempionce",0,"onoff","Show Synchronous Tempi Once");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showsynchronoustempionce",0,"1");
	CLASS_ATTR_ACCESSORS(c, "showsynchronoustempionce", (method)NULL, (method)score_setattr_showsynchronoustempionce);
	// @description Toggles the ability to display tempi which are synchronous in all voices just for the first voice.
	// More precisely: if a tempo is defined in all voices in the same way at a given point of the score, it will be only
	// shown on the upper voice. 
	
	CLASS_STICKY_ATTR_CLEAR(c, "category");

	
	CLASS_STICKY_ATTR(c,"category",0,"Behavior");

	CLASS_ATTR_CHAR(c,"changecentstomeasuresbydragging",0, t_notation_obj, change_cents_to_measures_by_dragging);
	CLASS_ATTR_STYLE_LABEL(c,"changecentstomeasuresbydragging",0,"onoff","Allow Whole Measure Dragging");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"changecentstomeasuresbydragging", 0, "0");
	// @description Toggles the ability to change pitches of notes inside the measure when a measure is selected and dragged up/down.
	// By default this is off: in order to change pitches of notes you have the select the notes, and not their measures. 
	
    CLASS_ATTR_CHAR(c,"tieassignspitch",0, t_notation_obj, tie_assign_pitch);
    CLASS_ATTR_STYLE_LABEL(c,"tieassignspitch",0,"onoff","Ties Assign Pitches");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"tieassignspitch", 0, "0");
    CLASS_ATTR_ACCESSORS(c, "tieassignspitch", (method)NULL, (method)score_setattr_tieassignspitch);
    // @description If this is set to 1, when a tie is put from a note to a second one, the midicents of the second one are forcedly made coincide
    // with the cents of the first one. Of course, the screen representation of the two notes must be the same, in order to tie them.
    // For instance, if you tie two middle C's, one having 6005mc, and the second one having 5996mc, the second one is given a pitch of 6005mc.
    // By default this is not the case (this attribute is off), i.e.: ties don't assign pitches.

    /*	CLASS_ATTR_CHAR(c,"oldtpsyntax", 0, t_notation_obj, old_timepoint_syntax_bw_compatibility);
     CLASS_ATTR_STYLE_LABEL(c,"oldtpsyntax",0,"onoff","Keep Old Timepoint Syntax");
     CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"oldtpsyntax", 0, "0");
     CLASS_ATTR_INVISIBLE(c, "oldtpsyntax", ATTR_GET_OPAQUE | ATTR_SET_OPAQUE); // hidden, just in case
     // @exclude bach.score
     */
    

    CLASS_STICKY_ATTR_CLEAR(c, "category");

    
    CLASS_STICKY_ATTR(c,"category",0,"Play");

    CLASS_ATTR_CHAR(c,"playtiedelementsseparately", 0, t_notation_obj, play_tied_elements_separately);
	CLASS_ATTR_STYLE_LABEL(c,"playtiedelementsseparately",0,"onoff","Play Tied Elements Separately");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"playtiedelementsseparately", 0, "0");
	// @description Toggles the ability to play tied notes (if <m>playmode</m> is Notewise) or completely-tied chords (if <m>playmode</m> is Chordwise)
	// separately, instead of merging all tied sequence into one longer note or chord and play it (i.e. send it through the playout).
	// If you set this attribute to 1, you might also want to change the <m>singleslotfortiednotes</m> slotinfo field.

	CLASS_ATTR_CHAR(c,"playrestsseparately", 0, t_notation_obj, play_rests_separately);
	CLASS_ATTR_STYLE_LABEL(c,"playrestsseparately",0,"onoff","Play Rests Separately");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"playrestsseparately", 0, "0");
	// @description Toggles the ability to play sequences of rests (if <m>playrest</m> is active)
	// separately, instead of merging all rests sequence into one longer rest.
	
	
	CLASS_STICKY_ATTR_CLEAR(c, "category");

	//
		
	CLASS_STICKY_ATTR(c,"category",0,"Notation");

    
        CLASS_ATTR_CHAR(c,"centeredwholerests",0, t_notation_obj, whole_rests_in_empty_measures);
        CLASS_ATTR_STYLE_LABEL(c,"centeredwholerests",0,"onoff","Centered Whole Rests In Empty Measures");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"centeredwholerests", 0, "1");
        CLASS_ATTR_ACCESSORS(c, "centeredwholerests", (method)NULL, (method)score_setattr_centeredwholerests);
        // @description Toggles the ability to represent empty measures, or measures having a single rest spanning the whole measure,
        // with centered whole rests. Default is 1.

    
		CLASS_ATTR_CHAR(c,"maxdots",0, t_notation_obj, max_dots);
		CLASS_ATTR_STYLE_LABEL(c,"maxdots",0,"text","Maximum Number Of Dots");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"maxdots", 0, "1");
		CLASS_ATTR_ACCESSORS(c, "maxdots", (method)NULL, (method)score_setattr_maxdots);
		// @description Sets the maximum number of dots to be used while notating music. Default is 1.

		CLASS_ATTR_CHAR(c,"showaccidentalstiepreferences",0, t_notation_obj, show_accidentals_tie_preferences);
		CLASS_ATTR_STYLE_LABEL(c,"showaccidentalstiepreferences",0,"enumindex","Accidental Upon Tie");
		CLASS_ATTR_ENUMINDEX(c,"showaccidentalstiepreferences", 0, "Never At Measure Beginning Always");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showaccidentalstiepreferences", 0, "0");
		CLASS_ATTR_ACCESSORS(c, "showaccidentalstiepreferences", (method)NULL, (method)score_setattr_showaccidentalstiepreferences);
		// @description Handles the display of accidentals with respect to tied notes: <br />
		// - Never (default): never display accidentals for notes where a tie is ending. <br />
		// - At Measure Beginning: display accidentals for notes where a tie is ending only if such notes are at the very beginning of a measure (i.e. if the tie
		// is across the measure barline). <br />
        // - Always: always display accidentals for notes where a tie is ending. This only works if <m>showaccidentalspreferences</m> is not set to Classically,
        // or if cautionary accidentals are allowed; otherwise this will essentially coincide with the next option. <br />

		CLASS_ATTR_CHAR(c,"cautionaryaccidentals",0, t_notation_obj, cautionary_accidentals);
		CLASS_ATTR_STYLE_LABEL(c,"cautionaryaccidentals",0,"enumindex","Show Cautionary Accidentals");
		CLASS_ATTR_ENUMINDEX(c,"cautionaryaccidentals", 0, "Never Same Octave Any Octave");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"cautionaryaccidentals", 0, "0");
		CLASS_ATTR_ACCESSORS(c, "cautionaryaccidentals", (method)NULL, (method)score_setattr_cautionaryaccidentals);
		// @description Handles the display of cautionary accidentals: <br />
		// - Never (default): never display cautionary accidentals. <br />
		// - Same Octave: display cautionary accidentals if notes are the same and in the same octave. <br />
		// - Any Octave: display cautionary accidentals if notes are the same, independently from their octave.

		CLASS_ATTR_LONG(c,"cautionaryaccidentalsdecay", 0, t_notation_obj, cautionary_accidentals_decay);
		CLASS_ATTR_STYLE_LABEL(c,"cautionaryaccidentalsdecay", 0, "text", "Cautionary Accidentals Decay");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"cautionaryaccidentalsdecay", 0, "3");
		CLASS_ATTR_ACCESSORS(c, "cautionaryaccidentalsdecay", (method)NULL, (method)score_setattr_cautionaryaccidentalsdecay);
		// @description Sets the scope of the cautionary naturals, 
		// It tells after how many chords we no longer bother to put a cautionary natural.
		// By default, if <m>cautionaryaccidentals</m> is not set on never, cautionary naturals are dropped if 3 or more chords have passed after the last occurrence of a given non-natural note.

		CLASS_ATTR_LONG(c,"cautionaryaccidentalsremind", 0, t_notation_obj, cautionary_accidentals_remind);
		CLASS_ATTR_STYLE_LABEL(c,"cautionaryaccidentalsremind", 0, "text", "Cautionary Accidentals Remind");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"cautionaryaccidentalsremind", 0, "10");
		CLASS_ATTR_ACCESSORS(c, "cautionaryaccidentalsremind", (method)NULL, (method)score_setattr_cautionaryaccidentalsremind);
		// @description Sets the scope of the cautionary accidentals. It represents the minimum number of chords that have to be in between 
		// two equal non-natural notes in order for the accidental of the second note to be cautionary reminded. 
		// By default, if <m>cautionaryaccidentals</m> is not set on never, accidentals are reminded if more than 10 chords have passed.

		CLASS_ATTR_CHAR(c,"spacingtype", 0, t_notation_obj, spacing_type);
		CLASS_ATTR_STYLE_LABEL(c,"spacingtype", 0, "enumindex", "Spacing Type");
		CLASS_ATTR_ENUMINDEX(c,"spacingtype", 0, "Smart Note/Time Signature Proportional Spring");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"spacingtype", 0, "0");
		CLASS_ATTR_ACCESSORS(c, "spacingtype", (method)NULL, (method)score_setattr_spacingtype);
		// @description Sets the spacing algorithm: <br />
		// - Smart (default): as the "Note/Time Signature" algorithm, but where the proportionality parameter is automatically inferred, and may 
		// vary from measure to measure depending on the musical content. <br />
		// - Note/Time Signature: strikes a balance between two algorithms: a "Note" algorithm, where spacing takes only into account the actual width of each 
		// chord and note (independently from its duration), and a "Time Signature" algorithm, where spacing takes only into account the symbolic onset of each
		// chord inside the measure (independently from how much space is required to display it). The balance between these two algorithms is handled via the
		// attribute <m>spacingproportionality</m>: when it is 0 one has the pure Note algorithm, when it is 1 one has the pure Time Signature algorithm. Any value in
		// between is allowed. <br />
		// - Proportional: music content is displayed as if it were in a <o>bach.roll</o>, i.e. proportionally to absolute durations (in milliseconds) and onsets.
		// notation elements such as time signatures or barlines are preserved, but might superpose to notes or chords. <br />
		// - Spring: display spacing using a modelization where each chords is attached to two imaginary "springs", connecting it to the next and the previous one. 
		// Spacing is thus the overall result of this physical model.


		CLASS_ATTR_DOUBLE(c,"spacingparameter", 0, t_notation_obj, spacing_parameter);
//		CLASS_ATTR_STYLE_LABEL(c,"spacingparameter", 0, "text", "Spacing Parameter");
//		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"spacingparameter", 0, "1.");
		CLASS_ATTR_ACCESSORS(c, "spacingparameter", (method)NULL, (method)score_setattr_spacingparameter);
		CLASS_ATTR_INVISIBLE(c, "spacingparameter", ATTR_GET_OPAQUE | ATTR_SET_OPAQUE); // kept just for legacy
		// @exclude bach.score

		CLASS_ATTR_DOUBLE(c,"spacingwidth", 0, t_notation_obj, spacing_width);
		CLASS_ATTR_STYLE_LABEL(c,"spacingwidth", 0, "text", "Spacing Width");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"spacingwidth", 0, "2.");
		CLASS_ATTR_ACCESSORS(c, "spacingwidth", (method)NULL, (method)score_setattr_spacingwidth);
		CLASS_ATTR_BASIC(c,"spacingwidth",0);
		// @description Sets a factor for globally tightening or loosening the spacing of the score. Small values will correspond to 
		// notation elements being closer; high values will separate them.

		CLASS_ATTR_DOUBLE(c,"spacingproportionality", 0, t_notation_obj, spacing_proportionality);
		CLASS_ATTR_STYLE_LABEL(c,"spacingproportionality", 0, "text", "Spacing Proportionality");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"spacingproportionality", 0, "0.5");
		CLASS_ATTR_ACCESSORS(c, "spacingproportionality", (method)NULL, (method)score_setattr_spacingproportionality);
		// @description If the <m>spacingtype</m> is Note/Time Signature, this is represents the linear interpolation between the pure Note and 
		// the pure Time Signature algorithms (the first one corresponding to 0, the last one corresponding to 1). Any value between 0 and 1 is accepted.
		// See <m>spacingtype</m> for more information.
	
		CLASS_ATTR_DOUBLE(c,"minmeaswidth", 0, t_notation_obj, minimum_uwidth_for_measure);
		CLASS_ATTR_STYLE_LABEL(c,"minmeaswidth", 0, "text", "Minimum Measure Width");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"minmeaswidth", 0, "60");
		CLASS_ATTR_ACCESSORS(c, "minmeaswidth", (method)NULL, (method)score_setattr_minmeaswidth);
		// @description Sets the minimum width that any measure must have, in pixels (rescaled with respect to the current <m>vzoom</m>).
	
		CLASS_ATTR_DOUBLE(c,"minmeaswidthpersymunit", 0, t_notation_obj, minimum_uwidth_per_sym_unit_for_measure);
		CLASS_ATTR_STYLE_LABEL(c,"minmeaswidthpersymunit", 0, "text", "Minimum Measure Width Per Symbolic Unit");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"minmeaswidthpersymunit", 0, "110");
		CLASS_ATTR_ACCESSORS(c, "minmeaswidthpersymunit", (method)NULL, (method)score_setattr_minmeaswidthpersymunit);
		// @description Sets the minimum width corresponding to a whole note, in pixels (rescaled with respect to the current <m>vzoom</m>).
		// More precisely: each notation element lasting a fraction of the whole note will have to last at least for the corresponding fraction 
		// of pixels. For instance, if our <m>minmeaswidthpersymunit</m> a quarter chord cannot graphically last less than 110/4 = 27,5 pixels
		// (rescaled then with respect to the current <m>vzoom</m>).
	
    
        CLASS_ATTR_CHAR(c,"slashgraceflags", 0, t_notation_obj, slash_grace_flags);
		CLASS_ATTR_STYLE_LABEL(c,"slashgraceflags", 0, "onoff", "Slash Grace Flags");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"slashgraceflags", 0, "1");
		// @description If this is 1, a slash is put on the flags of single grace chords. 

		CLASS_ATTR_CHAR(c,"slashgracebeams", 0, t_notation_obj, slash_grace_beams);
		CLASS_ATTR_STYLE_LABEL(c,"slashgracebeams", 0, "onoff", "Slash Grace Beams");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"slashgracebeams", 0, "1");
		// @description If this is 1, a slash is put on the beams of beamed grace chords. 
	
		CLASS_ATTR_CHAR(c,"reversegracedirection", 0, t_notation_obj, reverse_grace_chord_direction);
		CLASS_ATTR_STYLE_LABEL(c,"reversegracedirection", 0, "onoff", "Reverse Grace Chords Direction");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"reversegracedirection", 0, "1");
		CLASS_ATTR_ACCESSORS(c, "reversegracedirection", (method)NULL, (method)score_setattr_reversegracedirection);
		// @description Toggles the reversal of the grace chord direction with respect to the main chord to which it is connected.
		// If this is 1 (default), each grace chord will have as direction the opposite direction of the next chord to which they refer.
		// For instance, if the main chord has stem up, the grace chord will have stem down. Set this attribute to 0 if you want grace chords
		// to have direction only depending on their notes, and not depending on the main chord to which they refer.
	
		CLASS_ATTR_LONG(c,"maxbeamdeltay", 0, t_notation_obj, max_beam_delta_y);
		CLASS_ATTR_STYLE_LABEL(c,"maxbeamdeltay", 0, "text", "Maximum Delta Y For Beamings In Steps");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"maxbeamdeltay", 0, "1");
		CLASS_ATTR_ACCESSORS(c, "maxbeamdeltay", (method)NULL, (method)score_setattr_maxbeamdeltay);
		// @description Maximum vertical difference (in steps) between the starting and ending positions of a beam. 
		// @copy BACH_DOC_STAFFLINE_STEP

		CLASS_ATTR_LONG(c,"maxrestfloatsteps",0, t_notation_obj, max_rest_floating_steps);
		CLASS_ATTR_STYLE_LABEL(c,"maxrestfloatsteps",0,"text","Rests Floating Maximum Number Of Steps");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"maxrestfloatsteps", 0, "4");
		CLASS_ATTR_ACCESSORS(c, "maxrestfloatsteps", (method)NULL, (method)score_setattr_maxrestfloatingsteps);
		// @description Maximum vertical distance (in steps) of which rests are allowed to float, when they are under a beam. 
		// Set 0 if you never want rests to float.
		// @copy BACH_DOC_STAFFLINE_STEP

		CLASS_ATTR_CHAR(c,"showtupletspreferences",0, t_notation_obj, show_tuplets_preferences);
		CLASS_ATTR_STYLE_LABEL(c,"showtupletspreferences",0,"enumindex","Show Tuplets As");
		CLASS_ATTR_ENUMINDEX(c,"showtupletspreferences", 0, "None Auto Number Ratio Ratio And Figure Fraction");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"showtupletspreferences", 0, "1");
		CLASS_ATTR_FILTER_CLIP(c,"showtupletspreferences", 0, 5);
		CLASS_ATTR_ACCESSORS(c, "showtupletspreferences", (method)NULL, (method)score_setattr_showtupletspreferences);
		// @description Sets the display for tuplets: <br />
		// - None: tuplets are not marked in any particular way (only as ordinary beams, if any). <br />
		// - Auto (default): bach will automatically choose between one the "Number", "Ratio" and "Ratio And Figure" display modes, depending on the tuplet type. <br />
		// - Number: tuplets are always displayed as a single number, e.g. "3" for triplets, "5" for 5-plets, and so on. <br />
		// - Ratio: tuplets are always displayed with an explicit ratio, e.g. "3:2" for triplets, "5:4" for 5-plets, and so on. <br />
		// - Ratio And Figure: tuplets are always displayed with an explicit ratio where a small note appears, indicating the duration of notes inside the ratio, 
		// e.g. "3q:2" for eighth triplets, where "q" has to be imagined as an eighth note, and so on. <br />
		// - Fraction: each element inside a tuplet is marked with an explicit fraction over it, e.g. "2/3" for triplets, "4/5" for 5-plets, and so on.

		CLASS_ATTR_CHAR(c,"tupletshape",0, t_notation_obj, tuplet_shape);
		CLASS_ATTR_STYLE_LABEL(c,"tupletshape",0,"enumindex","Tuplet Shape");
		CLASS_ATTR_ENUMINDEX(c,"tupletshape", 0, "None Bracket Slur");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"tupletshape", 0, "1");
		CLASS_ATTR_FILTER_CLIP(c,"tupletshape", 0, 2);
		// @description Sets the display for tuplets shape: <br />
		// - None: no shape is ever displayed. <br />
		// - Bracked: an ordinary square bracked is displayed. <br />
		// - Slur: a slur is displayed.

		CLASS_ATTR_CHAR(c,"hidetupletshapeonbeams",0, t_notation_obj, only_use_shape_on_unbeamed_tuplets);
		CLASS_ATTR_STYLE_LABEL(c,"hidetupletshapeonbeams",0,"onoff","Hide Tuplet Shape If Tuplet Is Beamed");
		CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"hidetupletshapeonbeams", 0, "1");
		CLASS_ATTR_FILTER_CLIP(c,"hidetupletshapeonbeams", 0, 1);
		CLASS_ATTR_ACCESSORS(c, "hidetupletshapeonbeams", (method)NULL, (method)score_setattr_hidetupletshapeonbeams);
		// @description Toggles the display of tuplet brackets only when tuplets are not singularly beamed. 
		// By default this is set to 1: if a tuplet is already identified by a beam gathering it, no bracket will be shown.
		// The bracket will be shown for unbeam chords only.			

	CLASS_STICKY_ATTR_CLEAR(c, "category");


	CLASS_STICKY_ATTR(c,"category",0,"Play");

		CLASS_ATTR_LLLL(c, "loop", 0, t_notation_obj, loop_region_as_llll, score_getattr_loop, score_setattr_loop);
		CLASS_ATTR_STYLE_LABEL(c,"loop",0,"text_large","Loop Region");
		CLASS_ATTR_SAVE(c, "loop", 0);
		CLASS_ATTR_PAINT(c, "loop", 0);
		// @description Sets the loop region. Two elements are expected: the starting and ending points of the loop region.
		// Such elements can be simple numbers, thus representing absolute millisecond positions, or can be lllls, thus considered timepoints.
		// You can substitute to the first element the "start" symbol to have the loop region start from the beginning (equivalent to 0).
		// You can substitute to the second number the "end" symbol to have the loop region last till the end of the <o>bach.score</o>.
		// You can simply set a <b>loop all</b> to loop the entire <o>bach.score</o>.
		// @copy BACH_DOC_TIMEPOINT_SYNTAX_SCORE

	CLASS_STICKY_ATTR_CLEAR(c, "category");
	
	
	s_score_class = c;
	class_register(CLASS_BOX, s_score_class);
	
	dev_post("bach.score compiled %s %s", __DATE__, __TIME__);
	return 0;
}

t_max_err score_setattr_tonedivision(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		long s = atom_getlong(av); 
		scoreapi_set_tonedivision(x, s);
	}
	return MAX_ERR_NONE;
}

t_max_err score_setattr_accidentalsgraphic(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		long s = atom_getlong(av); 
		scoreapi_set_accidentalsgraphic(x, s);
	}
	return MAX_ERR_NONE;
}

t_max_err score_setattr_accidentalspreferences(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		long s = atom_getlong(av); 
		scoreapi_set_accidentalspreferences(x, s);
	}
	return MAX_ERR_NONE;
}

t_max_err score_setattr_enharmonictable(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		scoreapi_set_enharmonictable(x, ac, av);
	}
	return MAX_ERR_NONE;
}

t_max_err score_setattr_lyricsaffectspacing(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		long s = atom_getlong(av); 
		scoreapi_set_lyricsaffectspacing(x, s);
	}
	return MAX_ERR_NONE;
}

t_max_err score_setattr_dynamicsaffectspacing(t_score *x, t_object *attr, long ac, t_atom *av){
    if (ac && av) {
        long s = atom_getlong(av);
        scoreapi_set_dynamicsaffectspacing(x, s);
    }
    return MAX_ERR_NONE;
}


t_max_err score_setattr_nonantialiasedstaff(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_nonantialiasedstaff(x, atom_getlong(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_showmeasurenumbers(t_score *x, t_object *attr, long ac, t_atom *av){
	t_max_err err = notation_obj_setattr_showmeasurenumbers((t_notation_obj *) x, attr, ac, av);
	return err;
}


t_max_err score_setattr_voicespacing(t_score *x, t_object *attr, long ac, t_atom *av){
	t_max_err err = notation_obj_setattr_voicespacing((t_notation_obj *) x, attr, ac, av);
    reset_all_articulations_positions((t_notation_obj *)x);
	return err;
}

t_max_err score_setattr_hidevoices(t_score *x, t_object *attr, long ac, t_atom *av){
	t_max_err err = notation_obj_setattr_hidevoices((t_notation_obj *) x, attr, ac, av);
    reset_all_articulations_positions((t_notation_obj *)x);
	return err;
}

t_max_err score_setattr_showsynchronoustempionce(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		x->r_ob.hide_tempi_when_equal_on_all_voices = atom_getlong(av); 
		recompute_all_and_redraw(x);
	}
	return MAX_ERR_NONE;
}

t_max_err score_setattr_showstems(t_score *x, t_object *attr, long ac, t_atom *av){
    if (ac && av) {
        x->r_ob.show_stems = (e_show_stems_preferences) atom_getlong(av);
        recalculate_all_chord_parameters(x);
        invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
    }
    return MAX_ERR_NONE;
}


t_max_err score_setattr_zoom(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		double z = atom_getfloat(av); 
		scoreapi_set_zoom(x, z);
	}
	return MAX_ERR_NONE;
}

t_max_err score_setattr_vzoom(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		if (ac && atom_gettype(av) == A_SYM && (atom_getsym(av) == _llllobj_sym_Auto || atom_getsym(av) == _llllobj_sym_auto))
			x->r_ob.link_vzoom_to_height = 1;
//		else if (x->r_ob.version_number < 6500) // previous version than the introduction of vzoom 
//			x->r_ob.link_vzoom_to_height = 1;
		else
			x->r_ob.link_vzoom_to_height = 0;

		if (x->r_ob.link_vzoom_to_height) { // AUTO vzoom
			t_object *pv; // the patcherview
			t_rect rect;
			atom_setsym(&x->r_ob.vertical_zoom, _llllobj_sym_Auto);
			pv = (t_object *)jpatcher_get_firstview(x->r_ob.patcher_parent);
			jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, pv, &rect);
			notationobj_set_vzoom_depending_on_height((t_notation_obj *)x, rect.height);
			reset_all_articulations_positions((t_notation_obj *)x);
		} else if (ac && is_atom_number(av)) {
			double z = atom_getfloat(av); 
			scoreapi_set_vzoom(x, z);
			reset_all_articulations_positions((t_notation_obj *)x);
			x->r_ob.needed_uheight = notationobj_get_supposed_standard_height((t_notation_obj *)x);
			redraw_vscrollbar((t_notation_obj *) x, 1);
		}
	}
	return MAX_ERR_NONE;
}

t_max_err score_set_noteheads_font(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		long size = NULL;
		char *text = NULL;
		
		atom_gettext_debug(ac, av, &size, &text, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
		
		if (size && text) {
			scoreapi_set_noteheads_font(x, gensym(text));
			bach_freeptr(text);
		}
	}
	return MAX_ERR_NONE;
}

t_max_err score_set_accidentals_font(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av) {
		long size = NULL;
		char *text = NULL;
		
		atom_gettext_debug(ac, av, &size, &text, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
		
		if (size && text) {
			scoreapi_set_accidentals_font(x, gensym(text));
			bach_freeptr(text);
		}
	}
	return MAX_ERR_NONE;
}

t_max_err score_set_articulations_font(t_score *x, t_object *attr, long ac, t_atom *av){
    if (ac && av) {
        long size = NULL;
        char *text = NULL;
        
        atom_gettext_debug(ac, av, &size, &text, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (size && text) {
            scoreapi_set_articulations_font(x, gensym(text));
            bach_freeptr(text);
        }
    }
    return MAX_ERR_NONE;
}


t_max_err score_setattr_outputtrees(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_outputtrees(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_maketreecompatiblewithts(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_maketreecompatiblewithts(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_autoparserhythm(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_autoretranscribe(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_showaccidentalstiepreferences(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_showaccidentalstiepreferences(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_cautionaryaccidentals(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_cautionaryaccidentals(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_cautionaryaccidentalsdecay(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_cautionaryaccidentalsdecay(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_cautionaryaccidentalsremind(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_cautionaryaccidentalsremind(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_constraintbeamsinspaces(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_constraintbeamsinspaces(x, atom_getlong(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_restswithinbeaming(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_restswithinbeaming(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_extendbeamsoverrests(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_extendbeamsoverrests(x, atom_getlong(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_maxrestfloatingsteps(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_maxrestfloatingsteps(x, atom_getlong(av));
	return MAX_ERR_NONE;
}




t_max_err score_setattr_maxbeamslope(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_maxbeamslope(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_maxbeamdeltay(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_maxbeamdeltay(x, atom_getlong(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_reversegracedirection(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_reversegracedirection(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_tieassignspitch(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_tieassignpitch(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_maxdots(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_maxdots(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_centeredwholerests(t_score *x, t_object *attr, long ac, t_atom *av){
    if (ac && av)
        scoreapi_set_centeredwholerests(x, atom_getlong(av));
    return MAX_ERR_NONE;
}



t_max_err score_setattr_dontbeamtupletswithotherthings(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_dontbeamtupletswithotherthings(x, atom_getlong(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_breakrootlevelbeam(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_breakrootlevelbeam(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_leveltobeam(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_leveltobeam(x, atom_getlong(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_beamgracesequences(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_beamgracesequences(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_simplifytuplets(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_simplifytuplets(x, atom_getlong(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_jointuplets(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_jointuplets(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_gracedur(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_gracedur(x, atom_to_symbol(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_gracemaxperc(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av && is_atom_number(av))
		scoreapi_set_gracemaxperc(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}


t_max_err score_setattr_treehandling(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_treehandling(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_showtupletspreferences(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_showtupletspreferences(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_hidetupletshapeonbeams(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_hidetupletshapeonbeams(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_admitdottedtuplets(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_admitdottedtuplets(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_showtimesignatures(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_showtimesignatures(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_allowbeaming(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_allowbeaming(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_autocompletemeasures(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_autocompletemeasures(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_spacingtype(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		score_set_spacingtype(x, atom_getlong(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_spacingparameter(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_spacingparameter(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_spacingwidth(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_spacingwidth(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_spacingproportionality(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_spacingproportionality(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_minmeaswidth(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_minmeaswidth(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}
t_max_err score_setattr_minmeaswidthpersymunit(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_minmeaswidthpersymunit(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_syncopationasymratio(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_syncopationasymratio(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

t_max_err score_setattr_syncopationposdurratio(t_score *x, t_object *attr, long ac, t_atom *av){
	if (ac && av)
		scoreapi_set_syncopationposdurratio(x, atom_getfloat(av));
	return MAX_ERR_NONE;
}

void score_set_spacingtype(t_score *x, long l)
{
	x->r_ob.spacing_type = CLAMP(l, 0, 3);
	object_attr_setdisabled((t_object *)x, gensym("spacingproportionality"), x->r_ob.spacing_type != k_SPACING_NOTE_TS);
	object_attr_setdisabled((t_object *)x, gensym("align"), x->r_ob.spacing_type != k_SPACING_PROPORTIONAL);
	recompute_all_and_redraw(x);
}


void set_loop_region_from_llll(t_score *x, t_llll* loop, char lock_mutex)
{
    if (lock_mutex)
        lock_general_mutex((t_notation_obj *)x);
	
    if (loop) {
		if (loop->l_head && hatom_gettype(&loop->l_head->l_hatom) == H_SYM && hatom_getsym(&loop->l_head->l_hatom) == _llllobj_sym_all) {
			x->r_ob.loop_region.start.timepoint = build_timepoint_with_voice(0, long2rat(0), 0);
			if (x->firstvoice && x->firstvoice->lastmeasure)
				x->r_ob.loop_region.end.timepoint = build_timepoint_with_voice(x->firstvoice->lastmeasure->measure_number, measure_get_sym_duration(x->firstvoice->lastmeasure), 0);
			else
				x->r_ob.loop_region.end.timepoint = build_timepoint_with_voice(0, long2rat(0), 0);
		} else {
			if (loop->l_head) 
				parse_open_timepoint_syntax_from_llllelem((t_notation_obj *)x, loop->l_head, NULL,
														  &x->r_ob.loop_region.start.position_ms, &x->r_ob.loop_region.start.timepoint, false);
			
			if (loop->l_head && loop->l_head->l_next) 
				parse_open_timepoint_syntax_from_llllelem((t_notation_obj *)x, loop->l_head->l_next, NULL,
														  &x->r_ob.loop_region.end.position_ms, &x->r_ob.loop_region.end.timepoint, false);
			
			if (x->r_ob.loop_region.end.position_ms < x->r_ob.loop_region.start.position_ms) {
				t_timepoint temp = x->r_ob.loop_region.start.timepoint;
				swap_doubles(&x->r_ob.loop_region.start.position_ms, &x->r_ob.loop_region.end.position_ms);
				x->r_ob.loop_region.start.timepoint = x->r_ob.loop_region.end.timepoint;
				x->r_ob.loop_region.end.timepoint = temp;
			}
		}

		llll_free(x->r_ob.loop_region_as_llll);
		x->r_ob.loop_region_as_llll = get_loop_region_as_llll((t_notation_obj *)x, false);
		
		if (x->r_ob.playing)
			check_correct_scheduling((t_notation_obj *)x, false);
		if (!x->r_ob.creatingnewobj)
			sync_loop_ms_with_loop_tp(x);
	}
    
    if (lock_mutex)
        unlock_general_mutex((t_notation_obj *)x);
}


void set_loop_region_from_extremes_ms(t_score *x, double start, double end)
{
	t_llll *temp = llll_get();
	llll_appenddouble(temp, start, 0, WHITENULL_llll);
	llll_appenddouble(temp, end, 0, WHITENULL_llll);
	set_loop_region_from_llll(x, temp, true);
	llll_free(temp);
}


void set_loop_region_from_extremes_tp(t_score *x, t_timepoint start, t_timepoint end)
{
	t_llll *temp = llll_get();
	t_llll *startll = get_timepoint_as_llll((t_notation_obj *)x, start);
	t_llll *endll = get_timepoint_as_llll((t_notation_obj *)x, end);

	llll_appendllll(temp, startll, 0, WHITENULL_llll);
	llll_appendllll(temp, endll, 0, WHITENULL_llll);

	set_loop_region_from_llll(x, temp, true);
	llll_free(temp);
}


void sync_loop_ms_with_loop_tp(t_score *x)
{
	x->r_ob.loop_region.start.position_ms = timepoint_to_ms((t_notation_obj *)x, x->r_ob.loop_region.start.timepoint, x->r_ob.loop_region.start.timepoint.voice_num);
	x->r_ob.loop_region.end.position_ms = timepoint_to_ms((t_notation_obj *)x, x->r_ob.loop_region.end.timepoint, x->r_ob.loop_region.start.timepoint.voice_num);
}

t_max_err score_setattr_loop(t_score *x, t_object *attr, long ac, t_atom *av)
{
	t_llll *args = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, ac, av, LLLL_PARSE_RETAIN);
	set_loop_region_from_llll(x, args, true);
	llll_free(args);
	if (x->r_ob.notify_also_upon_messages && !x->r_ob.creatingnewobj)
		send_loop_region((t_notation_obj *)x, 7);
	return MAX_ERR_NONE;
}


void score_assist(t_score *x, void *b, long m, long a, char *s){
    if (m == ASSIST_INLET) {
		switch (a)
		{
			case 0:														// @in 0 @type llll/bang @digest bang or llll containing gathered syntax of the entire object
				sprintf(s, "llll: Entire Score or bang to Rebuild");	// @description See the <m>bang</m> and <m>llll</m> methods for more information.
				break;
			case 1:									// @in 1 @type llll @digest Measureinfo in separate syntax.
				sprintf(s, "llll: Measureinfo");	// @description See the <m>llll</m> method for more information.
													// @copy BACH_DOC_MEASUREINFO_SYNTAX
				break;
			case 2:									// @in 2 @type llll @digest Pitches or MIDIcents in separate syntax.
				sprintf(s, "llll: Pitches");			// @description See the <m>llll</m> method for more information.
				break;
			case 3:									// @in 3 @type llll @digest Durations (in symbolic units, fractions of the whole note) in separate syntax
				sprintf(s, "llll: Durations");		// @description See the <m>llll</m> method for more information.
				break;	
			case 4:									// @in 4 @type llll @digest Velocities (1 to 127) in separate syntax
				sprintf(s, "llll: Velocities");		// @description See the <m>llll</m> method for more information.
				break;								
			case 5:									// @in 5 @type llll @digest Ties (1 for notes starting a tie, 0 for any other note) in separate syntax
				sprintf(s, "llll: Ties");			// @description See the <m>llll</m> method for more information.
				break;
			case 6:									// @in 6 @type llll @digest Extras in separate syntax
				sprintf(s, "llll: Extras");			// @description See the <m>llll</m> method for more information.
				break;
		}
	}
    else {
		char *type = NULL;
		llllobj_get_llll_outlet_type_as_string((t_object *) x, LLLL_OBJ_UI, a, &type);
		switch (a)
		{
			case 0:	// @out 0 @type llll @digest Whole object dump
				sprintf(s, "llll (%s): Entire Score", type);	// @description The gathered syntax of the whole <o>bach.score</o> (or a part of it, depending on the <m>dump</m> message arguments)
				break;
			case 1: // @out 1 @type llll @digest Measureinfo
				sprintf(s, "llll (%s): Measureinfo", type);	// @description The measureinfo in separate syntax 
				break;										// @copy BACH_DOC_SCORE_SEPARATE_SYNTAX
															// @copy BACH_DOC_MEASUREINFO_SYNTAX
			case 2: // @out 2 @type llll @digest Pitches
				sprintf(s, "llll (%s): Pitches", type);		// @description The pitches or MIDIcents in separate syntax (see <m>outputpitchesseparate</m>).
				break;										// @copy BACH_DOC_SCORE_SEPARATE_SYNTAX
			case 3: // @out 3 @type llll @digest Durations
				sprintf(s, "llll (%s): Durations", type);	// @description The durations (in symbolic units, fractions of the whole note) in separate syntax
				break;										// @copy BACH_DOC_SCORE_SEPARATE_SYNTAX
			case 4:	// @out 4 @type llll @digest Velocities
				sprintf(s, "llll (%s): Velocities", type);	// @description The velocities (1 to 127) in separate syntax.
				break;										// @copy BACH_DOC_SCORE_SEPARATE_SYNTAX
			case 5:	// @out 5 @type llll @digest Ties
				sprintf(s, "llll (%s): Ties", type);		// @description The ties (1 for notes starting a tie, 0 for any other note) in separate syntax
				break;										// @copy BACH_DOC_SCORE_SEPARATE_SYNTAX
			case 6:	// @out 6 @type llll @digest Extras
				sprintf(s, "llll (%s): Extras", type);		// @description The extras in separate syntax.
				break;										// @copy BACH_DOC_SEPARATE_SYNTAX_EXTRAS
			case 7:	// @out 7 @type llll @digest Playout
				sprintf(s, "llll (%s): Playout", type);		// @description Outlet sending information about elements being played, as well as query answers and notifications.
				break;										// @copy BACH_DOC_PLAYOUT_SYNTAX_SCORE
			case 8: // @out 8 @type bang @digest bang when changed
				sprintf(s, "bang When Changed");			// @description This outlet sends a bang whenever any element has been changed via the interface.
				break;
		}
	}
}

void score_inletinfo(t_score *x, void *b, long a, char *t)
{
	if (a)
		*t = 1;
}


void score_openslotwin(t_score *x, t_symbol *s, long argc, t_atom *argv){
	notation_obj_openslotwin((t_notation_obj *)x, s, argc, argv);
}


void score_testdomain(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_llll *input_llll, *outlist;
	t_symbol *label = NULL;
	
	if (argc <= 0) 
		return;
	
	input_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE);
	label = get_querying_label_from_querying_llll((t_notation_obj *) x, input_llll, true);
	if (input_llll && input_llll->l_head)
		llll_behead(input_llll);
	
	outlist = scoreapi_testdomain(x, input_llll, label);
	if (outlist) {
		llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
		llll_free(outlist);
	}
	
	llll_free(input_llll);
}


void score_getdomain(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
	send_domain(x, 7, label);
}



void score_dumpvoicepixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
	send_voicepixelpos((t_notation_obj *) x, k_NOTATION_OBJECT_SCORE, x->r_ob.num_voices, x->firstvoice, 7, label);
}


void score_getdomainpixels(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	double start, end;
	t_llll *outlist = llll_get();
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);

	scoreapi_getdomainpixels(x, &start, &end);
	
	llll_appendsym(outlist, gensym("domainpixels"), 0, WHITENULL_llll);
	if (label)
		llll_appendsym(outlist, label, 0, WHITENULL_llll);
	llll_appenddouble(outlist, start, 0, WHITENULL_llll);

//	if (x->r_ob.view == k_VIEW_SCROLL)
		llll_appenddouble(outlist, end, 0, WHITENULL_llll);
//	else
//		llll_appenddouble(outlist, onset_to_xposition((t_notation_obj *) x, x->r_ob.screen_ms_start + x->r_ob.ux_on_a_line, NULL), 0, WHITENULL_llll);

	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
}


void llllelem_to_mstotpmode(t_llllelem *elem, e_ms_to_tp_modes *mode)
{
    if (elem && is_hatom_number(&elem->l_hatom)) {
        long num = hatom_getlong(&elem->l_hatom);
        if (num < 0)
            *mode = k_MS_TO_TP_RETURN_LEFT_CHORD;
        else if (num > 0)
            *mode = k_MS_TO_TP_RETURN_RIGHT_CHORD;
        else
            *mode = k_MS_TO_TP_RETURN_NEAREST;
    } else if (hatom_gettype(&elem->l_hatom) == H_SYM) {
        t_symbol *sym = hatom_getsym(&elem->l_hatom);
        if (sym == _llllobj_sym_interp)
            *mode = k_MS_TO_TP_RETURN_INTERPOLATION;
        else if (sym == _llllobj_sym_left)
            *mode = k_MS_TO_TP_RETURN_LEFT_CHORD;
        else if (sym == _llllobj_sym_right)
            *mode = k_MS_TO_TP_RETURN_RIGHT_CHORD;
        else if (sym == _llllobj_sym_nearest)
            *mode = k_MS_TO_TP_RETURN_NEAREST;
    }
}

// deprecated, use the following one
void score_gettimeatpixel(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	double ux, ms;
	t_scorevoice *voice;
	t_llll *outlist;
	t_symbol *label = NULL;
	t_llll *input_llll;
	double pixel = 0;
	e_ms_to_tp_modes mode = k_MS_TO_TP_RETURN_INTERPOLATION;
	
	if (argc < 1)
		return;
	
	input_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE);
	label = get_querying_label_from_querying_llll((t_notation_obj *) x, input_llll, true);

	if (input_llll && input_llll->l_head)
		llll_behead(input_llll);

	pixel = (input_llll && input_llll->l_head && is_hatom_number(&input_llll->l_head->l_hatom) ? hatom_getdouble(&input_llll->l_head->l_hatom) : 0);
	
	if (input_llll && input_llll->l_head && input_llll->l_head->l_next)
        llllelem_to_mstotpmode(input_llll->l_head->l_next, &mode);
	
	ux = xposition_to_unscaled_xposition((t_notation_obj *) x, pixel);
	ms = unscaled_xposition_to_ms((t_notation_obj *)x, ux, 1);
	outlist = llll_get();
	
	llll_appendsym(outlist, _llllobj_sym_timeatpixel, 0, WHITENULL_llll);
	if (label)
		llll_appendsym(outlist, label, 0, WHITENULL_llll);
	llll_appenddouble(outlist, ms, 0, WHITENULL_llll);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
		t_llll *innerlist = llll_get();
		t_timepoint tp = ms_to_timepoint((t_notation_obj *)x, ms, voice->v_ob.number, mode);
		llll_appendlong(innerlist, tp.measure_num + 1, 0, WHITENULL_llll);
		llll_appendrat(innerlist, tp.pt_in_measure, 0, WHITENULL_llll);
		llll_appendllll(outlist, innerlist, 0, WHITENULL_llll);
	}
	
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
	llll_free(input_llll);
}

// takes <pixel>
// and outputs a list containing <ms> (<meas> <position in measure>) for each voice
void score_pixeltotime(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    double ux, ms;
    t_scorevoice *voice;
    t_llll *outlist;
    t_symbol *label = NULL;
    t_llll *input_llll;
    double pixel = 0;
    e_ms_to_tp_modes mode = k_MS_TO_TP_RETURN_INTERPOLATION;
    
    if (argc < 1)
        return;
    
    input_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE);
    label = get_querying_label_from_querying_llll((t_notation_obj *) x, input_llll, true);
    
    if (input_llll && input_llll->l_head)
        llll_behead(input_llll);
    
    pixel = (input_llll && input_llll->l_head && is_hatom_number(&input_llll->l_head->l_hatom) ? hatom_getdouble(&input_llll->l_head->l_hatom) : 0);
    
    if (input_llll && input_llll->l_head && input_llll->l_head->l_next) {
        if (is_hatom_number(&input_llll->l_head->l_next->l_hatom)) {
            long num = hatom_getlong(&input_llll->l_head->l_next->l_hatom);
            if (num < 0)
                mode = k_MS_TO_TP_RETURN_LEFT_CHORD;
            else if (num > 0)
                mode = k_MS_TO_TP_RETURN_RIGHT_CHORD;
            else
                mode = k_MS_TO_TP_RETURN_NEAREST;
        } else if (hatom_gettype(&input_llll->l_head->l_next->l_hatom) == H_SYM) {
            t_symbol *sym = hatom_getsym(&input_llll->l_head->l_next->l_hatom);
            if (sym == _llllobj_sym_interp)
                mode = k_MS_TO_TP_RETURN_INTERPOLATION;
            else if (sym == _llllobj_sym_left)
                mode = k_MS_TO_TP_RETURN_LEFT_CHORD;
            else if (sym == _llllobj_sym_right)
                mode = k_MS_TO_TP_RETURN_RIGHT_CHORD;
            else if (sym == _llllobj_sym_nearest)
                mode = k_MS_TO_TP_RETURN_NEAREST;
        }
    }
    
    ux = xposition_to_unscaled_xposition((t_notation_obj *) x, pixel);
    ms = unscaled_xposition_to_ms((t_notation_obj *)x, ux, 1);
    outlist = llll_get();
    
    llll_appendsym(outlist, _llllobj_sym_time, 0, WHITENULL_llll);
    if (label)
        llll_appendsym(outlist, label, 0, WHITENULL_llll);
    llll_appenddouble(outlist, ms, 0, WHITENULL_llll);
    for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
        t_llll *innerlist = llll_get();
        t_timepoint tp = ms_to_timepoint((t_notation_obj *)x, ms, voice->v_ob.number, mode);
        llll_appendlong(innerlist, tp.measure_num + 1, 0, WHITENULL_llll);
        llll_appendrat(innerlist, tp.pt_in_measure, 0, WHITENULL_llll);
        llll_appendllll(outlist, innerlist, 0, WHITENULL_llll);
    }
    
    llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
    llll_free(outlist);
    llll_free(input_llll);
}




// deprecated, use the following one
void score_getpixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_llll *input_llll;
	double pos;
	char err = true;
	t_symbol *label;
	
	if (argc <= 0) 
		return;
	
	input_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE);
	label = get_querying_label_from_querying_llll((t_notation_obj *) x, input_llll, true);

	if (input_llll && input_llll->l_head)
		llll_behead(input_llll);
	
	err = scoreapi_getpixelpos(x, input_llll, &pos);
	
	if (!err) {
		t_llll *outlist = llll_get();
		
		llll_appendsym(outlist, gensym("pixelpos"), 0, WHITENULL_llll);
		if (label)
			llll_appendsym(outlist, label, 0, WHITENULL_llll);
		llll_appenddouble(outlist, pos, 0, WHITENULL_llll);
		
		llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
		llll_free(outlist);
	}
	
	llll_free(input_llll);
}

void score_timetopixel(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_llll *input_llll;
    double pos;
    char err = true;
    t_symbol *label;
    
    if (argc <= 0)
        return;
    
    input_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE);
    label = get_querying_label_from_querying_llll((t_notation_obj *) x, input_llll, true);
    
    if (input_llll && input_llll->l_head)
        llll_behead(input_llll);
    
    err = scoreapi_getpixelpos(x, input_llll, &pos);
    
    if (!err) {
        t_llll *outlist = llll_get();
        
        llll_appendsym(outlist, _llllobj_sym_pixel, 0, WHITENULL_llll);
        if (label)
            llll_appendsym(outlist, label, 0, WHITENULL_llll);
        llll_appenddouble(outlist, pos, 0, WHITENULL_llll);
        
        llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
        llll_free(outlist);
    }
    
    llll_free(input_llll);
}


void score_timepoint(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_llll *input_llll;
    t_symbol *label;
    
    if (argc <= 0)
        return;
    
    input_llll = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE);
    label = get_querying_label_from_querying_llll((t_notation_obj *) x, input_llll, true);
    
    if (input_llll && input_llll->l_head)
        llll_behead(input_llll);
    
    double ms;
    t_timepoint tp;
    
    if (!parse_open_timepoint_syntax((t_notation_obj *)x, input_llll, NULL, &ms, &tp, false)) {
        t_scorevoice *voice;
        t_llll *outlist = llll_get();
        e_ms_to_tp_modes mode = k_MS_TO_TP_RETURN_INTERPOLATION;
        
        llll_appendsym(outlist, _llllobj_sym_timepoint, 0, WHITENULL_llll);
        if (label)
            llll_appendsym(outlist, label, 0, WHITENULL_llll);
        
        llll_appenddouble(outlist, ms, 0, WHITENULL_llll);
        
        if (input_llll->l_size >= 2)
            llllelem_to_mstotpmode(input_llll->l_head->l_next, &mode);

        char sync = are_all_measureinfo_synchronous(x);
        for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
            t_timepoint this_tp;
            if (tp.voice_num == voice->v_ob.number)
                this_tp = tp;
            else if (sync)
                this_tp = build_timepoint_with_voice(tp.measure_num, tp.pt_in_measure, voice->v_ob.number);
            else
                this_tp = ms_to_timepoint((t_notation_obj *)x, ms, voice->v_ob.number, mode);
            llll_appendllll(outlist, get_timepoint_as_llll((t_notation_obj *)x, this_tp));
        }
        llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
        llll_free(outlist);
    } else
        object_error((t_object *)x, "Wrong input syntax!");
    
    llll_free(input_llll);
}




void score_getvzoom(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
	send_vzoom((t_notation_obj *) x, 7, label);
}

void score_getzoom(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
	send_zoom((t_notation_obj *) x, 7, label);
}


void score_dumpnotepixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_llll *outlist = get_all_pixel_values_as_llll(x);
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);

	if (label)
		llll_prependsym(outlist, label, 0, WHITENULL_llll);
    llll_prependsym(outlist, s == gensym("dumppixelpos") ? gensym("dumppixelpos") : gensym("notepixelpos"), 0, WHITENULL_llll);
	
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
}

void score_dumpmeasurepixelpos(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_llll *outlist = get_all_measure_pixel_values_as_llll(x);
    t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
    
    if (label)
        llll_prependsym(outlist, label, 0, WHITENULL_llll);
    llll_prependsym(outlist, gensym("measurepixelpos"), 0, WHITENULL_llll);
    
    llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
    llll_free(outlist);
}


void score_getlength(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// outputting total length (in milliseconds)
	t_llll *outlist = llll_get();
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
	
	llll_appendsym(outlist, _llllobj_sym_length, 0, WHITENULL_llll);
	if (label)
		llll_appendsym(outlist, label, 0, WHITENULL_llll);
	llll_appenddouble(outlist, scoreapi_getlength(x), 0, WHITENULL_llll);
	
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
}


void score_getnumvoices(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// outputting number of voices
	t_llll *outlist = llll_get();
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);

	llll_appendsym(outlist, gensym("numvoices"), 0, WHITENULL_llll);
	if (label)
		llll_appendsym(outlist, label, 0, WHITENULL_llll);
	llll_appendlong(outlist, x->r_ob.num_voices, 0, WHITENULL_llll);
	
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
}


void score_getnummeasures(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// outputting number of measures (for each voice)
	t_scorevoice *voice;
	t_llll *outlist = llll_get();
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
	
	llll_appendsym(outlist, gensym("nummeasures"), 0, WHITENULL_llll);
	if (label)
		llll_appendsym(outlist, label, 0, WHITENULL_llll);
	
	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) 
		llll_appendlong(outlist, voice->num_measures, 0, WHITENULL_llll);
	unlock_general_mutex((t_notation_obj *)x);
	
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
}


void score_getnumchords(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// outputting number of chors (for each measure, for eachvoice)
	t_scorevoice *voice; t_measure *meas;
	t_llll *outlist = llll_get();
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);
	
	llll_appendsym(outlist, gensym("numchords"), 0, WHITENULL_llll);
	if (label)
		llll_appendsym(outlist, label, 0, WHITENULL_llll);
	
	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
		t_llll *voicelist = llll_get();
		for (meas = voice->firstmeasure; meas; meas = meas->next) 
			llll_appendlong(voicelist, meas->num_chords, 0, WHITENULL_llll);
		llll_appendllll(outlist, voicelist, 0, WHITENULL_llll);
	}
	unlock_general_mutex((t_notation_obj *)x);
	
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
}


void score_getnumnotes(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	// outputting number of notes (for each chord, for each measure, for each voice)
	t_scorevoice *voice; t_measure *meas; t_chord *chord;
	t_llll *outlist = llll_get();
	t_symbol *label = get_querying_label_from_GIMME((t_notation_obj *) x, s, argc, argv);

	llll_appendsym(outlist, gensym("numnotes"), 0, WHITENULL_llll);
	if (label)
		llll_appendsym(outlist, label, 0, WHITENULL_llll);
	
	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
		t_llll *voicelist = llll_get();
		for (meas = voice->firstmeasure; meas; meas = meas->next) {
			t_llll *measlist = llll_get();
			for (chord = meas->firstchord; chord; chord = chord->next)
				llll_appendlong(measlist, chord->num_notes, 0, WHITENULL_llll);
			llll_appendllll(voicelist, measlist, 0, WHITENULL_llll);
		}
		llll_appendllll(outlist, voicelist, 0, WHITENULL_llll);
	}
	unlock_general_mutex((t_notation_obj *)x);
	
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 7, outlist);
	llll_free(outlist);
}


void score_domain(t_score *x, t_symbol *s, long argc, t_atom *argv){ 
// TODO!!! UNUSED FOR NOW
	
	// change x_zoom, so that the domain is argv[0]
	if (argc==1) { // set the length
		getdomain((t_notation_obj *) x);
		double domain = atom_getfloat(argv);
		double domain_start_ux = x->r_ob.screen_ux_start;
		double domain_end_ux = ms_to_unscaled_xposition((t_notation_obj *)x, unscaled_xposition_to_ms((t_notation_obj *)x, domain_start_ux, 1) + domain, 1);
		double domain_ux = domain_end_ux - domain_start_ux;
		double this_domain_ux = x->r_ob.screen_ux_end - x->r_ob.screen_ux_start;
		if (domain > 0){
			double old_zoom = x->r_ob.horizontal_zoom;
			double new_zoom = old_zoom * ((double) this_domain_ux)/((double) domain_ux);
			change_zoom((t_notation_obj *) x, new_zoom);

			//			x->r_ob.screen_ms_end = x->r_ob.screen_ms_start + domain; 
			getdomain((t_notation_obj *) x);
//			redraw_hscrollbar((t_notation_obj *)x, 1, k_NOTATION_OBJECT_SCORE);
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
		} else {
			object_warn((t_object *)x, "Can't handle negative domains.");
		}
	} else if (argc >= 2) { // set the length
		getdomain((t_notation_obj *) x);
		double domain_start = atom_getfloat(argv);
		double domain_end = atom_getfloat(argv+1);
		double domain_start_ux = ms_to_unscaled_xposition((t_notation_obj *)x, domain_start, 1);
		double domain_end_ux = ms_to_unscaled_xposition((t_notation_obj *)x, domain_end, 1);

		if (argc >= 3)
			domain_end_ux = xposition_to_unscaled_xposition((t_notation_obj *)x, unscaled_xposition_to_xposition((t_notation_obj *)x, domain_end_ux) + atom_getfloat(argv+2) * x->r_ob.zoom_y);

		double domain_ux = domain_end_ux - domain_start_ux;
		double this_domain_ux = x->r_ob.screen_ux_end - x->r_ob.screen_ux_start;

		if (domain_ux > 0){
			double old_zoom = x->r_ob.horizontal_zoom;
			double new_zoom = old_zoom * ((double) this_domain_ux)/((double) domain_ux);
			change_zoom((t_notation_obj *) x, new_zoom);

			getdomain((t_notation_obj *) x);
//			x->r_ob.hscrollbar_pos = ((double)domain_start)/(x->r_ob.length_ms - x->r_ob.domain);
			x->r_ob.screen_ux_start = domain_start_ux;
			x->r_ob.screen_ux_end = domain_end_ux; 
			redraw_hscrollbar((t_notation_obj *)x, 2);
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
		} else {
			object_warn((t_object *)x, "Can't handle negative domains.");
		}
	}
}


void score_dump(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *headers;
	if ((argc == 1) && (atom_gettype(argv) == A_SYM)) {
		t_symbol *sym = atom_getsym(argv);
		if ((sym == _llllobj_sym_measures) || (sym == _llllobj_sym_measure) || (sym == _llllobj_sym_measureinfo)) {
			send_measuresinfo_values_as_llll(x);
			return;
		} else if ((sym == _llllobj_sym_cents) || (sym == _llllobj_sym_cent)) {
			send_cents_values_as_llll(x, x->r_ob.output_trees == 2);
			return;
		} else if ((sym == _llllobj_sym_durations) || (sym == _llllobj_sym_duration)) {
			send_durations_values_as_llll(x, x->r_ob.output_trees == 2 ? (x->r_ob.output_full_duration_tree ? 2 : 1) : 0);
			return;
		} else if ((sym == _llllobj_sym_velocities) || (sym == _llllobj_sym_velocity)) {
			send_velocities_values_as_llll(x, x->r_ob.output_trees == 2);
			return;
		} else if ((sym == _llllobj_sym_ties) || (sym == _llllobj_sym_tie)) {
			send_ties_values_as_llll(x, x->r_ob.output_trees == 2);
			return;
		} else if ((sym == _llllobj_sym_extras) || (sym == _llllobj_sym_extra)) {
			send_extras_values_as_llll(x, x->r_ob.output_trees == 2);
			return;
		} else if (sym == _llllobj_sym_separate) {
			send_extras_values_as_llll(x, x->r_ob.output_trees == 2);
			send_ties_values_as_llll(x, x->r_ob.output_trees == 2);
			send_velocities_values_as_llll(x, x->r_ob.output_trees == 2);
			send_durations_values_as_llll(x, x->r_ob.output_trees == 2 ? (x->r_ob.output_full_duration_tree ? 2 : 1) : 0);
			send_cents_values_as_llll(x, x->r_ob.output_trees == 2);
			send_measuresinfo_values_as_llll(x);
			return;
		} else if (sym == _llllobj_sym_score) {
			send_score_values_as_llll(x, -1);
			return;
		}
	} else if (argc == 0) {
		send_all_values_as_llll(x, -1); // dump all
		return;
	}
	
	headers = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_RETAIN);
	send_score_values_as_llll(x, header_objects_to_long(headers));
	llll_free(headers);
}

void score2roll(t_score *x, char markmeasures, char marktimesig, char marktempi, char markdivisions)
{
	t_llll* out_llll = llll_get();
	char curr_play_tied_elem_sep = x->r_ob.play_tied_elements_separately;
	t_scorevoice *voice;
    t_measure *measure;
	x->r_ob.play_tied_elements_separately = false;
	
    lock_general_mutex((t_notation_obj *)x);

    llll_appendsym(out_llll, _llllobj_sym_score2roll, 0, WHITENULL_llll); // "score2roll" message
	llll_appendllll(out_llll, get_slotinfo_values_as_llll((t_notation_obj *) x, false, false, false), 0, WHITENULL_llll); // slotinfo
	llll_appendllll(out_llll, get_commands_values_as_llll((t_notation_obj *) x, false), 0, WHITENULL_llll); // command
	llll_appendllll(out_llll, get_clefs_as_llll((t_notation_obj *) x, true), 0, WHITENULL_llll); // clefs
	llll_appendllll(out_llll, get_keys_as_llll((t_notation_obj *) x, true), 0, WHITENULL_llll); // keys
	llll_appendllll(out_llll, get_stafflines_as_llll((t_notation_obj *) x, true), 0, WHITENULL_llll); // stafflines
    
    // markers
    {
        t_llll *markers = get_markers_as_llll((t_notation_obj *) x, 0, 0, 0, 0, k_CONSIDER_FOR_SCORE2ROLL, 0);
        if (markmeasures || marktempi || marktimesig || markdivisions) {
            // adding measure markers
            char all_together = are_all_measureinfo_synchronous(x);
            for (voice = x->firstvoice; voice && (voice->v_ob.number < x->r_ob.num_voices); voice = all_together ? NULL : voice->next) {
                for (measure = voice->firstmeasure; measure; measure = measure->next) {
                    if (marktimesig) {
                        t_llll *this_mk = llll_get();
                        llll_appenddouble(this_mk, notation_item_get_onset_ms((t_notation_obj *)x, (t_notation_item *)measure));
                        llll_appendsym(this_mk, _llllobj_sym_timesignature);
                        llll_appendsym(this_mk, _llllobj_sym_timesig);
                        llll_appendllll(this_mk, get_timesignature_as_llll(&measure->timesignature));
                        llll_appendllll(markers, this_mk);
                    }
                    if (markmeasures) {
                        t_llll *this_mk = llll_get();
                        llll_appenddouble(this_mk, notation_item_get_onset_ms((t_notation_obj *)x, (t_notation_item *)measure));
                        llll_appendsym(this_mk, _llllobj_sym_barline);
                        llll_appendsym(this_mk, _llllobj_sym_barline);
                        llll_appendllll(markers, this_mk);
                    }
                    if (marktempi) {
                        t_tempo *tempo;
                        for (tempo = measure->firsttempo; tempo; tempo = tempo->next) {
                            t_llll *this_mk = llll_get();
                            llll_appenddouble(this_mk, notation_item_get_onset_ms((t_notation_obj *)x, (t_notation_item *)tempo));
                            llll_appendsym(this_mk, _llllobj_sym_tempo);
                            llll_appendsym(this_mk, _llllobj_sym_tempo);
                            {
                                t_llll *tempov = llll_get();
                                llll_appendrat(tempov, tempo->tempo_figure);
                                llll_appenddouble(tempov, rat2double(tempo->figure_tempo_value));
                                llll_appendrat(tempov, tempo->changepoint);
                                llll_appendllll(this_mk, tempov);
                            }
                            if (tempo->interpolation_type)
                                llll_appendlong(this_mk, 1);
                            llll_appendllll(markers, this_mk);
                        }
                    }
                    if (markdivisions) {
                        t_rational division = ts_to_division((t_notation_obj *)x, &measure->timesignature);
                        t_rational cur = long2rat(0), tot = measure_get_sym_duration(measure);
                        while (rat_rat_cmp(cur, tot) < 0) {
                            if (cur.r_num > 0) {
                                t_llll *this_mk = llll_get();
                                llll_appenddouble(this_mk, timepoint_to_ms((t_notation_obj *)x, build_timepoint(measure->measure_number, cur), voice->v_ob.number));
                                llll_appendsym(this_mk, _llllobj_sym_division);
                                llll_appendsym(this_mk, _llllobj_sym_division);
                                llll_appendllll(markers, this_mk);
                            }
                            
                            cur = rat_rat_sum(cur, division);
                        }
                    }
                }
            }
            llll_mergesort_inplace(&markers, llll_sort_by_first, NULL);
        }
        llll_appendllll(out_llll, markers, 0, WHITENULL_llll);
    }
    
	llll_appendllll(out_llll, get_midichannels_as_llll((t_notation_obj *) x, true), 0, WHITENULL_llll); // midichannels
    llll_appendllll(out_llll, get_articulationinfo_as_llll((t_notation_obj *)x), 0, WHITENULL_llll); // articulations
    llll_appendllll(out_llll, get_noteheadinfo_as_llll((t_notation_obj *)x), 0, WHITENULL_llll); // noteheads
    llll_appendllll(out_llll, get_numparts_as_llll((t_notation_obj *)x), 0, WHITENULL_llll); // parts

    
    // actually appending notes
	for (voice = x->firstvoice; voice && (voice->v_ob.number < x->r_ob.num_voices); voice = voice->next){
		t_llll* voice_llll = llll_get();
		measure = voice->firstmeasure;
		while (measure) {

			t_chord *chord = measure->firstchord;
			while (chord) {
				
				t_note *note = chord->firstnote;
				while (chord->r_sym_duration.r_num >= 0 && note) { 
					if (!note->tie_from) {
						t_llll* note_llll = llll_get();
						llll_appendsym(note_llll, _llllobj_sym_note, 0, WHITENULL_llll);
						llll_appendlong(note_llll, chord->parent->voiceparent->v_ob.number + 1, 0, WHITENULL_llll); // voices are 1-based
						llll_appendlong(note_llll, chord->parent->voiceparent->v_ob.midichannel, 0, WHITENULL_llll);
						llll_appendllll(note_llll, get_single_scorenote_values_as_llll((t_notation_obj *) x, note, k_CONSIDER_FOR_PLAYING), 0, WHITENULL_llll);
						llll_appendllll(voice_llll, note_llll, 0, WHITENULL_llll);
					}
					note = note->next;
				}
				
				chord = chord->next;
			}
			
			measure = measure->next;
		}
	
		llll_appendllll(out_llll, voice_llll, 0, WHITENULL_llll);
	}
	unlock_general_mutex((t_notation_obj *)x);

	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 0, out_llll);
	llll_free(out_llll);
	x->r_ob.play_tied_elements_separately = curr_play_tied_elem_sep;
}


void score_score2roll(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_llll *args = llllobj_parse_llll((t_object *)x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
    long markmeasures = false, marktimesig = false, marktempi = false, markdivisions = false;
    llll_parseargs_and_attrs((t_object *) x, args, "iiii",
                   gensym("markmeasures"), &markmeasures,
                   gensym("marktimesigs"), &marktimesig,
                   gensym("marktempi"), &marktempi,
                   gensym("markdivisions"), &markdivisions);

    llll_free(args);
    score2roll(x, markmeasures, marktimesig, marktempi, markdivisions);
}

/*void clear_all(t_score *x) {
	long i; t_scorevoice *voice = x->firstvoice;
	for (i = 0; i < x->r_ob.num_voices; i++) { 
		if (voice)
			clear_voice(x, voice);
		voice = voice->next;
	}
	// gotta clear tuttipoint and tempi as well
	delete_all_tuttipoints(x);
}*/

void score_lambda(t_score *x, t_symbol *s, long argc, t_atom *argv){ 
	if (argc && atom_gettype(argv) == A_SYM){
		t_symbol *router = atom_getsym(argv);
		if (router == _llllobj_sym_cents){
			score_sel_change_cents(x, _llllobj_sym_lambda, argc - 1, argv + 1);
		} else if (router == _llllobj_sym_velocity){
			score_sel_change_velocity(x, _llllobj_sym_lambda, argc - 1, argv + 1);
        } else if (router == _llllobj_sym_tie){
            score_sel_change_tie(x, _llllobj_sym_lambda, argc - 1, argv + 1);
		} else if (router == _llllobj_sym_eraseslot){
			score_sel_erase_slot(x, _llllobj_sym_lambda, argc - 1, argv + 1);
		} else if (router == _llllobj_sym_changeslotvalue){
			score_sel_change_slot_value(x, _llllobj_sym_lambda, argc - 1, argv + 1);
		} else if (router == _llllobj_sym_addslot){
			score_sel_add_slot(x, _llllobj_sym_lambda, argc - 1, argv + 1);
		} else if (router == _llllobj_sym_addbreakpoint){
			score_sel_add_breakpoint(x, _llllobj_sym_lambda, argc - 1, argv + 1);
		} else if (router == _llllobj_sym_erasebreakpoints){
			score_sel_erase_breakpoints(x, _llllobj_sym_lambda, 0, NULL);
        } else if (router == _llllobj_sym_name){
            notation_obj_name((t_notation_obj *)x, _llllobj_sym_lambda, argc - 1, argv + 1);
		}
	}
}


// useful for score reversal
void score_copy_slots_to_tied_noted_sequences(t_score *x)
{
	t_scorevoice *voice;
	t_measure *meas;
	t_chord *chord; 
	t_note *note;
	lock_general_mutex((t_notation_obj *)x);
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
		for (meas = voice->firstmeasure; meas; meas = meas->next) {
			for (chord = meas->firstchord; chord; chord = chord->next) {
				for (note = chord->firstnote; note; note = note->next) {
					if (note->tie_to && note->tie_to != WHITENULL) {
						t_note *tmp = note->tie_to;
						t_llll *slots = note_get_slots_values_as_llll((t_notation_obj *)x, note, k_CONSIDER_FOR_DUMPING_ONLY_TIE_SPANNING, true);
						while (tmp && tmp != WHITENULL) {
							set_slots_values_to_note_from_llll((t_notation_obj *)x, tmp, slots);
							tmp = tmp->tie_to;
						}
					}
				}
			}
		}
	}
	unlock_general_mutex((t_notation_obj *)x);
}

void score_anything(t_score *x, t_symbol *s, long argc, t_atom *argv){
	long inlet = proxy_getinlet((t_object *) x); 

	if (x->r_ob.is_sending_automessage) // automessage loop; we don't want it
		return;
	
    if (inlet == 0) {
        t_llll *inputlist = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, s, argc, argv, LLLL_PARSE_CLONE); // We clone it: we operate destructively
        
        if (inputlist && (inputlist->l_size > 0)) {
            t_llllelem *firstelem = inputlist->l_head;
            char is_firstelem_symbol = (hatom_gettype(&firstelem->l_hatom) == H_SYM);
            t_symbol *router = is_firstelem_symbol ? hatom_getsym(&firstelem->l_hatom) : NULL;

            char append_via_gathered_syntax = false;
            if (router == _llllobj_sym_addmeasures || router == _llllobj_sym_appendmeasures)
                append_via_gathered_syntax = true;
            
            if (append_via_gathered_syntax) {
                x->must_append_measures = true;
                if (inputlist->l_size == 1) {
                    llll_free(inputlist);
                    score_bang(x);
                    return;
                } else {
                    router = NULL;
                }
            } else {
                x->must_append_measures = false;
            }
            
            if (!x->must_append_measures || (x->must_append_measures && !router)) { // should be always
                if (hatom_gettype(&firstelem->l_hatom) == H_LLLL || !router || (router == _llllobj_sym_score)) {
                    create_whole_score_undo_tick(x);
                    set_score_from_llll(x, inputlist, true);
                    handle_rebuild_done((t_notation_obj *) x);
                    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_SCORE);
                    
                } else if (is_firstelem_symbol) {
                    
                    if (router == _sym_paste) {
                        score_paste(x, s, argc, argv);
                    } else if (router == _sym_clear) {
                        create_whole_score_undo_tick(x);
                        lock_general_mutex((t_notation_obj *)x);
                        if (inputlist->l_size > 1 && inputlist->l_head->l_next && is_hatom_number(&inputlist->l_head->l_next->l_hatom))
                            clear_score_body(x, hatom_getlong(&inputlist->l_head->l_next->l_hatom) - 1);
                        else
                            score_clear_all(x);
                        unlock_general_mutex((t_notation_obj *)x);
                        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CLEAR_SCORE);
                        
                    } else if (router == _llllobj_sym_clearall) {
                        long i;
                        for (i = 1; i < 7; i++)
                            llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), i);
                        
                    } else if (router == _llllobj_sym_appendmeasure || router == _llllobj_sym_appendmeasures) {
                        long voice_start = 1, voice_end = x->r_ob.num_voices;
                        llll_behead(inputlist);
                        t_llll *meas = NULL;
                        
                        if (inputlist->l_size == 1) { // measure content
                            meas = inputlist;
                        } else if (inputlist->l_size > 1) {
                            if (hatom_gettype(&inputlist->l_head->l_hatom) == H_LLLL) {
                                t_llll *voice_ll = hatom_getllll(&inputlist->l_head->l_hatom);
                                if (voice_ll && voice_ll->l_size >= 2 && voice_ll->l_depth == 1) {
                                    voice_start = hatom_getlong(&voice_ll->l_head->l_hatom);
                                    voice_end = hatom_getlong(&voice_ll->l_head->l_next->l_hatom);
                                }
                            } else if (is_hatom_number(&inputlist->l_head->l_hatom))
                                voice_start = voice_end = hatom_getlong(&inputlist->l_head->l_hatom);
                            llll_behead(inputlist);
                            if (inputlist->l_size >= 1)
                                meas = inputlist;
                        }
                        
                        create_whole_score_undo_tick(x);
                        insert_measures_from_message(x, voice_start, voice_end, -1, meas, router == _llllobj_sym_appendmeasures);
                        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, router == _llllobj_sym_appendmeasures ? k_UNDO_OP_APPEND_MEASURES : k_UNDO_OP_APPEND_MEASURE);
                        
                    } else if (router == _llllobj_sym_insertmeasure || router == _llllobj_sym_insertmeasures) {
                        long voice_start = 1, voice_end = x->r_ob.num_voices;
                        long meas_num = -1;
                        llll_behead(inputlist);
                        t_llll *meas = NULL;
                        if (inputlist->l_size == 1) {
                            if (hatom_gettype(&inputlist->l_head->l_hatom) == H_LLLL) // append measure to all voices
                                meas = inputlist;
                            else  // insert single empty measure in all voices
                                meas_num = hatom_getlong(&inputlist->l_head->l_hatom);
                        } else if (inputlist->l_size == 2) {
                            if (is_hatom_number(&inputlist->l_head->l_next->l_hatom)) { // voice and measure specification
                                if (is_hatom_number(&inputlist->l_head->l_hatom))
                                    voice_start = voice_end = hatom_getlong(&inputlist->l_head->l_hatom);
                                else if (hatom_gettype(&inputlist->l_head->l_hatom) == H_LLLL) {
                                    t_llll *voice_ll = hatom_getllll(&inputlist->l_head->l_hatom);
                                    if (voice_ll && voice_ll->l_size >= 2 && voice_ll->l_depth == 1) {
                                        voice_start = hatom_getlong(&voice_ll->l_head->l_hatom);
                                        voice_end = hatom_getlong(&voice_ll->l_head->l_next->l_hatom);
                                    }
                                }
                                meas_num = hatom_getlong(&inputlist->l_head->l_next->l_hatom);
                            } else { // measure and content specification
                                meas_num = hatom_getlong(&inputlist->l_head->l_hatom);
                                llll_behead(inputlist);
                                meas = inputlist;
                            }
                        } else if (inputlist->l_size >= 3){
                            if (is_hatom_number(&inputlist->l_head->l_hatom))
                                voice_start = voice_end = hatom_getlong(&inputlist->l_head->l_hatom);
                            else if (hatom_gettype(&inputlist->l_head->l_hatom) == H_LLLL) {
                                t_llll *voice_ll = hatom_getllll(&inputlist->l_head->l_hatom);
                                if (voice_ll && voice_ll->l_size >= 2 && voice_ll->l_depth == 1) {
                                    voice_start = hatom_getlong(&voice_ll->l_head->l_hatom);
                                    voice_end = hatom_getlong(&voice_ll->l_head->l_next->l_hatom);
                                }
                            }
                            meas_num = hatom_getlong(&inputlist->l_head->l_next->l_hatom);
                            llll_behead(inputlist);
                            llll_behead(inputlist);
                            meas = inputlist;
                        }
                        
                        create_whole_score_undo_tick(x);
                        insert_measures_from_message(x, voice_start, voice_end, meas_num, meas, router == _llllobj_sym_insertmeasures);
                        
                        perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_DONT_CHANGE_LEVELS + k_BEAMING_CALCULATION_DONT_CHANGE_CHORDS + k_BEAMING_CALCULATION_DONT_CHANGE_TIES);
                        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, router == _llllobj_sym_insertmeasures ? k_UNDO_OP_INSERT_MEASURES : k_UNDO_OP_INSERT_MEASURE);
                        
                        
                    } else if (router == gensym("refresh")) {
                        //                    update_hscrollbar((t_notation_obj *)x, 1);
                        invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
                        recompute_all_and_redraw(x);
                        
                    } else if (router == gensym("setnotationcolors")) {
                        llll_destroyelem(firstelem);
                        notationobj_setnotationcolors((t_notation_obj *)x, inputlist);

                    } else if (router == _llllobj_sym_goto) {
                        llll_destroyelem(firstelem);
                        notationobj_goto_parseargs((t_notation_obj *)x, inputlist);

                    } else if (router == gensym("checkdynamics")) {
                        char selection_only = false;
                        long inconsistent = true, unnecessary = true;
                        llll_destroyelem(firstelem);
                        long slot_num = x->r_ob.link_dynamics_to_slot - 1;
                        if (inputlist->l_head && hatom_getsym(&inputlist->l_head->l_hatom) == _llllobj_sym_selection) {
                            selection_only = true;
                            llll_behead(inputlist);
                        }
                        if (inputlist->l_head && is_hatom_number(&inputlist->l_head->l_hatom))
                            slot_num = hatom_getlong(&inputlist->l_head->l_hatom) - 1;
                        llll_parseargs_and_attrs((t_object *) x, inputlist, "ii", gensym("inconsistent"), &inconsistent, gensym("unnecessary"), &unnecessary);
                        if (slot_num >= 0 && slot_num < CONST_MAX_SLOTS)
                            notationobj_check_dynamics((t_notation_obj *)x, slot_num, inconsistent, unnecessary, false, false, selection_only, true);
                        
                    } else if (router == gensym("fixdynamics")) {
                        char selection_only = false;
                        llll_destroyelem(firstelem);
                        long inconsistent = true, unnecessary = true;
                        long slot_num = x->r_ob.link_dynamics_to_slot - 1;
                        long fix_verbose = 0;
                        if (inputlist->l_head && hatom_getsym(&inputlist->l_head->l_hatom) == _llllobj_sym_selection) {
                            selection_only = true;
                            llll_behead(inputlist);
                        }
                        if (inputlist->l_head && is_hatom_number(&inputlist->l_head->l_hatom))
                            slot_num = hatom_getlong(&inputlist->l_head->l_hatom);
                        llll_parseargs_and_attrs((t_object *) x, inputlist, "iii", gensym("verbose"), &fix_verbose, gensym("inconsistent"), &inconsistent, gensym("unnecessary"), &unnecessary);
                        if (slot_num >= 0 && slot_num < CONST_MAX_SLOTS)
                            notationobj_check_dynamics((t_notation_obj *)x, slot_num, inconsistent, unnecessary, inconsistent, unnecessary, selection_only, fix_verbose);

                    } else if (router == gensym("dynamics2velocities")) {
                        char selection_only = false;
                        t_llll *mapping_ll = NULL;
                        llll_destroyelem(firstelem);
                        long slot_num = x->r_ob.link_dynamics_to_slot - 1;
                        double a_exp = CONST_DEFAULT_DYNAMICS_TO_VELOCITY_EXPONENT;
                        long maxchars = CONST_DEFAULT_DYNAMICS_SPECTRUM_WIDTH - 1;
                        if (inputlist->l_head && hatom_getsym(&inputlist->l_head->l_hatom) == _llllobj_sym_selection) {
                            selection_only = true;
                            llll_behead(inputlist);
                        }
                        if (inputlist->l_head && is_hatom_number(&inputlist->l_head->l_hatom)) {
                            slot_num = hatom_getlong(&inputlist->l_head->l_hatom) - 1;
                            llll_behead(inputlist);
                        }
                        llll_parseargs_and_attrs((t_object *)x, inputlist, "lid", gensym("mapping"), &mapping_ll, gensym("maxchars"), &maxchars, gensym("exp"), &a_exp);
                        if (slot_num >= 0 && slot_num < CONST_MAX_SLOTS)
                            notationobj_dynamics2velocities((t_notation_obj *)x, slot_num, mapping_ll, selection_only, MAX(0, maxchars + 1), CLAMP(a_exp, 0.001, 1.));
                        llll_free(mapping_ll);

                    } else if (router == gensym("velocities2dynamics")) {
                        char selection_only = false;
                        t_llll *mapping_ll = NULL;
                        llll_destroyelem(firstelem);
                        long slot_num = x->r_ob.link_dynamics_to_slot - 1, delete_unnecessary = true;
                        double a_exp = CONST_DEFAULT_DYNAMICS_TO_VELOCITY_EXPONENT, approx_thresh = CONST_DEFAULT_VELOCITIES_TO_DYNAMICS_HAIRPIN_THRESH;
                        long maxchars = CONST_DEFAULT_DYNAMICS_SPECTRUM_WIDTH - 1;
                        if (inputlist->l_head && hatom_getsym(&inputlist->l_head->l_hatom) == _llllobj_sym_selection) {
                            selection_only = true;
                            llll_behead(inputlist);
                        }
                        if (inputlist->l_head && is_hatom_number(&inputlist->l_head->l_hatom)) {
                            slot_num = hatom_getlong(&inputlist->l_head->l_hatom) - 1;
                            llll_behead(inputlist);
                        }
                        llll_parseargs_and_attrs((t_object *)x, inputlist, "lidid", gensym("mapping"), &mapping_ll, gensym("maxchars"), &maxchars, gensym("exp"), &a_exp, gensym("unnecessary"), &delete_unnecessary, gensym("thresh"), &approx_thresh);
                        if (slot_num >= 0 && slot_num < CONST_MAX_SLOTS)
                            notationobj_velocities2dynamics((t_notation_obj *)x, slot_num, mapping_ll, selection_only, MAX(0, maxchars + 1), a_exp, delete_unnecessary, approx_thresh);
                        llll_free(mapping_ll);
                        
                    } else if (router == gensym("clearbreakpoints")) {
                        delete_all_breakpoints((t_notation_obj *)x, firstelem->l_next && is_hatom_number(&firstelem->l_next->l_hatom) ? hatom_getlong(&firstelem->l_next->l_hatom) - 1 : -1);
                        handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CLEAR_BREAKPOINTS);
                        
                    } else if (router == gensym("autospell")) {
                        notationobj_autospell_parseargs((t_notation_obj *)x, inputlist);

                    } else if (router == gensym("realtime")) {
                        if (firstelem->l_next && is_hatom_number(&firstelem->l_next->l_hatom))
                            notationobj_toggle_realtime_mode((t_notation_obj *)x, hatom_getlong(&firstelem->l_next->l_hatom));
                        
                    } else if (router == gensym("notationtypoprefs")) {
                        llll_behead(inputlist);
                        set_notation_typo_preferences_from_llll((t_notation_obj *)x, inputlist);
                        recompute_all_and_redraw(x);
                        
                    } else if (firstelem->l_next && router == _llllobj_sym_interp) {
                        double ms = 0;
                        parse_open_timepoint_syntax_from_llllelem((t_notation_obj *)x, firstelem->l_next, NULL, &ms, NULL, false);
                        t_llll *out = notationobj_get_interp((t_notation_obj *)x, ms);
                        llll_prependsym(out, _llllobj_sym_interp, 0, WHITENULL_llll);
                        llllobj_outlet_llll((t_object *)x, LLLL_OBJ_UI, 7, out);
                        llll_free(out);
                        
                    } else if (router == gensym("overtype")) {
                        if (firstelem->l_next && firstelem->l_next->l_next && hatom_gettype(&firstelem->l_next->l_hatom) == H_LLLL && hatom_gettype(&firstelem->l_next->l_next->l_hatom) == H_LLLL) {
                            t_llll *params = hatom_getllll(&firstelem->l_next->l_hatom);
                            t_llll *content = hatom_getllll(&firstelem->l_next->l_next->l_hatom);
                            t_timepoint from_here, to_here;
                            char to_here_def = false;
                            if (params->l_head) {
                                parse_open_timepoint_syntax_from_llllelem((t_notation_obj *)x, params->l_head, NULL, NULL, &from_here, false);
                                if (params->l_head->l_next) {
                                    to_here_def = true;
                                    parse_open_timepoint_syntax_from_llllelem((t_notation_obj *)x, params->l_head->l_next, NULL, NULL, &to_here, false);
                                }
                                
                                // TO DO: this is a bit overkilling
                                create_whole_score_undo_tick(x);
                                
                                overtype(x, &from_here, to_here_def ? &to_here : NULL, content);
                            }
                            
                            handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_OVERTYPE);
                        }
                        
                    } else if (firstelem->l_next && router == _llllobj_sym_sample) {
                        t_llll *out = NULL;
                        if (is_hatom_number(&firstelem->l_next->l_hatom) && llllelem_exists_and_is_sym(firstelem->l_next->l_next, gensym("ms")))
                            out = notationobj_get_sampling_ms((t_notation_obj *)x, hatom_getdouble(&firstelem->l_next->l_hatom));
                        else if (hatom_gettype(&firstelem->l_next->l_hatom) == H_LLLL && llllelem_exists_and_is_sym(firstelem->l_next->l_next, gensym("timepoint"))) {
                            char voice_defined = false;
                            t_timepoint delta_tp = llll_to_timepoint((t_notation_obj *)x, hatom_getllll(&firstelem->l_next->l_hatom), &voice_defined, false);
                            delta_tp.measure_num += 1; // it's a DELTA measure number, it has of course to be 1-based: if user inserted 1, it meant "one measure", not "first measure"
                            if (!voice_defined)
                                get_max_num_measures(x, &delta_tp.voice_num); // we use the longest voice
                            clip_long(&delta_tp.voice_num, 0, x->r_ob.num_voices - 1);
                            out = score_get_sampling_timepoint(x, delta_tp);
                        } else if (is_hatom_number(&firstelem->l_next->l_hatom))
                            out = notationobj_get_sampling((t_notation_obj *)x, hatom_getlong(&firstelem->l_next->l_hatom));
                        if (out) {
                            llll_prependsym(out, _llllobj_sym_sample, 0, WHITENULL_llll);
                            llllobj_outlet_llll((t_object *)x, LLLL_OBJ_UI, 7, out);
                            llll_free(out);
                        }
                        
                    } else if (router == _llllobj_sym_deletevoice) {
                        if (firstelem->l_next && is_hatom_number(&firstelem->l_next->l_hatom)) {
                            if (x->r_ob.num_voices == 1) {
                                object_error((t_object *)x, "Can't delete voices in an object having a single voice.");
                            } else {
                                long voicenum = hatom_getlong(&firstelem->l_next->l_hatom) - 1;
                                t_scorevoice *voice = nth_scorevoice(x, voicenum);
                                if (voice && voice->v_ob.number < x->r_ob.num_voices) {
                                    create_whole_score_undo_tick(x);
                                    score_delete_voice(x, voice);
                                    handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_DELETE_VOICE);
                                }
                            }
                        }
                        
                    } else if (router == _llllobj_sym_insertvoice) {
                        if (firstelem->l_next && is_hatom_number(&firstelem->l_next->l_hatom)) {
                            long voicenum = CLAMP(hatom_getlong(&firstelem->l_next->l_hatom) - 1, 0, x->r_ob.num_voices);
                            t_scorevoice *voice = nth_scorevoice(x, voicenum);
                            
                            if (voice) {
                                t_llll *voice_content_ll = NULL;
                                char ref_def = true;
                                long ref_idx = (firstelem->l_next->l_next && hatom_gettype(&firstelem->l_next->l_next->l_hatom) == H_LONG) ? hatom_getlong(&firstelem->l_next->l_next->l_hatom) - 1 : -1;
                                if (ref_idx < 0 || ref_idx >= x->r_ob.num_voices) {
                                    ref_idx = (voice->prev ? voice->prev->v_ob.number : voice->v_ob.number);
                                    ref_def = false;
                                }
                                t_scorevoice *ref = nth_scorevoice(x, ref_idx);
                                
                                if (firstelem->l_next->l_next && hatom_gettype(&firstelem->l_next->l_next->l_hatom) == H_LLLL) {
                                    // Set voice content from llll
                                    voice_content_ll = llll_clone(hatom_getllll(&firstelem->l_next->l_next->l_hatom));
                                } else if (ref_def) {
                                    // copy measureinfo from M-th voice
                                    voice_content_ll = llll_get();
                                    t_scorevoice *mthvoice = nth_scorevoice(x, ref_idx);
                                    if (mthvoice && mthvoice->v_ob.number < x->r_ob.num_voices) {
                                        t_measure *meas;
                                        for (meas = mthvoice->firstmeasure; meas; meas = meas->next){
                                            t_llll *this_meas_ll = llll_get();
                                            llll_appendllll(this_meas_ll, measure_get_measureinfo_as_llll((t_notation_obj *)x, meas));
                                            llll_appendllll(voice_content_ll, this_meas_ll);
                                        }
                                    }
                                }
                                
                                create_whole_score_undo_tick(x);
                                
                                score_move_and_reinitialize_last_voice(x, voice->prev, x->r_ob.keys_as_symlist[ref_idx],
                                                                       ref->v_ob.clef, ref_def ? get_names_as_llll((t_notation_item *)ref, false) : llll_get(), ref->v_ob.midichannel, ref->v_ob.number + 1, NULL);
                                
                                if (voice_content_ll) {
                                    long i;
                                    t_llll *ll = llll_get();
                                    llll_appendllll(ll, voice_content_ll);
                                    for (i = 0; i < voicenum; i++)
                                        llll_prependllll(ll, llll_get());
                                    x->must_append_measures = true;
                                    set_score_from_llll(x, ll, true);
                                    x->must_append_measures = false;
                                    llll_free(ll);
                                }
                                
                                handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_INSERT_VOICE);
                            }
                        }
                        
                    } else if (!x->r_ob.itsme || router == _llllobj_sym_lambda) {
                        // send deparsed message to score
                        t_atom *av = NULL, rv;
                        long ac = llll_deparse(inputlist, &av, 0, 0);
                        if (ac) {
                            x->r_ob.itsme = true;
                            object_method_typed((t_object *)x, router, ac - 1, av + 1, &rv);
                            x->r_ob.itsme = false;
                        }
                        bach_freeptr(av);
                    } else {
                        post_unknown_message((t_object *) x, inputlist);
                    }
                } else {
                    post_unknown_message((t_object *) x, inputlist);
                }
            }
        }
        if (inputlist)
            llll_free(inputlist);

    // inlet > 0
	
	} else if (s == _sym_clear) {
		llllobj_store_llll((t_object *) x, LLLL_OBJ_UI, llll_get(), inlet);
		return;
	} else if (s == _sym_bang) {
		score_bang(x);
	} else {
		llllobj_parse_and_store((t_object *) x, LLLL_OBJ_UI, s, argc, argv, inlet);
	}
}
	

void score_merge(t_score *x, t_symbol *s, long argc, t_atom *argv)
{

	if (proxy_getinlet((t_object *) x) == 0) {
		t_llll *inputlist = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE); // We clone it: we operate destructively
		if (inputlist && (inputlist->l_head)) {
			t_llllelem *firstelem = inputlist->l_head;
			t_rational threshold_sym = long2rat(-1);  // negative = not computed
			double threshold_cents = -1.;
			char gathering_policy_sym = 0;
			char gathering_policy_cents = 0;
			char changed;
			char only_selected = false;
			
			if (firstelem && hatom_gettype(&firstelem->l_hatom) == H_SYM && hatom_getsym(&firstelem->l_hatom) == _llllobj_sym_selection) {
				only_selected = true;
				llll_behead(inputlist);
				firstelem = inputlist->l_head;
			}
			
			
			if (firstelem) {
				threshold_sym = ((hatom_gettype(&firstelem->l_hatom) == H_RAT) || (hatom_gettype(&firstelem->l_hatom) == H_LONG) || (hatom_gettype(&firstelem->l_hatom) == H_DOUBLE)) ? 
								hatom_getrational(&firstelem->l_hatom) : long2rat(-1);
				if (firstelem->l_next) {
					threshold_cents = hatom_getdouble(&firstelem->l_next->l_hatom);
					if (firstelem->l_next->l_next) {
						gathering_policy_sym = hatom_getlong(&firstelem->l_next->l_next->l_hatom);
						if (firstelem->l_next->l_next->l_next) {
							gathering_policy_cents = hatom_getlong(&firstelem->l_next->l_next->l_next->l_hatom);
						}
					}
				}
			}
			
			create_whole_score_undo_tick(x);
			
			// ok, ready to merge.
			lock_general_mutex((t_notation_obj *)x);
			changed = merge(x, threshold_sym, threshold_cents, gathering_policy_sym, gathering_policy_cents, only_selected);
			unlock_general_mutex((t_notation_obj *)x);

			llll_free(inputlist);

			handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_MERGE);
		}
	}

}


t_chord *clear_region(t_score *x, t_scorevoice *voice, t_timepoint *from_here, t_timepoint *to_here)
{
    
    t_chord *res = NULL;
    t_measure *start_meas = nth_measure_of_scorevoice(voice, from_here->measure_num);
    t_measure *end_meas = nth_measure_of_scorevoice(voice, to_here->measure_num);
    
    if (!start_meas || !end_meas)
        return NULL;
    
    if (start_meas == end_meas) {
        // single measure -- paradoxically, more complicated...!!!
        // TO BE TESTED
        // we have 3 regions (potentially): L(=left) D(=deleted) R(=right)
        t_chord *chord, *nextchord;
        t_rational cur = long2rat(0);
        for (chord =start_meas->firstchord; chord; chord = nextchord) {
            nextchord = chord->next;
            long sign = isign(chord->r_sym_duration.r_num);
            
            t_rational next_cur = chord->is_grace_chord ? cur : rat_rat_sum(cur, rat_abs(chord->r_sym_duration));
            
            if (rat_rat_cmp(cur, from_here->pt_in_measure) >= 0 && rat_rat_cmp(next_cur, to_here->pt_in_measure) <= 0) {
                // chord is D
                if (!res) res = chord->prev;
                chord_delete_from_measure((t_notation_obj *)x, chord, false);
            } else if (rat_rat_cmp(next_cur, from_here->pt_in_measure) > 0 && rat_rat_cmp(next_cur, to_here->pt_in_measure) <= 0) {
                // chord is LD: change duration
                if (!res) res = chord;
                chord->r_sym_duration = rat_long_prod(rat_rat_diff(from_here->pt_in_measure, cur), sign);
            } else if (rat_rat_cmp(cur, to_here->pt_in_measure) < 0 && rat_rat_cmp(cur, from_here->pt_in_measure) >= 0) {
                // chord is DR: change duration
                if (!res) res = chord->prev;
                chord->r_sym_duration = rat_long_prod(rat_rat_diff(next_cur, to_here->pt_in_measure), sign);
            } else if (rat_rat_cmp(cur, from_here->pt_in_measure) < 0 && rat_rat_cmp(next_cur, to_here->pt_in_measure) > 0) {
                // chord is LDR: split!
                t_rational left_dur = rat_long_prod(rat_rat_diff(from_here->pt_in_measure, cur), sign);
                t_rational right_dur = rat_long_prod(rat_rat_diff(next_cur, to_here->pt_in_measure), sign);
                t_chord *newleftchord = split_chord(x, chord, 2, true);
                tie_chord(newleftchord);
                newleftchord->r_sym_duration = left_dur;
                newleftchord->next->r_sym_duration = right_dur;
                if (!res) res = newleftchord;
            }
            
            cur = next_cur;
        }
        
    } else {
        
        // start measure
        t_chord *chord, *nextchord;
        t_rational cur = long2rat(0);
        for (chord =start_meas->firstchord; chord; chord = nextchord) {
            
            nextchord = chord->next;
            
            t_rational next_cur = chord->is_grace_chord ? cur : rat_rat_sum(cur, rat_abs(chord->r_sym_duration));
            if (rat_rat_cmp(cur, from_here->pt_in_measure) >= 0) {
                // delete chord
                if (!res) res = chord->prev;
                chord_delete_from_measure((t_notation_obj *)x, chord, false);
            } else if (rat_rat_cmp(next_cur, from_here->pt_in_measure) > 0) {
                // change chord duration
                if (!res) res = chord;
                long sign = isign(chord->r_sym_duration.r_num);
                chord->r_sym_duration = rat_long_prod(rat_rat_diff(from_here->pt_in_measure, cur), sign);
            }
            
            cur = next_cur;
        }
        
        // middle measures
        t_measure *meas;
        for (meas = start_meas->next; meas; meas = meas->next) {
            if (meas == end_meas)
                break;
            clear_measure(x, meas, false, false, false);
        }
        
        // last measure
        cur = long2rat(0);
        for (chord =end_meas->firstchord; chord; chord = nextchord) {
            
            nextchord = chord->next;
            
            t_rational next_cur = chord->is_grace_chord ? cur : rat_rat_sum(cur, rat_abs(chord->r_sym_duration));
            if (rat_rat_cmp(next_cur, to_here->pt_in_measure) <= 0) {
                // delete chord
                chord_delete_from_measure((t_notation_obj *)x, chord, false);
            } else if (rat_rat_cmp(cur, to_here->pt_in_measure) < 0) {
                // change chord duration
                long sign = isign(chord->r_sym_duration.r_num);
                chord->r_sym_duration = rat_long_prod(rat_rat_diff(next_cur, to_here->pt_in_measure), sign);
            }
            
            cur = next_cur;
        }
    }

    return res;
}


// a bit hardcore: does not assign hatom to NEW rhythmic elements, but rather chains elements DIRECTLY
// (so we won't lose the correct pointing from chords to corresponding rhythmic trees elements)
void llll_chain_dontfreeelems(t_llll *where, t_llll *what){
    t_llllelem *elem, *nextelem;
    if (!what) return;
    for (elem = what->l_head; elem; elem = nextelem) {
        nextelem = elem->l_next;
        llll_append(where, elem);
    }
    llll_check(where);
    
    what->l_count--; // hardcore stuff...
    if (what->l_thing.w_obj)
        bach_freeptr(what->l_thing.w_obj);
    llll_dispose(what);
}

void llll_insert_dontfreeelems(t_llll *where, t_llll *what, t_llllelem *after_this){
    t_llllelem *elem, *nextelem;
    if (!what) return;
    for (elem = what->l_head; elem; elem = nextelem) {
        nextelem = elem->l_next;
        if (!after_this) // beginning!
            llll_prepend(where, elem);
        else
            llll_insert_after(elem, after_this);
        after_this = elem;
    }
    llll_check(where);
    
    what->l_count--; // hardcore stuff...
    if (what->l_thing.w_obj)
        bach_freeptr(what->l_thing.w_obj);
    llll_dispose(what);
}


// tp1 + delta_sym_dur
t_timepoint timepoint_rat_sum(t_scorevoice *voice, t_timepoint tp1, t_rational delta_sym_dur)
{
    t_timepoint res = tp1;
    res.pt_in_measure = rat_rat_sum(res.pt_in_measure, delta_sym_dur);
    
    t_measure *meas = nth_measure_of_scorevoice(voice, tp1.measure_num);
    while (meas && rat_rat_cmp(res.pt_in_measure, ts_to_sym_duration(&meas->timesignature)) > 0) {
        t_rational measure_sym_dur = ts_to_sym_duration(&meas->timesignature);
        t_rational overflow = rat_rat_diff(res.pt_in_measure, measure_sym_dur);
        meas = meas->next;
        if (!meas) {
            res.pt_in_measure = measure_sym_dur;
            break;
        }
        res.measure_num++;
        res.pt_in_measure = overflow;
    }
    return res;
}



// Experimental!!!
void overtype_voice(t_score *x, t_scorevoice *voice, t_timepoint *from_here, t_timepoint *to_here, t_llll *new_content)
{
    if (voice && voice->v_ob.number < x->r_ob.num_voices && from_here && new_content && new_content->l_size > 0) {
        t_llll *new_content_modif = llll_clone(new_content);
        t_llll *measureinfo = llll_from_text_buf("(4 4)", false);
        t_measure *fakemeas = build_measure((t_notation_obj *)x, measureinfo);
        fakemeas->voiceparent = voice;
        llll_prependllll(new_content_modif, measureinfo);
        set_measure_from_llll(x, fakemeas, new_content_modif, true, false, NULL);
        process_rhythmic_tree((t_notation_obj *) x, fakemeas, k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
        llll_free(new_content_modif);
        
        // 0. inferring "to_here" if not defined
        t_timepoint to_here_ok;
        t_rational region_dur;
        
        if (to_here) {
            to_here_ok = *to_here;
            region_dur = get_sym_durations_between_timepoints(voice, *from_here, to_here_ok);
        } else {
            region_dur = measure_get_content_sym_duration(fakemeas);
            to_here_ok = timepoint_rat_sum(voice, *from_here, region_dur);
        }
        
        
        // 1. making sure that the content we're putting in has the correct duration (possibly padding with rests)
        long num = 4, den = 4;
        long l2den = perfect_log2(region_dur.r_den);
        if (l2den >= 0) {
            den = MAX(4, iexp2(l2den));
            num = region_dur.r_num * (den/region_dur.r_den);
        } else {
            num = region_dur.r_num;
            den = region_dur.r_den;
        }
        
        t_llll *ts = long_couple_to_llll(num, den);
        set_measure_ts_and_tempo_from_llll((t_notation_obj *) x, fakemeas, ts, NULL, 0, NULL, false);
        
        check_measure_autocompletion(x, fakemeas); // we now count on autocompletion to trim stuff properly
        compute_note_approximations_for_measure((t_notation_obj *)x, fakemeas, false);
        validate_accidentals_for_measure((t_notation_obj *)x, fakemeas);

        llll_free(ts);
  
        
        // 2. cleaning up region from start_meas and end_meas
        t_chord *insert_after_this_chord_orig = clear_region(x, voice, from_here, &to_here_ok);
        
        // 3. appending all content to the first measure
        t_measure *start_meas = nth_measure_of_scorevoice(voice, from_here->measure_num);
        t_measure *end_meas = nth_measure_of_scorevoice(voice, to_here_ok.measure_num);

//        llll_cpost(start_meas->rhythmic_tree);
        
        t_llllelem *insert_after_this_chord_rhythmic_tree_el = insert_after_this_chord_orig ? insert_after_this_chord_orig->rhythmic_tree_elem : NULL;
        if (start_meas != end_meas) {
            insert_after_this_chord_orig = start_meas->lastchord;
            insert_after_this_chord_rhythmic_tree_el = WHITENULL_llllelem;
        }

        t_chord *insert_after_this_chord = insert_after_this_chord_orig;
        if (fakemeas->firstchord && start_meas) {
            // Get last chord of first measure
            t_chord *ch, *nextch;
            for (ch = fakemeas->firstchord; ch; ch = nextch) {
                nextch = ch->next;
                shashtable_chuck_thing(x->r_ob.IDtable, ch->r_it.ID);
                insert_chord_in_measure((t_notation_obj *)x, start_meas, ch, insert_after_this_chord, ch->r_it.ID);
                if (start_meas == end_meas)
                    insert_after_this_chord = ch;
                else
                    insert_after_this_chord = start_meas->lastchord;
            }
            
            if (insert_after_this_chord_rhythmic_tree_el == WHITENULL_llllelem)
                llll_chain_dontfreeelems(start_meas->rhythmic_tree, fakemeas->rhythmic_tree);
            else
                llll_insert_dontfreeelems(start_meas->rhythmic_tree, fakemeas->rhythmic_tree, insert_after_this_chord_rhythmic_tree_el);
 
//            llll_cpost(start_meas->rhythmic_tree);
//            llll_check(start_meas->rhythmic_tree);
        }
        

        // 4. turning on auto-refill of measures
        t_measure *meas;
        for (meas = start_meas; meas; meas = meas->next) {
            if (meas == end_meas)
                break;
            meas->beaming_calculation_flags = k_BEAMING_CALCULATION_FROM_SCRATCH_AND_OVERFLOW_TO_NEXT;
            meas->need_check_autocompletion = true;
        }
        
        free_measure_ext((t_notation_obj *)x, fakemeas, true, true);
    }
}

void overtype(t_score *x, t_timepoint *from_here, t_timepoint *to_here, t_llll *new_content)
{
    long start_voice = from_here->voice_num;
    long end_voice = to_here ? to_here->voice_num : start_voice + new_content->l_size;
    long i;
    t_llllelem *elem;
    
    lock_general_mutex((t_notation_obj *)x);
    for (elem = new_content->l_head, i = start_voice; i <= end_voice; i++, elem = elem ? elem->l_next : NULL) {
        t_timepoint from_here_voice = build_timepoint_with_voice(from_here->measure_num, from_here->pt_in_measure, i);
        t_timepoint to_here_voice;
        if (to_here)
            to_here_voice = build_timepoint_with_voice(to_here->measure_num, to_here->pt_in_measure, i);
        overtype_voice(x, nth_scorevoice(x, i), &from_here_voice, to_here ? &to_here_voice : NULL, elem && hatom_gettype(&elem->l_hatom) == H_LLLL ? hatom_getllll(&elem->l_hatom) : NULL);
    }
    
    recompute_all_and_redraw((t_score *)x);
    perform_analysis_and_change(x, NULL, NULL, x->r_ob.take_rhythmic_tree_for_granted ? k_BEAMING_CALCULATION_DONT_CHANGE_ANYTHING : k_BEAMING_CALCULATION_FROM_SCRATCH);
    unlock_general_mutex((t_notation_obj *)x);
}





void score_ceilmeasures(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_llll *args = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);

    if (args && args->l_head && is_hatom_number(&args->l_head->l_hatom)) {
        long how_many = hatom_getlong(&args->l_head->l_hatom);
        score_ceilmeasures_ext(x, x->firstvoice, nth_scorevoice(x, x->r_ob.num_voices - 1), &how_many);
    } else {
        if (x->r_ob.num_voices >= 2)
            score_ceilmeasures_ext(x, x->firstvoice, nth_scorevoice(x, x->r_ob.num_voices - 1), NULL);
    }

    llll_free(args);
}
	
// outputs a "subscore"
// syntax: subscore (voice1 voice2 ... ) (start_measure end_measure)
// use () for all voices, use () for all durations.
// e.g. select (1 3 4) (2 4) 
//		selects voices 1, 3 and 4 from measure 2 to measure 4. Use negative values to say: default values (e.g. (2 -1) means from measure 2 to the end)
void score_subscore(t_score *x, t_symbol *s, long argc, t_atom *argv){

	if (proxy_getinlet((t_object *) x) == 0) {
		t_llll *inputlist = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE); // We clone it: we operate destructively
		if (inputlist && (inputlist->l_head)) {
			long from_meas, to_meas;
			t_llllelem *firstelem = inputlist->l_head;
			t_llll *which_voices;
			t_llll *what_to_dump = NULL;

			if (hatom_gettype(&firstelem->l_hatom) == H_LLLL)
				which_voices = hatom_getllll(&firstelem->l_hatom);
			else
				which_voices = llll_get(); // all voices
			
			from_meas = 0; to_meas = get_max_num_measures(x, NULL) - 1;
			if (inputlist->l_head->l_next) { // there is a ms specification
				t_llllelem *secondelem = inputlist->l_head->l_next;
				if (hatom_gettype(&secondelem->l_hatom) == H_LLLL) {
					t_llll *boundaries_meas = hatom_getllll(&secondelem->l_hatom);
					if (boundaries_meas->l_size >=2) {
						from_meas = hatom_getlong(&boundaries_meas->l_head->l_hatom) - 1;
						to_meas = hatom_getlong(&boundaries_meas->l_head->l_next->l_hatom) - 1;
						if (from_meas < 0) from_meas = 0;
						if (to_meas < 0) to_meas = get_max_num_measures(x, NULL) - 1;
					} else if (boundaries_meas->l_size >=1) {
						from_meas = hatom_getlong(&boundaries_meas->l_head->l_hatom) - 1;
						if (from_meas < 0) from_meas = 0;
						to_meas = from_meas;
					}
				}
				if (secondelem && secondelem->l_next && hatom_gettype(&secondelem->l_next->l_hatom) == H_LLLL)
					what_to_dump = hatom_getllll(&secondelem->l_next->l_hatom);
			}
			
			send_subscore_values_as_llll(x, which_voices, from_meas, to_meas, what_to_dump);

			llll_free(inputlist);
		}
	}
}



t_score* score_new(t_symbol *s, long argc, t_atom *argv)
{
	t_score* x = NULL;
	t_max_err err = MAX_ERR_GENERIC; 
	t_dictionary *d;
	long flags;

	if (!(d=object_dictionaryarg(argc,argv)))
		return NULL;

	x = (t_score*) object_alloc_debug(s_score_class);
	flags = 0 
			| JBOX_DRAWFIRSTIN 
			| JBOX_NODRAWBOX
			| JBOX_DRAWINLAST
	//		| JBOX_TRANSPARENT	
	//		| JBOX_NOGROW
	//		| JBOX_GROWY
			| JBOX_GROWBOTH
			| JBOX_HILITE
	//		| JBOX_BACKGROUND
			| JBOX_TEXTFIELD
	//		| JBOX_DRAWBACKGROUND
	//		| JBOX_DEFAULTNAMES
	//		| JBOX_MOUSEDRAGDELTA
			;

	x->r_ob.creatingnewobj = 1;
	
	err = jbox_new(&x->r_ob.j_box.l_box, flags, argc, argv); 
	x->r_ob.j_box.l_box.b_firstin = (t_object*) x;

//	bach_breakpoint(0);
	// retrieving patcher parent
	object_obex_lookup(x, gensym("#P"), &(x->r_ob.patcher_parent));

	initialize_notation_obj((t_notation_obj *) x, k_NOTATION_OBJECT_SCORE, (rebuild_fn) set_score_from_llll, 
							(notation_obj_fn) create_whole_score_undo_tick, (notation_obj_notation_item_fn) force_notation_item_inscreen);

    x->r_ob.timepoint_to_unscaled_xposition = (notation_obj_timepoint_to_ux_fn)timepoint_to_unscaled_xposition;
    
	scoreapi_initscore_step01(x);

	x->r_ob.first_shown_system = 0;
	x->r_ob.last_shown_system = 0;
	x->r_ob.take_rhythmic_tree_for_granted = 0;

	initialize_slots((t_notation_obj *) x, false);
    
    x->r_ob.inner_width = 526 - (2 * x->r_ob.j_inset_x); // 526 is the default object width
	
    // @arg 0 @name numvoices @optional 1 @type int @digest Number of voices
	notation_obj_arg_attr_dictionary_process_with_bw_compatibility(x, d);
	
	t_llll *right = llll_slice(x->r_ob.voicenames_as_llll, x->r_ob.num_voices);
	llll_free(right);

	notation_obj_setattr_stafflines((t_notation_obj *) x, NULL, -1, NULL); // -1 is to handle it inside
	
	parse_fullaccpattern_to_voices((t_notation_obj *) x);

	llllobj_jbox_setup((t_llllobj_jbox *) x, 7, "b44444444");
	
	initialize_textfield((t_notation_obj *) x);	
	initialize_commands((t_notation_obj *) x);
	initialize_popup_menus((t_notation_obj *) x);

	scoreapi_initscore_step02(x);

	// clock
	x->r_ob.m_clock = clock_new_debug((t_object *)x, (method) score_task);
	x->r_ob.step_y = CONST_STEP_UY * x->r_ob.zoom_y; // we need it before set_score_from_llll
	calculate_voice_offsets((t_notation_obj *) x);
	compute_middleC_position_for_all_voices((t_notation_obj *) x);
	
	// creating proxies
	x->m_proxy6 = proxy_new_debug((t_object *) x, 6, &x->m_in);
	x->m_proxy5 = proxy_new_debug((t_object *) x, 5, &x->m_in);
	x->m_proxy4 = proxy_new_debug((t_object *) x, 4, &x->m_in);
	x->m_proxy3 = proxy_new_debug((t_object *) x, 3, &x->m_in);
	x->m_proxy2 = proxy_new_debug((t_object *) x, 2, &x->m_in);
	x->m_proxy1 = proxy_new_debug((t_object *) x, 1, &x->m_in);

	jbox_ready(&x->r_ob.j_box.l_box);

	if (x) {
	
		// retrieving values
		t_llll *llll_for_rebuild = llll_retrieve_from_dictionary(d, "whole_score_data");
		if (llll_for_rebuild) { // new method
			llllobj_manage_dict_llll((t_object *)x, LLLL_OBJ_UI, llll_for_rebuild);
			set_score_from_llll(x, llll_for_rebuild, true);
			if (x->r_ob.send_rebuild_done_at_startup)
				handle_rebuild_done((t_notation_obj *) x);
			double first_onset_ms = get_first_onset_ms_for_grace_notes(x);
			x->r_ob.play_head_start_ms = (first_onset_ms < 0 ? first_onset_ms - CONST_EPSILON2 : first_onset_ms);
			llll_free(llll_for_rebuild);
		} else { // old method, or brand new object
			long ac; t_atom *av = NULL;
			t_max_err err;
			err = dictionary_getatoms(d, gensym("whole_score_data"), &ac, &av);
			if (!err && ac && av) {
				score_anything(x, NULL, ac, av);
			} else {
				t_measure *added_meas[CONST_MAX_VOICES];
				long num_added_meas = 0;
				// brand new object: we add a measure in order to distinguish between the newly instantiated bach.roll object and the newly instantiated bach.score object
				insert_new_measure_in_all_voices(x, x->firstvoice, 0, 1, added_meas, &num_added_meas, false, NULL);
                recompute_all(x);
                perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
			}
		}
		
		if (!USE_NEW_UNDO_SYSTEM && x->r_ob.allow_undo)
			x->r_ob.old_undo_llll[0] = get_score_values_as_llll(x, k_CONSIDER_FOR_SAVING, k_HEADER_BODY + k_HEADER_SLOTINFO + k_HEADER_VOICENAMES + k_HEADER_MARKERS + k_HEADER_ARTICULATIONINFO + k_HEADER_NOTEHEADINFO + k_HEADER_NUMPARTS, true, true, true, false);

		x->r_ob.last_undo_time = systime_ms();

		if (x->r_ob.old_timepoint_syntax_bw_compatibility) {
			// We were still using old syntax for timepoints concerning red markers and loop region extremes saved content
			// What we did is: if the version was < 7.2, we set those elements as for the "old syntax". 
			// Then at the next saving of the patch, everything will be updated properly to the "new syntax" (version number included, of course)

			x->r_ob.old_timepoint_syntax_bw_compatibility = false;

			// Now (after bw compatibility is set to false) we also need to re-obtain the loop region in the new syntax.
			if (x->r_ob.loop_region_as_llll) {
				llll_free(x->r_ob.loop_region_as_llll);
				x->r_ob.loop_region_as_llll = get_loop_region_as_llll((t_notation_obj *)x, false);
			}
		}
		
		sync_loop_ms_with_loop_tp(x);

		if (x->r_ob.automessage_ac > 0)
			x->r_ob.need_send_automessage = true;

        x->r_ob.version_number = BACH_CURRENT_VERSION;
        x->r_ob.creatingnewobj = 0;
        
		return x;
	}

	object_free_debug(x); // unlike freeobject(), this works even if the argument is NULL
	return NULL;

}

void score_int(t_score *x, t_atom_long num)
{
	t_atom argv[1]; atom_setlong(argv, num);
	score_anything(x, _llllobj_sym_list, 1, argv);
}

void score_float(t_score *x, double num){
	t_atom argv[1]; atom_setfloat(argv, num);
	score_anything(x, _llllobj_sym_list, 1, argv);
}


void score_free(t_score *x)
{
	scoreapi_destroyscore(x);
	
	free_notation_obj((t_notation_obj *)x);

	// freeing proxies
	object_free_debug(x->m_proxy1);
	object_free_debug(x->m_proxy2);
	object_free_debug(x->m_proxy3);
	object_free_debug(x->m_proxy4);
	object_free_debug(x->m_proxy5);
	object_free_debug(x->m_proxy6);
	
	object_free_debug(x->r_ob.m_clock);
}


// INLET INTERACTION:
// - a llll in the first inlet reconstruct the whole score
// - a llll in the following inlets sets a cache for onsets/cents/durations/velocities/extra
// - a bang in the first inlets change/builds things starting from the cache values


void score_jsave(t_score *x, t_dictionary *d)
{
	if (x->r_ob.save_data_with_patcher) {
		if (x->r_ob.j_box.l_dictll) {
			llll_store_in_dictionary(x->r_ob.j_box.l_dictll, d, "whole_score_data", NULL);
		} else {
			t_llll *whole_info = get_score_values_as_llll(x, x->r_ob.bwcompatible <= 7900 ? k_CONSIDER_FOR_SAVING_WITH_BW_COMPATIBILITY : k_CONSIDER_FOR_SAVING, k_HEADER_BODY + k_HEADER_SLOTINFO + k_HEADER_MARKERS + k_HEADER_MIDICHANNELS + k_HEADER_COMMANDS + k_HEADER_ARTICULATIONINFO + k_HEADER_NOTEHEADINFO, true, true, true, false); // clefs, keys, voicenames and parts are already saved as attributes
			llll_store_in_dictionary(whole_info, d, "whole_score_data", NULL);
			llll_free(whole_info);
		}
	}
}


///////////////////////////////////////////
///////////// MOUSE ACTIONS ///////////////
///////////////////////////////////////////

void score_mouseenter(t_score *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->r_ob.j_mouse_is_over = true;
    notationobj_redraw((t_notation_obj *) x);
}

void score_mouseleave(t_score *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->r_ob.j_mouse_is_over = false;
    notationobj_redraw((t_notation_obj *) x);
}

void score_mousemove(t_score *x, t_object *patcherview, t_pt pt, long modifiers)
{
	char redraw = false, mousepointerchanged = false;
	t_rect rect;
	jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, patcherview, &rect);

	llll_format_modifiers(&modifiers, NULL);

	x->r_ob.j_mouse_x = pt.x; 
	x->r_ob.j_mouse_y = pt.y; //track mouse position
	
    if (x->r_ob.mouse_hover) {
        handle_slot_mousemove((t_notation_obj *) x, patcherview, pt, modifiers, &redraw, &mousepointerchanged);
        
        if (!x->r_ob.active_slot_notationitem && !mousepointerchanged)
            notationobj_handle_change_cursors_on_mousemove((t_notation_obj *)x, patcherview, pt, modifiers, rect);
        else if (!mousepointerchanged)
            bach_set_cursor((t_object *)x, &x->r_ob.j_mouse_cursor, patcherview, BACH_CURSOR_DEFAULT);
    }

	if (redraw)
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
    else if (x->r_ob.legend > 1)
        notationobj_redraw((t_notation_obj *) x);
}

long note_get_num_breakpoints(t_note* note)
{
	return note->num_breakpoints;
}

double force_inscreen_ux_to_boundary_and_set_mouse_position(t_score *x, double ux, t_object *patcherview, t_pt pt, char only_if_pt_is_outside_active_screen)
{
	double out = pt.x;

	if (!only_if_pt_is_outside_active_screen || 
		(pt.x > unscaled_xposition_to_xposition((t_notation_obj *)x, x->r_ob.screen_ux_end) || 
			(pt.x < unscaled_xposition_to_xposition((t_notation_obj *)x, x->r_ob.screen_ux_start) && x->r_ob.screen_ux_start > 0))) {

		char force_inscreen = force_inscreen_ux_to_boundary(x, ux, true, true, true);
		if (force_inscreen == 1)
			jmouse_setposition_box(patcherview, (t_object *) x, out = unscaled_xposition_to_xposition((t_notation_obj *) x, x->r_ob.screen_ux_end), pt.y);
		else if (force_inscreen == -1)
			jmouse_setposition_box(patcherview, (t_object *) x, out = unscaled_xposition_to_xposition((t_notation_obj *) x, x->r_ob.screen_ux_start), pt.y);
	
	}
	
	return out;
}

void score_mousedrag(t_score *x, t_object *patcherview, t_pt pt, long modifiers)
{
	char slot_dragged = 0;
	char redraw = 0; // do we have to redraw?
	char changed = 0; // did we change things?
	t_rect rect;
	t_pt prev_mousedrag_point = x->r_ob.j_mousedrag_point;
	//	post("mousedrag. x->r_ob.j_mousedown_NN: %d, x->r_ob.j_selected_NN: %d", x->r_ob.j_mousedown_NN, x->r_ob.j_selected_NN);
	
	llll_format_modifiers(&modifiers, NULL);
	x->r_ob.j_isdragging = true;
	
	if (x->r_ob.main_dragging_direction == 0)
		x->r_ob.main_dragging_direction = (fabs(x->r_ob.j_mousedown_point.x - pt.x) > fabs(x->r_ob.j_mousedown_point.y - pt.y) ? -1 : 1);
	
	if (modifiers & eShiftKey && modifiers & eCommandKey) {
		x->r_ob.j_mousedrag_point_shift_ffk.x += (pt.x - x->r_ob.j_mousedrag_point.x) * CONST_FINER_FROM_KEYBOARD;
		x->r_ob.j_mousedrag_point_shift_ffk.y += (pt.y - x->r_ob.j_mousedrag_point.y) * CONST_FINER_FROM_KEYBOARD;
		x->r_ob.j_mousedrag_point = pt;
	} else if (modifiers == eShiftKey) {
		if (x->r_ob.main_dragging_direction < 0)
			x->r_ob.j_mousedrag_point_shift_ffk.x = pt.x;
		else
			x->r_ob.j_mousedrag_point_shift_ffk.y = pt.y;
		x->r_ob.j_mousedrag_point = pt;
	} else {
		x->r_ob.j_mousedrag_point_shift_ffk = x->r_ob.j_mousedrag_point = pt;
	}
	
	// getting rectangle dimensions
	jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, patcherview, &rect);
	
    notationobj_handle_change_cursors_on_mousedrag((t_notation_obj *)x, patcherview, pt, modifiers);

	// first of all: are we in a slot mode???? Cause if we are in a slot mode, we gotta handle that separately
	if (x->r_ob.active_slot_num > -1 && !is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) return;
	slot_dragged = handle_slot_mousedrag((t_notation_obj *) x, patcherview, pt, modifiers, &changed, &redraw);
	if (slot_dragged) {
		handle_change((t_notation_obj *) x, x->r_ob.continuously_output_changed_bang ? k_CHANGED_STANDARD_SEND_BANG : k_CHANGED_REDRAW_STATIC_LAYER, k_UNDO_OP_UNKNOWN);
		x->r_ob.changed_while_dragging = true;
		return;
	}
	
	if (x->r_ob.j_mousedown_obj_type >= k_DILATION_RECTANGLE_TOPLEFT_SQ && x->r_ob.j_mousedown_obj_type <= k_DILATION_RECTANGLE_MIDDLERIGHT_SQ) {
		if (x->r_ob.j_mousedown_obj_type == k_DILATION_RECTANGLE_TOPMIDDLE_SQ) {
			double bottom = mc_to_yposition((t_notation_obj *) x, x->r_ob.dilation_rectangle.bottom_mc, x->r_ob.dilation_rectangle.bottom_voice);
			double old_top = mc_to_yposition((t_notation_obj *) x, x->r_ob.dilation_rectangle.top_mc, x->r_ob.dilation_rectangle.top_voice);
			double factor;
			if (is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH)) {
				x->r_ob.dilation_rectangle.top_mc = yposition_to_mc((t_notation_obj *)x, x->r_ob.j_mousedrag_point_shift_ffk.y, (t_voice *)x->r_ob.dilation_rectangle.top_voice, NULL);
				if (x->r_ob.dilation_rectangle.top_voice == x->r_ob.dilation_rectangle.bottom_voice && x->r_ob.dilation_rectangle.top_mc <= x->r_ob.dilation_rectangle.bottom_mc)
					x->r_ob.dilation_rectangle.top_mc = x->r_ob.dilation_rectangle.bottom_mc + CONST_EPSILON5;
				factor = (x->r_ob.j_mousedrag_point_shift_ffk.y - bottom)/(old_top - bottom);
				factor = MAX(factor, CONST_EPSILON5);
				redraw = 1;
				changed = score_sel_dilate_mc(x, factor, bottom);
			}
		}
		if (x->r_ob.j_mousedown_obj_type == k_DILATION_RECTANGLE_BOTTOMMIDDLE_SQ) {
			double old_bottom = mc_to_yposition((t_notation_obj *) x, x->r_ob.dilation_rectangle.bottom_mc, x->r_ob.dilation_rectangle.bottom_voice);
			double top = mc_to_yposition((t_notation_obj *) x, x->r_ob.dilation_rectangle.top_mc, x->r_ob.dilation_rectangle.top_voice);
			double factor;
			if (is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH)) {
				x->r_ob.dilation_rectangle.bottom_mc = yposition_to_mc((t_notation_obj *)x, x->r_ob.j_mousedrag_point_shift_ffk.y, (t_voice *)x->r_ob.dilation_rectangle.bottom_voice, NULL);
				if (x->r_ob.dilation_rectangle.top_voice == x->r_ob.dilation_rectangle.bottom_voice && x->r_ob.dilation_rectangle.top_mc <= x->r_ob.dilation_rectangle.bottom_mc)
					x->r_ob.dilation_rectangle.bottom_mc = x->r_ob.dilation_rectangle.top_mc - CONST_EPSILON5;
				factor = (top - x->r_ob.j_mousedrag_point_shift_ffk.y)/(top - old_bottom);
				factor = MAX(factor, CONST_EPSILON5);
				redraw = 1;
				changed = score_sel_dilate_mc(x, factor, top);
			}
		}
	} else if (x->r_ob.j_mousedown_obj_type == k_DILATION_RECTANGLE_BODY) {
		double delta_mc = yposition_to_mc((t_notation_obj *)x, x->r_ob.j_mousedrag_point_shift_ffk.y, (t_voice *)x->firstvoice, NULL) - yposition_to_mc((t_notation_obj *)x, prev_mousedrag_point.y, (t_voice *)x->firstvoice, NULL);
		if (is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH)) {
			x->r_ob.dilation_rectangle.top_mc += delta_mc;
			x->r_ob.dilation_rectangle.bottom_mc += delta_mc;
			changed |= change_pitch_for_selection(x, delta_mc, 1, !((modifiers & eCommandKey) && (modifiers & eShiftKey)), false);
			redraw = 1;
		}
	} else if (x->r_ob.j_mousedown_obj_type == k_SCROLLBAR) { // the scrollbar is being drawn!
		// new scrollbar position
		t_rect rect;
		if (!is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) return;
		jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, patcherview, &rect);
		if (x->r_ob.j_mousedrag_point.x >= x->r_ob.j_inset_x && x->r_ob.j_mousedrag_point.x <= rect.width - x->r_ob.j_inset_x) {
			double delta_x =  pt.x - prev_mousedrag_point.x;
			if (modifiers & eShiftKey && modifiers & eCommandKey) 
				delta_x *= CONST_FINER_FROM_KEYBOARD;
			x->r_ob.hscrollbar_x += delta_x;
			redraw_hscrollbar((t_notation_obj *)x, 0);
			send_domain(x, 7, NULL);
			redraw = 0; // already redrawn in redraw_hscrollbar
			changed = 0;
		}
		
	} else if (x->r_ob.j_mousedown_obj_type == k_VSCROLLBAR) { // the vertical scrollbar is being drawn!
		// new vscrollbar position
		t_rect rect;
		if (!is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) return;
		jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, patcherview, &rect);
		if (x->r_ob.j_mousedrag_point.y >= x->r_ob.j_inset_y && x->r_ob.j_mousedrag_point.y <= rect.height - x->r_ob.j_inset_y) {
			double delta_y =  pt.y - prev_mousedrag_point.y;
			if (modifiers & eShiftKey && modifiers & eCommandKey) 
				delta_y *= CONST_FINER_FROM_KEYBOARD;
			x->r_ob.vscrollbar_y += delta_y;
			redraw_vscrollbar((t_notation_obj *)x, 0);
//			send_vdomain(x, 7, NULL);
			redraw = 0; // already redrawn in redraw_hscrollbar
			changed = 0;
		}
		
	} else if (x->r_ob.j_mousedown_obj_type == k_DRAG) { // hand grasp
		// both scrollbar position change
		t_rect rect;
		if (!is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) return;
		jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, patcherview, &rect);
		if (x->r_ob.j_mousedrag_point.x >= x->r_ob.j_inset_x && x->r_ob.j_mousedrag_point.x <= rect.width - x->r_ob.j_inset_x && 
			x->r_ob.j_mousedrag_point.y >= x->r_ob.j_inset_y && x->r_ob.j_mousedrag_point.y <= rect.height - x->r_ob.j_inset_y) {
			double delta_x =  pt.x - prev_mousedrag_point.x;
			double delta_y =  pt.y - prev_mousedrag_point.y;
			if (modifiers & eShiftKey && modifiers & eCommandKey) {
				delta_y *= CONST_FINER_FROM_KEYBOARD;
				delta_x *= CONST_FINER_FROM_KEYBOARD;
			}
			x->r_ob.hscrollbar_x -= delta_x;
			x->r_ob.vscrollbar_y -= delta_y;
			update_hscrollbar((t_notation_obj *)x, 0);
			redraw_vscrollbar((t_notation_obj *)x, 0);
			send_domain(x, 7, NULL);
			redraw = 0; // already redrawn in redraw_hscrollbar
			changed = 0;
		}
	} else if (x->r_ob.j_mousedown_obj_type == k_SLUR_START_POINT) { // the start point of a slur is being drawn
		t_slur *slur = (t_slur *) x->r_ob.j_mousedown_ptr;
		if (!is_editable((t_notation_obj *)x, k_SLUR, k_MODIFICATION_GENERIC)) return;
		slur->start_x = xposition_to_unscaled_xposition((t_notation_obj *) x, pt.x);
		slur->start_y = pt.y;
		redraw = 1;
		changed = 0;
		if (modifiers != eShiftKey){ // reassign start point?
			t_note *new_start_note = find_nearest_note(x, pt.x, pt.y);
			if (new_start_note && new_start_note != slur->start_note){
				change_slur_starting_note(x, slur, new_start_note);
				changed = 1;
			} 
		} 
	} else if (x->r_ob.j_mousedown_obj_type == k_SLUR_END_POINT) { // the end point of a slur is being drawn
		t_slur *slur = (t_slur *) x->r_ob.j_mousedown_ptr;
		if (!is_editable((t_notation_obj *)x, k_SLUR, k_MODIFICATION_GENERIC)) return;
		slur->end_x = xposition_to_unscaled_xposition((t_notation_obj *) x, pt.x);
		slur->end_y = pt.y;
		redraw = 1;
		changed = 0;
		if (modifiers != eShiftKey){ // reassign end point?
			t_note *new_end_note = find_nearest_note(x, pt.x, pt.y);
			if (new_end_note && new_end_note != slur->end_note){
				change_slur_ending_note(x, slur, new_end_note);
				changed = 1;
			} 
		} 
		
	} else if (x->r_ob.selection_type == k_MARKER && x->r_ob.j_mousedown_obj_type == k_MARKER) { // only markers are selected
		double delta_ms = unscaled_xposition_to_ms((t_notation_obj *)x, xposition_to_unscaled_xposition((t_notation_obj *)x, pt.x), 1) -
								unscaled_xposition_to_ms((t_notation_obj *)x, xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.floatdragging_x), 1);
		double mousedown_ux = xposition_to_unscaled_xposition((t_notation_obj *)x, (modifiers & eShiftKey && modifiers & eCommandKey) ? x->r_ob.j_mousedrag_point_shift_ffk.x : pt.x);
		double delta_ux = mousedown_ux - xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.floatdragging_x);

//		double marker_mousedown_ux = get_marker_ux_position(x, marker);

		if (modifiers & eAltKey && !(modifiers & eCommandKey) && !x->r_ob.j_mousedrag_copy_ptr && is_editable((t_notation_obj *)x, k_MARKER, k_CREATION)) { // copy it
			t_notation_item *temp;
			lock_markers_mutex((t_notation_obj *)x);;
			lock_general_mutex((t_notation_obj *)x);
			for (temp = x->r_ob.firstselecteditem; temp; temp = temp->next_selected) {
				if (temp->type == k_MARKER) {
					t_marker *temp_mk = ((t_marker *) temp);
					t_marker *newmarker;
					create_header_undo_tick((t_notation_obj *) x, k_HEADER_MARKERS);
					t_llll *content = NULL;
					if (temp_mk->content) 
						content = llll_clone(temp_mk->content);
					if (temp_mk->attach_to == k_MARKER_ATTACH_TO_MEASURE) {
						t_timepoint orig_tp = measure_attached_marker_to_timepoint((t_notation_obj *) x, temp_mk);
						double marker_ux = timepoint_to_unscaled_xposition((t_notation_obj *)x, orig_tp, false, CONST_MARKERS_ON_FIRST_MEASURE_CHORDS);
						double marker_ms = unscaled_xposition_to_ms((t_notation_obj *)x, marker_ux + delta_ux, 1);
                        long voicenumber = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, NULL, k_VOICEENSEMBLE_INTERFACE_ACTIVE);
                        if (voicenumber < 0)
                            voicenumber = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, NULL, k_VOICEENSEMBLE_INTERFACE_ANY);
						t_timepoint tp = ms_to_timepoint((t_notation_obj *)x, marker_ms, voicenumber, k_MS_TO_TP_RETURN_INTERPOLATION);
						t_llll *chosen = make_marker_name_unique((t_notation_obj *) x, temp_mk->r_it.names);
						newmarker = add_marker((t_notation_obj *) x, chosen, marker_ms, tp, k_MARKER_ATTACH_TO_MEASURE, temp_mk->role, content, 0);
						newmarker->ux_difference_with_mousedown_marker = (marker_ux + delta_ux - get_marker_ux_position((t_notation_obj *)x, (t_marker *)x->r_ob.j_mousedown_ptr));
						sync_marker_absolute_ms_onset(x, newmarker);
						llll_free(chosen);
					} else {
						double marker_ms = temp_mk->position_ms;
						t_llll *chosen = make_marker_name_unique((t_notation_obj *) x, temp_mk->r_it.names);
						newmarker = add_marker((t_notation_obj *) x, chosen, marker_ms + delta_ms, build_timepoint(0, long2rat(0)), k_MARKER_ATTACH_TO_MS, temp_mk->role, content, 0);
						newmarker->ux_difference_with_mousedown_marker = (ms_to_unscaled_xposition((t_notation_obj *)x, marker_ms + delta_ms, 1) - get_marker_ux_position((t_notation_obj *)x, (t_marker *)x->r_ob.j_mousedown_ptr));
						sync_marker_absolute_ms_onset(x, newmarker);
						llll_free(chosen);
					}
					notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)newmarker);
					if (temp == x->r_ob.j_mousedown_ptr) {
						set_mousedown((t_notation_obj *) x, newmarker, k_MARKER);
						x->r_ob.j_mousedrag_copy_ptr = newmarker;
					}
				}
			}
			clear_selection((t_notation_obj *) x);
			move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_INVOLUTIVE, false, false);
			unlock_general_mutex((t_notation_obj *)x);
			unlock_markers_mutex((t_notation_obj *)x);;
		}
	
		if (is_editable((t_notation_obj *)x, k_MARKER, k_MODIFICATION_ONSET)) {
			if (!(x->r_ob.header_undo_flags & k_HEADER_MARKERS)) {
				create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
				x->r_ob.header_undo_flags |= k_HEADER_MARKERS;
			}
			
			if (modifiers & eShiftKey && modifiers & eCommandKey) {
				delta_ux *= CONST_FINER_FROM_KEYBOARD;
				delta_ms *= CONST_FINER_FROM_KEYBOARD;
			}
			
            sync_all_markers_absolute_ms_onset(x);
			changed |= move_selected_measure_attached_markers(x, mousedown_ux, !(modifiers & eShiftKey && !(modifiers & eCommandKey)));
			changed |= move_selected_ms_attached_markers((t_notation_obj *) x, mousedown_ux - x->r_ob.ux_click_marker_diff, (modifiers & eShiftKey && !(modifiers & eCommandKey)), NULL);

			x->r_ob.floatdragging_y = pt.y;
			x->r_ob.floatdragging_x = force_inscreen_ux_to_boundary_and_set_mouse_position(x, xposition_to_unscaled_xposition((t_notation_obj *)x, pt.x), patcherview, pt, true);

			redraw = changed;
		}
		
	} else if (x->r_ob.j_mousedown_obj_type == k_PLAYCURSOR) { // the playcursor is being drawn!
		double min_onset;

		if (!is_editable((t_notation_obj *)x, k_PLAYCURSOR, k_ELEMENT_ACTIONS_NONE)) return;

		if (x->r_ob.playhead_dragging_mode == 1)
			x->r_ob.play_head_start_ux += deltaxpixels_to_deltauxpixels((t_notation_obj *)x, pt.x - prev_mousedrag_point.x);
		else
            x->r_ob.play_head_start_ux = CLAMP(xposition_to_unscaled_xposition((t_notation_obj *)x, pt.x), 0, scoreapi_get_end_ux(x));
		x->r_ob.play_head_start_ms = unscaled_xposition_to_ms((t_notation_obj *)x, x->r_ob.play_head_start_ux, 1);
		
		min_onset = get_first_onset_ms_for_grace_notes(x);
		if (x->r_ob.play_head_start_ms < min_onset)
			x->r_ob.play_head_start_ms = min_onset;

		force_inscreen_ux_to_boundary_and_set_mouse_position(x, x->r_ob.play_head_start_ux, patcherview, pt, false);

        send_moved_playhead_position((t_notation_obj *) x, 7);

        redraw = 1;
		changed = 0;
		
	} else if (x->r_ob.j_mousedown_obj_type == k_LOOP_START || x->r_ob.j_mousedown_obj_type == k_LOOP_END) {
		double this_ms, this_ux;
		t_timepoint this_tp, start_tp = x->r_ob.loop_region.start.timepoint, end_tp = x->r_ob.loop_region.end.timepoint;
		
		if (!is_editable((t_notation_obj *)x, k_LOOP_REGION, k_ELEMENT_ACTIONS_NONE)) return;
		
        create_header_undo_tick((t_notation_obj *)x, k_HEADER_LOOP);

		this_ux = xposition_to_unscaled_xposition((t_notation_obj *)x, pt.x);
		this_ms = unscaled_xposition_to_ms((t_notation_obj *)x, this_ux, 1);
		this_tp = ms_to_timepoint_autochoose_voice((t_notation_obj *)x, this_ms, modifiers & eShiftKey ? k_MS_TO_TP_RETURN_INTERPOLATION : k_MS_TO_TP_RETURN_NEAREST, NULL);
		
		force_inscreen_ux_to_boundary_and_set_mouse_position(x, this_ux, patcherview, pt, false);
		
		if (x->r_ob.j_mousedown_obj_type == k_LOOP_START)
			start_tp = this_tp;
		else if (x->r_ob.j_mousedown_obj_type == k_LOOP_END)
			end_tp = this_tp;
			
		if (timepoints_compare(start_tp, end_tp) > 0) {
			t_timepoint temp = start_tp;
			start_tp = end_tp;
			end_tp = temp;
			set_mousedown((t_notation_obj *) x, WHITENULL_llll, x->r_ob.j_mousedown_obj_type == k_LOOP_START ? k_LOOP_END : k_LOOP_START);
		}
		
		set_loop_region_from_extremes_tp(x, start_tp, end_tp);
		send_loop_region((t_notation_obj *) x, 7);
		redraw = 1;
		changed = 0;
		
	} else if (x->r_ob.j_mousedown_obj_type == k_LOOP_REGION) {
		t_timepoint start_tp = x->r_ob.loop_region.start.timepoint, end_tp = x->r_ob.loop_region.end.timepoint;
		double start_ms = x->r_ob.loop_region.start.position_ms, end_ms = x->r_ob.loop_region.end.position_ms;
		double delta_x = pt.x - x->r_ob.floatdragging_x;
		double orig_clickpoint_ms = unscaled_xposition_to_ms((t_notation_obj *)x, xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.floatdragging_x), 1);
		double new_click_point_ms = unscaled_xposition_to_ms((t_notation_obj *)x, xposition_to_unscaled_xposition((t_notation_obj *)x, pt.x), 1);
		double delta_ms = new_click_point_ms - orig_clickpoint_ms;
	 	double new_start_ms, new_end_ms, new_end_ux, new_start_ux;
		double fl = pt.x;
		
		if (!is_editable((t_notation_obj *)x, k_LOOP_REGION, k_ELEMENT_ACTIONS_NONE)) return;
		
        create_header_undo_tick((t_notation_obj *)x, k_HEADER_LOOP);

		if (delta_ms < 0 && -delta_ms > start_ms)
			delta_ms = -start_ms;
			
		new_start_ms = start_ms + delta_ms;
		new_end_ms = end_ms + delta_ms;
		new_start_ux = ms_to_unscaled_xposition((t_notation_obj *)x, new_start_ms, 1);
		new_end_ux = ms_to_unscaled_xposition((t_notation_obj *)x, new_end_ms, 1);

//		this_ux = xposition_to_unscaled_xposition((t_notation_obj *)x, pt.x);
//		this_ms = unscaled_xposition_to_ms((t_notation_obj *)x, this_ux, 1);
		start_tp = ms_to_timepoint_autochoose_voice((t_notation_obj *)x, new_start_ms, k_MS_TO_TP_RETURN_INTERPOLATION, NULL);
		end_tp = ms_to_timepoint_autochoose_voice((t_notation_obj *)x, new_end_ms, k_MS_TO_TP_RETURN_INTERPOLATION, NULL);
		
		force_inscreen_ux_to_boundary_and_set_mouse_position(x, delta_x > 0 ? new_end_ux : new_start_ux, patcherview, pt, false);

		x->r_ob.floatdragging_x = fl;
		set_loop_region_from_extremes_tp(x, start_tp, end_tp);
		send_loop_region((t_notation_obj *) x, 7);
		redraw = 1;
		changed = 0;
	
	} else if (x->r_ob.j_mousedown_obj_type == k_VOICE && x->r_ob.j_mousedown_ptr && (modifiers & eShiftKey)) {
		if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)x->r_ob.j_mousedown_ptr)){
			long number = ((t_scorevoice *)x->r_ob.j_mousedown_ptr)->v_ob.number;
			double delta_y =  pt.y - prev_mousedrag_point.y;
			if (modifiers & eControlKey) number++;
			
			if (!is_editable((t_notation_obj *)x, k_VOICE, k_MODIFICATION_POSITION)) return;
			if (number == 0) {
				x->r_ob.voiceuspacing_as_floatlist[0] += delta_y/x->r_ob.zoom_y;
				x->r_ob.head_vertical_additional_uspace += delta_y/x->r_ob.zoom_y;
			} else if (number < x->r_ob.num_voices + 1) {
				t_scorevoice *voice = nth_scorevoice(x, number-1);
                voice = (t_scorevoice *)voiceensemble_get_lastvoice((t_notation_obj *)x, (t_voice *)voice);
				if (voice) {
					x->r_ob.voiceuspacing_as_floatlist[number] += delta_y/x->r_ob.zoom_y;
					voice->v_ob.vertical_uspacing += delta_y/x->r_ob.zoom_y;
					if (modifiers & eAltKey && voice->next && number < CONST_MAX_VOICES){
						x->r_ob.voiceuspacing_as_floatlist[number + 1] -= delta_y/x->r_ob.zoom_y;
						voice->next->v_ob.vertical_uspacing -= delta_y/x->r_ob.zoom_y;
					}
				}
			}
			if (x->r_ob.link_vzoom_to_height)
				auto_set_rectangle_size((t_notation_obj *) x);
			else
				calculate_voice_offsets((t_notation_obj *) x);
            reset_all_articulations_positions((t_notation_obj *) x);
			redraw = 1;
			changed = 0;
		}
		
	} else if (x->r_ob.j_mousedown_obj_type == k_MEASURE_END_BARLINE && x->r_ob.j_mousedown_ptr && (modifiers & eShiftKey)) {
		// changing measure width (i.e. tuttipoint region width) for all selected barlines!
		t_tuttipoint *tpt;
		t_notation_item *item;
		if (!is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_GENERIC)) return;
		for (tpt = x->firsttuttipoint; tpt; tpt = tpt->next)
			tpt->flag &= ~k_FLAG_MODIFIED;
		
		for (item = x->r_ob.firstselecteditem; item; item = item->next_selected){
			if (item->type == k_MEASURE_END_BARLINE) {
				t_measure *meas = ((t_measure_end_barline *)item)->owner;
				if (meas && !notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)meas)){

					if (!(meas->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
						create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);

					tpt = meas->tuttipoint_reference;
					if (tpt && !(tpt->flag & k_FLAG_MODIFIED)) {
						if (tpt->is_spacing_fixed) {
							assign_fixed_spacing_uwidth((t_notation_obj *)x, tpt, tpt->width_ux + (pt.x - prev_mousedrag_point.x));
							assign_local_spacing_width_multiplier((t_notation_obj *)x, tpt, 1.);
						} else {
							double new_value = tpt->local_spacing_width_multiplier * (tpt->width_ux + (pt.x - prev_mousedrag_point.x)/x->r_ob.zoom_x)/tpt->width_ux;
							assign_local_spacing_width_multiplier((t_notation_obj *)x, tpt, MAX(new_value, CONST_EPSILON1));
						}
						recompute_all_for_measure((t_notation_obj *) x, meas, false);
						tpt->flag |= k_FLAG_MODIFIED;
						redraw = 1;
						changed = 1;
					}
				}
			}
		}
	
		for (tpt = x->firsttuttipoint; tpt; tpt = tpt->next)
			tpt->flag &= ~k_FLAG_MODIFIED;

	} else if (x->r_ob.j_mousedown_obj_type == k_REGION) { // a region has been selected
	// TO DO : xposition_to_onset must be changed
		if (!is_editable((t_notation_obj *)x, k_SELECTION, k_SINGLE_SELECTION) && !is_editable((t_notation_obj *)x, k_SELECTION, k_MULTIPLE_SELECTION)) return;
		x->r_ob.j_selected_region_ux1 = xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.j_mousedown_point.x);
		x->r_ob.j_selected_region_mc1 = yposition_to_mc((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, NULL, NULL);
		x->r_ob.j_selected_region_ux2 =  xposition_to_unscaled_xposition((t_notation_obj *)x, pt.x);
		x->r_ob.j_selected_region_mc2 = yposition_to_mc((t_notation_obj *)x, pt.y, NULL, NULL);
        
        if (x->r_ob.j_selected_region_ux1 > x->r_ob.j_selected_region_ux2)
            swap_doubles(&x->r_ob.j_selected_region_ux1, &x->r_ob.j_selected_region_ux2);
        
        if (x->r_ob.j_mousedown_point.y < pt.y) {
            swap_doubles(&x->r_ob.j_selected_region_mc1, &x->r_ob.j_selected_region_mc2);
            x->r_ob.j_selected_region_voice2 = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_FIRST);
            x->r_ob.j_selected_region_voice1 = yposition_to_voicenumber((t_notation_obj *)x, pt.y, -1, k_VOICEENSEMBLE_INTERFACE_LAST);
        } else {
            x->r_ob.j_selected_region_voice1 = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_LAST);
            x->r_ob.j_selected_region_voice2 = yposition_to_voicenumber((t_notation_obj *)x, pt.y, -1, k_VOICEENSEMBLE_INTERFACE_FIRST);
        }
        
//        dev_post("voice1: %ld, voice2: %ld - mc1: %.2f, mc2: %.2f", x->r_ob.j_selected_region_voice1, x->r_ob.j_selected_region_voice2, x->r_ob.j_selected_region_mc1, x->r_ob.j_selected_region_mc2);

		lock_general_mutex((t_notation_obj *)x);
		clear_preselection((t_notation_obj *)x);
		preselect_elements_in_region_for_mouse_selection(x, x->r_ob.j_selected_region_ux1, x->r_ob.j_selected_region_ux2, x->r_ob.j_selected_region_mc1, x->r_ob.j_selected_region_mc2, x->r_ob.j_selected_region_voice1, x->r_ob.j_selected_region_voice2);
		if ((x->r_ob.j_mousedown_point.y > 3 * x->r_ob.zoom_y && 3 * x->r_ob.zoom_y > pt.y) || (pt.y > 3 * x->r_ob.zoom_y && 3 * x->r_ob.zoom_y > x->r_ob.j_mousedown_point.y)) {
			double ms1 = unscaled_xposition_to_ms((t_notation_obj *)x, x->r_ob.j_selected_region_ux1, 1);
			double ms2 = unscaled_xposition_to_ms((t_notation_obj *)x, x->r_ob.j_selected_region_ux2, 1);
			preselect_markers_in_region((t_notation_obj *)x, ms1, ms2);
		}
		if (!is_editable((t_notation_obj *)x, k_SELECTION, k_MULTIPLE_SELECTION)) {
			if ((x->r_ob.num_selecteditems > 0 && x->r_ob.firstpreselecteditem) || 
				(x->r_ob.num_selecteditems == 0 && x->r_ob.firstpreselecteditem != x->r_ob.lastpreselecteditem))
				clear_preselection((t_notation_obj *)x);
		}
		unlock_general_mutex((t_notation_obj *)x);
		

		if (is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) {
			double screen_end_x = unscaled_xposition_to_xposition((t_notation_obj *)x, x->r_ob.screen_ux_end);
			force_inscreen_ux_to_boundary_and_set_mouse_position(x, pt.x > screen_end_x ? x->r_ob.j_selected_region_ux2 : x->r_ob.j_selected_region_ux1, patcherview, pt, true);
		}

		redraw = 1;
		changed = 0;
        
        
	} else if (x->r_ob.j_mousedown_obj_type == k_ZOOMING_REGION) {
		redraw = 1;
		changed = 0;
	
	} else if (x->r_ob.j_mousedown_obj_type > 0) { // if the mousedown is on a musical object
		
		// depends on selection
		x->r_ob.j_selection_hasbeendragged = 1;

        if (x->r_ob.j_mousedown_obj_type == k_DURATION_LINE && modifiers == eControlKey) { // slope change
            double delta_slope = (x->r_ob.floatdragging_y - pt.y)/(CONST_SLOPE_DRAG_UCHANGE * x->r_ob.zoom_y);
            if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_GENERIC)) return;
            if (modifiers & eShiftKey && modifiers & eCommandKey)
                delta_slope *= CONST_FINER_FROM_KEYBOARD;
            if (x->r_ob.allow_glissandi) {
                // retrieve correct bpt
                t_note *temp_note = ((t_duration_line *)x->r_ob.j_mousedown_ptr)->owner;
                t_bpt *temp_bpt, *bpt = NULL;
                double click_onset = unscaled_xposition_to_ms((t_notation_obj *)x, xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.j_mousedown_point.x), 1);
                for (temp_bpt = temp_note->firstbreakpoint ? temp_note->firstbreakpoint->next : NULL; temp_bpt; temp_bpt = temp_bpt->next) {
                    if (notation_item_get_onset_ms((t_notation_obj *)x, (t_notation_item *)temp_bpt) > click_onset) {
                        bpt = temp_bpt;
                        break;
                    }
                }
                if (bpt)
                    change_breakpoint_slope((t_notation_obj *)x, bpt, fabs(delta_slope), fsign(delta_slope));
                changed = 1;
                redraw = 1;
            }
            x->r_ob.floatdragging_y = pt.y;
            if (changed)
                x->r_ob.changed_while_dragging = true;
            
        } else if (x->r_ob.selection_type == k_DURATION_TAIL) { // only duration tails are selected
		
			if (modifiers == eControlKey && x->r_ob.breakpoints_have_velocity) { // velocity change
				double delta_vel = (x->r_ob.floatdragging_y - pt.y)/(CONST_VELOCITY_DRAG_UCHANGE * x->r_ob.zoom_y);				
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_VELOCITY)) return;
				changed = change_selection_velocity((t_notation_obj *) x, delta_vel);
				if (changed) 
					x->r_ob.changed_while_dragging = true;
				x->r_ob.floatdragging_y = pt.y;
				redraw = 1;
			} else if (modifiers == eControlKey + eShiftKey) { // slope change
				double delta_slope = (x->r_ob.floatdragging_y - pt.y)/(CONST_SLOPE_DRAG_UCHANGE * x->r_ob.zoom_y);
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_GENERIC)) return;
				if (x->r_ob.allow_glissandi) {
					change_selection_breakpoint_slope((t_notation_obj *)x, fabs(delta_slope), fsign(delta_slope));
					changed = 1;
				}
				x->r_ob.floatdragging_y = pt.y;
				if (changed) 
					x->r_ob.changed_while_dragging = true;
				redraw = 1;
			} else {
				// new duration tail
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_PITCH)) return;
				double delta_y = (modifiers & eShiftKey) ? 0 : yposition_to_mc((t_notation_obj *)x, pt.y, (t_voice *)x->firstvoice, NULL) - yposition_to_mc((t_notation_obj *)x, x->r_ob.floatdragging_y, (t_voice *)x->firstvoice, NULL); // if Shift, no vertical movement allowed
				if (x->r_ob.allow_glissandi) {
					if (modifiers & eShiftKey && modifiers & eCommandKey) 
						delta_y *= CONST_FINER_FROM_KEYBOARD;
					move_selection_breakpoint(x, 0., delta_y, 1.);

					if (x->r_ob.breakpoints_have_noteheads && x->r_ob.snap_pitch_to_grid_when_editing)
						snap_pitch_to_grid_for_selection((t_notation_obj *)x);

					changed = 1;
				}
				if (changed) 
					x->r_ob.changed_while_dragging = true;
				x->r_ob.floatdragging_x = pt.x;
				x->r_ob.floatdragging_y = pt.y;
				redraw = 1;
			}
		
		} else if ((x->r_ob.selection_type == k_PITCH_BREAKPOINT) || (x->r_ob.selection_type == k_PITCH_BREAKPOINT_OR_DURATION_TAIL)) { // only breakpoints are selected
		
			if (modifiers == eControlKey) { // velocity change
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_VELOCITY)) return;
				if (x->r_ob.breakpoints_have_velocity) {
					double delta_vel = (x->r_ob.floatdragging_y - pt.y)/(CONST_VELOCITY_DRAG_UCHANGE * x->r_ob.zoom_y);				
					if (modifiers & eShiftKey && modifiers & eCommandKey) delta_vel *= CONST_FINER_FROM_KEYBOARD;
					changed = change_selection_velocity((t_notation_obj *) x, delta_vel);
					if (changed) 
						x->r_ob.changed_while_dragging = true;
					x->r_ob.floatdragging_y = pt.y;
					redraw = 1;
				}
			} else if (modifiers == eControlKey + eShiftKey) { // slope change
				double delta_slope = (x->r_ob.floatdragging_y - pt.y)/(CONST_SLOPE_DRAG_UCHANGE * x->r_ob.zoom_y);
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_GENERIC)) return;
				if (modifiers & eShiftKey && modifiers & eCommandKey) delta_slope *= CONST_FINER_FROM_KEYBOARD;
				change_selection_breakpoint_slope((t_notation_obj *)x, fabs(delta_slope), fsign(delta_slope));
				changed = 1;
				if (changed) 
					x->r_ob.changed_while_dragging = true;
				x->r_ob.floatdragging_y = pt.y;
				redraw = 1;
				changed = 1;
			} else {	// new breakpoint position
				double delta_x = pt.x - x->r_ob.floatdragging_x;
				double delta_y = yposition_to_mc((t_notation_obj *)x, pt.y, (t_voice *)x->firstvoice, NULL) - yposition_to_mc((t_notation_obj *)x, x->r_ob.floatdragging_y, (t_voice *)x->firstvoice, NULL);
				char tail_only;
				char can_change_mc = true, can_change_onset = true;

				if (modifiers & eShiftKey && modifiers & eCommandKey) {
					delta_y *= CONST_FINER_FROM_KEYBOARD;
					delta_x *= CONST_FINER_FROM_KEYBOARD;
				}
				
				if (modifiers & eShiftKey) {
					if (x->r_ob.j_dragging_direction == 0)
						decide_dragging_direction((t_notation_obj *) x, pt);
					
					if (x->r_ob.j_dragging_direction == 1)
						can_change_mc = false;
					else if (x->r_ob.j_dragging_direction == -1)
						can_change_onset = false;
				}

				can_change_onset &= is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_ONSET);
				can_change_mc &= is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_PITCH);

				tail_only = (x->r_ob.selection_type == k_PITCH_BREAKPOINT_OR_DURATION_TAIL) ? 1 : 0;
				
				move_selection_breakpoint(x, delta_x * can_change_onset, delta_y * can_change_mc, tail_only);

				if (x->r_ob.breakpoints_have_noteheads && x->r_ob.snap_pitch_to_grid_when_editing)
					snap_pitch_to_grid_for_selection((t_notation_obj *)x);
				
				x->r_ob.floatdragging_x = pt.x;
				x->r_ob.floatdragging_y = pt.y;
				changed = 1;
				if (changed) 
					x->r_ob.changed_while_dragging = true;
				redraw = 1;
			}
			
		} else { // mixed selection, or just notes, or
		
			char can_change_onset = 0; // was: 1; but in [score] we DON'T allow onset change!!! 
			char can_change_mc = 1;
			char can_change_vel = 0;
			
			// modifiers: what can I change by dragging the note?
			if (modifiers & eShiftKey) {
				if (x->r_ob.j_dragging_direction == 0)
					decide_dragging_direction((t_notation_obj *) x, pt);
				
				if (x->r_ob.j_dragging_direction == 1)
					can_change_mc = 0;
				else if (x->r_ob.j_dragging_direction == -1)
					can_change_onset = 0;
			}
			if (modifiers == eControlKey){ // eControlKey it is used to change the velocities!
				can_change_mc = 0;
				can_change_onset = 0;
				can_change_vel = 1;
			}

			can_change_onset &= is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_ONSET);
			can_change_mc &= is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH);
			can_change_vel &= is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_VELOCITY);
			
			if (modifiers & eAltKey && !(modifiers & eCommandKey) && !x->r_ob.j_mousedrag_copy_ptr && is_editable((t_notation_obj *)x, k_NOTE, k_CREATION)){ // eAltKey is to quickly copy a note!!!
				long copy_mode = 1;
				t_notation_item *temp;
				can_change_mc = 0;
				can_change_onset = 0;
				can_change_vel = 0;
				
				// what are we supposed to do? each copied item is
				// but why does Daniele talk like Yoda????

//				if ((modifiers & eShiftKey) && (x->r_ob.j_dragging_direction == -1)) 
//					copy_mode = 1; // if we press Shift and drag vertically, we copy the NOTES in the same chords!!
//				else
//					copy_mode = 0; // copy to new chords

				lock_general_mutex((t_notation_obj *)x);	
				clear_preselection((t_notation_obj *) x); // we clear the preselection and put - one by one - the items in the selection.
				
				// we gotta copy _all_ the selected items
				temp = x->r_ob.firstselecteditem;
				while (temp) {
					x->r_ob.floatdragging_y = pt.y;	
					x->r_ob.floatdragging_x = pt.x;
					if (temp->type == k_NOTE) { // it's a note
						if (copy_mode == 1) { // add the note(s) to the SAME chord

							t_chord *thechord = ((t_note *)temp)->parent;
							if (!(thechord->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
								create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)thechord, k_CHORD, k_UNDO_MODIFICATION_CHANGE);

							if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)((t_note *)temp)->parent)) {
								t_note *new_note = clone_note((t_notation_obj *) x, (t_note *)temp, k_CLONE_FOR_SAME_CHORD); // we clone the note
								insert_note((t_notation_obj *) x, ((t_note *)temp)->parent, new_note, 0);
								((t_note *)temp)->parent->parent->need_recompute_beams_positions = true;
								recalculate_all_measure_chord_parameters((t_notation_obj *)x, ((t_note *)temp)->parent->parent);
								set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
								x->r_ob.j_mousedrag_copy_ptr = new_note;
								// checking if we have to transfer the mousedown pointer
								if (temp == x->r_ob.j_mousedown_ptr || ((t_note *)temp)->parent == x->r_ob.j_mousedown_ptr) {
									set_mousedown((t_notation_obj *) x, new_note, k_NOTE);
								}
								x->r_ob.changed_while_dragging = true;
								changed = 1;
								if (changed) 
									x->r_ob.changed_while_dragging = true;
								notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)new_note);
							}
						}
					} else if ((temp->type == k_CHORD) && (((t_chord *)temp)->r_sym_duration.r_num >= 0)) { // it's a chord
						if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)temp)){

							if (!(((t_chord *)temp)->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
								create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, temp, k_CHORD, k_UNDO_MODIFICATION_CHANGE);

                            t_chord *new_chord = clone_chord((t_notation_obj *) x, (t_chord *)temp, copy_mode == 1 ? k_CLONE_FOR_SAME_CHORD : k_CLONE_FOR_NEW); // we clone the note
							if (copy_mode == 1) { // it's a chord, but still we have to copy each note WITHIN the chord: so we MERGE it!
								// insert chord in the list
								t_note *new_mousedown_note = merge_chords(x, ((t_chord *)temp), new_chord, 1, true, true);
								x->r_ob.changed_while_dragging = true;
								((t_chord *)temp)->parent->need_recompute_beams_positions = true;
								set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
								recalculate_all_measure_chord_parameters((t_notation_obj *)x, ((t_chord *)temp)->parent);
								x->r_ob.j_mousedrag_copy_ptr = ((t_chord *)temp);
								if (new_mousedown_note) {
									set_mousedown((t_notation_obj *) x, new_mousedown_note, k_NOTE);
//								} else {
//									x->r_ob.j_mousedown_ptr = ((t_chord *)temp->item);
//									x->r_ob.j_mousedown_obj_type = k_CHORD;
								}
								changed = 1;
								if (changed) 
									x->r_ob.changed_while_dragging = true;
							}
						}
					}
					temp = temp->next_selected;
				}
				clear_selection((t_notation_obj *) x);
				move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_INVOLUTIVE, false, false);
				unlock_general_mutex((t_notation_obj *)x);

				verbose_print(x);
				redraw = 1;
			} 
			
			if (can_change_mc){
				// new midicents
				double delta_mc = 0;
				if ( ((x->r_ob.j_mousedown_obj_type == 1 && x->r_ob.j_mousedown_ptr) || 
					  (x->r_ob.j_mousedown_obj_type == 5 && x->r_ob.j_mousedown_ptr && ((t_chord *)x->r_ob.j_mousedown_ptr)->firstnote)) &&
					 !(modifiers & eShiftKey && modifiers & eCommandKey && !x->r_ob.snap_pitch_to_grid_when_editing)){
					
					if (pt.y >= 0 && pt.y <= rect.height) { // point is in the window
						t_note *nt = (x->r_ob.j_mousedown_obj_type == 1) ? (t_note *) x->r_ob.j_mousedown_ptr : ((t_chord *) x->r_ob.j_mousedown_ptr)->firstnote;
						delta_mc = yposition_to_mc((t_notation_obj *)x, pt.y, (t_voice *)nt->parent->parent->voiceparent, NULL) - nt->midicents;
					}
					
				} else {
					delta_mc = yposition_to_mc((t_notation_obj *)x, pt.y, (t_voice *)x->firstvoice, NULL) - yposition_to_mc((t_notation_obj *)x, x->r_ob.floatdragging_y, (t_voice *)x->firstvoice, NULL);
				}
				if (modifiers & eShiftKey && modifiers & eCommandKey && !x->r_ob.snap_pitch_to_grid_when_editing) 
					delta_mc *= CONST_FINER_FROM_KEYBOARD;
					
				changed = change_pitch_for_selection(x, delta_mc, 1, !((modifiers & eCommandKey) && (modifiers & eShiftKey)), false); //3); //1);
				
				if (changed) 
					x->r_ob.changed_while_dragging = true;
				x->r_ob.floatdragging_y = pt.y;
				redraw = 1;
			}
			
			if (can_change_vel){
				// new velocity, or new tempo!
				double delta_vel = (x->r_ob.floatdragging_y - pt.y)/(CONST_VELOCITY_DRAG_UCHANGE * x->r_ob.zoom_y);				
				double delta_tempo = (x->r_ob.floatdragging_y - pt.y)/(CONST_TEMPO_DRAG_UCHANGE * x->r_ob.zoom_y);				
				if (modifiers & eShiftKey && modifiers & eCommandKey) {
					delta_vel *= CONST_FINER_FROM_KEYBOARD;
					delta_tempo *= CONST_FINER_FROM_KEYBOARD;
				}
				changed = change_selection_velocity((t_notation_obj *) x, delta_vel) || change_selection_tempo(x, delta_tempo, true);
				if (changed) 
					x->r_ob.changed_while_dragging = true;
				x->r_ob.floatdragging_y = pt.y;
				redraw = 1;
			}
		}
	}
	
	x->r_ob.j_mouse_hasbeendragged = 1; // mouse has been dragged
	
	if (redraw) {
		invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
		if (changed && x->r_ob.j_mouse_is_down) {
			x->r_ob.changed_while_dragging = true;
			handle_change((t_notation_obj *) x, x->r_ob.continuously_output_changed_bang ? k_CHANGED_STANDARD_SEND_BANG : k_CHANGED_REDRAW_STATIC_LAYER, k_UNDO_OP_UNKNOWN);
		}
	}
	
}

t_chord *shift_note_allow_voice_change(t_score *x, t_note *note, double delta, char mode, char *old_chord_deleted, char allow_voice_change){
// mode == 0: delta are the delta-steps on the scale (e.g. from C3 to F3, it is 5 steps in the semitone scale, 10 steps in the quartertonal scale)
// mode == 1: delta is delta_midicents
// returns a pointer to the new chord, if the chord has changed.
// also updates note with the new dragging note if the note has changed voice
	
	t_chord *newch = NULL; 
	double note_y_real;
	long note_new_voice;
	double prev_mc = note->midicents; // mc before change

	if (old_chord_deleted) 
		*old_chord_deleted = false;
	
	if (mode == 0) {
		note->midicents = get_next_step_depending_on_editing_ranges((t_notation_obj *)x, note->midicents, note->parent->parent->voiceparent->v_ob.number, delta);
//		note->midicents += (delta * (200. / x->r_ob.tone_division));
	} else
		note->midicents += delta;

	note_y_real = mc_to_yposition((t_notation_obj *)x, note->midicents, (t_voice *) note->parent->parent->voiceparent);

	// retrieve legitimate voice for this position:
	note_new_voice = allow_voice_change ? yposition_to_voicenumber((t_notation_obj *)x, note_y_real, -1, k_VOICEENSEMBLE_INTERFACE_ACTIVE) : note->parent->parent->voiceparent->v_ob.number;
	
	if (note_new_voice <= 0 || // new active voice not found
        (note_new_voice == note->parent->parent->voiceparent->v_ob.number) ||	// if the legitimate voice is the same of the previous voice...
        do_voices_belong_to_same_voiceensemble((t_notation_obj *) x, (t_voice *)nth_scorevoice(x, note_new_voice), (t_voice *)note->parent->parent->voiceparent) || // if the two voices belong to the same ensemble
		((note->midicents - prev_mc) * (note_new_voice - note->parent->parent->voiceparent->v_ob.number) >= 0)) { // ...or if the voice movement is not in phase with the mc movement (e.g. i'm dragging upwards a very low note on a staff: i don't want it to go to the lower staff!)
		// we keep the same voice
		note_set_auto_enharmonicity(note); // automatic accidentals for retranscribing!
		constraint_midicents_depending_on_editing_ranges((t_notation_obj *)x, &note->midicents, note_new_voice); 
	} else { // note is changing voice!
		t_scorevoice *new_voice = nth_scorevoice(x, note_new_voice);
//		post("voices: FROM %d TO %d", note->parent->voiceparent->v_ob.number, new_voice->v_ob.number);
		// new midicents?
//		double new_mc = note->midicents

		t_note *note_in_new_voice = clone_note((t_notation_obj *)x, note, k_CLONE_FOR_ORIGINAL); // we clone the note
		double threshold_y, change_mc1, change_mc2;
		t_chord *chord_for_note_insertion = NULL;
		t_measure *temp_meas;
        note_in_new_voice->parent = NULL; // THIS IS VERY IMPORTANT: up to now, there is no parent for the new note
		note->midicents -= (mode == 0) ? (delta * (200. / x->r_ob.tone_division)) : delta; // step back
		threshold_y = (new_voice->v_ob.number > note->parent->parent->voiceparent->v_ob.number) ? 
						new_voice->v_ob.offset_y + CONST_VOICE_THRESHOLD * x->r_ob.zoom_y :
						note->parent->parent->voiceparent->v_ob.offset_y + CONST_VOICE_THRESHOLD * x->r_ob.zoom_y;
		change_mc1 = yposition_to_mc((t_notation_obj *)x, threshold_y, (t_voice *)new_voice, NULL);
		change_mc2 = yposition_to_mc((t_notation_obj *)x, threshold_y, (t_voice *)note->parent->parent->voiceparent, NULL);
		note_in_new_voice->midicents = change_mc1 - ((mode == 0 ? -delta * (200. / x->r_ob.tone_division) : -delta) - (note->midicents - change_mc2));

		constraint_midicents_depending_on_editing_ranges((t_notation_obj *)x, &note_in_new_voice->midicents, note_new_voice); 

		// we look if there's a chord EXACTLY with the same onset, we add the note to the chord!
		temp_meas = new_voice->firstmeasure;
		while (temp_meas && !chord_for_note_insertion) {
			if (temp_meas->tuttipoint_reference == note->parent->parent->tuttipoint_reference) {
				t_chord *temp_ch = temp_meas->firstchord;
				while (temp_ch) {
					long cmp = rat_rat_cmp(rat_rat_sum(temp_meas->r_tuttipoint_onset_sec, temp_ch->r_measure_onset_sec),  
										   rat_rat_sum(note->parent->parent->r_tuttipoint_onset_sec, note->parent->r_measure_onset_sec));
					if (cmp == 0) { // same onset
						chord_for_note_insertion = temp_ch;
						break;
					} else if (cmp == 1) { // temp_ch has a bigger onset
						break;
					}
					temp_ch = temp_ch->next; 
				}
			}
			temp_meas = temp_meas->next;
		}

		note_set_auto_enharmonicity(note_in_new_voice); // accidental will be automatically chosen!
		note_in_new_voice->r_it.flags = (e_bach_internal_notation_flags) (note_in_new_voice->r_it.flags & ~k_FLAG_SHIFT);
		
		if (chord_for_note_insertion) { // there's already a chord with the same onset: we add the note to the chord!

			create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)note->parent->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)chord_for_note_insertion->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);

			insert_note((t_notation_obj *) x, chord_for_note_insertion, note_in_new_voice, 0);
			note_compute_approximation((t_notation_obj *) x, note_in_new_voice);
			if (chord_for_note_insertion->r_sym_duration.r_num < 0) chord_for_note_insertion->r_sym_duration = rat_abs(chord_for_note_insertion->r_sym_duration); 
			newch = chord_for_note_insertion;
			chord_for_note_insertion->need_recompute_parameters = true; // we have to recalculate chord parameters 
			if (!notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)chord_for_note_insertion) && !notation_item_is_preselected((t_notation_obj *) x, (t_notation_item *)chord_for_note_insertion))
				notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)note_in_new_voice);
		} else { // there's no chord with the same onset. We simply free the note.
			note_delete((t_notation_obj *)x, note_in_new_voice, false);
			note_in_new_voice = NULL;
		}

		// let's check if we have to transfer the j_mousedown information
		if (note_in_new_voice && (x->r_ob.j_mousedown_ptr == note || (x->r_ob.j_mousedown_ptr == note->parent && note->parent->num_notes == 1))) {
			set_mousedown((t_notation_obj *) x, note_in_new_voice, k_NOTE);
		}
        
        if (chord_for_note_insertion)
            recompute_all_for_measure((t_notation_obj *) x, chord_for_note_insertion->parent, true);
        if (note->parent)
            recompute_all_for_measure((t_notation_obj *) x, note->parent->parent, true);
		
		// gotta delete the original note!!!
		t_chord *parent = note->parent;
		if ((!note->parent->firstnote || !note->parent->firstnote->next)) {
			if (old_chord_deleted) // chord is turned into rest?
				*old_chord_deleted = true;
		}
		note_delete((t_notation_obj *)x, note, false);
		
		// check lock/mute/solo compatibilities
		if (chord_for_note_insertion) {
			check_lock_mute_solo_compatibilities_for_chord_and_notes((t_notation_obj *)x, chord_for_note_insertion);
			check_lock_mute_solo_compatibilities_for_chord_and_notes((t_notation_obj *)x, parent);
		}
		

// THIS LINE IS WRONG: if we wanted to do something like that, we should pass t_note ** note, but it's not needed 
//		note = note_in_new_voice;
	}
	
	set_need_perform_analysis_and_change_flag((t_notation_obj *)x);

	verbose_print(x);	
	return newch;		
}

void clear_notes_flag(t_score *x, long flag)
{
	t_scorevoice *voice = x->firstvoice;
	while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
		t_measure *measure = voice->firstmeasure;
		while (measure) {
			t_chord *chord = measure->firstchord;
			while (chord) {
				t_note *note = chord->firstnote;
				while (note) {
					note->r_it.flags = (e_bach_internal_notation_flags) (note->r_it.flags & ~flag);
					note = note->next;
				}
				chord = chord->next;
			}
			measure = measure->next;
		}
		voice = voice->next;
	}
}

void clear_measures_and_chords_flag(t_score *x, long flag)
{
	t_scorevoice *voice = x->firstvoice;
	while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
		t_measure *measure = voice->firstmeasure;
		while (measure) {
			t_chord *chord = measure->firstchord;
			measure->r_it.flags = (e_bach_internal_notation_flags) (measure->r_it.flags & ~flag);
			while (chord) {
				chord->r_it.flags = (e_bach_internal_notation_flags) (chord->r_it.flags & ~flag);
				chord = chord->next;
			}
			measure = measure->next;
		}
		voice = voice->next;
	}
}

char change_pitch_for_selection(t_score *x, double delta, char mode, char allow_voice_change, char snap_pitch_to_grid)
{
	char changed = 0;
	t_notation_item *curr_it = x->r_ob.firstselecteditem;
	
	lock_general_mutex((t_notation_obj *)x);	
	
	clear_preselection((t_notation_obj *) x);

	// first we "mark" the selected notes. We really need this, in order to avoid confusions about the chord which will be created or merged.
	while (curr_it) { // cycle on the selected items
		if (curr_it->type == k_NOTE) { // it is a note
			t_note *nt = ((t_note *)curr_it);
			if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)nt)) {
				((t_note *)curr_it)->r_it.flags = (e_bach_internal_notation_flags) (((t_note *)curr_it)->r_it.flags | k_FLAG_SHIFT);
			}
		} else if (curr_it->type == k_CHORD) {
			t_note *curr_nt = ((t_chord *)curr_it)->firstnote;
			while (curr_nt) {
				if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_nt)) {
					curr_nt->r_it.flags = (e_bach_internal_notation_flags) (curr_nt->r_it.flags | k_FLAG_SHIFT);
				}
				curr_nt = curr_nt->next;
			}
		} else if ((curr_it->type == k_MEASURE) && (x->r_ob.change_cents_to_measures_by_dragging)) {
			t_chord *curr_ch = ((t_measure *)curr_it)->firstchord;
			while (curr_ch) {
				t_note *curr_nt = curr_ch->firstnote;
				while (curr_nt) {
					if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_nt)) {
						curr_nt->r_it.flags = (e_bach_internal_notation_flags) (curr_nt->r_it.flags | k_FLAG_SHIFT);
					}
					curr_nt = curr_nt->next;
				}
				curr_ch = curr_ch->next;
			}
		}
		curr_it = curr_it->next_selected;
	}


	curr_it = x->r_ob.firstselecteditem;
	while (curr_it) { // cycle on the selected items
		// things of which we'll have to calculate parameters: 
		t_chord *oldch = NULL; t_chord *newch = NULL; 
		char old_chord_deleted = 0;
        char is_in_voiceensemble = (voiceensemble_get_numparts((t_notation_obj *)x, notation_item_get_voice((t_notation_obj *)x, curr_it)) > 1);
	
		if (curr_it->type == k_NOTE && (((t_note *)curr_it)->r_it.flags & k_FLAG_SHIFT)) { // it is a note
			t_note *note = (t_note *)curr_it;
			oldch = note->parent;

			if (chord_has_a_tie_to(oldch) || chord_has_a_tie_from(oldch))
				create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)oldch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			else if (!(oldch->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
				create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)oldch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);

			newch = shift_note_allow_voice_change(x, note, delta, mode, &old_chord_deleted, allow_voice_change);
			
			if (newch) {
				t_note *nt;

				for (nt = newch->firstnote; nt; nt = nt->next) {
					if (nt->r_it.flags & k_FLAG_SHIFT) {
						note_compute_approximation((t_notation_obj *) x, nt);
						if (change_pitch_must_actually_snap_to_grid((t_notation_obj *)x, mode, snap_pitch_to_grid)) 
							snap_pitch_to_grid_for_note((t_notation_obj *) x, nt);
					}
				}
				newch->need_recompute_parameters = true; // we have to recalculate chord parameters 
//				newch->parent->need_recompute_beams_positions = true;
				newch->parent->need_recompute_beamings = true;
				oldch->parent->need_recompute_beamings = true;
				if (oldch->num_notes == 0)
					notation_item_delete_from_selection((t_notation_obj *)x, (t_notation_item *)oldch);
			} else {
				note_compute_approximation((t_notation_obj *) x, note);
				if (change_pitch_must_actually_snap_to_grid((t_notation_obj *)x, mode, snap_pitch_to_grid)) 
					snap_pitch_to_grid_for_note((t_notation_obj *) x, note);
			}
				
			if (true) { // !old_chord_deleted) {
				validate_accidentals_for_measure((t_notation_obj *) x, oldch->parent);
				oldch->need_recompute_parameters = true; // we have to recalculate chord parameters 
				oldch->parent->need_recompute_beams_positions = true;
			}

			if (!x->r_ob.j_mouse_is_down) {
				check_ties_around_measure(oldch->parent);
				oldch->parent->need_recompute_beams_positions = true;
				oldch->parent->need_check_ties = true;
                if (is_in_voiceensemble)
                    recompute_all_for_measures_in_voiceensemble((t_notation_obj *)x, oldch->parent, false);
                else
                    recalculate_all_measure_chord_parameters((t_notation_obj *)x, oldch->parent);
				oldch->parent->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE;
			}
			
//			set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
//			perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_DONT_CHANGE_CHORDS);
			changed = 1;
			
		} else if (curr_it->type == k_CHORD) {
			t_note *curr_nt, *temp_nt;
			oldch = ((t_chord *)curr_it);
			
			if (oldch->num_notes > 0) {
				if (chord_has_a_tie_to(oldch) || chord_has_a_tie_from(oldch))
					create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)oldch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
				else if (!(oldch->parent->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
					create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)oldch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
				
				curr_nt = oldch->firstnote;
				while (curr_nt) {
					if ((curr_nt->r_it.flags & k_FLAG_SHIFT) && (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)curr_nt))) {
						if (!curr_nt)
							object_warn((t_object *) x, "Error!");
						temp_nt = curr_nt;
						newch = shift_note_allow_voice_change(x, temp_nt, delta, mode, &old_chord_deleted, allow_voice_change);
						
						if (newch) {
							t_note *nt; 
							for (nt = newch->firstnote; nt; nt = nt->next) {
								if (nt->r_it.flags & k_FLAG_SHIFT) {
									note_compute_approximation((t_notation_obj *) x, nt);
									if (change_pitch_must_actually_snap_to_grid((t_notation_obj *)x, mode, snap_pitch_to_grid)) 
										snap_pitch_to_grid_for_note((t_notation_obj *) x, nt);
								}
							}
							validate_accidentals_for_measure((t_notation_obj *) x, newch->parent);
							newch->need_recompute_parameters = true; // we have to recalculate chord parameters 
							//						newch->parent->need_recompute_beams_positions = true;
							if (!x->r_ob.j_mouse_is_down)
								recompute_all_for_measure((t_notation_obj *)x, newch->parent, false);
							newch->parent->need_recompute_beamings = true;
							oldch->parent->need_recompute_beamings = true;
							if (oldch->num_notes == 0)
								notation_item_delete_from_selection((t_notation_obj *)x, (t_notation_item *)oldch);
						}
					}
					curr_nt = curr_nt->next;
					changed = 1;
				}
				if (true) { //!old_chord_deleted) {
					t_note *nt;
					for (nt = oldch->firstnote; nt; nt = nt->next) {
						if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
							note_compute_approximation((t_notation_obj *) x, nt);
							if (change_pitch_must_actually_snap_to_grid((t_notation_obj *)x, mode, snap_pitch_to_grid)) 
								snap_pitch_to_grid_for_note((t_notation_obj *) x, nt);
						}
					}
					validate_accidentals_for_measure((t_notation_obj *) x, oldch->parent);
					oldch->need_recompute_parameters = true;
					oldch->parent->need_recompute_beams_positions = true;
                    if (!x->r_ob.j_mouse_is_down) {
                        if (is_in_voiceensemble)
                            recompute_all_for_measures_in_voiceensemble((t_notation_obj *)x, oldch->parent, false);
                        else
                            recompute_all_for_measure((t_notation_obj *)x, oldch->parent, false);
                    }
				}
				//			set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
				//			perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_DONT_CHANGE_CHORDS);
			}
		} else if ((curr_it->type == k_MEASURE) && (x->r_ob.change_cents_to_measures_by_dragging)) {
			t_measure *meas = ((t_measure *)curr_it);
			oldch = meas->firstchord;
			while (oldch) {
				t_note *curr_nt = oldch->firstnote, *temp_nt;
				
                if (!curr_nt) {
                    oldch = oldch->next;
					continue;
                }
				
				if (chord_has_a_tie_to(oldch) || chord_has_a_tie_from(oldch)) {
					create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)oldch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
				} else if (!(oldch->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
					create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)oldch, k_CHORD, k_UNDO_MODIFICATION_CHANGE);

				while (curr_nt) {
					if ((curr_nt->r_it.flags & k_FLAG_SHIFT) && (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)curr_nt))) {
						if (!(curr_nt))
							object_warn((t_object *) x, "Error!");
						temp_nt = curr_nt;
						newch = shift_note_allow_voice_change(x, temp_nt, delta, mode, &old_chord_deleted, allow_voice_change);
//						note_compute_approximation((t_notation_obj *) x, temp_nt);
//						if (x->r_ob.snap_pitch_to_grid_when_editing) snap_pitch_to_grid_for_note((t_notation_obj *) x, curr_nt);

						if (newch) {
							t_note *nt; 
							for (nt = newch->firstnote; nt; nt = nt->next) {
								if (!notation_item_is_globally_locked((t_notation_obj *)x, (t_notation_item *)nt)) {
									if (nt->r_it.flags & k_FLAG_SHIFT) {
										note_compute_approximation((t_notation_obj *) x, nt);
										if (change_pitch_must_actually_snap_to_grid((t_notation_obj *)x, mode, snap_pitch_to_grid)) 
											snap_pitch_to_grid_for_note((t_notation_obj *) x, nt);
									}
								} 
							}
							validate_accidentals_for_measure((t_notation_obj *) x, newch->parent);
							newch->need_recompute_parameters = true; // we have to recalculate chord parameters 
//							newch->parent->need_recompute_beams_positions = true;
							oldch->parent->need_recompute_beamings = true;
							newch->parent->need_recompute_beamings = true;
							if (oldch->num_notes == 0)
								notation_item_delete_from_selection((t_notation_obj *)x, (t_notation_item *)oldch);
						}
					}
					curr_nt = curr_nt->next;
				}

				if (!old_chord_deleted) {
					t_note *nt;
					for (nt = oldch->firstnote; nt; nt = nt->next) {
						note_compute_approximation((t_notation_obj *) x, nt);
						if (change_pitch_must_actually_snap_to_grid((t_notation_obj *)x, mode, snap_pitch_to_grid)) 
							snap_pitch_to_grid_for_note((t_notation_obj *) x, nt);
					}
					validate_accidentals_for_measure((t_notation_obj *) x, oldch->parent);
					oldch->need_recompute_parameters = true;
					oldch->parent->need_recompute_beams_positions = true;
				}

				changed = 1;
				oldch = oldch->next;
			}
            if (changed && !x->r_ob.j_mouse_is_down) {
                if (is_in_voiceensemble)
                    recompute_all_for_measures_in_voiceensemble((t_notation_obj *)x, meas, false);
                else
                    recompute_all_for_measure((t_notation_obj *)x, meas, false);
            }

//			set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
//			perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_DONT_CHANGE_CHORDS);
		}
		curr_it = curr_it->next_selected;
		
	}
	
	if (changed){
		set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
		perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_DONT_CHANGE_CHORDS + k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
	}

    clear_measures_and_chords_flag(x, k_FLAG_SHIFT);

    move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_INVOLUTIVE, false, false);

	unlock_general_mutex((t_notation_obj *)x);	

	verbose_print(x);
	return changed;
}



char split_selection(t_score *x, long how_many, char merge_alltied_chords_first){ 
	char changed = 0;
	t_notation_item *temp_it;
	t_notation_item *curr_it = x->r_ob.firstselecteditem;

	lock_general_mutex((t_notation_obj *)x);

	// first we "mark" the selected notes. We really need this, in order to avoid confusions about the chord which will be created or merged.
	while (curr_it) { // cycle on the selected items
		if (curr_it->type == k_CHORD) {
			t_chord *curr_ch = ((t_chord *)curr_it);
			if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))
				curr_ch->r_it.flags = (e_bach_internal_notation_flags) (curr_ch->r_it.flags | k_FLAG_MERGE);
		} else if (curr_it->type == k_MEASURE) {
			t_chord *curr_ch = ((t_measure *)curr_it)->firstchord;
			while (curr_ch) {
				if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))
					curr_ch->r_it.flags = (e_bach_internal_notation_flags) (curr_ch->r_it.flags | k_FLAG_MERGE);
				curr_ch = curr_ch->next;
			}
		}
		curr_it = curr_it->next_selected;
	}

	
	// first we "mark" the selected notes. We really need this, in order to avoid confusions about the chord which will be created or merged.
	curr_it = x->r_ob.firstselecteditem; // x->r_ob.num_selecteditems
	while (curr_it) { // cycle on the selected items
		t_notation_item *next_it = curr_it->next_selected;
		if (curr_it->type == k_CHORD) {
			t_chord *curr_ch = ((t_chord *)curr_it);
			
			if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch)) {
				create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)curr_ch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
				
				if (merge_alltied_chords_first && curr_ch->r_it.flags & k_FLAG_MERGE) {
					char changed = false;
					t_chord *old_ch = curr_ch;
					changed |= gather_all_selected_chords_with_merge_flag(x, old_ch, true, &curr_ch, &next_it);
//					merge_rests_and_alltied_chords_one_step((t_notation_obj *) x, curr_ch->parent, &changed, curr_ch->parent->rhythmic_tree, &curr_ch);
					if (curr_ch != old_ch) 
						next_it = x->r_ob.firstselecteditem;
					recompute_all_for_measure((t_notation_obj *) x, curr_ch->parent, true);
					curr_ch->parent->r_it.flags = (e_bach_internal_notation_flags) (curr_ch->parent->r_it.flags | k_FLAG_MERGE);
				}
				
				curr_ch->r_it.flags = (e_bach_internal_notation_flags) (curr_ch->r_it.flags | k_FLAG_SPLIT);
			}
		} else if (curr_it->type == k_MEASURE) {
			t_measure *meas = (t_measure *)curr_it;
			t_chord *curr_ch;
			
			create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);

			if (merge_alltied_chords_first && !(meas->r_it.flags & k_FLAG_MERGE)) {
				char changed = false;
                while (next_it && notation_item_is_ancestor_of((t_notation_obj *)x, (t_notation_item *)meas, next_it))
                    next_it = next_it->next_selected;
				merge_rests_and_alltied_chords_one_step((t_notation_obj *) x, meas, &changed, meas->rhythmic_tree, NULL, k_MERGE_WHEN_ALWAYS);
				recompute_all_for_measure((t_notation_obj *) x, meas, true);
				meas->r_it.flags = (e_bach_internal_notation_flags) (meas->r_it.flags | k_FLAG_MERGE);
			}
			
			curr_ch = meas->firstchord;
			while (curr_ch) {
				if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))
					curr_ch->r_it.flags = (e_bach_internal_notation_flags) (curr_ch->r_it.flags | k_FLAG_SPLIT);
				curr_ch = curr_ch->next;
			}
		}
		curr_it = next_it;
	}

	curr_it = x->r_ob.firstselecteditem; 
	while (curr_it) { // cycle on the selected items
		// things of which we'll have to calculate parameters: 
		t_chord *oldch = NULL; 
		temp_it = curr_it->next_selected;
		if ((curr_it->type == k_CHORD) && (((t_chord *)curr_it)->r_it.flags & k_FLAG_SPLIT)) {
			oldch = ((t_chord *)curr_it);
			oldch->r_it.flags = (e_bach_internal_notation_flags) (oldch->r_it.flags & ~k_FLAG_SPLIT);
			split_chord(x, oldch, how_many, oldch->parent->lock_rhythmic_tree || x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_TAKE_FOR_GRANTED);
			changed = 1;
		} else if (curr_it->type == k_MEASURE) {
			t_measure *meas = ((t_measure *)curr_it);
			t_chord *tmp_ch;
			oldch = meas->firstchord; 
			while (oldch) {
				tmp_ch = oldch->next;
				if (oldch->r_it.flags & k_FLAG_SPLIT) {
					oldch->r_it.flags = (e_bach_internal_notation_flags) (oldch->r_it.flags & ~k_FLAG_SPLIT);
					split_chord(x, oldch, how_many, oldch->parent->lock_rhythmic_tree || x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_TAKE_FOR_GRANTED);
					changed = 1;
				}
				oldch = tmp_ch;
			}
		}
		curr_it = temp_it;
	}
	
	perform_analysis_and_change(x, NULL, NULL, x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_IGNORE ? k_BEAMING_CALCULATION_FROM_SCRATCH : k_BEAMING_CALCULATION_DONT_CHANGE_LEVELS);
	
    clear_measures_and_chords_flag(x, k_FLAG_SPLIT+k_FLAG_MERGE);

	unlock_general_mutex((t_notation_obj *)x);

	verbose_print(x);
	
	return changed;
}


char gather_all_selected_chords_with_merge_flag(t_score *x, t_chord *chord, char only_tied_ones, t_chord **reference, t_notation_item **next_sel_pointer)
{
	t_chord *from = chord;
	t_chord *to = chord;
	char changed = 0;
	while (from && (from->prev) && (notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)from->prev)) && 
			(from->prev->r_it.flags & k_FLAG_MERGE) && (!only_tied_ones || is_all_chord_tied_from(from, true)))
		from = from->prev;
	while (to && (to->next) && (notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)to->next)) && 
			(to->next->r_it.flags & k_FLAG_MERGE) && (!only_tied_ones || is_all_chord_tied_to((t_notation_obj *) x, to, true, to->next)))
		to = to->next;
	if (from != to) {
		t_chord *tmp = from->next;
        
        set_level_type_flag_for_level(from->parent->rhythmic_tree, k_RHYTHM_LEVEL_IGNORE);
        llll_funall(from->parent->rhythmic_tree, remove_force_tuplet_levels_fn, NULL, 1, -1, FUNALL_SKIP_ATOMS);

        while (tmp && (tmp != to)) {
			notation_item_delete_from_selection((t_notation_obj *) x, (t_notation_item *)tmp);
            if (next_sel_pointer && (t_notation_item *)tmp == *next_sel_pointer)
                *next_sel_pointer = (*next_sel_pointer)->next_selected;
			merge_chords_and_sum_durations(x, from, tmp);
			tmp = from->next;
		}
        if (next_sel_pointer && (t_notation_item *)to == *next_sel_pointer)
            *next_sel_pointer = (*next_sel_pointer)->next_selected;
		notation_item_delete_from_selection((t_notation_obj *) x, (t_notation_item *)to);
        
        verbose_post_rhythmic_tree((t_notation_obj *) x, from->parent, gensym("mergeC"), 1);
		merge_chords_and_sum_durations(x, from, to);
        verbose_post_rhythmic_tree((t_notation_obj *) x, from->parent, gensym("mergeD"), 1);
        
		from->r_it.flags = (e_bach_internal_notation_flags) (from->r_it.flags & ~k_FLAG_MERGE);
		changed = 1;


		if (reference)
			*reference = from;
	}
	return changed;
} 


char quick_merge_selection(t_score *x)
{
	t_notation_item *curr_it;
	char changed = 0;
    t_measure *meas = NULL;
	
	lock_general_mutex((t_notation_obj *)x);
	
	curr_it = x->r_ob.firstselecteditem;
	clear_preselection((t_notation_obj *) x);

	// first we "mark" the selected notes. We really need this, in order to avoid confusions about the chord which will be created or merged.
	while (curr_it) { // cycle on the selected items
		if (curr_it->type == k_CHORD) {
			t_chord *curr_ch = ((t_chord *)curr_it);
			if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))
				curr_ch->r_it.flags = (e_bach_internal_notation_flags) (curr_ch->r_it.flags | k_FLAG_MERGE);
		} else if (curr_it->type == k_MEASURE) {
			t_chord *curr_ch = ((t_measure *)curr_it)->firstchord;
			while (curr_ch) {
				if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))
					curr_ch->r_it.flags = (e_bach_internal_notation_flags) (curr_ch->r_it.flags | k_FLAG_MERGE);
				curr_ch = curr_ch->next;
			}
		}
		curr_it = curr_it->next_selected;
	}

	curr_it = x->r_ob.firstselecteditem;
	
	while (curr_it) { // cycle on the selected items
		// things of which we'll have to calculate parameters: 
		t_notation_item *next = curr_it->next_selected;
		if ((curr_it->type == k_CHORD) && (((t_chord *)curr_it)->r_it.flags & k_FLAG_MERGE)) {
			create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)((t_chord *)curr_it)->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
            meas = ((t_chord *)curr_it)->parent;
			changed |= gather_all_selected_chords_with_merge_flag(x, (t_chord *)curr_it, false, NULL, &next);
		} else if (curr_it->type == k_MEASURE) {
			t_measure *meas = ((t_measure *)curr_it);
			t_chord *oldch = meas->firstchord;

			create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);

			while (oldch) {
				if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)oldch)) {
					t_chord *from = oldch; t_chord *to = oldch;
					while (from && (from->prev) && (from->prev->r_it.flags & k_FLAG_MERGE))
						from = from->prev;
					while (to && (to->next) && (to->next->r_it.flags & k_FLAG_MERGE))
						to = to->next;

                    // setting rhythmic tree as ignore
                    llll_funall(meas->rhythmic_tree, remove_force_tuplet_levels_fn, NULL, 1, -1, FUNALL_SKIP_ATOMS);
                    set_level_type_flag_for_level(meas->rhythmic_tree, k_RHYTHM_LEVEL_IGNORE);

					if (from != to) {
						t_chord *tmp = from->next;
						while (tmp && (tmp != to)) {
							notation_item_delete_from_selection((t_notation_obj *) x, (t_notation_item *)tmp);
                            if ((t_notation_item *)tmp == next)
                                next = next->next_selected;
							merge_chords_and_sum_durations(x, from, tmp);
							tmp = from->next;
						}
                        
						notation_item_delete_from_selection((t_notation_obj *) x, (t_notation_item *)to);
						merge_chords_and_sum_durations(x, from, to);
						from->r_it.flags = (e_bach_internal_notation_flags) (from->r_it.flags & ~k_FLAG_MERGE);
						changed = 1;
					}
				}
				oldch = oldch->next;
			}
			
		}
		curr_it = next;
	}
	
    clear_measures_and_chords_flag(x, k_FLAG_MERGE);

    move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_INVOLUTIVE, false, false);

    perform_analysis_and_change(x, NULL, NULL, x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_IGNORE ? k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE : k_BEAMING_CALCULATION_DONT_DELETE_LEVELS + k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
	unlock_general_mutex((t_notation_obj *)x);
	return changed;
}


void tie_untie_note_smart(t_score *x, t_note *nt, char *changed)
{
    if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)nt)) {
        t_note *first_tied = get_first_tied_note(nt);
        t_note *last_tied = get_last_tied_note(nt);
        t_measure *meas = nt->parent->parent;
        create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nt->parent->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
        if (first_tied != nt || last_tied != nt) {
            // process all tied sequence
            t_note *temp;
            for (temp = first_tied; temp; temp = temp->tie_to) {
                if (notation_item_is_globally_selected((t_notation_obj *)x, (t_notation_item *)temp))
                    untie_note(nt);
                if (temp == last_tied)
                    break;
            }
            nt->r_it.flags &= ~k_FLAG_TO_BE_MODIFIED;
        } else {
            if (nt->tie_to == WHITENULL)
                untie_note(nt);
            else
                tie_note(nt);
            nt->r_it.flags &= ~k_FLAG_TO_BE_MODIFIED;
        }
        *changed = 1;
        check_ties_around_measure(meas);
        meas->need_recompute_beamings = true;
        recalculate_all_measure_chord_parameters((t_notation_obj *)x, meas);
        meas->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE;
        set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
    }
}


char tie_untie_selection(t_score *x)
{
	char changed = 0;
	t_notation_item *curr_it;

	lock_general_mutex((t_notation_obj *)x);

    for (curr_it = x->r_ob.firstselecteditem; curr_it; curr_it = curr_it->next_selected) {
        if (curr_it->type == k_NOTE)
            ((t_note *)curr_it)->r_it.flags |= k_FLAG_TO_BE_MODIFIED;
        else if (curr_it->type == k_CHORD) {
            t_note *note;
            for (note = ((t_chord *)curr_it)->firstnote; note; note = note->next)
                note->r_it.flags |= k_FLAG_TO_BE_MODIFIED;
        }
    }
    
    if (x->r_ob.edit_tied_notes_together) {
        for (curr_it = x->r_ob.firstselecteditem; curr_it; curr_it = curr_it->next_selected) {
            if (curr_it->type == k_NOTE) {
                t_note *nt = ((t_note *)curr_it);
                if (nt->r_it.flags & k_FLAG_TO_BE_MODIFIED)
                    tie_untie_note_smart(x, nt, &changed);
            } else if (curr_it->type == k_CHORD) {
                t_chord *ch = ((t_chord *)curr_it);
                t_note *nt;
                for (nt = ch->firstnote; nt; nt = nt->next)
                    if (nt->r_it.flags & k_FLAG_TO_BE_MODIFIED)
                        tie_untie_note_smart(x, nt, &changed);
            }
        }
        
    } else {
        
        for (curr_it = x->r_ob.firstselecteditem; curr_it; curr_it = curr_it->next_selected) {
            if (curr_it->type == k_NOTE) {
                t_note *nt = ((t_note *)curr_it);
                if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)nt)) {
                    t_measure *meas;
                    create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nt->parent->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                    tie_untie_note((t_note *)curr_it);
                    changed = 1;
                    meas = ((t_note *)curr_it)->parent->parent;
                    check_ties_around_measure(meas);
                    meas->need_recompute_beamings = true;
                    recalculate_all_measure_chord_parameters((t_notation_obj *)x, meas);
                    meas->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE;
                    set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
                }
            } else if (curr_it->type == k_CHORD) {
                t_chord *ch = ((t_chord *)curr_it);
                if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)ch)) {
                    t_measure *meas;
                    create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)ch->parent, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                    tie_untie_chord((t_chord *)curr_it);
                    changed = 1;
                    meas = ((t_chord *)curr_it)->parent;
                    check_ties_around_measure(meas);
                    meas->need_recompute_beamings = true;
                    recalculate_all_measure_chord_parameters((t_notation_obj *)x, meas);
                    meas->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE;
                    set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
                }
            }
        }
    }
    
    for (curr_it = x->r_ob.firstselecteditem; curr_it; curr_it = curr_it->next_selected) {
        if (curr_it->type == k_NOTE)
            ((t_note *)curr_it)->r_it.flags &= ~k_FLAG_TO_BE_MODIFIED;
        else if (curr_it->type == k_CHORD) {
            t_note *note;
            for (note = ((t_chord *)curr_it)->firstnote; note; note = note->next)
                note->r_it.flags &= ~k_FLAG_TO_BE_MODIFIED;
        }
    }
    

	unlock_general_mutex((t_notation_obj *)x);

	verbose_print(x);
	return changed;
}

// selection and trees
char destroy_selected_tree_nodes(t_score *x, char also_lock_general_mutex, char also_lock_measure_rhythmic_tree, char also_add_undo_tick){ 
	
	t_llllelem **nodes = NULL;
	t_measure **node_measures = NULL;
	long num_nodes, i;
	char changed = 0;

	if (also_lock_general_mutex)
		lock_general_mutex((t_notation_obj *)x);
	
	selected_chords_and_measures_to_selected_rhythmic_tree_nodes((t_notation_obj *) x, &num_nodes, &nodes, &node_measures);
	
	for (i = 0; i < num_nodes; i++){
		if (nodes[i] == WHITENULL_llllelem || hatom_gettype(&(nodes[i]->l_hatom)) == H_LLLL){
			t_measure *meas = node_measures[i];
			t_llll *ll = (nodes[i] == WHITENULL_llllelem ? meas->rhythmic_tree : hatom_getllll(&(nodes[i]->l_hatom)));
			
			if (also_add_undo_tick)
				create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			
			// flatting ll
			flat_preserving_leveltypes(ll, 0, 0, k_RHYTHM_LEVEL_GRACE, LLLL_FREETHING_MEM, NULL);
			
			// adding singletons to each element
/*			t_llllelem *elem = ll->l_head;
			while (elem){
				t_llllelem *nextelem = elem->l_next;
				llll_wrap_element_range(elem, elem);
				elem = nextelem;
			}
*/			
			// splattering ll
			if (nodes[i] != WHITENULL_llllelem && !is_level_grace(ll))
				llll_splatter(nodes[i], LLLL_FREETHING_MEM);
			
			if (also_lock_measure_rhythmic_tree)
				meas->lock_rhythmic_tree = true; // we lock the tree for the measure!
			recompute_all_for_measure((t_notation_obj *) x, meas, true);
			set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
			changed = 1;
		}
	}

	if (also_lock_general_mutex)
		unlock_general_mutex((t_notation_obj *)x);

	bach_freeptr(nodes);
	bach_freeptr(node_measures);
	
	return changed;
}


char splatter_selected_tree_nodes(t_score *x, char also_lock_general_mutex, char lock_measure_rhythmic_trees, char create_undo_tick){ 
	
	t_llllelem **nodes = NULL;
	t_measure **node_measures = NULL;
	long num_nodes, i;
	char changed = 0;
	
	if (also_lock_general_mutex)
		lock_general_mutex((t_notation_obj *)x);
	
	selected_chords_and_measures_to_selected_rhythmic_tree_nodes((t_notation_obj *) x, &num_nodes, &nodes, &node_measures);
	
	for (i = 0; i < num_nodes; i++){
		if (nodes[i] != WHITENULL_llllelem) {
			t_measure *meas = node_measures[i];
			
			if (create_undo_tick)
				create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			
			if (hatom_gettype(&(nodes[i]->l_hatom)) == H_LLLL) {
				llll_splatter(nodes[i], LLLL_FREETHING_MEM);
				recompute_all_for_measure((t_notation_obj *) x, meas, true);
				set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
				if (lock_measure_rhythmic_trees)
					meas->lock_rhythmic_tree = true; // we lock the tree for the measure!
				changed = 1;
			} else if (hatom_gettype(&(nodes[i]->l_hatom)) == H_OBJ) {
				if (nodes[i]->l_parent->l_owner){
					llll_splatter(nodes[i]->l_parent->l_owner, LLLL_FREETHING_MEM);
					recompute_all_for_measure((t_notation_obj *) x, meas, true);
					set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
					if (lock_measure_rhythmic_trees)
						meas->lock_rhythmic_tree = true; // we lock the tree for the measure!
					changed = 1;
				}
			}
		}
	}
	
	if (also_lock_general_mutex)
		unlock_general_mutex((t_notation_obj *)x);
	
	bach_freeptr(nodes);
	bach_freeptr(node_measures);
	
	return changed;
}



char fix_levels_of_selected_tree_nodes_as_original(t_score *x){ 
	t_llllelem **nodes = NULL;
	t_measure **node_measures = NULL;
	long num_nodes, i;
	char changed = 0;
	
	lock_general_mutex((t_notation_obj *)x);
	
	selected_chords_and_measures_to_selected_rhythmic_tree_nodes((t_notation_obj *) x, &num_nodes, &nodes, &node_measures);
	
	for (i = 0; i < num_nodes; i++){
		if (nodes[i]){
			t_measure *meas = node_measures[i];
			char this_changed = 0;
			
			create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);

			if (nodes[i] != WHITENULL_llllelem && hatom_gettype(&nodes[i]->l_hatom) == H_LLLL)
				llll_funall(hatom_getllll(&nodes[i]->l_hatom), fix_level_type_flag_for_level_as_original_fn, &this_changed, 1, -1, FUNALL_SKIP_ATOMS);

			if (nodes[i] == WHITENULL_llllelem){ // are_all_chords_in_measure_selected((t_notation_obj *)x, meas)){
				this_changed |= (!meas->rhythmic_tree->l_thing.w_obj || ((t_rhythm_level_properties *)meas->rhythmic_tree->l_thing.w_obj)->level_type != k_RHYTHM_LEVEL_ORIGINAL);
				if (this_changed)
					set_level_type_flag_for_level(meas->rhythmic_tree, k_RHYTHM_LEVEL_ORIGINAL);
			}
			
			if (this_changed) {
				recompute_all_for_measure((t_notation_obj *) x, meas, true);
				meas->lock_rhythmic_tree = true; // we lock the tree for the measure!
			}
			changed |= this_changed;
		}
	}
	
	if (changed)
		set_need_perform_analysis_and_change_flag((t_notation_obj *)x);

	unlock_general_mutex((t_notation_obj *)x);
	
	bach_freeptr(nodes);
	bach_freeptr(node_measures);
	
	return changed;
}

long fix_level_type_flag_for_level_as_ignore_fn(void *data, t_hatom *a, const t_llll *address){
	if (hatom_gettype(a) == H_LLLL) {
		if (!hatom_getllll(a)->l_thing.w_obj) {
			hatom_getllll(a)->l_thing.w_obj = build_rhythm_level_properties();
			((t_rhythm_level_properties *)hatom_getllll(a)->l_thing.w_obj)->level_type = k_RHYTHM_LEVEL_IGNORE;
			*((char *) data) = true;
		} else if (!(((t_rhythm_level_properties *)hatom_getllll(a)->l_thing.w_obj)->level_type & k_RHYTHM_LEVEL_IGNORE)){
			((t_rhythm_level_properties *)hatom_getllll(a)->l_thing.w_obj)->level_type &= ~k_RHYTHM_LEVEL_FORCE_TUPLET; // we remove the force tuplet flag!
			((t_rhythm_level_properties *)hatom_getllll(a)->l_thing.w_obj)->level_type |= k_RHYTHM_LEVEL_IGNORE;
			*((char *) data) = true;
		}
	}
	return 0;
}


char rebeam_levels_of_selected_tree_nodes(t_score *x, char also_destroy_tuplets, char force_autoparse, long flags)
{
	t_llllelem **nodes = NULL;
	t_measure **node_measures = NULL;
	long num_nodes, i;
	char changed = 0;
	
	lock_general_mutex((t_notation_obj *)x);
	
	selected_chords_and_measures_to_selected_rhythmic_tree_nodes((t_notation_obj *) x, &num_nodes, &nodes, &node_measures);
	
	for (i = 0; i < num_nodes; i++){
		if (nodes[i]){
			t_llll *llll = NULL;
			t_measure *meas = node_measures[i];
			
			if (nodes[i] == WHITENULL_llllelem && meas)
				llll = meas->rhythmic_tree;
			else if (nodes[i] != WHITENULL_llllelem && hatom_gettype(&nodes[i]->l_hatom) == H_LLLL)
				llll = hatom_getllll(&nodes[i]->l_hatom);
			else
				llll = NULL;
			
			if (llll && meas) {
				char this_changed = 0;
				create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
				
				verbose_post_rhythmic_tree((t_notation_obj *) x, meas, gensym("Tab Pressed1"), 1);
				if (llll) {
					llll_funall(llll, fix_level_type_flag_for_level_as_ignore_fn, &this_changed, 1, -1, FUNALL_SKIP_ATOMS);
					if (this_changed && !also_destroy_tuplets)
						set_tuplet_levels_as_keep_levels(llll);
				}
				verbose_post_rhythmic_tree((t_notation_obj *) x, meas, gensym("Tab Pressed2"), 1);
				
				if (are_all_chords_in_measure_selected((t_notation_obj *)x, meas)){
					this_changed |= (!meas->rhythmic_tree->l_thing.w_obj || ((t_rhythm_level_properties *)meas->rhythmic_tree->l_thing.w_obj)->level_type != k_RHYTHM_LEVEL_IGNORE);
					if (this_changed) {
						llll_funall(meas->rhythmic_tree, fix_level_type_flag_for_level_as_ignore_fn, &this_changed, 1, -1, FUNALL_SKIP_ATOMS);
						if (!also_destroy_tuplets)
							set_tuplet_levels_as_keep_levels(meas->rhythmic_tree);
					}
				}
				
				if (this_changed) 
					recompute_all_for_measure((t_notation_obj *) x, meas, true);
				changed |= this_changed;
				verbose_post_rhythmic_tree((t_notation_obj *) x, meas, gensym("Tab Pressed"), 1);
			}
		}
	}
	
	if (changed) {
        char val = 0;
        
        set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
        if (force_autoparse) {
            val = x->r_ob.autoparse_rhythms;
            x->r_ob.autoparse_rhythms = true; // we force parsing to be done
        }
        perform_analysis_and_change(x, NULL, NULL, flags); //k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
        if (force_autoparse)
            x->r_ob.autoparse_rhythms = val;
	}
	
	unlock_general_mutex((t_notation_obj *)x);
	
	bach_freeptr(nodes);
	bach_freeptr(node_measures);
	
	return changed;
}

char create_level_for_selected_tree_nodes(t_score *x){ 
	
	t_llllelem **nodes = NULL;
	t_measure **node_measures = NULL;
	long num_nodes, i, j;
	char changed = 0;
	
	lock_general_mutex((t_notation_obj *)x);
	
	selected_chords_and_measures_to_selected_rhythmic_tree_nodes((t_notation_obj *) x, &num_nodes, &nodes, &node_measures);
	
	for (i = 0; i < num_nodes; i++){
		if (nodes[i]){
			t_measure *meas = node_measures[i];
			t_llllelem *last_element_in_row = nodes[i];
			t_llllelem *first_element_in_row = nodes[i];
			char inner_changed = true;
			t_llll *new_ll;
			
			create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			
			if (first_element_in_row == WHITENULL_llllelem && meas->firstchord && meas->lastchord) {
				first_element_in_row = meas->firstchord->rhythmic_tree_elem;
				last_element_in_row = meas->lastchord->rhythmic_tree_elem;

			} else if (first_element_in_row == WHITENULL_llllelem || last_element_in_row == WHITENULL_llllelem) {
				continue; // should never happen!!! Anyway, just in case, we really don't want to apply llll_wrap_element_range on WHITENULL lllls
			
			} else {
				nodes[i] = NULL;
				while (inner_changed){
					inner_changed = false;
					for (j = 0; j < num_nodes; j++){
						if (nodes[j] && nodes[j] == first_element_in_row->l_prev){
							first_element_in_row = nodes[j];
							nodes[j] = NULL;
							inner_changed = true;
						} else if (nodes[j] && nodes[j] == last_element_in_row->l_next){
							last_element_in_row = nodes[j];
							nodes[j] = NULL;
							inner_changed = true;
						}
					}
				}
			}
			
			
			
			new_ll = llll_wrap_element_range(first_element_in_row, last_element_in_row);
			set_level_type_flag_for_level(new_ll, k_RHYTHM_LEVEL_ORIGINAL);
			recompute_all_for_measure((t_notation_obj *) x, meas, true);
			set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
			changed = 1;
		}
	}
	
	unlock_general_mutex((t_notation_obj *)x);
	
	bach_freeptr(nodes);
	bach_freeptr(node_measures);
	
	return changed;
}




int is_in_tempo_shape(t_score *x, t_tempo *tempo, long point_x, long point_y){
// returns 1 if the point (point_x, point_y) is within the shape of the note NN.
	double tempo_x, tempo_width, tempo_y;
	
	if (tempo->hidden)
		return 0;
	
	tempo_x = unscaled_xposition_to_xposition((t_notation_obj *)x, tempo->owner->tuttipoint_reference->offset_ux + tempo->tuttipoint_offset_ux) - CONST_USHIFT_TEMPI_LEFT * x->r_ob.zoom_y;
	if (tempo->owner && !tempo->owner->prev && tempo->changepoint.r_num == 0 && !get_next_tempo(tempo)) // very first tempo!
		tempo_x = x->r_ob.j_inset_x + 1 + x->r_ob.notation_typo_preferences.clef_ux_shift + x->r_ob.voice_names_uwidth * x->r_ob.zoom_y;
	tempo_width = tempo->real_x_width;
	tempo_y = get_staff_top_y((t_notation_obj *)x, (t_voice *) tempo->owner->voiceparent, false) + (-x->r_ob.tempi_uy_pos + tempo->uy_offset) * x->r_ob.zoom_y;

//	post("***** note_NN: %dd    note_x: %.2f   note_y: %.2f   point_x: %ld   point_y: %ld   dist: %.2f", note_NN, note_x, note_y, point_x, point_y, dist);
	if ((point_x > tempo_x) && (point_x < tempo_x + tempo_width) && (point_y > tempo_y) && (point_y < tempo_y + 10 * x->r_ob.zoom_y))
		return 1;
	else
		return 0;
}



// returns 1 if the point (point_x, point_y) is on the markername
int is_in_markername_shape(t_score *x, long point_x, long point_y, t_marker *marker){
	double marker_x;
	double marker_namewidth, marker_name_y_start, marker_nameheight;
	
	if (marker->attach_to == k_MARKER_ATTACH_TO_MEASURE) {
//		t_timepoint tp = measure_attached_marker_to_timepoint((t_notation_obj *)x, marker);
		marker_x = unscaled_xposition_to_xposition((t_notation_obj *)x, ms_to_unscaled_xposition((t_notation_obj *)x, marker->position_ms, 1));
	} else
		marker_x = ms_to_xposition((t_notation_obj *)x, marker->position_ms, 1);
	
	marker_namewidth = marker->name_uwidth * x->r_ob.zoom_y;
	marker_name_y_start = x->r_ob.j_inset_y + 10 * x->r_ob.zoom_y + 3 * x->r_ob.zoom_y;
	marker_nameheight = x->r_ob.markers_font_size * x->r_ob.zoom_y;
	if (marker->name_painted_direction > 0) {
		if (point_x > marker_x && point_x < marker_x + marker_namewidth + 2 * x->r_ob.zoom_y && 
			point_y > marker_name_y_start && point_y < marker_name_y_start + marker_nameheight)
			return 1;
	} else {
		if (point_x > marker_x - marker_namewidth - 2 * x->r_ob.zoom_y && point_x < marker_x && 
			point_y > marker_name_y_start && point_y < marker_name_y_start + marker_nameheight)
			return 1;
	}
	
	return 0;
}



void score_okclose(t_score *x, char *s, short *result)
{
	*result = 3;
}

void score_edclose(t_score *x, char **ht, long size){
	notation_obj_edclose((t_notation_obj *) x, ht, size);
}

void score_mousedown(t_score *x, t_object *patcherview, t_pt pt, long modifiers)
{
	double this_x, this_y;
	void* clicked_ptr = NULL; 
	e_element_types clicked_obj = k_ELEMENT_TYPES_UNDEFINED;
	char need_set_selection_dragging_velocity = false;
	char clicked_slot = 0, changed = 0;
	char need_popup = modifiers & eRightButton;
	char need_send_changed_bang = false;
	
	evnum_incr();

	x->r_ob.j_isdragging = false;
	x->r_ob.j_clicked_obj_has_been_selected = false;
    x->r_ob.check_selection_restraint_for_item = NULL;
	x->r_ob.changed_while_dragging = false;
	x->r_ob.j_mouse_hasbeendragged = 0; // we'll need this later, to check whether the mouse has been dragged
	x->r_ob.j_selection_hasbeendragged = 0;
	x->r_ob.j_mousedown_point = pt;
	x->r_ob.j_mousedrag_point = x->r_ob.j_mousedrag_point_shift_ffk = pt; 
	// gotta find if there's an element selected! We start with notes:
	x->r_ob.j_mouse_is_down = true;
//	x->r_ob.dragging_x = pt.x;
//	x->r_ob.dragging_y = pt.y;
	x->r_ob.floatdragging_x = pt.x;
	x->r_ob.floatdragging_y = pt.y;
	
	llll_format_modifiers(&modifiers, NULL);
	this_x = pt.x; 
	this_y = pt.y; // click-point coordinates
	
	x->r_ob.main_dragging_direction = 0;

	if (x->r_ob.notation_cursor.measure && !(!(modifiers & eAltKey) && !(modifiers & eCommandKey) && (modifiers & eControlKey))){
		exit_linear_edit(x);
		return;
	}
	
	// dilation rectangle??
	if (x->r_ob.show_dilation_rectangle) {
		e_element_types clicked_point_in_dilation_rectangle = pt_to_dilation_rectangle_obj((t_notation_obj *)x, pt);
		if (!is_editable((t_notation_obj *)x, k_DILATION_RECTANGLE, k_ELEMENT_ACTIONS_NONE)) return;
		if (clicked_point_in_dilation_rectangle > 0) {
			clicked_ptr = WHITENULL;
			clicked_obj = clicked_point_in_dilation_rectangle;
			set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
			return;
		} else {
			x->r_ob.show_dilation_rectangle = false;
			bach_set_cursor((t_object *)x, &x->r_ob.j_mouse_cursor, patcherview, BACH_CURSOR_DEFAULT);
		}
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	}
	
	
	// popup menu?
	if (need_popup) {
		t_scorevoice *voice;
		if (!is_editable((t_notation_obj *)x, k_POPUP_MENU, k_ELEMENT_ACTIONS_NONE)) return;
		for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {

			if (((t_voice *)voice)->hidden || ((t_voice *)voice)->part_index != 0) continue;
			
			if (is_in_clef_shape((t_notation_obj *) x, this_x, this_y, (t_voice *) voice)) {
				int chosenelem = 0, chosenclef = k_CLEF_WRONG; 
				t_symbol *chosenkeysym;

				// tries to find if one of the common commands has been selected, if not we use <chosenelem>
				char res = handle_voice_popup((t_notation_obj *)x, (t_voice *)voice, modifiers, &chosenelem);
				
				if (res != k_CHANGED_DO_NOTHING) {
					handle_change((t_notation_obj *)x, res, k_UNDO_OP_UNKNOWN);
				} else {
					// insert voice?
					if (chosenelem == 2001 || chosenelem == 2002) {
						if (!is_editable((t_notation_obj *)x, k_VOICE, k_CREATION)) return;
						create_whole_score_undo_tick(x);
                        score_move_and_reinitialize_last_voice(x, chosenelem == 2002 ? (t_scorevoice *)voiceensemble_get_lastvoice((t_notation_obj *)x, (t_voice *)voice) : (voiceensemble_get_firstvoice((t_notation_obj *)x, (t_voice *)voice) ? ((t_scorevoice *)voiceensemble_get_firstvoice((t_notation_obj *)x, (t_voice *)voice))->prev : voice->prev), x->r_ob.keys_as_symlist[voice->v_ob.number], get_voice_clef((t_notation_obj *)x, (t_voice *)voice), llll_get(), voice->v_ob.midichannel, voice->v_ob.number + 1, voice);
                        handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_INSERT_VOICE);

                        return;
					}
				
					// move voice
					if (chosenelem == 2003 || chosenelem == 2004) {
						if (!is_editable((t_notation_obj *)x, k_VOICE, k_MODIFICATION_GENERIC)) return;
						create_whole_score_undo_tick(x);
						score_swap_voiceensembles(x, voice, chosenelem == 2003 ? voice->prev : voice->next);
						handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_MOVE_VOICE);
						return;
					}

					// delete voice?
					if (chosenelem == 2000) {
						if (!is_editable((t_notation_obj *)x, k_VOICE, k_DELETION)) return;
						create_whole_score_undo_tick(x);
						score_delete_voiceensemble(x, (t_voice *)voice);
						handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_VOICE);
						return;
					}
					
					// clefs ?
					chosenclef = popup_menu_result_to_clef((t_notation_obj *) x, chosenelem);
					if (chosenclef != k_CLEF_WRONG) {
						if (!is_editable((t_notation_obj *)x, k_VOICE, k_MODIFICATION_CLEF)) return;
						change_voiceensemble_clef((t_notation_obj *)x, (t_voice *)voice, chosenclef, true);
						handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_CLEFS);
					} 
					
					// keys?
					chosenkeysym = popup_menu_result_to_keysymbol((t_notation_obj *) x, chosenelem);
					if (chosenkeysym) {
						t_atom av[CONST_MAX_VOICES];
						long i; t_scorevoice *tmpvoice;
						if (!is_editable((t_notation_obj *)x, k_VOICE, k_MODIFICATION_KEY)) return;
						create_header_undo_tick((t_notation_obj *)x, k_HEADER_KEYS);
						for (i = 0, tmpvoice = x->firstvoice; i < x->r_ob.num_voices && tmpvoice; i++, tmpvoice = tmpvoice->next) {
							if (do_voices_belong_to_same_voiceensemble((t_notation_obj *)x, (t_voice *)tmpvoice, (t_voice *)voice))
								atom_setsym(av+i, chosenkeysym);
							else
								atom_setsym(av+i, x->r_ob.keys_as_symlist[i]);
						}
						score_setattr_keys(x, NULL, x->r_ob.num_voices, av);
						handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_KEYS);
					}
					
					// midichannels?
					if (chosenelem > 150 && chosenelem <= 166){
						if (!is_editable((t_notation_obj *)x, k_VOICE, k_MODIFICATION_GENERIC)) return;
						change_voiceensemble_midichannel((t_notation_obj *) x, (t_voice *)voice, chosenelem - 150, true);
						handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_MIDICHANNELS);
					} 
				}
				
				return; 
			}
		}
	}
	
	
	lock_general_mutex((t_notation_obj *)x);

	clear_preselection((t_notation_obj *) x); // we clear the previous pre-selection. 
	// not necessarily do we clear the selection as well: think at the case when a selection is already done, 
	// and the user click on a note of that selection, in order to drag it! 
	// So we handle that later.

	
	// first of all: are we in a slot mode???? Cause if we are in a slot mode, we gotta handle that separately
	if (x->r_ob.active_slot_num > -1 && !is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) {
		unlock_general_mutex((t_notation_obj *)x);	
		return;
	}

	clicked_slot = handle_slot_mousedown((t_notation_obj *) x, patcherview, pt, modifiers, &clicked_obj, &clicked_ptr, &changed, need_popup);
	
	if (clicked_slot && is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) {
		unlock_general_mutex((t_notation_obj *)x);	
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_SEND_BANG, k_UNDO_OP_CHANGE_SLOT);
		return;
	}
	
	if (modifiers == eControlKey + eAltKey) {
		clicked_ptr = WHITENULL;
		clicked_obj = k_DRAG;
		if (is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE))
			set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
		unlock_general_mutex((t_notation_obj *)x);	
		return;
	} 
	
	// clicked hscrollbar?
	if (!clicked_ptr && x->r_ob.need_hscrollbar && x->r_ob.show_hscrollbar) {
		char res;
		
		res = is_in_scrollbar_shape((t_notation_obj *) x, this_x, this_y);

		if (res != 0 && !is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) {
			unlock_general_mutex((t_notation_obj *)x);	
			return;
		}

		if (res == 1) {
			clicked_ptr = WHITENULL; //doesn't really matter... but NON 0
			clicked_obj = k_SCROLLBAR;
		} else if (res == -1) {
			clicked_ptr = WHITENULL;
			clicked_obj = k_SCROLLBAR_LEFT_AREA;
			x->r_ob.hscrollbar_x = x->r_ob.hscrollbar_x - x->r_ob.hscrollbar_width;
			update_hscrollbar((t_notation_obj *) x, 0);
			unlock_general_mutex((t_notation_obj *)x);
			send_domain(x, 7, NULL);
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
			return;
		} else if (res == -2) {
			clicked_ptr = WHITENULL;
			clicked_obj = k_SCROLLBAR_RIGHT_AREA;
			x->r_ob.hscrollbar_x = x->r_ob.hscrollbar_x + x->r_ob.hscrollbar_width;
			update_hscrollbar((t_notation_obj *) x, 0);
			unlock_general_mutex((t_notation_obj *)x);	
			send_domain(x, 7, NULL);
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
			return;
		}
	}

	// clicked vscrollbar?
	if (!clicked_ptr && x->r_ob.need_vscrollbar && x->r_ob.show_vscrollbar) {	
		char res;
		
		if (!is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) {
			unlock_general_mutex((t_notation_obj *)x);	
			return;
		}

		res = is_in_vscrollbar_shape((t_notation_obj *) x, this_x, this_y);
		if (res == 1) {
			clicked_ptr = WHITENULL; //doesn't really matter, but NON 0...
			clicked_obj = k_VSCROLLBAR;
		} else if (res == -1) {
			clicked_ptr = WHITENULL;
			clicked_obj = k_SCROLLBAR_TOP_AREA;
			x->r_ob.vscrollbar_y -= x->r_ob.vscrollbar_height;
			update_vscrollbar((t_notation_obj *) x, 0);
			unlock_general_mutex((t_notation_obj *)x);	
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
			//			send_vdomain(x, 7, NULL);
			return;
		} else if (res == -2) {
			clicked_ptr = WHITENULL;
			clicked_obj = k_SCROLLBAR_BOTTOM_AREA;
			x->r_ob.vscrollbar_y += x->r_ob.vscrollbar_height;
			update_vscrollbar((t_notation_obj *) x, 0);
			unlock_general_mutex((t_notation_obj *)x);	
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			//			send_vdomain(x, 7, NULL);
			set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
			return;
		}
	}
	
	
	// changing pos of a loop extreme (or both)?
	if (!clicked_ptr && x->r_ob.show_loop_region && !(modifiers & eAltKey) && !(modifiers & eCommandKey) && !(modifiers & eControlKey)) {
		t_rect rect;
		jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, patcherview, &rect);
		e_element_types pos = is_in_loop_region((t_notation_obj *)x, rect, this_x, this_y);
		if (pos != k_NONE) {
			unlock_general_mutex((t_notation_obj *)x);	
            notationobj_redraw((t_notation_obj *) x);
			if (is_editable((t_notation_obj *)x, k_LOOP_REGION, k_ELEMENT_ACTIONS_NONE))
				send_loop_region((t_notation_obj *) x, 7);
			set_mousedown((t_notation_obj *) x, WHITENULL_llll, pos);
			return;
		}
	}
	
    // clicked staff?
    if (!clicked_ptr) {
        long voicenum = yposition_to_voicenumber((t_notation_obj *)x, this_y, -1, k_VOICEENSEMBLE_INTERFACE_FIRST);
        t_scorevoice *voice = nth_scorevoice(x, voicenum);
        if (is_in_clef_shape((t_notation_obj *)x, pt.x, pt.y, (t_voice *)voice)) {
            clicked_ptr = voice;
            clicked_obj = k_VOICE;
            if (is_editable((t_notation_obj *)x, k_SELECTION, k_SINGLE_SELECTION) || is_editable((t_notation_obj *)x, k_SELECTION, k_MULTIPLE_SELECTION))
                delete_item_type_from_selection((t_notation_obj *) x, -k_VOICE); // we only keep voices in selection
        }
    }
    
	// clicked control point of slur
	if (!clicked_ptr && x->r_ob.show_slurs && is_editable((t_notation_obj *)x, k_SLUR, k_MODIFICATION_GENERIC)) { 
		long a;
		t_scorevoice *voice; t_measure *curr_meas; t_chord *curr_ch; t_note *curr_nt;
		for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices && !clicked_ptr; voice = voice->next)
			for (curr_meas = voice->firstmeasure; curr_meas && !clicked_ptr; curr_meas = curr_meas->next)
				for (curr_ch = curr_meas->firstchord; curr_ch && !clicked_ptr; curr_ch = curr_ch->next) 
					for (curr_nt = curr_ch->firstnote; curr_nt && !clicked_ptr; curr_nt = curr_nt->next)
						for (a = 0; a < curr_nt->num_slurs_to; a++)
							if (curr_nt->slur_to[a]->r_it.selected) {
								double start_pt_x, end_pt_x, start_pt_y, end_pt_y;
								start_pt_x = unscaled_xposition_to_xposition((t_notation_obj *)x, curr_nt->slur_to[a]->start_x);
								end_pt_x = unscaled_xposition_to_xposition((t_notation_obj *)x, curr_nt->slur_to[a]->end_x);
								start_pt_y = curr_nt->slur_to[a]->start_y;
								end_pt_y = curr_nt->slur_to[a]->end_y;
								if (fabs(this_x - start_pt_x) < CONST_SLUR_START_END_POINT_SEL_WIDTH + 1 && fabs(this_y - start_pt_y) < CONST_SLUR_START_END_POINT_SEL_WIDTH + 1) {
									clicked_ptr = curr_nt->slur_to[a];
									clicked_obj = k_SLUR_START_POINT;
								} else if (fabs(this_x - end_pt_x) < CONST_SLUR_START_END_POINT_SEL_WIDTH + 1 && fabs(this_y - end_pt_y) < CONST_SLUR_START_END_POINT_SEL_WIDTH + 1) {
									clicked_ptr = curr_nt->slur_to[a];
									clicked_obj = k_SLUR_END_POINT;
								}
							}
	}
	
	// clicked tail?
	if (!clicked_ptr && x->r_ob.show_durations && modifiers != eCommandKey) {
		t_scorevoice *voice = x->firstvoice;
		t_measure *curr_meas; t_chord *curr_ch;
		while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
			curr_meas = voice->firstmeasure;
			while (curr_meas && !clicked_ptr) {
				curr_ch = curr_meas->firstchord;
				while (curr_ch && !clicked_ptr) { // cycle on the chords
					t_note *curr_nt = curr_ch->firstnote;
					while (curr_nt && !clicked_ptr) { // cycle on the notes
						if (is_in_tail_shape((t_notation_obj *)x,curr_nt,this_x,this_y)) {
							clicked_ptr = curr_nt->lastbreakpoint;
							clicked_obj = k_PITCH_BREAKPOINT;
							need_set_selection_dragging_velocity = true; 
							break;
						}
						curr_nt = curr_nt->next;
					} 
					curr_ch = curr_ch->next;
				}
				curr_meas = curr_meas->next;
			}
			voice = voice->next;
		}
	}

	
	// clicked note? 
	// but if also the same note's tail is clicked, we keep the tail as clicked! (so we can handle tails of very short notes)
	// that's why we did "clicked tail?" before "clicked note?"
	if (!clicked_ptr) { // looking for the clicked note, if any
		t_scorevoice *voice = x->firstvoice;
		t_measure *curr_meas; t_chord *curr_ch;
		while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
			curr_meas = voice->firstmeasure;
			while (curr_meas && !clicked_ptr) {
				curr_ch = curr_meas->firstchord;
				
				while (curr_ch && !clicked_ptr) { // cycle on the chords
					if (curr_ch->r_sym_duration.r_num > 0) {
						t_note *curr_nt = curr_ch->firstnote;
						while (curr_nt && !clicked_ptr) { // cycle on the notes
							if (is_in_note_shape((t_notation_obj *)x,curr_nt,this_x,this_y)) {
								if (need_popup){
									long res = k_CHANGED_DO_NOTHING;
									unlock_general_mutex((t_notation_obj *)x);	

                                    if (is_editable((t_notation_obj *)x, k_POPUP_MENU, k_ELEMENT_ACTIONS_NONE))
										res = handle_note_popup((t_notation_obj *) x, curr_nt, modifiers, clipboard.type);
                                    
                                    if (res == 502 || res == 503) {
                                        lock_markers_mutex((t_notation_obj *)x);;
                                        lock_general_mutex((t_notation_obj *)x);
                                        double onset = notation_item_get_onset_ms((t_notation_obj *)x, (t_notation_item *)curr_nt);
                                        if (onset >= 0) {
                                            create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
                                            t_llll *names = find_unused_marker_names((t_notation_obj *) x, NULL, NULL);
                                            if (res == 502) {
                                                t_timepoint tp = build_timepoint_with_voice(curr_nt->parent->parent->measure_number, curr_nt->parent->r_sym_onset, curr_nt->parent->parent->voiceparent->v_ob.number);
                                                clicked_ptr = add_marker((t_notation_obj *) x, names, onset, tp, k_MARKER_ATTACH_TO_MEASURE, k_MARKER_ROLE_NONE, NULL, 0);
                                            } else
                                                clicked_ptr = add_marker((t_notation_obj *) x, names, onset, build_timepoint(0, long2rat(0)), k_MARKER_ATTACH_TO_MS, k_MARKER_ROLE_NONE, NULL, 0);
                                            sync_marker_absolute_ms_onset(x, (t_marker *)clicked_ptr);
                                            clicked_obj = k_MARKER;
                                            x->r_ob.item_changed_at_mousedown = 1;
                                            //			x->r_ob.changed_while_dragging = true;
                                            handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_ADD_MARKER);
                                            llll_free(names);
                                        }
                                        unlock_markers_mutex((t_notation_obj *)x);;
                                        unlock_general_mutex((t_notation_obj *)x);
                                        return;
                                    } else if (res == 475 && is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_DURATION)) {
										score_legato(x);
										res = k_CHANGED_DO_NOTHING; // undo ticks, undo markers and bang already done in roll_legato
									} else if (res == 600 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
										rebeam_levels_of_selected_tree_nodes(x, false, true, k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
										handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_AUTO_RHYTHMIC_TREE_KEEPING_EXISTING_TUPLETS);
									} else if (res == 601 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
										fix_levels_of_selected_tree_nodes_as_original(x);
										handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_STICK_RHYTHMIC_TREE);
									} else if (res == 602 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
										destroy_selected_tree_nodes(x, true, true, true);
										handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DESTROY_RHYTHMIC_TREE);
									} else if (res == 603 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
										create_level_for_selected_tree_nodes(x);
										handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_ADD_RHYTHMIC_TREE_LEVEL);
									} else if (res == 604 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
										splatter_selected_tree_nodes(x, true, true, true);
										handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DESTROY_RHYTHMIC_TREE_LEVEL);
									} else if (res == 605 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
										rebeam_levels_of_selected_tree_nodes(x, true, true, k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
										handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_AUTO_RHYTHMIC_TREE_IGNORING_EXISTING_TUPLETS);
                                    } else if (res == 971) { // copy duration line
                                        notation_obj_copy_durationline((t_notation_obj *)x, &clipboard, curr_nt, false);
                                    } else if (res >= 9000 && res <= 9000 + CONST_MAX_SLOTS + 1) { // copy slot
                                        notation_obj_copy_slot((t_notation_obj *)x, &clipboard, (t_notation_item *)curr_nt, res - 9000 - 1, false);
                                    } else if (res == 981) { // paste duration line
                                        if (clipboard.type == k_DURATION_LINE)
                                            notation_obj_paste_durationline((t_notation_obj *)x, &clipboard);
                                    } else if (res >= 10000 && res <= 10000 + CONST_MAX_SLOTS + 1) { // paste slot
                                        if (clipboard.type == k_SLOT)
                                            notation_obj_paste_slot((t_notation_obj *) x, &clipboard, res - 10000 - 1);
                                    } else if (res != k_CHANGED_DO_NOTHING)
										handle_change((t_notation_obj *)x, res, k_UNDO_OP_UNKNOWN);
									return;
								} else if ((modifiers == eCommandKey) && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_nt))) { // delete note!
									if (is_editable((t_notation_obj *)x, curr_nt->parent->num_notes == 1 ? k_CHORD : k_NOTE, k_DELETION)) {
										char need_recompute_beamings = (curr_nt->parent->num_notes <= 1 || curr_nt->parent->is_grace_chord);
										char there_is_a_tied_note = get_num_tied_to_notes(curr_nt->parent) > 0 || get_num_tied_from_notes(curr_nt->parent) > 0;
										t_measure *meas = curr_ch->parent;
										clicked_ptr = NULL;
										create_simple_notation_item_undo_tick((t_notation_obj *)x, (need_recompute_beamings || there_is_a_tied_note) ? (t_notation_item *)curr_nt->parent->parent : (t_notation_item *)curr_nt->parent, k_UNDO_MODIFICATION_CHANGE);
										if (curr_ch->is_grace_chord) {
											chord_delete_from_measure((t_notation_obj *)x, curr_ch, true);
										} else {
#ifdef BACH_CHORDS_HAVE_SLOTS
                                            if (curr_ch->num_notes == 1) {
                                                erase_all_notationitem_slots((t_notation_obj *)x, (t_notation_item *)curr_ch);
                                                if (notation_item_has_slot_content((t_notation_obj *)x, (t_notation_item *)curr_nt)) {
                                                    clone_slots_for_notation_item((t_notation_obj *)x, (t_notation_item *)curr_nt, (t_notation_item *)curr_ch, k_CLONE_FOR_ORIGINAL, true);
                                                }
                                            }
#endif

											note_delete((t_notation_obj *)x, curr_nt, false);
											check_if_need_to_splatter_level_when_turning_note_to_rest(x, curr_ch);
										}
										x->r_ob.item_changed_at_mousedown = 1;
										recompute_all_for_measure((t_notation_obj *)x, meas, need_recompute_beamings);
									}
									unlock_general_mutex((t_notation_obj *)x);	
									handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_NOTE);
									verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->firstmeasure, gensym("after"), 1);
									return;
								} else if ((t_note *)clicked_ptr != curr_nt) {
									clicked_ptr = curr_nt;
									clicked_obj = k_NOTE;
									need_set_selection_dragging_velocity = true;
									break;
								}
							}
							curr_nt = curr_nt->next;
						}
                        
                    } else {
						if (is_in_rest_shape((t_notation_obj *)x,curr_ch,this_x,this_y)){

							if ((modifiers == eCommandKey) && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))) {
								// change rest to note
								if (is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_CREATION)) {
									t_note *newnote;
									create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_ch->parent, k_UNDO_MODIFICATION_CHANGE);
                                    newnote = build_note((t_notation_obj *)x, yposition_to_mc((t_notation_obj *)x, pt.y, NULL, NULL), 1000, CONST_DEFAULT_NEW_NOTE_VELOCITY);
                                    if (newnote) {
#ifdef BACH_CHORDS_HAVE_SLOTS
                                        if (notation_item_has_slot_content((t_notation_obj *)x, (t_notation_item *)curr_ch)) {
                                            clone_slots_for_notation_item((t_notation_obj *)x, (t_notation_item *)curr_ch, (t_notation_item *)newnote, k_CLONE_FOR_ORIGINAL);
                                            erase_all_notationitem_slots((t_notation_obj *)x, (t_notation_item *)curr_ch);
                                        } else {
                                            set_slots_values_to_note_from_llll((t_notation_obj *)x, newnote, x->r_ob.default_noteslots);
                                        }
#else 
                                        set_slots_values_to_note_from_llll((t_notation_obj *)x, newnote, x->r_ob.default_noteslots);
#endif
                                    }
                                    
                                    insert_note((t_notation_obj *)x, curr_ch, newnote, 0);
									x->r_ob.item_changed_at_mousedown = 1;
									curr_ch->r_sym_duration = rat_abs(curr_ch->r_sym_duration);
									notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)curr_ch);
									note_compute_approximation((t_notation_obj *) x, newnote);
									if (x->r_ob.snap_pitch_to_grid_when_editing) 
										snap_pitch_to_grid_for_note((t_notation_obj *) x, newnote);
									constraint_midicents_depending_on_editing_ranges((t_notation_obj *)x, &newnote->midicents, curr_ch->parent->voiceparent->v_ob.number);
                            
									recompute_all_for_measure((t_notation_obj *)x, curr_ch->parent, true);
									check_if_need_to_flatten_level_when_turning_rest_to_note(x, curr_ch);
									//								x->r_ob.changed_while_dragging = true;
								}
								unlock_general_mutex((t_notation_obj *)x);	
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_TURN_REST_INTO_NOTE);
								return;
							} else {
								clicked_ptr = curr_ch;
								clicked_obj = k_CHORD;
								need_set_selection_dragging_velocity = true;
								break;
							}
						}
					}
                    
                    // lyrics
                    if (!clicked_ptr && x->r_ob.link_lyrics_to_slot > 0 && x->r_ob.show_lyrics) {
                        if (is_in_chord_lyrics_shape((t_notation_obj *) x, curr_ch, this_x, this_y)){
                            clicked_ptr = curr_ch->lyrics;
                            clicked_obj = k_LYRICS;
                            break;
                        }
                    }
                    
                    // dynamics
                    if (!clicked_ptr && x->r_ob.link_dynamics_to_slot > 0 && x->r_ob.show_dynamics) {
                        if (is_in_chord_dynamics_shape((t_notation_obj *) x, curr_ch, this_x, this_y)){
                            clicked_ptr = curr_ch->dynamics;
                            clicked_obj = k_DYNAMICS;
                            break;
                        }
                    }

                    
					curr_ch = curr_ch->next;
				}
                
                
                
                // articulations?
                // clicked articulation?
                if (!clicked_ptr) { // looking for the clicked note, if any
                    long a;
                    // OLD WAY: articulation as a field
                    t_chord *curr_ch; t_note *curr_nt;
                    for (curr_ch = curr_meas->firstchord; curr_ch && !clicked_ptr; curr_ch = curr_ch->next) {
                        for (a = 0; a < curr_ch->num_articulations && !clicked_ptr; a++) {
                            if (is_in_articulation_shape((t_notation_obj *)x, &(curr_ch->articulation[a]), this_x, this_y)){
                                if (modifiers == eCommandKey && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))) { // delete articulation
                                    if (is_editable((t_notation_obj *)x, k_ARTICULATION, k_DELETION)) {
                                        create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_ch, k_UNDO_MODIFICATION_CHANGE);
                                        delete_articulation_from_notation_item((t_notation_obj *) x, (t_notation_item *)curr_ch, a);
                                        handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_ARTICULTATION);
                                        x->r_ob.item_changed_at_mousedown = 1;
                                    }
                                    unlock_general_mutex((t_notation_obj *)x);
                                    return;
                                } else {
                                    clicked_ptr = &(curr_ch->articulation[a]);
                                    clicked_obj = k_ARTICULATION;
                                }
                            }
                        }
                        if (curr_ch->r_sym_duration.r_num > 0) {
                            for (curr_nt = curr_ch->firstnote; curr_nt && !clicked_ptr; curr_nt = curr_nt->next) {
                                for (a = 0; a < curr_nt->num_articulations && !clicked_ptr; a++) {
                                    if (is_in_articulation_shape((t_notation_obj *)x, &(curr_nt->articulation[a]), this_x, this_y)){
                                        if (modifiers == eCommandKey && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_nt))) { // delete articulation
                                            if (is_editable((t_notation_obj *)x, k_ARTICULATION, k_DELETION)) {
                                                create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_ch, k_UNDO_MODIFICATION_CHANGE);
                                                delete_articulation_from_notation_item((t_notation_obj *) x, (t_notation_item *)curr_nt, a);
                                                handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_ARTICULTATION);
                                                x->r_ob.item_changed_at_mousedown = 1;
                                            }
                                            unlock_general_mutex((t_notation_obj *)x);
                                            return;
                                        } else {
                                            clicked_ptr = &(curr_nt->articulation[a]);
                                            clicked_obj = k_ARTICULATION;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                
                // NEW WAY: articulations as a slot
                if (!clicked_ptr && x->r_ob.show_articulations && x->r_ob.link_articulations_to_slot > 0 && x->r_ob.link_articulations_to_slot <= CONST_MAX_SLOTS) {
                    long s = x->r_ob.link_articulations_to_slot - 1;
                    if (x->r_ob.slotinfo[s].slot_type == k_SLOT_TYPE_ARTICULATIONS) {
                        t_slotitem *item;
                        t_chord *curr_ch; t_note *curr_nt;
                        for (curr_ch = curr_meas->firstchord; curr_ch && !clicked_ptr; curr_ch = curr_ch->next) {
                            if (!curr_ch->firstnote) {
                                for (item = curr_ch->slot[s].firstitem; item; item = item->next){
                                    t_articulation *art = (t_articulation *)item->item;
                                    if (is_in_articulation_shape((t_notation_obj *)x, art, this_x, this_y)){
                                        if (need_popup) {
                                            long res = k_CHANGED_DO_NOTHING;
                                            unlock_general_mutex((t_notation_obj *)x);
                                            if (is_editable((t_notation_obj *)x, k_ARTICULATION, k_MODIFICATION_GENERIC)) {
                                                res = handle_articulations_popup((t_notation_obj *) x, art, modifiers);
                                                handle_change((t_notation_obj *)x, res, k_UNDO_OP_UNKNOWN);
                                            }
                                            return;
                                        } else if (modifiers == eCommandKey && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))) { // delete articulation
                                            if (is_editable((t_notation_obj *)x, k_ARTICULATION, k_DELETION)) {
                                                create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_ch, k_UNDO_MODIFICATION_CHANGE);
                                                delete_slotitem((t_notation_obj *)x, s, item);
                                                handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_ARTICULTATION);
                                                x->r_ob.item_changed_at_mousedown = 1;
                                            }
                                            unlock_general_mutex((t_notation_obj *)x);
                                            return;
                                        } else {
                                            clicked_ptr = art;
                                            clicked_obj = k_ARTICULATION;
                                        }
                                    }
                                }
                            } else {
                                for (curr_nt = curr_ch->firstnote; curr_nt && !clicked_ptr; curr_nt = curr_nt->next) {
                                    for (item = curr_nt->slot[s].firstitem; item; item = item->next){
                                        t_articulation *art = (t_articulation *)item->item;
                                        if (is_in_articulation_shape((t_notation_obj *)x, art, this_x, this_y)){
                                            if (need_popup) {
                                                long res = k_CHANGED_DO_NOTHING;
                                                unlock_general_mutex((t_notation_obj *)x);
                                                if (is_editable((t_notation_obj *)x, k_ARTICULATION, k_MODIFICATION_GENERIC)) {
                                                    res = handle_articulations_popup((t_notation_obj *) x, art, modifiers);
                                                    handle_change((t_notation_obj *)x, res, k_UNDO_OP_UNKNOWN);
                                                }
                                                return;
                                            } else if (modifiers == eCommandKey && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_ch))) { // delete articulation
                                                if (is_editable((t_notation_obj *)x, k_ARTICULATION, k_DELETION)) {
                                                    create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_ch, k_UNDO_MODIFICATION_CHANGE);
                                                    delete_slotitem((t_notation_obj *)x, s, item);
                                                    handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_ARTICULTATION);
                                                    x->r_ob.item_changed_at_mousedown = 1;
                                                }
                                                unlock_general_mutex((t_notation_obj *)x);
                                                return;
                                            } else {
                                                clicked_ptr = art;
                                                clicked_obj = k_ARTICULATION;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                
                
                
				curr_meas = curr_meas->next;
			}
			voice = voice->next;
		}
	}


    
		
	// clicked breakpoint?
	if (!clicked_ptr && x->r_ob.show_durations && x->r_ob.allow_glissandi) { // looking for the clicked durationline, if any
		t_scorevoice *voice = x->firstvoice;
		t_measure *curr_meas; t_chord *curr_ch;
		while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
			curr_meas = voice->firstmeasure;
			while (curr_meas && !clicked_ptr) {
				curr_ch = curr_meas->firstchord;
				
				while (curr_ch && !clicked_ptr) { // cycle on the chords
					t_note *curr_nt = curr_ch->firstnote;
					while (curr_nt && !clicked_ptr) { // cycle on the notes
						if (curr_nt->num_breakpoints>2) { // if there are nontrivial breakpoints
							t_bpt *curr_bpt = curr_nt->firstbreakpoint->next;
							while (curr_bpt && !clicked_ptr) { // cycle on the breakpoints
								if (is_in_breakpoint_shape((t_notation_obj *)x, curr_bpt, this_x, this_y)) {
									if ((modifiers == eCommandKey) && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_nt))) { // delete bpt!
										if (is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_DELETION)) {
											clicked_ptr = NULL;
											create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_ch, k_UNDO_MODIFICATION_CHANGE);
											delete_breakpoint((t_notation_obj *) x, curr_bpt);
											x->r_ob.item_changed_at_mousedown = 1;
											//										x->r_ob.changed_while_dragging = true;
											recompute_all_for_measure((t_notation_obj *) x, curr_meas, false); // it's a bit overkilling!! TODO make it better
										}
										unlock_general_mutex((t_notation_obj *)x);	
										handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_PITCH_BREAKPOINT);
										return;
									} else {
										clicked_ptr = curr_bpt;
										clicked_obj = k_PITCH_BREAKPOINT;
										need_set_selection_dragging_velocity = true; 
										break;
									}
								}
								curr_bpt = curr_bpt->next;
							}
						}
						curr_nt = curr_nt->next;
					} 
					curr_ch = curr_ch->next;
				}
				curr_meas = curr_meas->next;
			}
			voice = voice->next;
		}
	}
	
	// clicked durationline?
	if (!clicked_ptr && x->r_ob.show_durations && (modifiers != eCommandKey)) { // looking for the clicked durationline, if any
		t_scorevoice *voice = x->firstvoice;
		t_measure *curr_meas; t_chord *curr_ch;
		while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
			curr_meas = voice->firstmeasure;
			while ((curr_meas)&&(!(clicked_ptr))) {
				curr_ch = curr_meas->firstchord;
				while ((curr_ch)&&(!(clicked_ptr))) { // cycle on the chords
					t_note *curr_nt = curr_ch->firstnote;
					while (curr_nt && !clicked_ptr) { // cycle on the notes
						if (is_in_durationline_shape((t_notation_obj *)x,curr_nt,this_x,this_y)) {
							if ((modifiers == eAltKey) && (x->r_ob.allow_glissandi) && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_nt))) { 
								// add a breakpoint!
								if (is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_CREATION)) {
									double start_x = unscaled_xposition_to_xposition((t_notation_obj *)x, curr_ch->parent->tuttipoint_reference->offset_ux + curr_ch->parent->start_barline_offset_ux + curr_ch->stem_offset_ux);
                                    t_note *last_tied;
                                    double end_x;
                                    if (x->r_ob.dl_spans_ties && (last_tied = get_last_tied_note(curr_nt)))
                                        end_x = unscaled_xposition_to_xposition((t_notation_obj *)x, last_tied->parent->parent->tuttipoint_reference->offset_ux + last_tied->parent->parent->start_barline_offset_ux + last_tied->parent->stem_offset_ux + last_tied->parent->duration_ux);
                                    else
                                        end_x = unscaled_xposition_to_xposition((t_notation_obj *)x, curr_ch->parent->tuttipoint_reference->offset_ux + curr_ch->parent->start_barline_offset_ux + curr_ch->stem_offset_ux + curr_ch->duration_ux);
									double rel_x_pos = (end_x > start_x) ? (this_x-start_x)/(end_x-start_x) : -1.;
									double y_pos = 0; //yposition_to_mc(x, this_y) - curr_nt->midicents;
									t_bpt *this_bpt;
									
									create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_ch, k_UNDO_MODIFICATION_CHANGE);
									
									this_bpt = add_breakpoint((t_notation_obj *) x, curr_nt, rel_x_pos, y_pos, 0., true, CONST_DEFAULT_NEW_NOTE_VELOCITY, true); 
									need_send_changed_bang = 1;
									clicked_ptr = this_bpt;
									clicked_obj = k_PITCH_BREAKPOINT;
									recompute_all_for_measure((t_notation_obj *) x, curr_meas, false); // it's a bit overkilling!! TODO make it better
//									handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_ADD_PITCH_BREAKPOINT);
									x->r_ob.item_changed_at_mousedown = 1;
								}
								break;
							} else {
								clicked_ptr = curr_nt->durationline;
								clicked_obj = k_DURATION_LINE;
								need_set_selection_dragging_velocity = true;
								break;
							}
						}
						curr_nt = curr_nt->next;
					} 
					curr_ch = curr_ch->next;
				}
				curr_meas = curr_meas->next;
			}
			voice = voice->next;
		}
	}

	
	// clicked tempi?
	// clicked durationline?
	if (!clicked_ptr && x->r_ob.show_tempi) { // looking for the clicked durationline, if any
		t_scorevoice *voice = x->firstvoice;
		t_measure *curr_meas; t_tempo *curr_tempo;
		while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
			curr_meas = voice->firstmeasure;
			while (curr_meas && !clicked_ptr) {
				curr_tempo = curr_meas->firsttempo;
				while (curr_tempo && !clicked_ptr) { // cycle on the tempi
					if (is_in_tempo_shape(x,curr_tempo,this_x,this_y)) {
						if ((modifiers == eCommandKey) && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)curr_meas))) {
							if (is_editable((t_notation_obj *)x, k_TEMPO, k_DELETION)) {
								create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)curr_meas, k_UNDO_MODIFICATION_CHANGE);
								delete_tempo((t_notation_obj *)x, curr_tempo);
								recompute_all(x);
							}
							unlock_general_mutex((t_notation_obj *)x);	
							handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_TEMPO);
							x->r_ob.item_changed_at_mousedown = 1;
							return;
						} else {
							clicked_ptr = curr_tempo;
							clicked_obj = k_TEMPO;
							break;
						}
					} 
					curr_tempo = curr_tempo->next;
				}
				curr_meas = curr_meas->next;
			}
			voice = voice->next;
		}
	}

	// clicked slurs?
	if (!clicked_ptr) { 
		long a;
		double this_ux = xposition_to_unscaled_xposition((t_notation_obj *) x, this_x);
		t_scorevoice *voice; t_measure *curr_meas; t_chord *curr_ch; t_note *curr_nt;
		for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices && !clicked_ptr; voice = voice->next)
			for (curr_meas = voice->firstmeasure; curr_meas && !clicked_ptr; curr_meas = curr_meas->next)
				for (curr_ch = curr_meas->firstchord; curr_ch && !clicked_ptr; curr_ch = curr_ch->next) 
					for (curr_nt = curr_ch->firstnote; curr_nt && !clicked_ptr; curr_nt = curr_nt->next)
						for (a = 0; a < curr_nt->num_slurs_to; a++) {
							double base = curr_nt->slur_to[a]->end_x - curr_nt->slur_to[a]->start_x;
							if (is_pt_in_quadrilater(	this_ux, this_y, curr_nt->slur_to[a]->start_x, curr_nt->slur_to[a]->start_y,
														curr_nt->slur_to[a]->start_x + curr_nt->slur_to[a]->cp1_rel_x * base, curr_nt->slur_to[a]->cp1_y, 
														curr_nt->slur_to[a]->start_x + curr_nt->slur_to[a]->cp2_rel_x * base, curr_nt->slur_to[a]->cp2_y,
														curr_nt->slur_to[a]->end_x, curr_nt->slur_to[a]->end_y)){
									if (modifiers == eCommandKey && is_editable((t_notation_obj *)x, k_SLUR, k_DELETION)) { // delete slur
										delete_slur((t_notation_obj *) x, curr_nt->slur_to[a]);
										x->r_ob.item_changed_at_mousedown = 1;
									} else {
										clicked_ptr = curr_nt->slur_to[a];
										clicked_obj = k_SLUR;
									}
							}
						}
	}
	
	// start editing in linear editing system?
	if (!clicked_ptr && !(modifiers & eAltKey) && !(modifiers & eCommandKey) && (modifiers & eControlKey) && x->r_ob.allow_linear_edit){
		long voicenum = yposition_to_voicenumber((t_notation_obj *)x, this_y, -1, k_VOICEENSEMBLE_INTERFACE_ACTIVE);
		t_scorevoice *voice = nth_scorevoice(x, voicenum);
		double mc = yposition_to_mc((t_notation_obj *)x, this_y, (t_voice *)voice, NULL);
		if (voicenum >= 0 && is_y_within_voice_staff((t_notation_obj *) x, this_y, (t_voice *)voice)) {
			double ux;
			t_measure *meas;
			ux = xposition_to_unscaled_xposition((t_notation_obj *)x, this_x);
			if (ux >= x->r_ob.screen_ux_start) {
				meas = ux_to_measure((t_notation_obj *)x, ux, (t_voice *)voice, NULL);
				if (meas) {
					t_chord *nearest_chord = find_ux_nearest_chord_in_measure((t_notation_obj *) x, meas, ux);
					long screen_nt;
					t_rational screen_acc;
					x->r_ob.notation_cursor.measure = meas;
					x->r_ob.notation_cursor.chord = nearest_chord;
					x->r_ob.notation_cursor.midicents = round(mc/100) * 100;
					mc_to_screen_approximations((t_notation_obj *) x, x->r_ob.notation_cursor.midicents, &screen_nt, &screen_acc, voice->v_ob.acc_pattern, voice->v_ob.full_repr);
					x->r_ob.notation_cursor.midicents = screen_nt;
					x->r_ob.notation_cursor.step = midicents_to_diatsteps_from_middleC((t_notation_obj *) x, screen_nt);
					move_linear_edit_cursor_depending_on_edit_ranges((t_notation_obj *)x, 0, 0);
					
                    llll_clear(x->r_ob.notation_cursor.touched_measures);
                    llll_appendobj(x->r_ob.notation_cursor.touched_measures, meas);
                    
					x->r_ob.is_linear_editing = true;
					
					if (is_measure_empty(x->r_ob.notation_cursor.measure)) {
						delete_all_chords_from_measure(x, x->r_ob.notation_cursor.measure);
						x->r_ob.notation_cursor.chord = NULL;					
					}
					unlock_general_mutex((t_notation_obj *)x);	
					return;
				}
			}
		}
	}

	// clicked measures? or measure barlines?
	if (!clicked_ptr && modifiers != eControlKey) {
		long voicenum = yposition_to_voicenumber((t_notation_obj *)x, this_y, -1, k_VOICEENSEMBLE_INTERFACE_ACTIVE);
		t_scorevoice *voice = nth_scorevoice(x, voicenum);
		char instaff = false, between_staff_and_next = false;
        
        if (voicenum < 0)
            object_warn((t_object *)x, "Warning: trying to edit a non-existing part. Check the ""activepart"" attribute.");
		
        if ((voicenum >= 0) && ((instaff = is_y_within_voice_staff((t_notation_obj *) x, this_y, (t_voice *)voice)) ||
			(x->r_ob.draw_barlines_across_staves && (between_staff_and_next = is_y_between_this_staff_and_the_next_or_prev((t_notation_obj *) x, this_y, (t_voice *)voice))))) {
			
			double ux;
			t_measure *meas;
			ux = xposition_to_unscaled_xposition((t_notation_obj *)x, this_x);
			if (ux >= x->r_ob.screen_ux_start) {
				char barline_clicked = 0;

				meas = ux_to_measure((t_notation_obj *)x, ux, (t_voice *)voice, &barline_clicked);
				if (barline_clicked == -1) { // left barline clicked? we'll switch to right barline of previous measure
					if (meas->prev) {
						barline_clicked = 1;
						meas = meas->prev;
					} else {
						barline_clicked = 0;
						meas = NULL;
					}
				}
				
				if (!(!instaff && (!barline_clicked || !is_barline_tuttipoint((t_notation_obj *)x, meas->end_barline)))) {
					
					if (!barline_clicked && (is_editable((t_notation_obj *)x, k_SELECTION, k_SINGLE_SELECTION) || is_editable((t_notation_obj *)x, k_SELECTION, k_MULTIPLE_SELECTION))) {
						// we clear the non-measure selection!
						delete_item_type_from_selection((t_notation_obj *) x, -k_MEASURE); // we only keep measures in selection
						x->r_ob.selection_type = k_MEASURE;
					}
					
					if (need_popup && meas){
						long res = k_CHANGED_DO_NOTHING;
						unlock_general_mutex((t_notation_obj *)x);	
						if (barline_clicked) {
							if (is_editable((t_notation_obj *)x, k_POPUP_MENU, k_ELEMENT_ACTIONS_NONE))
								res = handle_barline_popup((t_notation_obj *) x, meas, modifiers);
							if (res != k_CHANGED_DO_NOTHING)
								handle_change((t_notation_obj *)x, res, k_UNDO_OP_UNKNOWN);
						} else { 
							if (is_editable((t_notation_obj *)x, k_POPUP_MENU, k_ELEMENT_ACTIONS_NONE))
								res = handle_measure_popup((t_notation_obj *) x, meas, modifiers);
                            if (res == 150) {
                                score_copy_selected_measures(x, false);
                            } else if (res == 160) {
                                score_paste_replace_measures(x, true);
                            } else if (res == 161) {
                                score_paste_replace_measures(x, false);
                            } else if (res == 600 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
								rebeam_levels_of_selected_tree_nodes(x, false, true, k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_AUTO_RHYTHMIC_TREE_KEEPING_EXISTING_TUPLETS);
							} else if (res == 601 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
								fix_levels_of_selected_tree_nodes_as_original(x);
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_STICK_RHYTHMIC_TREE);
							} else if (res == 602 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
								destroy_selected_tree_nodes(x, true, true, true);
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DESTROY_RHYTHMIC_TREE);
							} else if (res == 605 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)){
								rebeam_levels_of_selected_tree_nodes(x, true, true, k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_AUTO_RHYTHMIC_TREE_IGNORING_EXISTING_TUPLETS);
							} else if (res == 590 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_GENERIC)) {
								lock_general_mutex((t_notation_obj *)x);	
								if (meas->firstchord && measure_barlines_coincide_for_all_voices(x, meas->measure_number)) {
									t_scorevoice *tempvc;
									for (tempvc = x->firstvoice; tempvc && tempvc->v_ob.number < x->r_ob.num_voices; tempvc = tempvc->next)
										create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nth_measure_of_scorevoice(tempvc, meas->measure_number), k_UNDO_MODIFICATION_CHANGE);
									pop_tempo_over_chord(x, meas->firstchord, true);
								} else {
									create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_UNDO_MODIFICATION_CHANGE);
									pop_tempo_over_chord(x, meas->firstchord, false);
								}
								unlock_general_mutex((t_notation_obj *)x);	
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_POP_OUT_TEMPO); 
                            } else if (res == 591 && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_GENERIC)) {
                                clear_selected_measures(x);
                                handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CLEAR_MEASURES_IN_SELECTION);
                            } else if (res == 592 && is_editable((t_notation_obj *)x, k_MEASURE, k_DELETION)) {
                                delete_selected_measures(x);
                                handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_MEASURES_IN_SELECTION);
							} else if (res != k_CHANGED_DO_NOTHING) {
								if (is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_GENERIC)) {
									if (res & k_CHANGED_PERFORM_ANALYSIS_AND_CHANGE) {	// changed time signature
										recompute_all(x);
										perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
									}
									handle_change((t_notation_obj *)x, res, k_UNDO_OP_UNKNOWN);
								}
							}
						}
						return;
					} else if (barline_clicked){
						// we add the measure barline to the selection!
						clicked_ptr = meas->end_barline;
						clicked_obj = k_MEASURE_END_BARLINE;
					} else if (meas && modifiers == eAltKey + eCommandKey && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)meas))){
						// we pop the tempo out
						t_chord *nearest_chord = find_ux_nearest_chord_in_measure((t_notation_obj *) x, meas, ux);
						if (nearest_chord && is_editable((t_notation_obj *)x, k_TEMPO, k_CREATION)) {
							if (measure_barlines_coincide_for_all_voices(x, meas->measure_number)) {
								t_scorevoice *tempvc;
								for (tempvc = x->firstvoice; tempvc && tempvc->v_ob.number < x->r_ob.num_voices; tempvc = tempvc->next)
									create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)nth_measure_of_scorevoice(tempvc, meas->measure_number), k_UNDO_MODIFICATION_CHANGE);
								pop_tempo_over_chord(x, nearest_chord, true);
							} else {
								create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_UNDO_MODIFICATION_CHANGE);
								pop_tempo_over_chord(x, nearest_chord, false);
							}
						}
						unlock_general_mutex((t_notation_obj *)x);	
						handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_POP_OUT_TEMPO); 
						x->r_ob.item_changed_at_mousedown = 1;
						return;
					} else if (modifiers == eCommandKey + eControlKey || modifiers == eCommandKey + eControlKey + eShiftKey){
						// we add an empty measure after this measure
						if (is_editable((t_notation_obj *)x, k_MEASURE, k_CREATION)) {
							char direction = modifiers & eShiftKey ? -1 : 1;
							if (meas && direction > 0)
								create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_UNDO_MODIFICATION_CHANGE); // because of ties
							else if (meas && meas->prev && direction > 0)
								create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas->prev, k_UNDO_MODIFICATION_CHANGE); // because of ties
							t_measure *newmeasure = create_and_insert_new_measure(x, voice, meas ? meas : voice->lastmeasure, direction, 0, meas ? meas : voice->lastmeasure, false);
							create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)newmeasure, k_UNDO_MODIFICATION_DELETE);
							unlock_general_mutex((t_notation_obj *)x);
							handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_ADD_MEASURE); 
							x->r_ob.item_changed_at_mousedown = 1;
						} else {
							unlock_general_mutex((t_notation_obj *)x);	
						}
						return;
					} else if (modifiers == eCommandKey || modifiers == eCommandKey + eShiftKey){
						// we add an empty measure in all voices measure
						if (is_editable((t_notation_obj *)x, k_MEASURE, k_CREATION)) {
							t_measure *added_meas[CONST_MAX_VOICES];
							long num_added_meas = 0;
							long clicked_meas_id = meas ? meas->measure_number : -1;
							char direction = (modifiers & eShiftKey) ? -1 : 1;
							e_undo_operations undo_op = k_UNDO_OP_ADD_MEASURES;
							insert_new_measure_in_all_voices(x, voice, clicked_meas_id, direction, added_meas, &num_added_meas, true, &undo_op);
							//						create_multiple_notation_items_undo_ticks((t_notation_obj *)x, num_added_meas, (t_notation_item **)added_meas, k_UNDO_MODIFICATION_DELETE, &undo_op);
							unlock_general_mutex((t_notation_obj *)x);
							handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, undo_op); 
							x->r_ob.item_changed_at_mousedown = 1;
						} else {
							unlock_general_mutex((t_notation_obj *)x);	
						}
						return;
					} else if (meas) {
						// we add the measure to the selection!
						clicked_ptr = meas;
						clicked_obj = k_MEASURE;
//						if (!instaff)
//							preselect_measure_in_all_voices(x, meas, true);
						need_set_selection_dragging_velocity = true; 
					}
				}
			}
		}
	}

	// zooming region?
	if (!clicked_ptr && (modifiers == eCommandKey + eAltKey || modifiers == eCommandKey + eAltKey + eShiftKey)) {
		clicked_obj = k_ZOOMING_REGION;
		clicked_ptr = WHITENULL;
		if (is_editable((t_notation_obj *)x, k_ZOOMING_REGION, k_ELEMENT_ACTIONS_NONE)) 
			set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
		unlock_general_mutex((t_notation_obj *)x);	
		return;
	}


	// clicked marker?
	lock_markers_mutex((t_notation_obj *)x);;	
	if (!clicked_ptr && x->r_ob.show_markers && x->r_ob.firstmarker) {
		t_marker *marker;
		for (marker = x->r_ob.firstmarker; marker; marker = marker->next) {
			if (is_in_marker_shape((t_notation_obj *)x, marker, this_x, this_y) || is_in_markername_shape((t_notation_obj *)x, marker, this_x, this_y)){
				if (modifiers == eCommandKey) {
					if (is_editable((t_notation_obj *)x, k_MARKER, k_DELETION)) {
						create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
						delete_marker((t_notation_obj *) x, marker); 
						x->r_ob.item_changed_at_mousedown = 1;
						changed = true;
						recompute_total_length((t_notation_obj *)x);
						update_hscrollbar((t_notation_obj *)x, 0);
					}
//					x->r_ob.changed_while_dragging = true;
					unlock_markers_mutex((t_notation_obj *)x);;	
					unlock_general_mutex((t_notation_obj *)x);	
					handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_MARKER);
					return;
				} else if (modifiers == eControlKey) {
					if (is_editable((t_notation_obj *)x, k_MARKER, k_MODIFICATION_GENERIC)) {
						long voicenum = yposition_to_voicenumber((t_notation_obj *)x, this_y, -1, k_VOICEENSEMBLE_INTERFACE_ACTIVE);
                        if (voicenum < 0)
                            voicenum = yposition_to_voicenumber((t_notation_obj *)x, this_y, -1, k_VOICEENSEMBLE_INTERFACE_ANY);
						create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
						change_marker_attachment(x, marker, marker->attach_to == k_MARKER_ATTACH_TO_MS ? k_MARKER_ATTACH_TO_MEASURE : k_MARKER_ATTACH_TO_MS, voicenum);
					}
					unlock_markers_mutex((t_notation_obj *)x);;	
					unlock_general_mutex((t_notation_obj *)x);	
					handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_MARKER_ATTACHMENT);
					return;
				} else {
                    x->r_ob.ux_click_marker_diff = xposition_to_unscaled_xposition((t_notation_obj *)x, this_x) - ms_to_unscaled_xposition((t_notation_obj *)x, marker->position_ms, 1);

					t_marker *temp;
					double marker_mousedown_ux = get_marker_ux_position((t_notation_obj *)x, marker);
					for (temp = x->r_ob.firstmarker; temp; temp = temp->next){
						double this_ux = get_marker_ux_position((t_notation_obj *)x, temp);
						temp->ux_difference_with_mousedown_marker = this_ux - marker_mousedown_ux;
					}
					
					clicked_ptr = marker; 
					clicked_obj = k_MARKER;
					break;
				}
			}
		} 
	}
    
    
    
    // changing pos of playcursor?
    if (!clicked_ptr && x->r_ob.show_playhead && (modifiers == eAltKey)) {
        clicked_ptr = WHITENULL; //doesn't really matter, but NON 0...
        clicked_obj = k_PLAYCURSOR;
        if (x->r_ob.playhead_dragging_mode != 1 && is_editable((t_notation_obj *)x, k_PLAYCURSOR, k_ELEMENT_ACTIONS_NONE)) {
            x->r_ob.play_head_start_ux = CLAMP(xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.j_mousedrag_point.x), 0, scoreapi_get_end_ux(x));
            x->r_ob.play_head_start_ms = unscaled_xposition_to_ms((t_notation_obj *)x, x->r_ob.play_head_start_ux, 1);
        }
        unlock_markers_mutex((t_notation_obj *)x);;
        unlock_general_mutex((t_notation_obj *)x);
        notationobj_redraw((t_notation_obj *) x);
        if (x->r_ob.playhead_dragging_mode != 1 && is_editable((t_notation_obj *)x, k_PLAYCURSOR, k_ELEMENT_ACTIONS_NONE)) {
            send_moved_playhead_position((t_notation_obj *) x, 7);
        }
        set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);
        return;
    }

	
	// adding a marker?
	if (!clicked_ptr && x->r_ob.show_markers && is_editable((t_notation_obj *)x, k_MARKER, k_CREATION) && (modifiers == eAltKey + eShiftKey || modifiers == eAltKey + eShiftKey + eControlKey)) {
		double onset = unscaled_xposition_to_ms((t_notation_obj *)x, xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.j_mousedown_point.x), 1);
		if (onset >= 0) { 
			create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
			t_llll *names = find_unused_marker_names((t_notation_obj *) x, NULL, NULL);
			if (!(modifiers & eControlKey)) {
                long voicenum = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_ACTIVE);
                if (voicenum < 0)
                    voicenum = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_ANY);
				t_timepoint tp = ms_to_timepoint((t_notation_obj *)x, onset, voicenum, k_MS_TO_TP_RETURN_INTERPOLATION);
				clicked_ptr = add_marker((t_notation_obj *) x, names, onset, tp, k_MARKER_ATTACH_TO_MEASURE, k_MARKER_ROLE_NONE, NULL, 0);
			} else 
				clicked_ptr = add_marker((t_notation_obj *) x, names, onset, build_timepoint(0, long2rat(0)), k_MARKER_ATTACH_TO_MS, k_MARKER_ROLE_NONE, NULL, 0);
			sync_marker_absolute_ms_onset(x, (t_marker *)clicked_ptr);
			clicked_obj = k_MARKER;
			x->r_ob.item_changed_at_mousedown = 1;
//			x->r_ob.changed_while_dragging = true;
			unlock_markers_mutex((t_notation_obj *)x);;	
			unlock_general_mutex((t_notation_obj *)x);	
			handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_ADD_MARKER);
			llll_free(names);
			return;
		}
	}
	unlock_markers_mutex((t_notation_obj *)x);;	


	if (!clicked_ptr && modifiers == eControlKey && is_editable((t_notation_obj *)x, k_SLUR, k_CREATION) && false) { // add a slur
		t_note *nt = find_nearest_note(x, pt.x, pt.y);
		if (nt) {
			t_slur *slur = add_slur(x, nt, nt);
			if (slur) {
				clicked_ptr = slur;
				clicked_obj = k_SLUR_END_POINT;
			}
		}
	}

	
	
	
	// setting loop?
	if (!clicked_ptr && x->r_ob.show_loop_region && modifiers & eControlKey && !(modifiers & eAltKey) && !(modifiers & eCommandKey)) {
		double this_ux = xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.j_mousedown_point.x);
		double this_ms = unscaled_xposition_to_ms((t_notation_obj *)x, this_ux, 1);
		t_timepoint this_tp = ms_to_timepoint_autochoose_voice((t_notation_obj *)x, this_ms, k_MS_TO_TP_RETURN_NEAREST, NULL);
		unlock_general_mutex((t_notation_obj *)x);	
		set_loop_region_from_extremes_tp(x, this_tp, this_tp);
        notationobj_redraw((t_notation_obj *) x);
		if (is_editable((t_notation_obj *)x, k_LOOP_REGION, k_ELEMENT_ACTIONS_NONE))
			send_loop_region((t_notation_obj *) x, 7);
		set_mousedown((t_notation_obj *) x, WHITENULL_llll, k_LOOP_END);
		return;
	}
	
		
	//		post("clicked_NN: %d, clicked obj: %d", clicked_NN, clicked_obj);

	if (!clicked_ptr && need_popup) {
		long res;
		unlock_general_mutex((t_notation_obj *)x);	
		if (!is_editable((t_notation_obj *)x, k_POPUP_MENU, k_ELEMENT_ACTIONS_NONE)) return;
		res = handle_background_popup((t_notation_obj *)x, modifiers);
        if (res == 502 || res == 503) {
            lock_markers_mutex((t_notation_obj *)x);;
            lock_general_mutex((t_notation_obj *)x);
            double onset = unscaled_xposition_to_ms((t_notation_obj *)x, xposition_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.j_mousedown_point.x), 1);
            if (onset >= 0) {
                create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
                t_llll *names = find_unused_marker_names((t_notation_obj *) x, NULL, NULL);
                if (res == 502) {
                    long voicenum = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_ACTIVE);
                    if (voicenum < 0)
                        voicenum = yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_ANY);
                    t_timepoint tp = ms_to_timepoint((t_notation_obj *)x, onset, voicenum, k_MS_TO_TP_RETURN_INTERPOLATION);
                    clicked_ptr = add_marker((t_notation_obj *) x, names, onset, tp, k_MARKER_ATTACH_TO_MEASURE, k_MARKER_ROLE_NONE, NULL, 0);
                } else
                    clicked_ptr = add_marker((t_notation_obj *) x, names, onset, build_timepoint(0, long2rat(0)), k_MARKER_ATTACH_TO_MS, k_MARKER_ROLE_NONE, NULL, 0);
                sync_marker_absolute_ms_onset(x, (t_marker *)clicked_ptr);
                clicked_obj = k_MARKER;
                x->r_ob.item_changed_at_mousedown = 1;
                //			x->r_ob.changed_while_dragging = true;
                handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_ADD_MARKER);
                llll_free(names);
            }
            unlock_markers_mutex((t_notation_obj *)x);;
            unlock_general_mutex((t_notation_obj *)x);
            return;
        } else
            handle_change((t_notation_obj *)x, res, k_UNDO_OP_UNKNOWN);
		return;
	}
	
	// redraw if needed
	if (clicked_ptr)
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	
    if (!clicked_ptr && clicked_obj != k_SCROLLBAR && clicked_obj != k_VSCROLLBAR) {
		// we start a region determination (the user is either just clicking, or (pre)selecting a region.
		clicked_obj = k_REGION;
	}
	
	e_element_actions selection_action = (x->r_ob.num_selecteditems == 0) ? k_SINGLE_SELECTION : k_MULTIPLE_SELECTION;

	// see if we've clicked on a note which itself constitues a chord. In this case, change to chord selection.
	if (clicked_obj == k_NOTE && 
		( !(((t_note *)clicked_ptr)->r_it.selected || ((t_note *)clicked_ptr)->parent->r_it.selected) || // the note is NOT selected...
		  (((t_note *)clicked_ptr)->parent->r_it.selected && ((t_note *)clicked_ptr)->parent->num_notes == 1)) // ...or: the note is selected, but as the single-note whole chord
		) { 

		// let's see if we're selecting a note of a chord with just 1 note. In that case, the whole chord is selected.
		long count_notes_in_region = 0;
		char is_chord_selected = notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)((t_note *)clicked_ptr)->parent);
		t_note *curr_nt = ((t_note *)clicked_ptr)->parent->firstnote;
		while (curr_nt) { // how many notes are inside the selection?
			if ((curr_nt != clicked_ptr) && (is_chord_selected || ((notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)curr_nt)) && (modifiers & eShiftKey))))
				count_notes_in_region++;
			curr_nt = curr_nt->next;
		}

		selection_action = (x->r_ob.num_selecteditems == count_notes_in_region) ? k_SINGLE_SELECTION : k_MULTIPLE_SELECTION;

		if (count_notes_in_region + 1 == ((t_note *)clicked_ptr)->parent->num_notes) {
			if (is_editable((t_notation_obj *)x, k_SELECTION, selection_action)) {
				curr_nt = ((t_note *)clicked_ptr)->parent->firstnote;
				while (curr_nt) { // how many notes are inside the selection?
					notation_item_delete_from_selection((t_notation_obj *) x, (t_notation_item *)curr_nt);
					curr_nt = curr_nt->next;
				}
			}
			clicked_ptr = ((t_note *)clicked_ptr)->parent;
			clicked_obj = k_CHORD;
		}
	}

	// mouse change??
	if (clicked_obj == k_NOTE || clicked_obj == k_CHORD) {
		if (modifiers & eAltKey && !(modifiers & eControlKey)) 
			bach_set_cursor((t_object *)x, &x->r_ob.j_mouse_cursor, patcherview, BACH_CURSOR_COPY);
	}
	
	set_mousedown((t_notation_obj *) x, clicked_ptr, clicked_obj);

	// handle selections
	if (is_editable((t_notation_obj *)x, k_SELECTION, selection_action)) {
		if (x->r_ob.j_mousedown_ptr && 
			(x->r_ob.j_mousedown_obj_type == k_NOTE || x->r_ob.j_mousedown_obj_type == k_DURATION_LINE || x->r_ob.j_mousedown_obj_type == k_CHORD || x->r_ob.j_mousedown_obj_type == k_DURATION_TAIL || 
			 x->r_ob.j_mousedown_obj_type == k_MEASURE || x->r_ob.j_mousedown_obj_type == k_PITCH_BREAKPOINT || x->r_ob.j_mousedown_obj_type == k_ARTICULATION || 
			 x->r_ob.j_mousedown_obj_type == k_SLUR_END_POINT || x->r_ob.j_mousedown_obj_type == k_SLUR_START_POINT || x->r_ob.j_mousedown_obj_type == k_SLUR || 
			 x->r_ob.j_mousedown_obj_type == k_TEMPO || x->r_ob.j_mousedown_obj_type == k_VOICE || x->r_ob.j_mousedown_obj_type == k_MEASURE_END_BARLINE ||
			 x->r_ob.j_mousedown_obj_type == k_LYRICS || x->r_ob.j_mousedown_obj_type == k_DYNAMICS || x->r_ob.j_mousedown_obj_type == k_MARKER)){
				
				if (clicked_obj == k_DURATION_LINE) {
					clicked_ptr = ((t_duration_line *)clicked_ptr)->owner;
					clicked_obj = k_NOTE;
				}
				
				if (notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)clicked_ptr) &&
					(x->r_ob.j_mousedown_obj_type != k_MEASURE || (modifiers & eControlKey))) { // item is selected: selection does change!!!
					// this case will be handled at the mouseup: if we won't drag, we'll delete the element from the selection

                    if (((t_notation_item *)clicked_ptr)->type == k_CHORD || ((t_notation_item *)clicked_ptr)->type == k_NOTE)
                        x->r_ob.check_selection_restraint_for_item = (t_notation_item *)clicked_ptr;

					if (!(modifiers & eShiftKey))
						x->r_ob.j_clicked_obj_has_been_selected = true;
				} else if (clicked_obj == k_NOTE && notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)((t_note *)clicked_ptr)->parent)) {
					// this case will be handled at the mouseup too (if we won't drag)
					// particular case of a chord selected and we click on a note, just to deselect the note. but to accomplish this we have to:
					// 1) deselect the chord; 2) select all the notes but this one

                    if (((t_notation_item *)clicked_ptr)->type == k_CHORD || ((t_notation_item *)clicked_ptr)->type == k_NOTE)
                        x->r_ob.check_selection_restraint_for_item = (t_notation_item *)clicked_ptr;
                } else {
					if (!(modifiers & eShiftKey))
						clear_selection((t_notation_obj *) x);
					notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)clicked_ptr);
					// move the preselection to the overall selection
					move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_INVOLUTIVE, !(modifiers & eControlKey), x->r_ob.edit_tied_notes_together);
					if (clicked_obj == k_MEASURE && !(modifiers & eControlKey)) {
						select_all_measure_in_selected_measures_range(x); // selection of a "square range" of measures
					} 
					x->r_ob.j_clicked_obj_has_been_selected = true;
				}
				
			} else if (x->r_ob.j_mousedown_obj_type != k_SCROLLBAR && x->r_ob.j_mousedown_obj_type != k_VSCROLLBAR) {
				if (!(modifiers & eShiftKey)) {
					clear_selection((t_notation_obj *) x);
				}
			}
	}
	
	unlock_general_mutex((t_notation_obj *)x);	

	if (need_send_changed_bang)
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_SEND_BANG, k_UNDO_OP_UNKNOWN);
	else
		handle_change_selection((t_notation_obj *)x);
	
	if (need_set_selection_dragging_velocity) 
		set_selection_dragging_velocity((t_notation_obj *) x);
}

// dirty!
char measure_position_cmp(t_measure *meas1, t_measure *meas2)
{
	double onset1 = measure_get_overall_onset(meas1);
	double onset2 = measure_get_overall_onset(meas2);
	if (onset1 < onset2)
		return -1;
	if (onset2 > onset1)
		return 1;
	return 0;
}

void select_all_measure_in_selected_measures_range(t_score *x) {
	t_tuttipoint *tpt_left = NULL, *tpt_right = NULL;
	double tpt_left_measure_offset = -1, tpt_right_measure_offset = -1;
	long voice_min = -1, voice_max = -1;
	t_notation_item *thisitem;
	long i;
	
	t_measure *leftmost_selected[CONST_MAX_VOICES];
	t_measure *rightmost_selected[CONST_MAX_VOICES];
	
	for (i = 0; i < x->r_ob.num_voices; i++)
		leftmost_selected[i] = rightmost_selected[i] = NULL;
	
	for (thisitem = x->r_ob.firstselecteditem; thisitem; thisitem = thisitem->next_selected) {
		if (thisitem->type == k_MEASURE) {
			t_measure *this_meas = ((t_measure *)thisitem);
			long this_voice_num = CLAMP(this_meas->voiceparent->v_ob.number, 0, CONST_MAX_VOICES - 1);
			if (voice_min < 0 || this_voice_num < voice_min)
				voice_min = this_voice_num;
			if (voice_max < 0 || this_voice_num > voice_max)
				voice_max = this_voice_num;
				
			if (!leftmost_selected[this_voice_num] || measure_position_cmp(this_meas, leftmost_selected[this_voice_num]) < 0)
				leftmost_selected[this_voice_num] = this_meas;

			if (!rightmost_selected[this_voice_num] || measure_position_cmp(this_meas, rightmost_selected[this_voice_num]) > 0)
				rightmost_selected[this_voice_num] = this_meas;
			
			t_tuttipoint *this_tpt = this_meas->tuttipoint_reference;
			if (!tpt_left || this_tpt == tpt_left || this_tpt->onset_ms < tpt_left->onset_ms) {
				tpt_left = this_tpt;
				if (tpt_left->measure[this_voice_num] != this_meas) {
					if (this_tpt->onset_ms < tpt_left->onset_ms)
						tpt_left_measure_offset = this_meas->tuttipoint_onset_ms;
					else if (tpt_left_measure_offset < 0 || this_meas->tuttipoint_onset_ms < tpt_left_measure_offset)
						tpt_left_measure_offset = this_meas->tuttipoint_onset_ms;
				} else {
					if (this_tpt->onset_ms < tpt_left->onset_ms)
						tpt_left_measure_offset = 0;
					else if (tpt_left_measure_offset < 0)
						tpt_left_measure_offset = 0;
				}
			}
			
			if (!tpt_right || this_tpt == tpt_right || this_tpt->onset_ms > tpt_right->onset_ms) {
				tpt_right = this_tpt;
				if (tpt_right->measure[this_voice_num] != this_meas) {
					if (this_tpt->onset_ms > tpt_right->onset_ms)
						tpt_right_measure_offset = this_meas->tuttipoint_onset_ms + this_meas->total_duration_ms;
					else if (tpt_left_measure_offset < 0 || this_meas->tuttipoint_onset_ms > tpt_right_measure_offset)
						tpt_right_measure_offset = this_meas->tuttipoint_onset_ms + this_meas->total_duration_ms;
				} else {
					if (this_tpt->onset_ms > tpt_right->onset_ms)
						tpt_right_measure_offset = this_meas->total_duration_ms;
					else if (tpt_right_measure_offset < 0)
						tpt_right_measure_offset = this_meas->total_duration_ms;
				}
			
			}
		}
	}
	
	if (!tpt_left) {
		tpt_left = x->firsttuttipoint;
		tpt_left_measure_offset = 0;
	}

	if (!tpt_right) {
		tpt_right = x->lasttuttipoint;
		tpt_right_measure_offset = 100000000; // whatever, big
	}
	
	if (voice_min >= 0 && voice_max >= 0) {
		clear_selection((t_notation_obj *) x);
		clear_preselection((t_notation_obj *) x);
		
		long i;
		for (i = voice_min; i <= voice_max; i++) {
			t_measure *start_meas = NULL, *end_meas = NULL;
			
			start_meas = tpt_left->measure[i];
			for (; start_meas && start_meas->tuttipoint_reference == tpt_left && start_meas->tuttipoint_onset_ms < tpt_left_measure_offset;)
				start_meas = start_meas->next;
				
			if (leftmost_selected[i] && measure_position_cmp(leftmost_selected[i], start_meas) < 0)
					start_meas = leftmost_selected[i];

			end_meas = tpt_right->measure[i];
			for (; end_meas && end_meas->tuttipoint_reference == tpt_right && 
					end_meas->next && end_meas->next->tuttipoint_reference == tpt_right &&
					end_meas->next->tuttipoint_onset_ms < tpt_right_measure_offset;)
				end_meas = end_meas->next;
			if (!end_meas)
				end_meas = nth_scorevoice(x, i)->lastmeasure;
			
			if (rightmost_selected[i] && measure_position_cmp(end_meas, rightmost_selected[i]) < 0)
				end_meas = rightmost_selected[i];
				
			if (start_meas && end_meas) {
				t_measure *meas;
				for (meas = start_meas; meas; meas = meas->next) {
					notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)meas);
					if (meas == end_meas)
						break;
				}
			}
		}
	}
}

/*
void select_all_measure_in_selected_measures_range(t_score *x, long end_voice, long end_meas_ID) {
	// finding leftmost and rightmost measure
	long voice_min = -1, voice_max = -1, measID_min = -1, measID_max = -1;
	t_notation_item *thisitem;
	for (thisitem = x->r_ob.firstselecteditem; thisitem; thisitem = thisitem->next_selected) {
		if (thisitem->type == k_MEASURE) {
			if (measID_min < 0 || ((t_measure *)thisitem)->measure_number < measID_min)
				measID_min = ((t_measure *)thisitem)->measure_number;
			if (measID_max < 0 || ((t_measure *)thisitem)->measure_number > measID_max)
				measID_max = ((t_measure *)thisitem)->measure_number;
			if (voice_min < 0 || ((t_measure *)thisitem)->voiceparent->v_ob.number < voice_min)
				voice_min = ((t_measure *)thisitem)->voiceparent->v_ob.number;
			if (voice_max < 0 || ((t_measure *)thisitem)->voiceparent->v_ob.number > voice_max)
				voice_max = ((t_measure *)thisitem)->voiceparent->v_ob.number;
		}
	}

//	if (end_meas_ID >= measID_min)
//		measID_max = end_meas_ID;
//	else {
//		measID_max = measID_min;
//		measID_min = end_meas_ID;
//	}
//	if (end_voice >= voice_min)
//		voice_max = end_voice;
//	else {
//		voice_max = voice_min;
//		voice_min = end_voice; 
//	}

	if (voice_min >= 0 && voice_max >= 0 && measID_min >= 0 && measID_max >= 0) {
		t_scorevoice *voice; t_measure *meas;
		clear_selection((t_notation_obj *) x);
		clear_preselection((t_notation_obj *) x);
		for (voice = nth_scorevoice(x, voice_min); voice && voice->v_ob.number <= voice_max; voice = voice->next) {
			for (meas = nth_measure_of_scorevoice(voice, measID_min); meas && meas->measure_number <= measID_max; meas = meas->next) {
				notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)meas);
			}
		}
	}
}*/

t_measure *find_measure_containing_ux(t_score *x, long num_voice, double ux, char always_return_something)
{
	t_scorevoice *voice = nth_scorevoice(x, num_voice);
	t_measure *meas;
	if (ux < 0) return (always_return_something ? voice->firstmeasure : NULL);
	for (meas = voice->firstmeasure; meas; meas = meas->next)
		if (ux < meas->tuttipoint_reference->offset_ux + meas->start_barline_offset_ux + meas->width_ux)
			return meas;
	return (always_return_something ? voice->lastmeasure : NULL);
}

t_note *find_nearest_note(t_score *x, double xpos, double ypos)
{
	double ux = xposition_to_unscaled_xposition((t_notation_obj *)x, xpos);
	long num_voice = yposition_to_voicenumber((t_notation_obj *)x, ypos, -1, k_VOICEENSEMBLE_INTERFACE_ANY);
	t_measure *meas = find_measure_containing_ux(x, num_voice, ux, true);
	t_chord *ch = find_ux_nearest_chord_in_measure((t_notation_obj *)x, meas, ux);
	t_note *nt, *best_nt = NULL; double best_diff = 0.;
	for (nt = ch->firstnote; nt; nt = nt->next)
		if (!best_nt || fabs(nt->center.y - ypos) < best_diff) {
			best_diff = fabs(nt->center.y - ypos);
			best_nt = nt;
		}
	return best_nt;
}

/*
void insert_measure_in_scorevoice(t_score *x, t_scorevoice *voice, t_measure *measure_to_insert, t_measure *after_this_measure){
// insert an (already built) measure in a scorevoice, at a given position.
	
	measure_to_insert->r_it.flags = 0; // one never knows... better to state that again, to avoid bad surprises...

	if (after_this_measure) { // not at the beginning
		if (after_this_measure->next) { // insert between these
			after_this_measure->next->prev = measure_to_insert;
			measure_to_insert->next = after_this_measure->next;
			measure_to_insert->prev = after_this_measure;
			after_this_measure->next = measure_to_insert;
		} else { // insert at the end
			measure_to_insert->next = NULL;
			measure_to_insert->prev = after_this_measure;
			after_this_measure->next = measure_to_insert;
			voice->lastmeasure = measure_to_insert; 
		}
	} else { // at the beginning
		if (voice->firstmeasure) { // there's already a chord
			measure_to_insert->next = voice->firstmeasure;
			measure_to_insert->prev = NULL;
			voice->firstmeasure->prev = measure_to_insert;
			voice->firstmeasure = measure_to_insert;
		} else { // no chords in the voices yet
			measure_to_insert->next = NULL;
			measure_to_insert->prev = NULL;
			voice->firstmeasure = measure_to_insert;
			voice->lastmeasure = measure_to_insert;
		}	
	}
	

	voice->num_measures++;
	refresh_measure_numbers(x, voice);
	recompute_all(x);
	//recompute_all_and_redraw(x);
}
*/






// insert a new measure in <reference_voice> after or before the measure #ref_meas_ID (depending on direction)
// but also insert the same measure in all other voices having LESS or EQUAL number of measures than the <reference> voice
void insert_new_measure_in_all_voices(t_score *x, t_scorevoice *reference_voice, long ref_meas_ID, char direction, 
										t_measure **added_measures, long *num_added_meas, char also_add_undo_ticks, e_undo_operations *undo_op)
{
	t_scorevoice *temp_voice;
	long ref_num_meas = reference_voice->num_measures;
	long number_of_added_meas = 0;

	if (undo_op)
		*undo_op = k_UNDO_OP_ADD_MEASURE;
	
	for (temp_voice = x->firstvoice; temp_voice && temp_voice->v_ob.number < x->r_ob.num_voices; temp_voice = temp_voice->next){
		if (temp_voice->num_measures <= ref_num_meas) {
			t_measure *added_meas = NULL, *temp_meas = (ref_meas_ID >= 0) ? nth_measure_of_scorevoice(temp_voice, ref_meas_ID) : NULL;
			if (added_measures && num_added_meas && number_of_added_meas < CONST_MAX_VOICES) {
				if (also_add_undo_ticks) {
					if (temp_meas && direction == -1 && temp_meas->prev)	
						create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)temp_meas->prev, k_UNDO_MODIFICATION_CHANGE);
					else if (temp_meas && direction != -1)
						create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)temp_meas, k_UNDO_MODIFICATION_CHANGE);
				}
				added_meas = create_and_insert_new_measure(x, temp_voice, temp_meas ? temp_meas : temp_voice->lastmeasure, temp_meas ? direction : 1, 0, temp_meas ? temp_meas : temp_voice->lastmeasure, false);
				added_measures[number_of_added_meas++] = added_meas;
				if (number_of_added_meas > 1 && undo_op && *undo_op != k_UNDO_OP_ADD_MEASURES)
					*undo_op = k_UNDO_OP_ADD_MEASURES;
				if (also_add_undo_ticks)
					create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)added_meas, k_UNDO_MODIFICATION_DELETE);
			}
		}
	}

	if (num_added_meas)
		*num_added_meas = number_of_added_meas;
}


//ref_voice and ref_meas_num are 1-based, and 1_based_ref_meas_num can be negative
void insert_measures_from_message(t_score *x, long start_voice_num_one_based, long end_voice_num_one_based, long ref_meas_num_one_based, t_llll *meas_ll, char allow_multiple_measures_per_voice)
{
    char need_update_solos = false;
    long i;
    lock_general_mutex((t_notation_obj *)x);
    t_llllelem *elem;
    long this_voice;
    
    // checking if we are adding a WHOLE TUTTIPOINT.
    // In which case, we'll bother adding it manually, and we won't parse the tuttipoints llll again.
    char tuttipoint_aligned = allow_multiple_measures_per_voice ? false : true;
    if (tuttipoint_aligned && (start_voice_num_one_based > 1 || end_voice_num_one_based < x->r_ob.num_voices)) {
        if (ref_meas_num_one_based < 0) {
/*            if (ref_meas_num_one_based < -1)
                tuttipoint_aligned = false; */
        } else {
            for (this_voice = 1; this_voice < x->r_ob.num_voices; this_voice++) {
                if (this_voice < start_voice_num_one_based || this_voice > end_voice_num_one_based)
                    continue;
                
                t_scorevoice *voice = nth_scorevoice(x, this_voice - 1);
                if (voice->num_measures > ref_meas_num_one_based) {
                    tuttipoint_aligned = false;
                    break;
                }
            }
        }
    }
    
    t_tuttipoint *ref_tpt = (t_tuttipoint *)WHITENULL;
    t_measure *meas[CONST_MAX_VOICES];
    for (i = 0; i < x->r_ob.num_voices; i++)
        meas[i] = NULL;
    start_voice_num_one_based = MAX(start_voice_num_one_based, 1);
    end_voice_num_one_based = MIN(end_voice_num_one_based, x->r_ob.num_voices);
    for (this_voice = start_voice_num_one_based, elem = meas_ll ? meas_ll->l_head : NULL; this_voice <= end_voice_num_one_based; elem = elem && elem->l_next? elem->l_next : elem, this_voice++) {
        if (true) {
//        if (!elem || hatom_gettype(&elem->l_hatom) == H_LLLL) {
            long elem_type = hatom_gettype(&elem->l_hatom);
            t_scorevoice *voice = nth_scorevoice(x, this_voice - 1);
            t_measure *after_this_measure = nth_measure_of_scorevoice(voice, ref_meas_num_one_based < 0 ? MAX(-1, voice->num_measures + ref_meas_num_one_based) : MIN(ref_meas_num_one_based  - 1 - 1, voice->num_measures - 1));
            
            if (tuttipoint_aligned && (((ref_tpt != (t_tuttipoint *)WHITENULL) && (after_this_measure ? after_this_measure->tuttipoint_reference : NULL) != ref_tpt) || (after_this_measure && after_this_measure->tuttipoint_onset_ms != 0))) {
                tuttipoint_aligned = false;
            } else if (tuttipoint_aligned)
                ref_tpt = after_this_measure ? after_this_measure->tuttipoint_reference : NULL;
            
            long num_measures = allow_multiple_measures_per_voice && elem ? (elem_type == H_LLLL ? hatom_getllll(&elem->l_hatom)->l_size : hatom_getlong(&elem->l_hatom)) : 1;
            t_llllelem *meas_elem = allow_multiple_measures_per_voice && elem ? (elem_type == H_LLLL ? hatom_getllll(&elem->l_hatom)->l_head : NULL) : elem;
            for (i = 0; i < num_measures; i++) {
                t_measure *new_meas = build_measure((t_notation_obj *)x, NULL);
                meas[this_voice - 1] = new_meas;
                
                // insert measure
                insert_measure(x, voice, new_meas, after_this_measure, 0);
                
                t_llll *meas_ll = meas_elem ? hatom_getllll(&meas_elem->l_hatom) : get_nilnil();
                if (!meas_ll)
                        meas_ll = get_nilnil();
                set_measure_from_llll(x, new_meas, meas_ll, true, true, &need_update_solos);
                if (!meas_elem || !meas_ll) llll_free(meas_ll);
                
                recompute_all_for_measure((t_notation_obj *)x, new_meas, true);
                if (new_meas->prev)
                    check_measure_ties((t_notation_obj *)x, new_meas->prev, x->r_ob.tie_assign_pitch, true);
                if (new_meas->next)
                    check_measure_ties((t_notation_obj *)x, new_meas->next, x->r_ob.tie_assign_pitch, true);
                
                if (allow_multiple_measures_per_voice)
                    meas_elem = meas_elem ? meas_elem->l_next : NULL;
                after_this_measure= new_meas;
            }
        }
    }
    
    if (need_update_solos)
        update_solos((t_notation_obj *)x);
    
    if (ref_tpt == (t_tuttipoint *)WHITENULL)
        tuttipoint_aligned = false;
    
    if (tuttipoint_aligned) {
        t_tuttipoint *ref_tpt_next = ref_tpt ? ref_tpt->next : x->firsttuttipoint;
        // calculate measure durations
        for (i = 0; i < x->r_ob.num_voices; i++)
            if (meas[i])
                calculate_rat_measure_durations_ms((t_notation_obj *)x, meas[i]);

        // check if inserted measures duration are the same
        double dur = -1;
        const double CONST_THRESHOLD = 1;
        for (i = 0; i < x->r_ob.num_voices; i++) {
            if (meas[i]){
                if (dur < 0) {
                    dur = meas[i]->total_duration_ms;
                } else {
                    long cmp = double_double_cmp_with_threshold(dur, meas[i]->total_duration_ms, CONST_THRESHOLD);
                    if (cmp != 0) {
                        tuttipoint_aligned = 0;
                        break;
                    }
                }
            }
        }
        
        if (tuttipoint_aligned) {
            t_tuttipoint *tpt = build_tuttipoint(x);
            
            // increasing IDs of following tuttipoints
            for (t_tuttipoint *temp = ref_tpt_next; temp; temp = temp->next)
                temp->ID++;
            tpt->ID = ref_tpt ? ref_tpt->ID + 1 : 0;
            
            for (i = 0; i < x->r_ob.num_voices; i++)
                tpt->measure[i] = meas[i];
            
            for (i = 0; i < x->r_ob.num_voices; i++)
                if (meas[i])
                    meas[i]->tuttipoint_reference = tpt;
            
            char changed = false;
            double dur_ms = meas[start_voice_num_one_based - 1]->total_duration_ms;
            t_rational r_dur_sec = approx_rat_with_rat_notify(meas[start_voice_num_one_based - 1]->r_total_duration_sec, CONST_RAT_MAX_NUM_SAFETY, CONST_RAT_MAX_DEN_SAFETY, &changed);
            
            tpt->r_duration_sec = r_dur_sec;
            tpt->r_onset_sec = (ref_tpt && ref_tpt_next) ? ref_tpt_next->r_onset_sec : (ref_tpt ? rat_rat_sum(ref_tpt->r_onset_sec, ref_tpt->r_duration_sec) : long2rat(0));
            tpt->onset_ms = (ref_tpt && ref_tpt_next) ? ref_tpt_next->onset_ms : (ref_tpt ? ref_tpt->onset_ms + ref_tpt->duration_ms : 0);
            for (t_tuttipoint *temp = ref_tpt_next; temp; temp = temp->next) {
                temp->r_onset_sec = rat_rat_sum(temp->r_onset_sec, r_dur_sec);
                temp->onset_ms += dur_ms;
            }
            
            char all_voices_start = true;
            for (i = 0; i < x->r_ob.num_voices; i++) {
                if (!meas[i]) {
                    all_voices_start = false;
                    break;
                }
            }
            tpt->all_voices_are_together = all_voices_start;


            insert_tuttipoint(x, tpt, ref_tpt);
            
        } else {
            recompute_all(x);
        }
        
    } else {
        recompute_all(x);
    }
    
    unlock_general_mutex((t_notation_obj *)x);
}





void send_subscore_values_as_llll(t_score *x, t_llll* whichvoices, long start_meas, long end_meas, t_llll *what_to_dump){
	t_llll* out_llll = get_subscore_values_as_llll(x, whichvoices, start_meas, end_meas, what_to_dump);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 0, out_llll);
	llll_free(out_llll);
}


void send_score_values_as_llll(t_score *x, long send_what){
	t_llll* out_llll = get_score_values_as_llll(x, k_CONSIDER_FOR_SAVING, send_what, x->r_ob.output_trees, x->r_ob.output_and_save_level_types, true, false);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 0, out_llll);
	llll_free(out_llll);
}

void send_all_values_as_llll(t_score *x, long send_what_for_header) 
{
	send_extras_values_as_llll(x, x->r_ob.output_trees == 2);
	send_ties_values_as_llll(x, x->r_ob.output_trees == 2);
	send_velocities_values_as_llll(x, x->r_ob.output_trees == 2);
	send_durations_values_as_llll(x, x->r_ob.output_trees == 2 ? (x->r_ob.output_full_duration_tree ? 2 : 1) : 0);
	send_cents_values_as_llll(x, x->r_ob.output_trees == 2 );
	send_measuresinfo_values_as_llll(x);
	send_score_values_as_llll(x, send_what_for_header);
}


void send_extras_values_as_llll(t_score *x, char tree){
	t_llll* out_llll = get_all_extras_values_as_llll(x, tree);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 6, out_llll);
	llll_free(out_llll);
}

void send_ties_values_as_llll(t_score *x, char tree){
	t_llll* out_llll = get_all_ties_values_as_llll(x, tree);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 5, out_llll);
	llll_free(out_llll);
}

void send_velocities_values_as_llll(t_score *x, char tree){
	t_llll* out_llll = get_all_velocities_values_as_llll(x, tree);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 4, out_llll);
	llll_free(out_llll);
}

void send_cents_values_as_llll(t_score *x, char tree){
	t_llll* out_llll = get_all_cents_values_as_llll(x, tree);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 2, out_llll);
	llll_free(out_llll);
}

void send_durations_values_as_llll(t_score *x, char tree){
	t_llll* out_llll = get_all_durations_values_as_llll(x, tree);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 3, out_llll);
	llll_free(out_llll);
}

void send_measuresinfo_values_as_llll(t_score *x){
	t_llll* out_llll = get_all_measuresinfo_values_as_llll(x);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, 1, out_llll);
	llll_free(out_llll);
}



t_llll* get_subscore_values_as_llll(t_score *x, t_llll* whichvoices, long start_meas, long end_meas, t_llll *what_to_dump){
	
	t_llll* out_llll = llll_get();
	t_llll *midichannels = llll_get(); 
	t_llll *clefs = llll_get();
	t_llll *keys = llll_get();
	t_llll *voicenames = llll_get();
	t_llll *stafflines = llll_get();

	t_scorevoice *voice;
	char we_take_it[CONST_MAX_VOICES];
	char what_to_dump_is_empty = (what_to_dump && has_llll_symbols_in_first_level(what_to_dump)) ? false : true;

	lock_general_mutex((t_notation_obj *)x);
	llll_appendsym(out_llll, _llllobj_sym_score, 0, WHITENULL_llll); // "score" message
	
	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_slotinfo))
		llll_appendllll(out_llll, get_slotinfo_values_as_llll((t_notation_obj *) x, false, false, false), 0, WHITENULL_llll); // slotinfo
	
	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_commands))
		llll_appendllll(out_llll, get_commands_values_as_llll((t_notation_obj *) x, false), 0, WHITENULL_llll); // command

	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_markers)) {
		if (x->r_ob.firstmarker && x->firstvoice) { // markers 
			t_measure *m1 = nth_measure_of_scorevoice(x->firstvoice, start_meas);
			t_measure *m2 = nth_measure_of_scorevoice(x->firstvoice, end_meas);
            if (m1 && m2) {
                double start_ms = (m1->firstchord) ? chord_get_onset_ms(m1->firstchord) : unscaled_xposition_to_ms((t_notation_obj *)x, m1->tuttipoint_reference->offset_ux + m1->start_barline_offset_ux, 1);
                double end_ms = (m2->next && m2->next->firstchord) ? chord_get_onset_ms(m2->next->firstchord) : unscaled_xposition_to_ms((t_notation_obj *)x, m2->tuttipoint_reference->offset_ux + m2->start_barline_offset_ux + m2->width_ux, 1);
                llll_appendllll(out_llll, get_markers_as_llll((t_notation_obj *) x, 1, start_ms, end_ms, false, k_CONSIDER_FOR_SUBDUMPING, start_meas), 0, WHITENULL_llll);
            }
		}
	}

	llll_appendsym(midichannels, _llllobj_sym_midichannels, 0, WHITENULL_llll);
	llll_appendsym(clefs, _llllobj_sym_clefs, 0, WHITENULL_llll);
	llll_appendsym(keys, _llllobj_sym_keys, 0, WHITENULL_llll);
	llll_appendsym(voicenames, _llllobj_sym_voicenames, 0, WHITENULL_llll);
	llll_appendsym(stafflines, _llllobj_sym_stafflines, 0, WHITENULL_llll);

	voice = x->firstvoice;
	while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
		// check if we have to take the voice
		we_take_it[CLAMP(voice->v_ob.number, 0, CONST_MAX_VOICES - 1)] = 0;
		if (whichvoices->l_size == 0)
			we_take_it[CLAMP(voice->v_ob.number, 0, CONST_MAX_VOICES - 1)] = 1;
		else {
			t_llllelem* elem;
			for (elem = whichvoices->l_head; elem; elem = elem->l_next) {
				if (hatom_gettype(&elem->l_hatom) == H_LONG) {
					if (hatom_getlong(&elem->l_hatom) == voice->v_ob.number + 1) {
						we_take_it[CLAMP(voice->v_ob.number, 0, CONST_MAX_VOICES - 1)] = 1;
						break;
					}
				}
			}
		}
		
		if (we_take_it[voice->v_ob.number]) {
			llll_appendlong(midichannels, voice->v_ob.midichannel, 0, WHITENULL_llll);
			llll_appendsym(clefs, x->r_ob.clefs_as_symlist[voice->v_ob.number], 0, WHITENULL_llll);
			llll_appendsym(keys, x->r_ob.keys_as_symlist[voice->v_ob.number], 0, WHITENULL_llll);
			llll_append_notation_item_name(voicenames, (t_notation_item *)voice);
//			llll_appendsym(voicenames, voice->v_ob.r_it.name, 0, WHITENULL_llll);
			llll_append(stafflines, get_voice_stafflines_as_llllelem((t_notation_obj *)x, (t_voice *)voice), WHITENULL_llll);
		}
		voice = voice->next;
	}

	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_clefs))
		llll_appendllll(out_llll, clefs, 0, WHITENULL_llll); // clefs
	else
		llll_free(clefs);

	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_keys)) 
		llll_appendllll(out_llll, keys, 0, WHITENULL_llll); // keys
	else
		llll_free(keys);
	
	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_voicenames)) 
		llll_appendllll(out_llll, voicenames, 0, WHITENULL_llll); // voicenames
	else
		llll_free(voicenames);

	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_midichannels))
		llll_appendllll(out_llll, midichannels, 0, WHITENULL_llll); // midichannels
	else 
		llll_free(midichannels);

	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_stafflines))
		llll_appendllll(out_llll, stafflines, 0, WHITENULL_llll); // midichannels
	else 
		llll_free(stafflines);
    
    if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_articulationinfo))
        llll_appendllll(out_llll, get_articulationinfo_as_llll((t_notation_obj *)x));
    
    if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_noteheadinfo))
        llll_appendllll(out_llll, get_noteheadinfo_as_llll((t_notation_obj *)x));

	 
	if (what_to_dump_is_empty || is_symbol_in_llll_first_level(what_to_dump, _llllobj_sym_body)) {
		voice = x->firstvoice;
		while (voice && (voice->v_ob.number < x->r_ob.num_voices)) { // append chord lllls
			if (we_take_it[voice->v_ob.number])
				llll_appendllll(out_llll, get_subvoice_values_as_llll(x, voice, start_meas, end_meas, true, x->r_ob.output_and_save_level_types), 0, WHITENULL_llll);
			
			voice = voice->next;
		}
	} 
	
	unlock_general_mutex((t_notation_obj *)x);	
	
	return out_llll;
}



t_llll* get_score_values_for_xml_as_llll(t_score *x){
// get all the information concerning the score in order to perform the xml import/export
	t_llll* out_llll = llll_get();
	t_scorevoice *voice;
	lock_general_mutex((t_notation_obj *)x);	
	long tuplet_count = 1;
	t_llll *last_tuplet_level = NULL;
	
	voice = x->firstvoice;
	while (voice && (voice->v_ob.number < x->r_ob.num_voices)) { // append chord lllls

		t_llll* voice_llll = llll_get();

		t_llll* voice_info = llll_get();
		t_measure *temp_meas;
		llll_appendlong(voice_info, voice->v_ob.clef, 0, WHITENULL_llll);	
		llll_appendlong(voice_info, voice->v_ob.key, 0, WHITENULL_llll);	
		llll_appendlong(voice_info, voice->v_ob.mode, 0, WHITENULL_llll);
		llll_append_notation_item_name(voice_info, (t_notation_item *)voice);
//		llll_appendsym(voice_info, voice->v_ob.r_it.name, 0, WHITENULL_llll);	
		llll_appendllll(voice_llll, voice_info, 0, WHITENULL_llll);	

		temp_meas = voice->firstmeasure;
		while (temp_meas) { // append chord lllls

			t_llll* meas_llll = llll_get();
			t_chord *temp_chord = temp_meas->firstchord;
			
			// measure info: ts and tempi
			t_llll* ts_tempo_llll = llll_get();
			llll_appendllll(ts_tempo_llll, get_timesignature_as_llll(&temp_meas->timesignature), 0, WHITENULL_llll);
			llll_appendllll(ts_tempo_llll, measure_get_tempi_as_llll(temp_meas), 0, WHITENULL_llll);
			llll_appendlong(ts_tempo_llll, temp_meas->end_barline->barline_type, 0, WHITENULL_llll);
			llll_appendllll(meas_llll, ts_tempo_llll, 0, WHITENULL_llll);	
			
			while (temp_chord) { // append chord lllls
				t_llll* chord_llll = llll_get(); 
				char beaming2next = false;
				t_llll *tuplet_llll = llll_get();
				t_note *temp_note;

				llll_appendrat(chord_llll, temp_chord->r_sym_duration, 0, WHITENULL_llll); // rational_duration
				if (temp_chord->next && temp_chord->next->rhythmic_tree_elem && temp_chord->rhythmic_tree_elem &&
						temp_chord->next->rhythmic_tree_elem->l_parent && temp_chord->next->rhythmic_tree_elem->l_parent == temp_chord->rhythmic_tree_elem->l_parent && 
						temp_chord->next->rhythmic_tree_elem->l_parent->l_thing.w_obj && ((t_rhythm_level_properties *)temp_chord->next->rhythmic_tree_elem->l_parent->l_thing.w_obj)->beam_number > 0)
					beaming2next = true;

                llll_appendlong(chord_llll, beaming2next, 0, WHITENULL_llll);
                
                if (rat_rat_cmp(temp_chord->overall_tuplet_ratio, long2rat(1)) != 0) {
                    t_llll *tuplet_level = get_innermost_tuplet_level_for_chord(temp_chord);
                    t_rhythm_level_properties *prop = (t_rhythm_level_properties *)tuplet_level->l_thing.w_obj;
                    
                    char position_in_tuplet = 0; // 0 = tuplet_continues, 1 = tuplet_begins_with_this_chord, 2 = tuplet_ends_with_this_chord
                    if (get_level_start_chord(tuplet_level, true) == temp_chord)
                        position_in_tuplet = 1;
                    else if (get_level_end_chord(tuplet_level, true) == temp_chord)
                        position_in_tuplet = 2;
                    
                    llll_appendlong(tuplet_llll, prop->tupletinfo.tuplet_local_ratio.r_den * prop->tupletinfo.tuplet_multiplier.r_num, 0, WHITENULL_llll);
                    llll_appendlong(tuplet_llll, prop->tupletinfo.tuplet_local_ratio.r_num * prop->tupletinfo.tuplet_multiplier.r_num, 0, WHITENULL_llll);
                    llll_appendrat(tuplet_llll, prop->tupletinfo.tuplet_graphical_unit, 0, WHITENULL_llll);
                    llll_appendlong(tuplet_llll, position_in_tuplet, 0, WHITENULL_llll);
                    llll_appendlong(tuplet_llll, true, 0, WHITENULL_llll);
                    llll_appendlong(tuplet_llll, tuplet_count, 0, WHITENULL_llll);
                    if (tuplet_level != last_tuplet_level)
                        tuplet_count ++;
                }
				llll_appendllll(chord_llll, tuplet_llll, 0, WHITENULL_llll);					
				
				temp_note = temp_chord->firstnote;
				while (temp_note) { // append notes lllls
				
					t_llll* note_llll = llll_get();
					
					llll_appendlong(note_llll, note_get_screen_midicents(temp_note), 0, WHITENULL_llll); // step midicents
					llll_appendrat(note_llll, note_get_screen_accidental(temp_note), 0, WHITENULL_llll); // screen accidental
					llll_appendlong(note_llll, temp_note->show_accidental, 0, WHITENULL_llll); // is accidental shown?
					llll_appendlong(note_llll, temp_note->velocity, 0, WHITENULL_llll); // velocity
					llll_appendlong(note_llll, (temp_note->tie_to) ? 1 : 0, 0, WHITENULL_llll); // start_tie?					
					
					llll_appendllll(chord_llll, note_llll, 0, WHITENULL_llll);	
					temp_note = temp_note->next;
				}
				
				llll_appendllll(meas_llll, chord_llll, 0, WHITENULL_llll);
				temp_chord = temp_chord->next;
			}

			llll_appendllll(voice_llll, meas_llll, 0, WHITENULL_llll);	
			temp_meas = temp_meas->next;

		}
		
		llll_appendllll(out_llll, voice_llll, 0, WHITENULL_llll);	
		voice = voice->next;
	}
	unlock_general_mutex((t_notation_obj *)x);	

	return out_llll;
}


/// *** INSCREEN DOMAIN AND PLAYING *** ///

void send_domain(t_score *x, long outlet, t_symbol *label) {
	t_llll *outlist = scoreapi_getdomain(x, label);
	llllobj_outlet_llll((t_object *) x, LLLL_OBJ_UI, outlet, outlist);
	llll_free(outlist);
}

t_llll* get_score_values_as_llll_for_pwgl(t_score *x){
// get all the information concerning the score in order to perform the xml import/export
	t_llll* out_llll = llll_get();
	t_scorevoice *voice;
	t_measure *temp_measure;
	lock_general_mutex((t_notation_obj *)x);	
	
	x->r_ob.private_count = 1; // needed for slots: for PWGL each slot is a different instance of an objects, we'll keep the count with this
	
	voice = x->firstvoice;
	while (voice && (voice->v_ob.number < x->r_ob.num_voices)) { // append chord lllls

		t_llll *part_llll = llll_get();
		t_llll *voice_llll = llll_get();
		
		llll_appendsym(part_llll, gensym(":staff"), 0, WHITENULL_llll);
		switch (voice->v_ob.clef) {
			case k_CLEF_F: case k_CLEF_FF:
				llll_appendsym(part_llll, gensym(":bass-staff"), 0, WHITENULL_llll);
				break;
			case k_CLEF_FG: case k_CLEF_FFG: case k_CLEF_FGG:
				llll_appendsym(part_llll, gensym(":piano-staff"), 0, WHITENULL_llll);
				break;
			default:
				llll_appendsym(part_llll, gensym(":treble-staff"), 0, WHITENULL_llll);
		}

		char need_set_double = false;
		for (temp_measure = voice->firstmeasure; temp_measure; temp_measure = temp_measure->next) { // append measure lllls
			t_llll *meas_llll = bach_measure_to_pwgl_measure((t_notation_obj *) x, temp_measure, &need_set_double);
			llll_appendllll(voice_llll, meas_llll, 0, WHITENULL_llll);
		}

		llll_appendllll(part_llll, voice_llll, 0, WHITENULL_llll);	
		llll_appendllll(out_llll, part_llll, 0, WHITENULL_llll);	
		voice = voice->next;
	}
	
	llll_wrap_once(&out_llll);
	unlock_general_mutex((t_notation_obj *)x);	

	return out_llll;
}




void score_mouseup(t_score *x, t_object *patcherview, t_pt pt, long modifiers)
{
	char there_are_free_undo_ticks = are_there_free_undo_ticks((t_notation_obj *)x, false);
	x->r_ob.j_mouse_is_down = false;
	x->r_ob.j_isdragging = false;
	
	lock_general_mutex((t_notation_obj *)x);	
	handle_mouseup_in_bach_inspector((t_notation_obj *) x, &x->r_ob.m_inspector, patcherview, pt);
	handle_slot_mouseup((t_notation_obj *)x, patcherview, pt, modifiers);
	unlock_general_mutex((t_notation_obj *)x);	

	verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before"), 1);

	if (x->r_ob.changed_while_dragging && x->r_ob.active_slot_num < 0) {
		if (x->r_ob.snap_pitch_to_grid_when_editing)
			snap_pitch_to_grid_for_selection((t_notation_obj *)x);
	}

	
	if (there_are_free_undo_ticks)
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_MOUSEDRAG_CHANGE);
		
	bach_set_cursor((t_object *)x, &x->r_ob.j_mouse_cursor, patcherview, BACH_CURSOR_DEFAULT);
	
	if (!x->r_ob.j_mouse_hasbeendragged) { // mouse hasn't been dragged
        
        
        if (x->r_ob.check_selection_restraint_for_item) {
            // Restraining selection?
            if (x->r_ob.check_selection_restraint_for_item->type == k_CHORD) {
                if (are_all_selecteditems_tied_to_chord((t_notation_obj *) x, (t_chord *)x->r_ob.check_selection_restraint_for_item)) {
                    clear_selection((t_notation_obj *) x);
                    notation_item_add_to_selection((t_notation_obj *) x, x->r_ob.check_selection_restraint_for_item);
                }
            } else if (x->r_ob.check_selection_restraint_for_item->type == k_NOTE) {
                if (are_all_selecteditems_tied_to_note((t_notation_obj *) x, (t_note *)x->r_ob.check_selection_restraint_for_item)) {
                    clear_selection((t_notation_obj *) x);
                    notation_item_add_to_selection((t_notation_obj *) x, x->r_ob.check_selection_restraint_for_item);
                } else if (are_all_selecteditems_tied_to_chord((t_notation_obj *) x, ((t_note *)x->r_ob.check_selection_restraint_for_item)->parent)) {
                    clear_selection((t_notation_obj *) x);
                    notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)((t_note *)x->r_ob.check_selection_restraint_for_item)->parent);
                } else if (x->r_ob.firstselecteditem == (t_notation_item *)((t_note *)x->r_ob.check_selection_restraint_for_item)->parent && x->r_ob.firstselecteditem == x->r_ob.lastselecteditem) {
                    clear_selection((t_notation_obj *) x);
                    notation_item_add_to_selection((t_notation_obj *) x, x->r_ob.check_selection_restraint_for_item);
                }
            }
        }
        
		if (!x->r_ob.j_clicked_obj_has_been_selected && modifiers & eShiftKey) {
			lock_general_mutex((t_notation_obj *)x);	
			if ((x->r_ob.j_mousedown_ptr) && ((x->r_ob.j_mousedown_obj_type == k_NOTE) || (x->r_ob.j_mousedown_obj_type == k_CHORD))) {
				if (notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)x->r_ob.j_mousedown_ptr)) {
					notation_item_delete_from_selection((t_notation_obj *)x, (t_notation_item *)x->r_ob.j_mousedown_ptr);
				} else if (x->r_ob.j_mousedown_obj_type == k_NOTE && notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)((t_note *)x->r_ob.j_mousedown_ptr)->parent)) {
					// particular case of a chord selected and we click on a note, just to deselect the note. but to accomplish this we have to:
					// 1) deselect the chord; 2) select all the notes but this one
					t_chord *this_chord = ((t_note *)x->r_ob.j_mousedown_ptr)->parent;
					t_note *this_note;
					notation_item_delete_from_selection((t_notation_obj *)x, (t_notation_item *)this_chord); // delete the chord from the selection
					for (this_note = this_chord->firstnote; this_note; this_note = this_note->next)
						if (this_note != x->r_ob.j_mousedown_ptr)
							notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)this_note);
				}
			} else if ((x->r_ob.j_mousedown_ptr) && 
					   (x->r_ob.j_mousedown_obj_type == k_DURATION_TAIL || x->r_ob.j_mousedown_obj_type == k_PITCH_BREAKPOINT || x->r_ob.j_mousedown_obj_type == k_MEASURE )) {
				if (notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)x->r_ob.j_mousedown_ptr)) {
					notation_item_delete_from_selection((t_notation_obj *)x, (t_notation_item *)x->r_ob.j_mousedown_ptr);
				}
			}
			unlock_general_mutex((t_notation_obj *)x);	
		}
	}

	if (x->r_ob.j_selection_hasbeendragged == 1) {
		// recalculating measure beamings, chord parameters, and spacing
		t_notation_item *curr_it = x->r_ob.firstselecteditem;
		lock_general_mutex((t_notation_obj *)x);
		while (curr_it) { // cycle on the selected items
			// things of which we'll have to calculate parameters:
			t_chord *tempch = NULL; t_measure *tempmeas = NULL;
            char is_in_voiceensemble = (voiceensemble_get_numparts((t_notation_obj *)x, notation_item_get_voice((t_notation_obj *)x, curr_it)) > 1);
			if (curr_it->type == k_NOTE) { // it is a note
				tempch = ((t_note *)curr_it)->parent;
				tempch->parent->need_recompute_beams_positions = true;
                if (is_in_voiceensemble) // rests floating position might change!
                    recompute_all_for_measures_in_voiceensemble((t_notation_obj *)x, tempch->parent, false);
                else
                    recalculate_all_measure_chord_parameters((t_notation_obj *)x, tempch->parent);
				check_ties_around_measure(tempch->parent);
				tempch->parent->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE;
				set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
			} else if (curr_it->type == k_CHORD) {
				tempch = ((t_chord *)curr_it);
				tempch->parent->need_recompute_beams_positions = true;
				check_ties_around_measure(tempch->parent);
                if (is_in_voiceensemble) // rests floating position might change!
                    recompute_all_for_measures_in_voiceensemble((t_notation_obj *)x, tempch->parent, false);
                else
                    recalculate_all_measure_chord_parameters((t_notation_obj *)x, tempch->parent);
				tempch->parent->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE;
				set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
			} else if (curr_it->type == k_MEASURE) {
				tempmeas = ((t_measure *)curr_it);
				tempmeas->need_recompute_beams_positions = true;
				check_ties_around_measure(tempmeas);
                if (is_in_voiceensemble) // rests floating position might change!
                    recompute_all_for_measures_in_voiceensemble((t_notation_obj *)x, tempmeas, false);
                else
                    recalculate_all_measure_chord_parameters((t_notation_obj *)x, tempmeas);
				tempmeas->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE;
				set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
			}
			curr_it = curr_it->next_selected;
		}
		unlock_general_mutex((t_notation_obj *)x);	
	}
	
	if (x->r_ob.j_mousedown_ptr && (x->r_ob.j_mousedown_obj_type == k_SLUR || x->r_ob.j_mousedown_obj_type == k_SLUR_END_POINT || x->r_ob.j_mousedown_obj_type == k_SLUR_START_POINT)) {
		t_slur *slur = ((t_slur *)x->r_ob.j_mousedown_ptr);
		slur->need_recompute_position = true;
	}

	if (!there_are_free_undo_ticks && !x->r_ob.item_changed_at_mousedown && x->r_ob.j_mousedown_obj_type == k_ZOOMING_REGION && x->r_ob.j_mousedown_point.x != pt.x) { 
		// new zoom
		double start_x, end_x;
		double start_ux, end_ux;
		double old_zoom, new_zoom;
		if (pt.x > x->r_ob.j_mousedown_point.x) {
			start_x = x->r_ob.j_mousedown_point.x;
			end_x = pt.x;
		} else {
			start_x = pt.x;
			end_x = x->r_ob.j_mousedown_point.x;
		}
		if (end_x > start_x + 35 * x->r_ob.zoom_y) {
			start_ux = xposition_to_unscaled_xposition((t_notation_obj *) x, start_x);
			end_ux = xposition_to_unscaled_xposition((t_notation_obj *) x, end_x);
			
			if (end_ux > start_ux) {
				getdomain((t_notation_obj *) x);
				old_zoom = x->r_ob.horizontal_zoom;
				if (modifiers & eShiftKey)
					new_zoom = old_zoom / (((double) x->r_ob.domain_ux)/(end_ux-start_ux));
				else
					new_zoom = old_zoom * (((double) x->r_ob.domain_ux)/(end_ux-start_ux));
				change_zoom((t_notation_obj *) x, new_zoom);
				x->r_ob.screen_ux_start = start_ux;
				update_hscrollbar((t_notation_obj *)x, 2);
				send_domain(x, 7, NULL);
				invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			}
		}
	}
	
	if (!there_are_free_undo_ticks && !x->r_ob.item_changed_at_mousedown && (x->r_ob.j_mousedown_obj_type == k_LOOP_REGION) && !(modifiers & eShiftKey)) { 
		double start_ms = x->r_ob.loop_region.start.position_ms;
		double end_ms = x->r_ob.loop_region.end.position_ms;
		t_timepoint start_tp = ms_to_timepoint_autochoose_voice((t_notation_obj *)x, start_ms, k_MS_TO_TP_RETURN_NEAREST, NULL);
		t_timepoint end_tp = ms_to_timepoint_autochoose_voice((t_notation_obj *)x, end_ms, k_MS_TO_TP_RETURN_NEAREST, NULL);

		set_loop_region_from_extremes_tp(x, start_tp, end_tp);
		send_loop_region((t_notation_obj *) x, 7);
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
	}
	
	if (!there_are_free_undo_ticks && !x->r_ob.item_changed_at_mousedown && x->r_ob.j_mousedown_obj_type == k_REGION && (x->r_ob.j_mousedown_point.x != pt.x || x->r_ob.j_mousedown_point.y != pt.y)) { 
		if (modifiers & eCommandKey){
			// set dilation rectangle
			x->r_ob.dilation_rectangle.left_ux = xposition_to_unscaled_xposition((t_notation_obj *) x, x->r_ob.j_mousedown_point.x);
			x->r_ob.dilation_rectangle.right_ux = xposition_to_unscaled_xposition((t_notation_obj *) x, pt.x);
            
            
            if (x->r_ob.dilation_rectangle.left_ux > x->r_ob.dilation_rectangle.right_ux)
                swap_doubles(&x->r_ob.dilation_rectangle.left_ux, &x->r_ob.dilation_rectangle.right_ux);
            
            if (x->r_ob.j_mousedown_point.y < pt.y) {
                x->r_ob.dilation_rectangle.top_mc = yposition_to_mc((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, NULL, NULL);
                x->r_ob.dilation_rectangle.bottom_mc = yposition_to_mc((t_notation_obj *)x, pt.y, NULL, NULL);
                
                x->r_ob.dilation_rectangle.top_voice = (t_voice *)nth_scorevoice(x, yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_FIRST));
                x->r_ob.dilation_rectangle.bottom_voice = (t_voice *)nth_scorevoice(x, yposition_to_voicenumber((t_notation_obj *)x, pt.y, -1, k_VOICEENSEMBLE_INTERFACE_LAST));
            } else {
                x->r_ob.dilation_rectangle.bottom_mc = yposition_to_mc((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, NULL, NULL);
                x->r_ob.dilation_rectangle.top_mc = yposition_to_mc((t_notation_obj *)x, pt.y, NULL, NULL);
                
                x->r_ob.dilation_rectangle.top_voice = (t_voice *)nth_scorevoice(x, yposition_to_voicenumber((t_notation_obj *)x,  pt.y, -1, k_VOICEENSEMBLE_INTERFACE_FIRST));
                x->r_ob.dilation_rectangle.bottom_voice = (t_voice *)nth_scorevoice(x, yposition_to_voicenumber((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, -1, k_VOICEENSEMBLE_INTERFACE_LAST));

            }
            
			
/*			if (x->r_ob.dilation_rectangle.left_ux > x->r_ob.dilation_rectangle.right_ux) {
				long temp = x->r_ob.dilation_rectangle.right_ux;
				x->r_ob.dilation_rectangle.right_ux = x->r_ob.dilation_rectangle.left_ux;
				x->r_ob.dilation_rectangle.left_ux = temp;
			}
			if (x->r_ob.dilation_rectangle.top_voice->number > x->r_ob.dilation_rectangle.bottom_voice->number ||
				(x->r_ob.dilation_rectangle.top_voice->number == x->r_ob.dilation_rectangle.bottom_voice->number &&
				 x->r_ob.dilation_rectangle.top_mc < x->r_ob.dilation_rectangle.bottom_mc)) {
					t_voice *tempv = x->r_ob.dilation_rectangle.top_voice;
					double temp = x->r_ob.dilation_rectangle.top_mc;
					x->r_ob.dilation_rectangle.top_voice = x->r_ob.dilation_rectangle.bottom_voice;
					x->r_ob.dilation_rectangle.bottom_voice = tempv;
					x->r_ob.dilation_rectangle.top_mc = x->r_ob.dilation_rectangle.bottom_mc;
					x->r_ob.dilation_rectangle.bottom_mc = temp;
				} */
            
			x->r_ob.show_dilation_rectangle = true;
		} else {
			// accomplish a region selection
			x->r_ob.j_selected_region_ms1 = xposition_to_onset((t_notation_obj *) x, x->r_ob.j_mousedown_point.x, -1);
			x->r_ob.j_selected_region_mc1 = yposition_to_mc((t_notation_obj *)x, x->r_ob.j_mousedown_point.y, NULL, NULL);
			x->r_ob.j_selected_region_ms2 = xposition_to_onset((t_notation_obj *) x, pt.x, -1);
			x->r_ob.j_selected_region_mc2 = yposition_to_mc((t_notation_obj *)x, pt.y, NULL, NULL);
			if (x->r_ob.j_selected_region_ms1 > x->r_ob.j_selected_region_ms2) {
				long temp = x->r_ob.j_selected_region_ms1;
				x->r_ob.j_selected_region_ms1 = x->r_ob.j_selected_region_ms2;
				x->r_ob.j_selected_region_ms2 = temp;
			}
			if (x->r_ob.j_selected_region_mc1 > x->r_ob.j_selected_region_mc2) {
				long temp = x->r_ob.j_selected_region_mc1;
				x->r_ob.j_selected_region_mc1 = x->r_ob.j_selected_region_mc2;
				x->r_ob.j_selected_region_mc2 = temp;
			}
		}
	}

	// move the preselection to the overall selection
	lock_general_mutex((t_notation_obj *)x);	
	move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_INVOLUTIVE, !(modifiers & eControlKey), x->r_ob.edit_tied_notes_together);
	unlock_general_mutex((t_notation_obj *)x);	

	handle_change_selection((t_notation_obj *)x);
	
	x->r_ob.j_mousedrag_copy_ptr = NULL;
    set_mousedown((t_notation_obj *)x, NULL, k_NONE, false);
	x->r_ob.j_dragging_direction = 0;
	
	// reset the dragged flags
	x->r_ob.j_mouse_hasbeendragged = 0;
	x->r_ob.j_selection_hasbeendragged = 0;
	x->r_ob.item_changed_at_mousedown = 0;
	
    notationobj_redraw((t_notation_obj *) x);
}

void score_mousewheel(t_score *x, t_object *view, t_pt pt, long modifiers, double x_inc, double y_inc){
//	post("%f, %f, mod: %d, x_inc: %f, y_inc: %f", pt.x, pt.y, modifiers, x_inc, y_inc);
	char res = 0;
	
	llll_format_modifiers(&modifiers, NULL);

	lock_general_mutex((t_notation_obj *)x);	
	res = handle_slot_mousewheel((t_notation_obj *) x, view, pt, modifiers, x_inc, y_inc);
	unlock_general_mutex((t_notation_obj *)x);	

	if (res)
		return;

	if (modifiers == eCommandKey || modifiers == eShiftKey + eCommandKey) { // change zoom
		double old_zoom = x->r_ob.horizontal_zoom;
		double new_zoom = old_zoom;
		double pt_ux = xposition_to_unscaled_xposition((t_notation_obj *) x, pt.x);
		
		if (!is_editable((t_notation_obj *)x, k_ZOOMING_REGION, k_ELEMENT_ACTIONS_NONE)) return;

		new_zoom -= y_inc * CONST_Y_MOUSEWHEEL_FACTOR * (modifiers & eShiftKey ? CONST_FINER_FROM_KEYBOARD : 1.);
		if (new_zoom < 1) new_zoom = 1;
		change_zoom((t_notation_obj *) x, new_zoom);
		
		// we preserve pt.x position upon zoom
		x->r_ob.screen_ux_start = pt_ux - (pt_ux - x->r_ob.screen_ux_start) * (old_zoom / x->r_ob.horizontal_zoom);
		if (x->r_ob.screen_ux_start < 0) x->r_ob.screen_ux_start = 0.;
		update_hscrollbar((t_notation_obj *)x, 2);

		send_domain(x, 7, NULL);
		invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);

	} else { // simple scroll 
	
		if (!is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) return;

		if (x->r_ob.show_hscrollbar) {
			double delta_x = -25 * x_inc * CONST_X_MOUSEWHEEL_FACTOR;
			if (modifiers & eShiftKey && modifiers & eCommandKey) 
				delta_x *= CONST_FINER_FROM_KEYBOARD;
			x->r_ob.screen_ux_start = MAX(0, x->r_ob.screen_ux_start + delta_x / x->r_ob.zoom_x);
			redraw_hscrollbar((t_notation_obj *)x, 2);
			send_domain(x, 7, NULL);
			invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
		}
		if (x->r_ob.show_vscrollbar) {
			double delta_y = y_inc * CONST_Y_MOUSEWHEEL_FACTOR;
			if (modifiers & eShiftKey && modifiers & eCommandKey) 
				delta_y *= CONST_FINER_FROM_KEYBOARD;
			x->r_ob.vscrollbar_y -= delta_y;
			redraw_vscrollbar((t_notation_obj *)x, 0);
			//send_vdomain(x, 7, NULL);
			invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
		}
	}
}

void preselect_measure_in_all_voices(t_score *x, t_measure *ref_measure, char except_ref_measure)
{
	t_measure_end_barline *barline[CONST_MAX_VOICES];
	if (ref_measure->prev && get_all_tuttipoint_barlines((t_notation_obj *)x, ref_measure->prev->end_barline, barline)) {
		long i;
		for (i = 0; i < x->r_ob.num_voices; i++) {
			if (barline[i]->owner && barline[i]->owner->next && (!except_ref_measure || barline[i]->owner->next != ref_measure))
				notation_item_add_to_preselection((t_notation_obj *)x, (t_notation_item *)barline[i]->owner->next);
		}
	} else {
		t_scorevoice *voice;
		long n = ref_measure->measure_number;
		if (is_editable((t_notation_obj *)x, k_SELECTION, k_ELEMENT_ACTIONS_NONE)) {
			for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
				t_measure *thismeas = nth_measure_of_scorevoice(voice, n);
				if (thismeas && !notation_item_is_selected((t_notation_obj *) x, (t_notation_item *)thismeas)
					&& (!except_ref_measure || thismeas != ref_measure)) {
					notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)thismeas);
				}
			}
		}
	}
}

void score_mousedoubleclick(t_score *x, t_object *patcherview, t_pt pt, long modifiers) {
	// first of all: are we in a slot mode???? Cause if we are in a slot mode, we gotta handle that separately
	char changed;
	char clicked_slot;
	t_scorevoice *voice;

	if (x->r_ob.active_slot_num > -1 && !is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) return;

	lock_general_mutex((t_notation_obj *)x);	
	clicked_slot = handle_slot_mousedoubleclick((t_notation_obj *) x, patcherview, pt, modifiers, &changed);

	if (clicked_slot) {
		unlock_general_mutex((t_notation_obj *)x);	
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_SLOT);
		return;

	}
	for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
		if (is_in_clef_shape((t_notation_obj *)x, pt.x, pt.y, (t_voice *)voice)) {
			clear_preselection((t_notation_obj *)x);
			preselect_elements_in_region_for_mouse_selection(x, 0, x->r_ob.length_ms, -1000, 16000, voice->v_ob.number, voice->v_ob.number);
			move_preselecteditems_to_selection((t_notation_obj *)x, k_SELECTION_MODE_FORCE_SELECT, false, false);
		}
	}

	if (x->r_ob.dblclick_sends_values) {
		unlock_general_mutex((t_notation_obj *)x);	
		evaluate_selection(x, 0, true);
		return;
	} else if (x->r_ob.j_last_mousedown_obj_type == k_MEASURE) {
		preselect_measure_in_all_voices(x, (t_measure *) x->r_ob.j_last_mousedown_ptr, true);
		move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_INVOLUTIVE, false, false);
	} else {
		// voice names?
		t_scorevoice *voice;
		for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
			if (is_in_voicename_shape((t_notation_obj *) x, pt.x, pt.y, (t_voice *) voice)){
				unlock_general_mutex((t_notation_obj *)x);	
				if (is_editable((t_notation_obj *)x, k_VOICE, k_MODIFICATION_NAME))
					start_editing_voicename((t_notation_obj *) x, patcherview, (t_voice *) voice);
				return;
			}
		}
		// markers?
		if (x->r_ob.firstmarker) {
			t_marker *marker;
			for (marker = x->r_ob.firstmarker; marker; marker = marker->next) {
				if (is_in_markername_shape(x, pt.x, pt.y, marker) || is_in_marker_shape((t_notation_obj *)x, marker, pt.x, pt.y)){
					unlock_general_mutex((t_notation_obj *)x);	
					if (is_editable((t_notation_obj *)x, k_MARKER, k_MODIFICATION_NAME))
						start_editing_markername((t_notation_obj *) x, patcherview, marker, ms_to_xposition((t_notation_obj *)x, marker->position_ms, 1) + 3 * x->r_ob.zoom_y);
					return;
				}
			}
		}
        
		// lyrics?
		if (x->r_ob.show_lyrics && x->r_ob.link_lyrics_to_slot > 0) {
			t_scorevoice *voice;
			t_measure *meas;
			t_chord *chord;
			for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
				for (meas = voice->firstmeasure; meas; meas = meas->next) {
					for (chord = meas->firstchord; chord; chord = chord->next){
						if (chord->lyrics && is_in_chord_lyrics_shape((t_notation_obj *) x, chord, pt.x, pt.y)) {
							unlock_general_mutex((t_notation_obj *)x);	
							if (is_editable((t_notation_obj *)x, k_LYRICS, k_MODIFICATION_GENERIC))
								start_editing_lyrics((t_notation_obj *) x, patcherview, chord);
							return;
						}
					}
				}
			}
		}

        // dynamics?
        if (x->r_ob.show_dynamics && x->r_ob.link_dynamics_to_slot > 0) {
            t_scorevoice *voice;
            t_measure *meas;
            t_chord *chord;
            for (voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next){
                for (meas = voice->firstmeasure; meas; meas = meas->next) {
                    for (chord = meas->firstchord; chord; chord = chord->next){
                        if (chord->dynamics && chord->dynamics->text && is_in_chord_dynamics_shape((t_notation_obj *) x, chord, pt.x, pt.y)) {
                            unlock_general_mutex((t_notation_obj *)x);
                            if (is_editable((t_notation_obj *)x, k_LYRICS, k_MODIFICATION_GENERIC))
                                start_editing_dynamics((t_notation_obj *) x, patcherview, chord);
                            return;
                        }
                    }
                }
            }
        }
    
    }
	unlock_general_mutex((t_notation_obj *)x);	
}




long score_keyfilter(t_score *x, t_object *patcherview, long *keycode, long *modifiers, long *textcharacter)
{
	t_atom arv;
	long rv = 1;
	long k = *keycode;	
	
	if (k == JKEY_TAB) {
		if (x->r_ob.is_editing_type == k_LYRICS && x->r_ob.is_editing_chord && x->r_ob.is_editing_chord->firstnote) {
			char *text	= NULL;
			long size = 0;
			t_chord *prev_chord = get_prev_nonrest_chord(x->r_ob.is_editing_chord);
			t_chord *next_chord = get_next_nonrest_chord(x->r_ob.is_editing_chord);
			t_object *textfield = jbox_get_textfield((t_object *)x);
			object_method(textfield, gensym("gettextptr"), &text, &size);
			create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)x->r_ob.is_editing_chord, k_UNDO_MODIFICATION_CHANGE);
			set_textfield_info_to_lyrics_slot((t_notation_obj *) x, text);
			end_editing_textfield((t_notation_obj *) x);
			if (!((*modifiers) & eShiftKey) && next_chord)
				start_editing_lyrics((t_notation_obj *) x, patcherview, next_chord);
			else if (((*modifiers) & eShiftKey) && prev_chord)
				start_editing_lyrics((t_notation_obj *) x, patcherview, prev_chord);
			else
				object_method_typed(patcherview, gensym("endeditbox"), 0, NULL, &arv); 
			rv = 0;
        } else if (x->r_ob.is_editing_type == k_DYNAMICS && x->r_ob.is_editing_chord && x->r_ob.is_editing_chord->firstnote) {
            char *text	= NULL;
            long size = 0;
            t_chord *prev_chord = get_prev_nonrest_chord(x->r_ob.is_editing_chord);
            t_chord *next_chord = get_next_nonrest_chord(x->r_ob.is_editing_chord);
            t_object *textfield = jbox_get_textfield((t_object *)x);
            object_method(textfield, gensym("gettextptr"), &text, &size);
            create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)x->r_ob.is_editing_chord, k_UNDO_MODIFICATION_CHANGE);
            set_textfield_info_to_dynamics_slot((t_notation_obj *) x, text);
            end_editing_textfield((t_notation_obj *) x);
            if (!((*modifiers) & eShiftKey) && next_chord)
                start_editing_dynamics((t_notation_obj *) x, patcherview, next_chord);
            else if (((*modifiers) & eShiftKey) && prev_chord)
                start_editing_dynamics((t_notation_obj *) x, patcherview, prev_chord);
            else
                object_method_typed(patcherview, gensym("endeditbox"), 0, NULL, &arv); 
            rv = 0;
		}
	} else if (k == JKEY_ENTER || k == JKEY_RETURN || k == JKEY_ESC) {
		object_method_typed(patcherview, gensym("endeditbox"), 0, NULL, &arv); 
		rv = 0;		// don't pass those keys to uitextfield
	}
	return rv;
}


void score_enter(t_score *x)	// enter is triggerd at "endeditbox time"
{
	long size	= 0;
	char *text	= NULL;
	t_object *textfield;
	
	textfield = jbox_get_textfield((t_object *)x);
	object_method(textfield, gensym("gettextptr"), &text, &size);

	if (x->r_ob.is_editing_type == k_VOICENAME && x->r_ob.is_editing_voice_name >= 0) {
		long ac = 0;
		t_atom *av = NULL;
		atom_setparse_debug(&ac, &av, text);
		change_single_voicename_from_ac_av((t_notation_obj *) x, (t_voice *) nth_scorevoice(x, x->r_ob.is_editing_voice_name), ac, av, true);
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_VOICE_NAME);
		bach_freeptr(av);
	} else if (x->r_ob.is_editing_type == k_TEXT_IN_SLOT) {
		t_llll *new_text_as_llll = llll_get();
		llll_appendsym(new_text_as_llll, gensym(text), 0, WHITENULL_llll);
		create_simple_notation_item_undo_tick((t_notation_obj *) x, get_activeitem_undo_item((t_notation_obj *) x), k_UNDO_MODIFICATION_CHANGE);
		change_notation_item_slot_value((t_notation_obj *) x, x->r_ob.active_slot_notationitem, x->r_ob.active_slot_num, 1, new_text_as_llll);
		llll_free(new_text_as_llll);
        if (x->r_ob.link_lyrics_to_slot > 0 && x->r_ob.link_lyrics_to_slot - 1 == x->r_ob.active_slot_num) {
            t_chord *ch = notation_item_chord_get_parent((t_notation_obj *) x, x->r_ob.active_slot_notationitem);
            if (ch)
                recompute_all_for_measure((t_notation_obj *) x, ch->parent, false);
        }
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_SLOT);
    } else if (x->r_ob.is_editing_type == k_DYNAMICS_IN_SLOT) {
        t_llll *new_text_as_llll = llll_get();
        llll_appendsym(new_text_as_llll, gensym(text), 0, WHITENULL_llll);
        create_simple_notation_item_undo_tick((t_notation_obj *) x, get_activeitem_undo_item((t_notation_obj *) x), k_UNDO_MODIFICATION_CHANGE);
        change_notation_item_slot_value((t_notation_obj *) x, x->r_ob.active_slot_notationitem, x->r_ob.active_slot_num, 1, new_text_as_llll);
        llll_free(new_text_as_llll);
        if (x->r_ob.link_dynamics_to_slot > 0 && x->r_ob.link_dynamics_to_slot - 1 == x->r_ob.active_slot_num) {
            t_chord *ch = notation_item_chord_get_parent((t_notation_obj *)x, x->r_ob.active_slot_notationitem);
            if (ch)
                ch->need_recompute_parameters = true;
        }
        handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_SLOT);
	} else if (x->r_ob.is_editing_type == k_MARKERNAME && x->r_ob.is_editing_marker) {
		t_llll *names = llll_from_text_buf(text, 0);
		lock_markers_mutex((t_notation_obj *)x);;
		create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
		change_marker_names((t_notation_obj *) x, x->r_ob.is_editing_marker, names);
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_MARKER_NAME);
		unlock_markers_mutex((t_notation_obj *)x);;
		llll_free(names);
	} else if (x->r_ob.is_editing_type == k_LYRICS && x->r_ob.is_editing_chord && x->r_ob.is_editing_chord->firstnote) {
		create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.is_editing_chord, k_UNDO_MODIFICATION_CHANGE);
		set_textfield_info_to_lyrics_slot((t_notation_obj *) x, text);
		handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_LYRICS);
    } else if (x->r_ob.is_editing_type == k_DYNAMICS && x->r_ob.is_editing_chord) {
        create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.is_editing_chord, k_UNDO_MODIFICATION_CHANGE);
        set_textfield_info_to_dynamics_slot((t_notation_obj *) x, text);
        handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_DYNAMICS);
	} else if (x->r_ob.is_editing_type == k_LLLL_IN_SLOT) {
		t_llll *my_llll = llll_from_text_buf(text, false);
		if (my_llll) {
			create_simple_notation_item_undo_tick((t_notation_obj *) x, get_activeitem_undo_item((t_notation_obj *) x), k_UNDO_MODIFICATION_CHANGE);
#ifdef BACH_NEW_LLLLSLOT_SYNTAX
            // nothing to do
#else
			llll_wrap_once(&my_llll);
#endif
			change_notation_item_slot_value((t_notation_obj *) x, x->r_ob.active_slot_notationitem, x->r_ob.active_slot_num, 1, my_llll);
			llll_free(my_llll);
			handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_SLOT);
		}
    } else if (x->r_ob.is_editing_type == k_NUMBER_IN_SLOT) {
        t_llll *ll = llll_from_text_buf(text, false);
        create_simple_notation_item_undo_tick((t_notation_obj *) x, get_activeitem_undo_item((t_notation_obj *) x), k_UNDO_MODIFICATION_CHANGE);
        change_notation_item_slot_value((t_notation_obj *) x, x->r_ob.active_slot_notationitem, x->r_ob.active_slot_num, 1, ll);
        llll_free(ll);
        handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_SLOT);
	}
	end_editing_textfield((t_notation_obj *) x);
}

t_chord *get_operating_chord_for_linear_edit(t_score *x) {
	if (!x->r_ob.notation_cursor.chord && !x->r_ob.notation_cursor.measure->firstchord)
		return (x->r_ob.notation_cursor.measure->prev ? x->r_ob.notation_cursor.measure->prev->lastchord : NULL);
	else if (x->r_ob.notation_cursor.chord)
		return x->r_ob.notation_cursor.chord;
	else 
		return (x->r_ob.notation_cursor.measure ? x->r_ob.notation_cursor.measure->lastchord : NULL);
}

t_chord *tie_untie_notes_on_linear_edit(t_score *x){
	t_chord *chord = get_operating_chord_for_linear_edit(x);

	if (chord) {
		t_note *nt, *cursor_nt = NULL;
		
		create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)chord->parent, k_UNDO_MODIFICATION_CHANGE);
		
		for (nt = chord->firstnote; nt; nt = nt->next) {
			if (note_get_screen_midicents(nt) == x->r_ob.notation_cursor.midicents) {
				cursor_nt = nt;
				break;
			}
		}
		
		for (nt = chord->firstnote; nt; nt = nt->next) {
			if (!cursor_nt || cursor_nt == nt) {
				tie_untie_note(nt);
				validate_accidentals_for_measure((t_notation_obj *) x, nt->parent->parent);
				calculate_chord_parameters((t_notation_obj *) x, nt->parent, get_voice_clef((t_notation_obj *)x, (t_voice *)nt->parent->parent->voiceparent), true);
			}
		}		
	}
	return chord;
}

t_chord *make_chord_grace_on_linear_edit(t_score *x){
	t_chord *chord = get_operating_chord_for_linear_edit(x);
	t_chord *orig_chord = chord;
	
	if (chord) {
		create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)chord->parent, k_UNDO_MODIFICATION_CHANGE);
		toggle_grace_for_chord((t_notation_obj *)x, chord, chord->is_grace_chord ? false : true);
	}
	
	return orig_chord;
}

t_chord *make_chord_or_note_sharp_or_flat_on_linear_edit(t_score *x, char direction){
	double step = (200/x->r_ob.tone_division);
	t_rational step_acc = genrat(1, x->r_ob.tone_division);
	t_chord *chord = get_operating_chord_for_linear_edit(x);
	t_chord *orig_chord = chord, *last_chord = chord, *temp;
	
	if (chord) {
		
		while (orig_chord && is_all_chord_tied_from(orig_chord, false))
			orig_chord = get_prev_chord(orig_chord);
		
		for (temp = orig_chord; temp; temp = get_next_chord(temp)) {
			t_note *nt, *cursor_nt = NULL;

			create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)temp, k_UNDO_MODIFICATION_CHANGE);

			for (nt = temp->firstnote; nt; nt = nt->next) {
				if (note_get_screen_midicents(nt) == x->r_ob.notation_cursor.midicents) {
					cursor_nt = nt;
					break;
				}
			}
			
			for (nt = temp->firstnote; nt; nt = nt->next) {
				if (!cursor_nt || cursor_nt == nt) {
					nt->midicents = nt->midicents + step * direction;
					constraint_midicents_depending_on_editing_ranges((t_notation_obj *)x, &nt->midicents, chord->parent->voiceparent->v_ob.number); 
					
                    note_set_user_enharmonicity(nt, t_pitch(nt->pitch_displayed.degree(), rat_rat_sum(nt->pitch_displayed.alter(), rat_long_prod(step_acc, direction)), nt->pitch_displayed.octave()));

                    calculate_chord_parameters((t_notation_obj *) x, nt->parent, get_voice_clef((t_notation_obj *)x, (t_voice *)nt->parent->parent->voiceparent), true);
				}
			}		
			
			if (temp == last_chord)
				break;
		}

		if (orig_chord)
			validate_accidentals_for_measure((t_notation_obj *) x, orig_chord->parent);
	}
	return orig_chord;
}




t_chord *change_pitch_from_linear_edit(t_score *x, long diatonic_step)
{
	t_chord *chord = get_operating_chord_for_linear_edit(x);
	t_chord *orig_chord = chord, *last_chord = chord, *temp;
	long oct = x->r_ob.notation_cursor.midicents / 1200;
	long mc = diatonicstep2midicents(diatonic_step, oct);
	if (labs((mc + 1200) - x->r_ob.notation_cursor.midicents) < labs(mc - x->r_ob.notation_cursor.midicents))
		mc += 1200;
	else if (labs((mc - 1200) - x->r_ob.notation_cursor.midicents) < labs(mc - x->r_ob.notation_cursor.midicents))
		mc -= 1200;
	
	double mc_double = mc;
	constraint_midicents_depending_on_editing_ranges((t_notation_obj *)x, &mc_double, chord->parent->voiceparent->v_ob.number); 
	mc = round(mc_double);

	if (chord) {
		
		while (orig_chord && is_all_chord_tied_from(orig_chord, false))
			orig_chord = get_prev_chord(orig_chord);
		
		for (temp = orig_chord; temp; temp = get_next_chord(temp)) {
			t_note *nt, *cursor_nt = NULL;

			create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)temp, k_UNDO_MODIFICATION_CHANGE);

			for (nt = temp->firstnote; nt; nt = nt->next) {
				if (note_get_screen_midicents(nt) == x->r_ob.notation_cursor.midicents) {
					cursor_nt = nt;
					break;
				}
			}
			

			for (nt = temp->firstnote; nt; nt = nt->next) {
				if (!cursor_nt || cursor_nt == nt) {

                    nt->pitch_displayed.set(diatonic_step, long2rat(0), oct);
                    note_set_auto_enharmonicity(nt);

					note_compute_approximation((t_notation_obj *)x, nt);
					calculate_chord_parameters((t_notation_obj *) x, nt->parent, get_voice_clef((t_notation_obj *)x, (t_voice *)nt->parent->parent->voiceparent), true);
				}
			}
			
			if (temp == last_chord)
				break;
		}

		if (orig_chord)
			validate_accidentals_for_measure((t_notation_obj *) x, orig_chord->parent);
	}
	return orig_chord;
}

// if force_diatonic_step == -1, the note is added automatically depending on the notation cursor y position
void add_note_to_chord_from_linear_edit(t_score *x, long force_diatonic_step){
	t_note *this_nt;
	double argv[2]; 
	
	if (x->r_ob.notation_cursor.chord) {
		argv[0] = CONST_DEFAULT_NEW_NOTE_DURATION; 
		if (force_diatonic_step >= 0) {
			long oct = x->r_ob.notation_cursor.midicents / 1200;
			long mc = diatonicstep2midicents(force_diatonic_step, oct);
			if (labs((mc + 1200) - x->r_ob.notation_cursor.midicents) < labs(mc - x->r_ob.notation_cursor.midicents))
				mc += 1200;
			else if (labs((mc - 1200) - x->r_ob.notation_cursor.midicents) < labs(mc - x->r_ob.notation_cursor.midicents))
				mc -= 1200;
			argv[1] = mc;	
		} else
			argv[1] = x->r_ob.notation_cursor.midicents;
		
		constraint_midicents_depending_on_editing_ranges((t_notation_obj *)x, &(argv[1]), x->r_ob.notation_cursor.chord->parent->voiceparent->v_ob.number); 
		
		this_nt = build_note_from_ac_av((t_notation_obj *) x, 2, argv);
        if (this_nt)
            set_slots_values_to_note_from_llll((t_notation_obj *)x, this_nt, x->r_ob.default_noteslots);
        
		create_simple_notation_item_undo_tick((t_notation_obj *) x, x->r_ob.notation_cursor.chord->r_sym_duration.r_num < 0 ? (t_notation_item *)x->r_ob.notation_cursor.chord->parent : (t_notation_item *)x->r_ob.notation_cursor.chord, k_UNDO_MODIFICATION_CHANGE);
		
		x->r_ob.notation_cursor.chord->r_sym_duration = rat_abs(x->r_ob.notation_cursor.chord->r_sym_duration);
		insert_note((t_notation_obj *) x, x->r_ob.notation_cursor.chord, this_nt, 0);
		note_compute_approximation((t_notation_obj *) x, this_nt);
		calculate_chord_parameters((t_notation_obj *) x, x->r_ob.notation_cursor.chord, get_voice_clef((t_notation_obj *)x, (t_voice *)x->r_ob.notation_cursor.chord->parent->voiceparent), false);
		validate_accidentals_for_measure((t_notation_obj *) x, x->r_ob.notation_cursor.measure);
		x->r_ob.notation_cursor.measure->need_recompute_beams_positions = true;
	}
}

t_rational number_to_duration_for_linear_edit(char number){
	switch (number) {
		case 1: return RAT_1OVER64;
		case 2: return RAT_1OVER32;
		case 3: return RAT_1OVER16;
		case 4: return RAT_1OVER8;
		case 5: return RAT_1OVER4;
		case 6: return RAT_1OVER2;
		case 7: return genrat(1, 1);
		case 8: return genrat(2, 1);
		default: return RAT_1OVER8;
	}
}

// if force_diatonic_step = -1, the note depends on the y position of the notation curosr
t_chord *add_new_chord_in_measure_from_linear_edit(t_score *x, t_chord *after_this_chord, char number, long force_diatonic_step){
    t_chord *new_chord;
    
    // 1. deciding after which tree element we have to insert the chord
    t_rational inserted_duration = number_to_duration_for_linear_edit(number);
    t_llllelem *after_this_elem = after_this_chord ? after_this_chord->rhythmic_tree_elem : NULL;
    t_llll *parent;
    while (after_this_chord && after_this_elem->l_parent && after_this_elem->l_parent->l_owner && after_this_elem == after_this_elem->l_parent->l_tail &&
           (is_level_grace(after_this_elem->l_parent) ||
            (is_level_tuplet(after_this_elem->l_parent) && !(((t_rhythm_level_properties *)after_this_elem->l_parent->l_thing.w_obj)->level_type & k_RHYTHM_LEVEL_FORCE_TUPLET))))
        after_this_elem = after_this_elem->l_parent->l_owner;
    
    parent = (after_this_elem ? after_this_elem->l_parent : x->r_ob.notation_cursor.measure->rhythmic_tree);
    
    // 2. seeing if we have to add tuplet levels
    if (x->r_ob.num_speedy_tuplets > 0){
        long i;
        for (i = 0; i < x->r_ob.num_speedy_tuplets; i++){
            t_llll *to_insert = llll_get();
            
            // inserting list
            if (!after_this_elem)
                llll_prependllll(parent, to_insert, 0, WHITENULL_llll);
            else
                llll_insertllll_after(to_insert, after_this_elem, 0, WHITENULL_llll);
            parent = to_insert;
            after_this_elem = NULL;
            
            // setting tupletinfo
            t_rational tuplet_local_ratio = genrat(x->r_ob.speedy_tuplet[i].r_num, x->r_ob.speedy_tuplet[i].r_den);
            to_insert->l_thing.w_obj = build_rhythm_level_properties();
            set_level_type_flag_for_level(to_insert, k_RHYTHM_LEVEL_KEEP | k_RHYTHM_LEVEL_TUPLET | k_RHYTHM_LEVEL_FORCE_TUPLET);
            
            t_tupletinfo *tupletinfo = &((t_rhythm_level_properties *)to_insert->l_thing.w_obj)->tupletinfo;
            ((t_rhythm_level_properties *)to_insert->l_thing.w_obj)->is_tuplet = true;
            long multiplier = gcd(x->r_ob.speedy_tuplet[i].r_num, x->r_ob.speedy_tuplet[i].r_den);
            fill_tupletinfo_fields2((t_notation_obj *) x, tupletinfo, tuplet_local_ratio, inserted_duration, get_overall_tuplet_ratio_for_outer_levels(parent), long2rat(multiplier));
        }
        x->r_ob.num_speedy_tuplets = 0;
    }
    
    // actually inserting chord in measure with proper ratio
    t_rational overall_ratio = get_overall_tuplet_ratio_for_outer_levels_and_this_level(parent);
    new_chord = addchord_in_measure_from_notes(x, x->r_ob.notation_cursor.measure, after_this_chord, rat_rat_prod(inserted_duration, overall_ratio), -1, 0, NULL, NULL, 0);
    
    llll_check(x->r_ob.notation_cursor.measure->rhythmic_tree);
    
    // actually inserting beaming tree element
    if (!after_this_elem)
        new_chord->rhythmic_tree_elem = llll_prependobj(parent, new_chord, 0, WHITENULL_llll);
    else
        new_chord->rhythmic_tree_elem = llll_insertobj_after(new_chord, after_this_elem, 0, WHITENULL_llll);
    
	x->r_ob.notation_cursor.chord = new_chord;
	add_note_to_chord_from_linear_edit(x, force_diatonic_step);
	return new_chord;
}

void force_inscreen_ux_rolling_while_editing(t_score *x){
	t_measure *meas = x->r_ob.notation_cursor.measure;
	
	if (!meas)
		return;
	
	if (x->r_ob.domain_ux < meas->width_ux) {
		double cursor_ux = get_linear_edit_cursor_ux_position(x);
		force_inscreen_ux_rolling(x, cursor_ux, 0, true, false);
	} else {
		double ratio = (meas->width_ux/x->r_ob.domain_ux);
		ratio = ceil(ratio/0.2) * 0.2;
		force_inscreenpos_ux(x, 1 - ratio, meas->tuttipoint_reference->offset_ux + meas->start_barline_offset_ux, true, false);
//		if (!(meas->tuttipoint_reference->offset_ux + meas->start_barline_offset_ux >= x->r_ob.screen_ux_start &&
//			meas->tuttipoint_reference->offset_ux + meas->start_barline_offset_ux + meas->width_ux <= x->r_ob.screen_ux_end))
	}
}

void linear_edit_jump_to_next_chord(t_score *x){
	char chord_was_null = (!x->r_ob.notation_cursor.chord);
	x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.chord ? x->r_ob.notation_cursor.chord->next : NULL;
	if (chord_was_null || (!x->r_ob.notation_cursor.chord && x->r_ob.notation_cursor.measure->measure_filling >= 0)) {	
		// change measure
		end_editing_measure_in_linear_edit(x, x->r_ob.notation_cursor.measure);
		if (x->r_ob.notation_cursor.measure->next) {
			x->r_ob.notation_cursor.measure = x->r_ob.notation_cursor.measure->next;
            if (x->r_ob.notation_cursor.touched_measures)
                llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
			x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->firstchord;
		} else {
			// create a measure from linear_edit!
			t_measure *new_measure = NULL;
			if (measure_barlines_coincide_for_all_voices(x, x->r_ob.notation_cursor.measure->measure_number)) {
				t_measure *added_meas[CONST_MAX_VOICES];
				long i, num_added_meas = 0;
				insert_new_measure_in_all_voices(x, x->r_ob.notation_cursor.measure->voiceparent, x->r_ob.notation_cursor.measure->measure_number, 
												 1, added_meas, &num_added_meas, true, NULL);
				for (i = 0; i < num_added_meas; i++)
					if (added_meas[i] && added_meas[i]->prev)
						set_measure_ts((t_notation_obj *)x, added_meas[i], added_meas[i]->prev->timesignature);
				new_measure = x->r_ob.notation_cursor.measure->next;
			} else {
				new_measure = build_measure((t_notation_obj *)x, NULL);
				set_measure_ts((t_notation_obj *)x, new_measure, x->r_ob.notation_cursor.measure->timesignature);
				insert_measure(x, x->r_ob.notation_cursor.measure->voiceparent, new_measure, x->r_ob.notation_cursor.measure, 0);
				create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)new_measure, k_UNDO_MODIFICATION_DELETE);
			}
			
			x->r_ob.notation_cursor.measure = new_measure;
            if (x->r_ob.notation_cursor.touched_measures)
                llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
			recompute_all_and_redraw(x);
			perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
			// delete measure firstchord
			delete_all_chords_from_measure(x, new_measure);
			x->r_ob.notation_cursor.chord = NULL;
			handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_LINEAR_EDIT_ADD_CHORD);
		}
	}
}

void end_editing_measure_in_linear_edit(t_score *x, t_measure *measure){
	t_chord *temp;
	for (temp = measure->firstchord; temp; temp = temp->next)
		temp->dont_split_for_ts_boxes = false;
	
	if (measure->rhythmic_tree) {
		char this_changed = false;
		llll_funall(measure->rhythmic_tree, fix_level_type_flag_for_level_as_ignore_fn, &this_changed, 1, -1, FUNALL_SKIP_ATOMS);
		if (this_changed)
			set_tuplet_levels_as_keep_levels(measure->rhythmic_tree);
		
		this_changed |= (!measure->rhythmic_tree->l_thing.w_obj || ((t_rhythm_level_properties *)measure->rhythmic_tree->l_thing.w_obj)->level_type != k_RHYTHM_LEVEL_IGNORE);
		if (this_changed) {
			llll_funall(measure->rhythmic_tree, fix_level_type_flag_for_level_as_ignore_fn, &this_changed, 1, -1, FUNALL_SKIP_ATOMS);
			set_tuplet_levels_as_keep_levels(measure->rhythmic_tree);
			
			if (this_changed)  {
				recompute_all_for_measure((t_notation_obj *) x, measure, true);
				set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
				perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
			}
		}
	}
}


void exit_linear_edit(t_score *x)
{
	if (x->r_ob.notation_cursor.measure)
		end_editing_measure_in_linear_edit(x, x->r_ob.notation_cursor.measure);
	
    if (x->r_ob.auto_complete_measures) {
        // TO DO: reset all "touched" measure as autocomplete
        t_llllelem *elem;
        for (elem = x->r_ob.notation_cursor.touched_measures->l_head; elem; elem = elem->l_next) {
            t_measure *m = (t_measure *)hatom_getobj(&elem->l_hatom);
            m->need_check_autocompletion = true;
            recompute_all_for_measure((t_notation_obj *)x, m, false);
        }
    }
    llll_clear(x->r_ob.notation_cursor.touched_measures);
    
    
	if (x->r_ob.obj_type == k_NOTATION_OBJECT_SCORE) {
		t_scorevoice *voice;
		t_measure *meas;
		for (voice = (t_scorevoice *)x->r_ob.firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next)
			for (meas = voice->firstmeasure; meas; meas = meas->next)
				llll_funall(meas->rhythmic_tree, remove_force_tuplet_levels_fn, NULL, 1, -1, FUNALL_SKIP_ATOMS);
	}
	
    set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
	x->r_ob.num_speedy_tuplets = 0;
	x->r_ob.notation_cursor.measure = NULL;	// This means: NO cursor
	x->r_ob.notation_cursor.chord = NULL;
	x->r_ob.notation_cursor.midicents = 6000;
	x->r_ob.notation_cursor.step = 0;
	x->r_ob.is_linear_editing = false;
	invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
}


char delete_selected_markers(t_score *x){ 
	t_notation_item *curr_it = x->r_ob.firstselecteditem;
	char changed = 0;
	
	lock_general_mutex((t_notation_obj *)x);	
	while (curr_it) { // cycle on the selected items
		t_notation_item *nextitem = curr_it->next_selected; 
		
		if (curr_it->type == k_MARKER) {
			create_header_undo_tick((t_notation_obj *)x, k_HEADER_MARKERS);
			delete_marker((t_notation_obj *) x, (t_marker *) curr_it);
			changed = true;
		}
		curr_it = nextitem;
	}
	unlock_general_mutex((t_notation_obj *)x);
	
	return changed;
}

t_chord *get_last_chord_before_notation_cursor(t_score *x){
	t_measure *meas = x->r_ob.notation_cursor.measure;
	t_chord *ch = x->r_ob.notation_cursor.measure->lastchord;
	while (!ch && meas) {
		meas = meas->prev;
		if (meas)
			ch = meas->lastchord;
	}
	return ch;
}

/*
void score_clearnames(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	notation_obj_clearnames((t_notation_obj *)x, s, argc, argv);
}
*/

void score_name(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	notation_obj_name((t_notation_obj *)x, s, argc, argv);
}


void score_nameappend(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	notation_obj_nameappend((t_notation_obj *)x, s, argc, argv);
}

void score_slottoname(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    notation_obj_slottoname((t_notation_obj *)x, s, argc, argv);
}

void score_nametoslot(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    notation_obj_nametoslot((t_notation_obj *)x, s, argc, argv);
}


void score_clearnames(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	t_llll *args = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_RETAIN);
	long voices = 1, measures = 1, chords = 1, notes = 1, markers = 1;
//	llll_parseargs((t_object *)x, args, "iiiii", _llllobj_sym_markers, &markers, _llllobj_sym_voices, &voices, _llllobj_sym_measures, &measures, _llllobj_sym_chords, &chords, _llllobj_sym_notes, &notes);
    voices = (args->l_size == 0 || is_symbol_in_llll_first_level(args, _llllobj_sym_voices));
    measures = (args->l_size == 0 || is_symbol_in_llll_first_level(args, _llllobj_sym_measures));
    chords = (args->l_size == 0 || is_symbol_in_llll_first_level(args, _llllobj_sym_chords));
    notes = (args->l_size == 0 || is_symbol_in_llll_first_level(args, _llllobj_sym_notes));
    markers = (args->l_size == 0 || is_symbol_in_llll_first_level(args, _llllobj_sym_markers));

    notation_obj_clear_names((t_notation_obj *)x, voices, measures, chords, notes, markers);
	llll_free(args);
}

void score_copy_selected_measures(t_score *x, char cut)
{
	t_notation_item *selitem;
	if (clipboard.gathered_syntax) { 
		llll_free(clipboard.gathered_syntax); 
		clipboard.gathered_syntax = NULL; 
	}
    
	lock_general_mutex((t_notation_obj *)x);	
	clipboard.gathered_syntax = llll_get();
    
    for (t_scorevoice *voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
        t_llll *this_voice = llll_get();
        for (t_measure *meas = voice->firstmeasure; meas; meas = meas->next) {
            if (notation_item_is_selected((t_notation_obj *)x, (t_notation_item *)meas))
                llll_appendllll(this_voice, measure_get_values_as_llll((t_notation_obj *) x, meas, k_CONSIDER_FOR_SAVING, true, true));
        }
        llll_appendllll(clipboard.gathered_syntax, this_voice);
    }
    clipboard.type = k_GATHERED_SYNTAX;
    
    /// OLD CODE: WE ONLY SAVED MEASURES, NO VOICE LEVELS
/*	selitem = x->r_ob.firstselecteditem;
	while (selitem) {
		if (selitem->type == k_MEASURE) 
			llll_appendllll(clipboard.gathered_syntax, measure_get_values_as_llll((t_notation_obj *) x, (t_measure *)selitem, k_CONSIDER_FOR_SAVING, true, true), 0, WHITENULL_llll);
		selitem = selitem->next_selected;
	}
    
	clipboard.type = k_MEASURE; */
    
	clipboard.object = k_NOTATION_OBJECT_SCORE;
	
	if (cut) {
		char need_recompute = false;
		selitem = x->r_ob.firstselecteditem;
		while (selitem) {
			t_notation_item *next_sel = selitem->next_selected;
			if (selitem->type == k_MEASURE) {
				need_recompute = true;
				create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, selitem, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
				clear_measure(x, (t_measure *)selitem, true, true, true);
				recompute_all_for_measure((t_notation_obj *)x, (t_measure *)selitem, true);
			}
			selitem = next_sel;
		}
		if (need_recompute) {
			recompute_all_except_for_beamings_and_autocompletion(x);
			perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
		}
	}
	
	unlock_general_mutex((t_notation_obj *)x);
    
    if (cut)
        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CUT_MEASURES);
}


void score_paste_replace_single_measures(t_score *x, char also_paste_tempi)
{
	// we paste the measures
	t_notation_item *selitem;
	t_llllelem *llllelem;
	char changed = 0;
	
	lock_general_mutex((t_notation_obj *)x);	
	selitem = x->r_ob.firstselecteditem;
	llllelem = clipboard.gathered_syntax->l_head;
	while (selitem && llllelem) {
		if (selitem->type == k_MEASURE) {
            t_measure *meas = (t_measure *)selitem;
            t_timesignature old_ts = meas->timesignature;
			t_llll *measuretopaste_as_llll = llll_get();
			
			create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, selitem, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			
			changed = 1;
			if (hatom_gettype(&llllelem->l_hatom) == H_LLLL)
				llll_clone_upon(hatom_getllll(&llllelem->l_hatom), measuretopaste_as_llll);
			clear_measure(x, meas, true, false, also_paste_tempi);

            set_measure_from_llll(x, meas, measuretopaste_as_llll, also_paste_tempi, false, NULL);
            
            if (are_ts_equal(&old_ts, &meas->timesignature)) // only need to recompute measure-related stuff
                recompute_all_for_measure((t_notation_obj *)x, meas, true);
            else // pasted different time signatures: potentially all tuttipoints might have changed! need to recompute everything.
                recompute_all(x);
			
            llll_free(measuretopaste_as_llll);
		}
		selitem = selitem->next_selected;
		if (llllelem->l_next) 
			llllelem = llllelem->l_next;
	}
    
    
	for (selitem = x->r_ob.firstselecteditem; selitem; selitem = selitem->next_selected) 
		if (selitem->type == k_MEASURE) {
			compute_note_approximations_for_measure((t_notation_obj *)x, (t_measure *)selitem, true); // needed before check_measure_ties
			check_measure_ties((t_notation_obj *)x, (t_measure *)selitem, false, false);
		}
    update_solos((t_notation_obj *)x);
    
    if (also_paste_tempi) { // tuttipoints might have changed
        recompute_all_and_redraw(x);
        perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
    }
    unlock_general_mutex((t_notation_obj *)x);
    
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_PASTE_MEASURES);
}


// in gathered syntax
void score_paste_replace_measures(t_score *x, char also_paste_tempi)
{
    // we paste the measures
    t_notation_item *selitem;
    t_llllelem *llllelem;
    t_llll *voice_ll = NULL;
    t_llllelem *voice_llelem = NULL;
    char changed = 0;
    
    if (clipboard.gathered_syntax) {
        voice_llelem = clipboard.gathered_syntax->l_head;
        voice_ll = hatom_gettype(&voice_llelem->l_hatom) == H_LLLL ? hatom_getllll(&voice_llelem->l_hatom) : NULL;
        while (voice_ll && !voice_ll->l_head) {
            voice_llelem = voice_llelem->l_next;
            voice_ll = hatom_gettype(&voice_llelem->l_hatom) == H_LLLL ? hatom_getllll(&voice_llelem->l_hatom) : NULL;
        }
    }
    
    lock_general_mutex((t_notation_obj *)x);
    char already_pasted = false;
    for (t_scorevoice *voice = x->firstvoice; voice_llelem && voice_ll && voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
        t_llllelem *cur_el = voice_ll->l_head;
        for (t_measure *meas = voice->firstmeasure; cur_el && voice_llelem && voice_ll && meas; meas = meas->next) {
            if (notation_item_is_selected((t_notation_obj *)x, (t_notation_item *)meas)) {
                t_timesignature old_ts = meas->timesignature;
                t_llll *measuretopaste_as_llll = llll_get();
                
                create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
                
                changed = 1;
                if (cur_el && hatom_gettype(&cur_el->l_hatom) == H_LLLL)
                    llll_clone_upon(hatom_getllll(&cur_el->l_hatom), measuretopaste_as_llll);
                clear_measure(x, meas, true, false, also_paste_tempi);
                
                set_measure_from_llll(x, meas, measuretopaste_as_llll, also_paste_tempi, false, NULL);
                
                if (are_ts_equal(&old_ts, &meas->timesignature)) // only need to recompute measure-related stuff
                    recompute_all_for_measure((t_notation_obj *)x, meas, true);
                else // pasted different time signatures: potentially all tuttipoints might have changed! need to recompute everything.
                    recompute_all(x);
                
                llll_free(measuretopaste_as_llll);
                already_pasted = true;
                if (cur_el)
                    cur_el = cur_el->l_next;
            }
        }
        
        if (already_pasted) {
            voice_llelem = voice_llelem->l_next;
            voice_ll = NULL;
            if (hatom_gettype(&voice_llelem->l_hatom) == H_LLLL)
                voice_ll = hatom_getllll(&voice_llelem->l_hatom);
        }
    }
    
    for (selitem = x->r_ob.firstselecteditem; selitem; selitem = selitem->next_selected)
        if (selitem->type == k_MEASURE) {
            compute_note_approximations_for_measure((t_notation_obj *)x, (t_measure *)selitem, true); // needed before check_measure_ties
            check_measure_ties((t_notation_obj *)x, (t_measure *)selitem, false, false);
        }
    update_solos((t_notation_obj *)x);
    
    if (also_paste_tempi) { // tuttipoints might have changed
        recompute_all_and_redraw(x);
        perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
    }
    unlock_general_mutex((t_notation_obj *)x);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_PASTE_MEASURES);
}



// beware: after_this_measure_1based and from_this_voice_1based are 1-based!!!
void score_paste_measures(t_score *x, long at_this_measure_1based, long from_this_voice_1based, char also_paste_tempi, char ignore_copied_voice_offset, long how_many_times)
{
    create_whole_score_undo_tick(x);
    
    t_llll *ll = llll_clone(clipboard.gathered_syntax);
    
    if (ignore_copied_voice_offset && ll) {
        t_llllelem *voice_llelem = ll->l_head;
        t_llll *voice_ll = hatom_gettype(&voice_llelem->l_hatom) == H_LLLL ? hatom_getllll(&voice_llelem->l_hatom) : NULL;
        while (voice_ll && !voice_ll->l_head) {
            llll_behead(ll);
            voice_llelem = voice_llelem->l_next;
            voice_ll = hatom_gettype(&voice_llelem->l_hatom) == H_LLLL ? hatom_getllll(&voice_llelem->l_hatom) : NULL;
        }
    }
    
    for (long i = 0; i < how_many_times; i++)
        insert_measures_from_message(x, from_this_voice_1based, x->r_ob.num_voices, at_this_measure_1based, ll, true);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_PASTE_MEASURES);
    llll_free(ll);
}



t_notation_item *are_all_selected_items_tied(t_score *x)
{
    t_notation_item *it, *first = get_leftmost_selected_notation_item((t_notation_obj *)x);
    for (it = x->r_ob.firstselecteditem; it; it = it->next_selected){
        if (it == first || notation_item_is_ancestor_of((t_notation_obj *)x, first, it)) {
            // all right
        } else {
            if (it->type == k_NOTE || (it->type == k_CHORD && ((t_chord *)it)->num_notes == 1)) {
                char found = false;
                t_note *temp = it->type == k_NOTE ? (t_note *)it : ((t_chord *)it)->firstnote;
                while (temp) {
                    temp = temp->tie_from;
                    if (!temp)
                        return NULL;
                    if (first == (t_notation_item *)temp || notation_item_is_ancestor_of((t_notation_obj *)x, first, (t_notation_item *)temp)) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return NULL;
            } else
                return NULL;
        }
    }
    
    return first;
}

void select_only_first_item_if_tied_sequence_is_selected(t_score *x)
{
    if (x->r_ob.num_selecteditems > 1) {
        t_notation_item *first = are_all_selected_items_tied(x);
        if (first) {
            clear_selection((t_notation_obj *)x);
            notation_item_add_to_selection((t_notation_obj *)x, first);
        }
    }
}

long score_key(t_score *x, t_object *patcherview, long keycode, long modifiers, long textcharacter)
{
	int j;
	// first of all: are we pressing a key associated with some slot?
	llll_format_modifiers(&modifiers, &keycode);
	
    
	if (keycode == 'i' && modifiers == eCommandKey && is_editable((t_notation_obj *)x, k_BACH_INSPECTOR, k_ELEMENT_ACTIONS_NONE)) {
		if (x->r_ob.active_slot_num >= 0 && x->r_ob.active_slot_notationitem) {
			x->non_inspector_ux_screen_start = x->r_ob.screen_ux_start;
			if (x->r_ob.m_inspector.inspector_patcher)
				bring_external_inspector_to_front(&x->r_ob.m_inspector);
			open_bach_inspector((t_notation_obj *)x, &x->r_ob.m_inspector, &x->r_ob.slotinfo[x->r_ob.active_slot_num], k_SLOTINFO);
			invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
			return 1;
		} else if (x->r_ob.firstselecteditem && (x->r_ob.firstselecteditem->type == k_CHORD || x->r_ob.firstselecteditem->type == k_NOTE ||
					x->r_ob.firstselecteditem->type == k_MEASURE || x->r_ob.firstselecteditem->type == k_VOICE ||
					x->r_ob.firstselecteditem->type == k_TEMPO || x->r_ob.firstselecteditem->type == k_MARKER ||
                   x->r_ob.firstselecteditem->type == k_PITCH_BREAKPOINT)) {
            
            select_only_first_item_if_tied_sequence_is_selected(x);
            
            if (x->r_ob.num_selecteditems == 1) {
                if (x->r_ob.m_inspector.inspector_patcher)
                    bring_external_inspector_to_front(&x->r_ob.m_inspector);
                open_bach_inspector_for_notation_item((t_notation_obj *) x, x->r_ob.firstselecteditem);
                invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
                return 1;
            }
        }
		
	} else if (keycode == JKEY_RETURN && is_editable((t_notation_obj *)x, k_PLAYCURSOR, k_ELEMENT_ACTIONS_NONE)) {
		if (!x->r_ob.playing) {
			if (x->r_ob.show_loop_region && !(modifiers & eShiftKey)) {
				x->r_ob.play_head_start_ux = timepoint_to_unscaled_xposition((t_notation_obj *)x, x->r_ob.loop_region.start.timepoint, false, true);
				x->r_ob.play_head_start_ms = unscaled_xposition_to_ms((t_notation_obj *)x, x->r_ob.play_head_start_ux, 1);
			} else {
				double first_onset_ms = get_first_onset_ms_for_grace_notes(x);
				x->r_ob.play_head_start_ux = 0;
				x->r_ob.play_head_start_ms = (first_onset_ms < 0 ? first_onset_ms - CONST_EPSILON2 : first_onset_ms);
			}
            send_moved_playhead_position((t_notation_obj *) x, 7);
			if (!x->r_ob.show_loop_region || modifiers & eShiftKey) {
				x->r_ob.hscrollbar_pos = 0.;
				redraw_hscrollbar((t_notation_obj *) x, 1);
			} else {
				force_inscreen_ux_to_boundary(x, x->r_ob.play_head_start_ux, false, true, false);
				invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
			}
			send_domain(x, 7, NULL);
			return 1;
		}
	}

	if (keycode == 'z' && x->r_ob.allow_undo) {
		if (modifiers == eCommandKey) {
			score_undo(x);
			return 1;
		} else if (modifiers == eCommandKey + eShiftKey) {
			score_redo(x);
			return 1;
		}
	}
	
	
	// HANDLING LINEAR EDIT
	if (x->r_ob.notation_cursor.measure && x->r_ob.allow_linear_edit) { // linear edit?
		
		verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before2"), 1);
		if (textcharacter == '+') {
			t_chord *ch = make_chord_or_note_sharp_or_flat_on_linear_edit(x, 1);	// edited chord
			if (ch) 
				ch->need_recompute_parameters = true;
			
			handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_ADD_SHARP);
			
			if (x->r_ob.playback_during_linear_editing && ch)
				send_chord_as_llll((t_notation_obj *) x, ch, 7, k_CONSIDER_FOR_DUMPING, -1);
		} else if (textcharacter == '-') {
			t_chord *ch = make_chord_or_note_sharp_or_flat_on_linear_edit(x, -1);	// edited chord
			if (ch)
				ch->need_recompute_parameters = true;
			
			handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_ADD_FLAT);
			
			if (x->r_ob.playback_during_linear_editing && ch)
				send_chord_as_llll((t_notation_obj *) x, ch, 7, k_CONSIDER_FOR_DUMPING, -1);
		} else {
			switch (keycode) {
				case JKEY_ESC:
					if (modifiers & eAltKey) {
						x->r_ob.num_speedy_tuplets = 0;
						invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
					} else {
						exit_linear_edit(x);
						recompute_all_and_redraw(x);
					}
					break;
				case JKEY_LEFTARROW: 
				{
                    char jump_to_prev_meas = false;
					if (modifiers & eCommandKey) {
						if (x->r_ob.notation_cursor.measure->prev) {
							x->r_ob.notation_cursor.measure = x->r_ob.notation_cursor.measure->prev;
                            if (x->r_ob.notation_cursor.touched_measures)
                                llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
							x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->firstchord;
						}
					} else {
						if (!x->r_ob.notation_cursor.chord) {
                            jump_to_prev_meas = true;
							if (!x->r_ob.notation_cursor.measure->lastchord) {
								if (x->r_ob.notation_cursor.measure->prev) {
									end_editing_measure_in_linear_edit(x, x->r_ob.notation_cursor.measure);
									x->r_ob.notation_cursor.measure = x->r_ob.notation_cursor.measure->prev;
                                    if (x->r_ob.notation_cursor.touched_measures)
                                        llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
									x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->lastchord;
								}
							} else  {
								x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->lastchord;
							}
						} else {
							t_chord *prev_chord = get_prev_chord(x->r_ob.notation_cursor.chord);
                            if (!x->r_ob.notation_cursor.chord->prev)
                                jump_to_prev_meas = true;
							if (prev_chord) {
								x->r_ob.notation_cursor.chord = prev_chord;
								x->r_ob.notation_cursor.measure = x->r_ob.notation_cursor.chord->parent;
                                if (x->r_ob.notation_cursor.touched_measures)
                                    llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
							}
						}
					}
					
					if (jump_to_prev_meas && is_measure_empty(x->r_ob.notation_cursor.measure)) {
						delete_all_chords_from_measure(x, x->r_ob.notation_cursor.measure);
						x->r_ob.notation_cursor.chord = NULL;
					}
					
					force_inscreen_ux_rolling_while_editing(x);
					
					invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
					break;
				}
				case JKEY_RIGHTARROW: 
				{
					char new_meas = false, jump_to_next_meas = false;
					if (modifiers & eCommandKey) {
						if (x->r_ob.notation_cursor.measure->next) {
							end_editing_measure_in_linear_edit(x, x->r_ob.notation_cursor.measure);
							x->r_ob.notation_cursor.measure = x->r_ob.notation_cursor.measure->next;
							x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->firstchord;
                            if (x->r_ob.notation_cursor.touched_measures)
                                llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
						}
					} else {
						if (!x->r_ob.notation_cursor.chord) {	// gotta jump to next measure
                            jump_to_next_meas = true;
							if (!x->r_ob.notation_cursor.measure->next) {
								// create a measure in all voices?
								t_measure *new_measure = NULL;
								if (measure_barlines_coincide_for_all_voices(x, x->r_ob.notation_cursor.measure->measure_number)) {
									t_measure *added_meas[CONST_MAX_VOICES];
									long i, num_added_meas = 0;
									insert_new_measure_in_all_voices(x, x->r_ob.notation_cursor.measure->voiceparent, x->r_ob.notation_cursor.measure->measure_number, 
																	 1, added_meas, &num_added_meas, true, NULL);
									for (i = 0; i < num_added_meas; i++)
										if (added_meas[i] && added_meas[i]->prev)
											set_measure_ts((t_notation_obj *)x, added_meas[i], added_meas[i]->prev->timesignature);
									new_measure = x->r_ob.notation_cursor.measure->next;
								} else {
									new_measure = build_measure((t_notation_obj *)x, NULL);
									set_measure_ts((t_notation_obj *)x, new_measure, x->r_ob.notation_cursor.measure->timesignature);
									insert_measure(x, x->r_ob.notation_cursor.measure->voiceparent, new_measure, x->r_ob.notation_cursor.measure, 0);
									create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)new_measure, k_UNDO_MODIFICATION_DELETE);
								}
								x->r_ob.notation_cursor.measure = new_measure;
                                if (x->r_ob.notation_cursor.touched_measures)
                                    llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
								recompute_all_and_redraw(x);
								perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
								// delete measure chords
								delete_all_chords_from_measure(x, new_measure);
								x->r_ob.notation_cursor.chord = NULL;
								new_meas = true;
								handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_ADD_NEW_MEASURE);
							} else {
								x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->next->firstchord;
								x->r_ob.notation_cursor.measure = x->r_ob.notation_cursor.measure->next;
                                if (x->r_ob.notation_cursor.touched_measures)
                                    llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
							}
						} else {
							if (!x->r_ob.notation_cursor.chord->next && x->r_ob.notation_cursor.measure->measure_filling < 0){
								x->r_ob.notation_cursor.chord = NULL;
							} else {
								t_chord *next_chord = get_next_chord(x->r_ob.notation_cursor.chord);
								x->r_ob.notation_cursor.chord = next_chord;
                                if (next_chord) {
									x->r_ob.notation_cursor.measure = x->r_ob.notation_cursor.chord->parent;
                                    if (x->r_ob.notation_cursor.touched_measures)
                                        llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
                                }
							}
						}
					}
					
					if (jump_to_next_meas && is_measure_empty(x->r_ob.notation_cursor.measure) && x->r_ob.notation_cursor.measure->firstchord) {
						if (!new_meas)
							create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.notation_cursor.measure, k_UNDO_MODIFICATION_CHANGE);
						delete_all_chords_from_measure(x, x->r_ob.notation_cursor.measure);
						x->r_ob.notation_cursor.chord = NULL;
						handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_CLEAR_MEASURE);
					}
					
					force_inscreen_ux_rolling_while_editing(x);
					
					invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
					break;
				}
				case JKEY_UPARROW:
				{
					if (modifiers & eCommandKey) {
						if (x->r_ob.notation_cursor.measure->voiceparent->prev) {
							t_measure *meas = nth_measure_of_scorevoice(x->r_ob.notation_cursor.measure->voiceparent->prev, x->r_ob.notation_cursor.measure->measure_number);
							if (meas) {
								x->r_ob.notation_cursor.measure = meas;
                                if (x->r_ob.notation_cursor.touched_measures)
                                    llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
								x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->firstchord;
							}
							if (is_measure_empty(x->r_ob.notation_cursor.measure) && x->r_ob.notation_cursor.measure->firstchord) {
								create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.notation_cursor.measure, k_UNDO_MODIFICATION_CHANGE);
								delete_all_chords_from_measure(x, x->r_ob.notation_cursor.measure);
								x->r_ob.notation_cursor.chord = NULL;
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_CLEAR_MEASURE);
							}
							
							force_inscreen_ux_rolling_while_editing(x);
						}
					} else {
						move_linear_edit_cursor_depending_on_edit_ranges((t_notation_obj *)x, modifiers & eShiftKey ? 7 : 1, modifiers);
						x->r_ob.notation_cursor.midicents = scaleposition_to_midicents(x->r_ob.notation_cursor.step);
						
					}
					invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
					break;
				}
				case JKEY_DOWNARROW:
				{
					if (modifiers & eCommandKey) {
						if (x->r_ob.notation_cursor.measure->voiceparent->next) {
							t_measure *meas = nth_measure_of_scorevoice(x->r_ob.notation_cursor.measure->voiceparent->next, x->r_ob.notation_cursor.measure->measure_number);
							if (meas) {
								x->r_ob.notation_cursor.measure = meas;
                                if (x->r_ob.notation_cursor.touched_measures)
                                    llll_appendobj(x->r_ob.notation_cursor.touched_measures, x->r_ob.notation_cursor.measure);
								x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->firstchord;
							}
							if (is_measure_empty(x->r_ob.notation_cursor.measure) && x->r_ob.notation_cursor.measure->firstchord) {
								create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.notation_cursor.measure, k_UNDO_MODIFICATION_CHANGE);
								delete_all_chords_from_measure(x, x->r_ob.notation_cursor.measure);
								x->r_ob.notation_cursor.chord = NULL;
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_CLEAR_MEASURE);
							}
							
							force_inscreen_ux_rolling_while_editing(x);
						}
					} else {
						move_linear_edit_cursor_depending_on_edit_ranges((t_notation_obj *)x, modifiers & eShiftKey ? -7 : -1, modifiers);
						x->r_ob.notation_cursor.midicents = scaleposition_to_midicents(x->r_ob.notation_cursor.step);
					}
					invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
					break;
				}
				case 'r':
                {
                    t_chord *ch = x->r_ob.notation_cursor.chord ? x->r_ob.notation_cursor.chord : get_last_chord_before_notation_cursor(x);
					if (ch && ch->parent) {
						create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)ch->parent, k_UNDO_MODIFICATION_CHANGE);
						
						turn_chord_into_rest_or_into_note(x, ch, x->r_ob.notation_cursor.midicents);
						validate_accidentals_for_measure((t_notation_obj *) x, ch->parent);
						calculate_chord_parameters((t_notation_obj *) x, ch, get_voice_clef((t_notation_obj *)x, (t_voice *)ch->parent->voiceparent), true);
						
						if (x->r_ob.playback_during_linear_editing && x->r_ob.notation_cursor.chord)
							send_chord_as_llll((t_notation_obj *) x, ch, 7, k_CONSIDER_FOR_DUMPING, -1);
						
						handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_TOGGLE_REST);
					}
                }
					break;
				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
				{
					if (x->r_ob.notation_cursor.chord) {
						// change pitch of active chord

						create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.notation_cursor.chord, k_UNDO_MODIFICATION_CHANGE);

						change_pitch_from_linear_edit(x, ((keycode - 'a') + 5) % 7);
						
						if (x->r_ob.playback_during_linear_editing && x->r_ob.notation_cursor.chord)
							send_chord_as_llll((t_notation_obj *) x, x->r_ob.notation_cursor.chord, 7, k_CONSIDER_FOR_DUMPING, -1);
						
						handle_change((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_CHANGE_PITCH);
						invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
					} else {
						// add new chord
						t_chord *ch = get_last_chord_before_notation_cursor(x);
						t_rational dur = ch ? ch->figure : RAT_1OVER8;
						int log2 = perfect_log2(dur.r_den);
						x->r_ob.force_diatonic_step = ((keycode - 'a') + 5) % 7;
						// we add a new note with the given forced diatonic step by calling score_key and "pretending" the user has clicked on a
						// numeric key (determining the note duration).
						score_key(x, patcherview, log2 >= 0 ? 48 + 7 - log2 : 4, modifiers, textcharacter);  
						return 1;
					}
					break;
				}
				case 't': // tie/untie
				{
					t_chord *ch = tie_untie_notes_on_linear_edit(x);	// edited chord
					if (ch)
						ch->need_recompute_parameters = true;
					handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_TOGGLE_TIES);
					break;
				}
				case 'x': // make chord grace
				{	
					t_chord *ch = make_chord_grace_on_linear_edit(x);	// edited chord
					if (ch) {
						x->r_ob.notation_cursor.measure->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE; // k_SPACING_REFINE_ONLY;
						x->r_ob.need_recompute_chords_double_onset = true;
						x->r_ob.notation_cursor.measure->need_recompute_beamings = true;
						set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
					}
					handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_TOGGLE_GRACE);
					break;
				}
				case 'n': // add new note to chord
				{
					if (x->r_ob.notation_cursor.chord) {
						add_note_to_chord_from_linear_edit(x, -1);
						if (x->r_ob.auto_jump_to_next_chord)
							linear_edit_jump_to_next_chord(x);
						force_inscreen_ux_rolling_while_editing(x);
						handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_ADD_NOTE);
						if (x->r_ob.playback_during_linear_editing && x->r_ob.notation_cursor.chord)
							send_chord_as_llll((t_notation_obj *) x, x->r_ob.notation_cursor.chord, 7, k_CONSIDER_FOR_DUMPING, -1);
					}
					break;
				}
                case JKEY_DELETE:
				case JKEY_BACKSPACE: // delete chord or note
				{
					t_chord *ch = x->r_ob.notation_cursor.chord ? x->r_ob.notation_cursor.chord : x->r_ob.notation_cursor.measure->lastchord;
					if (ch) {
						if (modifiers & eShiftKey){
							t_note *nt;
							for (nt = ch->firstnote; nt; nt = nt->next) {
								if (note_get_screen_midicents(nt) == x->r_ob.notation_cursor.midicents) {
									char num_notes = ch->num_notes;
									create_simple_notation_item_undo_tick((t_notation_obj *) x, num_notes > 1 ? (t_notation_item *)ch : (t_notation_item *)ch->parent, k_UNDO_MODIFICATION_CHANGE);
									note_delete((t_notation_obj *)x, nt, false);
									ch->need_recompute_parameters = true;
									
									if (x->r_ob.playback_during_linear_editing && ch)
										send_chord_as_llll((t_notation_obj *) x, ch, 7, k_CONSIDER_FOR_DUMPING, -1);
									
									if (num_notes == 1) { 		
                                        set_tuplet_levels_as_keep_levels(x->r_ob.notation_cursor.measure->rhythmic_tree);
                                        set_level_type_flag_for_level(x->r_ob.notation_cursor.measure->rhythmic_tree, k_RHYTHM_LEVEL_IGNORE);
										x->r_ob.notation_cursor.measure->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE; // k_SPACING_REFINE_ONLY;
										x->r_ob.need_recompute_chords_double_onset = true;
										x->r_ob.notation_cursor.measure->need_recompute_beamings = true;
										set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
									}
                                    
                                    x->r_ob.notation_cursor.measure->need_check_autocompletion = false;
									
									handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_DELETE_NOTE);
									break;
								}
							}
						} else {
							t_chord *nextchord = ch->next;
							
							create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)ch->parent, k_UNDO_MODIFICATION_CHANGE);
							
							chord_delete_from_measure((t_notation_obj *) x, ch, false);
							
							ch = nextchord;
							
                            set_tuplet_levels_as_keep_levels(x->r_ob.notation_cursor.measure->rhythmic_tree);
                            set_level_type_flag_for_level(x->r_ob.notation_cursor.measure->rhythmic_tree, k_RHYTHM_LEVEL_IGNORE);
							x->r_ob.notation_cursor.measure->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE; // k_SPACING_REFINE_ONLY;
							x->r_ob.need_recompute_chords_double_onset = true;
							x->r_ob.notation_cursor.measure->need_recompute_beamings = true;
							set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
							
							handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_DELETE_CHORD);
						}
					}
					break;
				}
                case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:	// 46 is the dot
				case 46: case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:	// 46 is the dot
				case JKEY_NUMPAD0: case JKEY_NUMPAD1: case JKEY_NUMPAD2: case JKEY_NUMPAD3: case JKEY_NUMPAD4: case JKEY_NUMPAD5: case JKEY_NUMPAD6: case JKEY_NUMPAD7: case JKEY_NUMPAD8: case JKEY_NUMPAD9:

					// numbers
					if (keycode == 46 && modifiers & eShiftKey)
						break;
					
                    if (keycode >= 0 && keycode <= 9) keycode += 48;
                    
					if (keycode == JKEY_NUMPAD0) keycode = 48;
					else if (keycode == JKEY_NUMPAD1) keycode = 49;
					else if (keycode == JKEY_NUMPAD2) keycode = 50;
					else if (keycode == JKEY_NUMPAD3) keycode = 51;
					else if (keycode == JKEY_NUMPAD4) keycode = 52;
					else if (keycode == JKEY_NUMPAD5) keycode = 53;
					else if (keycode == JKEY_NUMPAD6) keycode = 54;
					else if (keycode == JKEY_NUMPAD7) keycode = 55;
					else if (keycode == JKEY_NUMPAD8) keycode = 56;
					else if (keycode == JKEY_NUMPAD9) keycode = 57;
                    
					if (modifiers & eCommandKey && keycode > 49) {
						if (x->r_ob.notation_cursor.chord) {
							create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.notation_cursor.measure, k_UNDO_MODIFICATION_CHANGE);
							lock_general_mutex((t_notation_obj *)x);
							split_chord(x, x->r_ob.notation_cursor.chord, keycode - 48, x->r_ob.notation_cursor.chord->parent->lock_rhythmic_tree || x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_TAKE_FOR_GRANTED);
							perform_analysis_and_change(x, NULL, NULL, x->r_ob.tree_handling == k_RHYTHMIC_TREE_HANDLING_IGNORE ? k_BEAMING_CALCULATION_FROM_SCRATCH : k_BEAMING_CALCULATION_DONT_CHANGE_LEVELS);
							unlock_general_mutex((t_notation_obj *)x);
							x->r_ob.need_recompute_chords_double_onset = true;
							set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
							handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LINEAR_EDIT_SPLIT_CHORD);
						}
					} else if (modifiers & eAltKey && keycode > 50 && keycode != 56 && keycode != 52) {	// no 4, no 8
						if (x->r_ob.defining_numerator && x->r_ob.num_speedy_tuplets >= 0) {
							x->r_ob.speedy_tuplet_numerator_defined[x->r_ob.num_speedy_tuplets - 1] = true;
							x->r_ob.speedy_tuplet[x->r_ob.num_speedy_tuplets - 1].r_num = keycode - 48;
							invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
						} else if (x->r_ob.num_speedy_tuplets >= 0 && x->r_ob.num_speedy_tuplets < CONST_MAX_NESTED_TUPLETS_FOR_SPEEDY){
							t_urrational this_ratio = genurrat(1, 1);
							switch (keycode - 48) {
								case 2: 
									this_ratio = genurrat(3, 2);
									break;
								case 3: 
									this_ratio = genurrat(2, 3);
									break;
								case 4: 
									this_ratio = genurrat(3, 4);
									break;
								case 5: 
									this_ratio = genurrat(4, 5);
									break;
								case 6: 
									this_ratio = genurrat(4, 6);
									break;
								case 7: 
									this_ratio = genurrat(4, 7);
									break;
								case 8: 
									this_ratio = genurrat(6, 8);
									break;
								case 9: 
									this_ratio = genurrat(8, 9);
									break;
							}
							x->r_ob.speedy_tuplet_numerator_defined[x->r_ob.num_speedy_tuplets] = false;
							x->r_ob.speedy_tuplet[x->r_ob.num_speedy_tuplets] = this_ratio;
							x->r_ob.num_speedy_tuplets++;
							invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
						}
					} else {
						if (keycode == 46 || (keycode > 48 && keycode < 57)) {
							t_chord *edited_chord = NULL;
							char was_last = false;
							
							if (keycode == 46) {
								verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before3"), 1);
								was_last = (!x->r_ob.notation_cursor.chord || !x->r_ob.notation_cursor.chord->next);
								edited_chord = x->r_ob.notation_cursor.chord ? x->r_ob.notation_cursor.chord : (x->r_ob.notation_cursor.measure ? x->r_ob.notation_cursor.measure->lastchord : NULL);
								if (edited_chord) {
                                    create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.notation_cursor.measure, k_UNDO_MODIFICATION_CHANGE);
                                    edited_chord->r_sym_duration = rat_rat_prod(edited_chord->r_sym_duration, genrat(3, 2));
                                    edited_chord->dont_split_for_ts_boxes = true;
                                    calculate_chord_parameters((t_notation_obj *) x, edited_chord, get_voice_clef((t_notation_obj *)x, (t_voice *)edited_chord->parent->voiceparent), true);
                                    set_tuplet_levels_as_keep_levels(x->r_ob.notation_cursor.measure->rhythmic_tree);
                                    set_level_type_flag_for_level(x->r_ob.notation_cursor.measure->rhythmic_tree, k_RHYTHM_LEVEL_IGNORE);
									handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_LINEAR_EDIT_ADD_DOT);
								}
							} else {
								verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before4"), 1);
								create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)x->r_ob.notation_cursor.measure, k_UNDO_MODIFICATION_CHANGE);
								if (x->r_ob.notation_cursor.chord && !(modifiers & eControlKey)) {
									char sign = isign(x->r_ob.notation_cursor.chord->r_sym_duration.r_num);
									x->r_ob.notation_cursor.chord->r_sym_duration = rat_long_prod(number_to_duration_for_linear_edit(keycode - 48), sign);
									x->r_ob.notation_cursor.chord->dont_split_for_ts_boxes = true;
									x->r_ob.notation_cursor.chord->need_recompute_parameters = true;
									set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
									edited_chord = x->r_ob.notation_cursor.chord;
									handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_LINEAR_EDIT_CHANGE_CHORD_DURATION);
								} else {
									// append new chord in measure
									t_chord *after_this_chord = NULL;
									was_last = (!(modifiers & eShiftKey) && (!x->r_ob.notation_cursor.chord || !x->r_ob.notation_cursor.chord->next));
									if (!x->r_ob.notation_cursor.chord)
										after_this_chord = x->r_ob.notation_cursor.measure->lastchord;
									else if (x->r_ob.notation_cursor.chord->prev)
										after_this_chord = x->r_ob.notation_cursor.chord->prev;
									edited_chord = add_new_chord_in_measure_from_linear_edit(x, after_this_chord, keycode - 48, x->r_ob.force_diatonic_step);
									edited_chord->dont_split_for_ts_boxes = true;
									handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_LINEAR_EDIT_ADD_CHORD);
									x->r_ob.force_diatonic_step = -1;
								}
								calculate_chord_parameters((t_notation_obj *) x, edited_chord, get_voice_clef((t_notation_obj *)x, (t_voice *)edited_chord->parent->voiceparent), true);
                                set_tuplet_levels_as_keep_levels(x->r_ob.notation_cursor.measure->rhythmic_tree);
                                verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before4b"), 1);
                                if (!is_level_tuplet(x->r_ob.notation_cursor.measure->rhythmic_tree))
                                    set_level_type_flag_for_level(x->r_ob.notation_cursor.measure->rhythmic_tree, k_RHYTHM_LEVEL_IGNORE);
								verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before4c"), 1);
							}
							x->r_ob.need_recompute_chords_double_onset = true;
							x->r_ob.notation_cursor.measure->need_recompute_beamings = true;
							x->r_ob.notation_cursor.measure->tuttipoint_reference->need_recompute_spacing = k_SPACING_RECALCULATE; // k_SPACING_REFINE_ONLY;
							set_need_perform_analysis_and_change_flag((t_notation_obj *)x);
							
							if (x->r_ob.playback_during_linear_editing && edited_chord && keycode != 46) {
								t_rational r_chord_dur = get_rat_durations_sec_between_timepoints((t_notation_obj *)x, edited_chord->parent->voiceparent, 
																								  build_timepoint(edited_chord->parent->measure_number, edited_chord->r_sym_onset), build_timepoint(edited_chord->parent->measure_number, rat_rat_sum(edited_chord->r_sym_onset, rat_abs(edited_chord->r_sym_duration))));
								edited_chord->duration_ms = 1000 * rat2double(r_chord_dur);
                                edited_chord->r_duration_sec = r_chord_dur;
                                edited_chord->play_r_duration_sec = r_chord_dur;
								if (edited_chord->prev)
									edited_chord->onset = edited_chord->prev->onset + edited_chord->prev->duration_ms;
								else
									edited_chord->onset = edited_chord->parent->tuttipoint_reference->onset_ms + 1000 * rat2double(edited_chord->parent->r_tuttipoint_onset_sec);
								send_chord_as_llll((t_notation_obj *) x, edited_chord, 7, k_CONSIDER_FOR_DUMPING, -1);
							}
							
							verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before4d"), 1);
							
							perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_DONT_CHANGE_TIES + k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
							
							verbose_post_rhythmic_tree((t_notation_obj *)x, x->firstvoice->lastmeasure, gensym("before4e"), 1);
							
							if (x->r_ob.auto_jump_to_next_chord) {
								if (was_last)	// chord might have been "split" by retranscription
									x->r_ob.notation_cursor.chord = x->r_ob.notation_cursor.measure->lastchord;
								linear_edit_jump_to_next_chord(x);
							}
							
							force_inscreen_ux_rolling_while_editing(x);
							
							handle_change((t_notation_obj *)x, k_CHANGED_STANDARD_SEND_BANG, k_UNDO_OP_UNKNOWN);
						}
					}
					break;
				default:
					break;
			}
		}
		
		if (keycode == 46 && modifiers & eShiftKey) {
			x->r_ob.defining_numerator = true;
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
		} else 
			x->r_ob.defining_numerator = false;
		
		if (!(modifiers & eCommandKey) || (keycode != 'c' && keycode != 'v'))
			return 1;
		
		if (x->r_ob.notation_cursor.measure)
			llll_check(x->r_ob.notation_cursor.measure->rhythmic_tree);
	}
	
	
	if (!(modifiers & eCommandKey) && !(modifiers & eAltKey) && !(modifiers & eControlKey) && is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) {
		for (j=0; j<CONST_MAX_SLOTS; j++) {
			if (x->r_ob.slotinfo[j].slot_key == textcharacter) {
				// change slot view
				// detect the selection type
                
                select_only_first_item_if_tied_sequence_is_selected(x);
                
				if ((x->r_ob.num_selecteditems == 1) && ((x->r_ob.firstselecteditem->type == k_NOTE) || (x->r_ob.firstselecteditem->type == k_CHORD))) 	{
					open_slot_window((t_notation_obj *) x, j, notation_item_to_notation_item_for_slot_win_opening((t_notation_obj *)x, x->r_ob.firstselecteditem));
					return 1;
				}
			}
		}
	}
	
	if (keycode == JKEY_TAB && x->r_ob.active_slot_num > -1 && x->r_ob.active_slot_notationitem && is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) {
		if (modifiers == eShiftKey){
			// shift+tab on a slot goes to the next one
			open_slot_window((t_notation_obj *) x, (x->r_ob.active_slot_num + 1) % CONST_MAX_SLOTS, x->r_ob.active_slot_notationitem);
			return 1;
		} else if (modifiers == (eShiftKey | eAltKey)) {
			// shift+tab on a slot goes to the next one
			open_slot_window((t_notation_obj *) x, MAX(0, x->r_ob.active_slot_num - 1), x->r_ob.active_slot_notationitem);
			return 1;
		}
	}
	
	if (!(modifiers & eCommandKey) && !(modifiers & eAltKey) && !(modifiers & eControlKey)) {
		for (j=0; j<CONST_MAX_COMMANDS; j++) {
			if (x->r_ob.command_key[j] == textcharacter) {
				// send command values
				selection_send_command(x, modifiers, j, true);
				return 1;
			}
		}
	}
	
	if (is_editable((t_notation_obj *)x, k_SELECTION, k_SINGLE_SELECTION)) {
		if (keycode == JKEY_LEFTARROW && modifiers == eCommandKey) { // change selection
			t_notation_item *first_sel_item = get_leftmost_selected_notation_item((t_notation_obj *)x);
			t_notation_item *to_select = first_sel_item ? notation_item_get_at_left((t_notation_obj *)x, first_sel_item, false) : NULL;
			if (to_select) {
				select_single_notation_item_and_force_inscreen((t_notation_obj *)x, to_select);
				return 1;
			}
		} else if (keycode == JKEY_RIGHTARROW && modifiers == eCommandKey) { // change selection
			t_notation_item *last_sel_item = get_rightmost_selected_notation_item((t_notation_obj *)x);
			t_notation_item *to_select = last_sel_item ? notation_item_get_at_right((t_notation_obj *)x, last_sel_item, false) : NULL;
			if (to_select) {
				select_single_notation_item_and_force_inscreen((t_notation_obj *)x, to_select);
				return 1;
			}
		} else if (keycode == JKEY_UPARROW && modifiers == eCommandKey) { // change selection
			t_notation_item *first_sel_item = get_leftmost_selected_notation_item((t_notation_obj *)x);
			t_notation_item *to_select = first_sel_item ? notation_item_get_at_top((t_notation_obj *)x, first_sel_item, false) : NULL;
			if (to_select) {
				select_single_notation_item_and_force_inscreen((t_notation_obj *)x, to_select);
				return 1;
			}
		} else if (keycode == JKEY_DOWNARROW && modifiers == eCommandKey) { // change selection
			t_notation_item *last_sel_item = get_rightmost_selected_notation_item((t_notation_obj *)x);
			t_notation_item *to_select = last_sel_item ? notation_item_get_at_bottom((t_notation_obj *)x, last_sel_item, false) : NULL;
			if (to_select) {
				select_single_notation_item_and_force_inscreen((t_notation_obj *)x, to_select);
				return 1;
			}
		}
	}
	
	if (x->r_ob.need_hscrollbar && x->r_ob.show_hscrollbar && is_editable((t_notation_obj *)x, k_SCROLLBAR, k_ELEMENT_ACTIONS_NONE)) {
		switch (keycode) { // scrollbar scrolling (with command key)
			case JKEY_LEFTARROW:
				if (modifiers & eCommandKey){ // scroll
					if (modifiers & eShiftKey)
						x->r_ob.hscrollbar_x -= CONST_FASTER_FROM_KEYBOARD * CONST_UX_SCROLLBAR_SHIFT_FROM_KEYBOARD * x->r_ob.zoom_y;
					else
						x->r_ob.hscrollbar_x -= CONST_UX_SCROLLBAR_SHIFT_FROM_KEYBOARD * x->r_ob.zoom_y;
					redraw_hscrollbar((t_notation_obj *)x, 0);
					return 1;
				}
				case JKEY_RIGHTARROW:
				if (modifiers & eCommandKey){ // scroll
					if (modifiers & eShiftKey)
						x->r_ob.hscrollbar_x += CONST_FASTER_FROM_KEYBOARD * CONST_UX_SCROLLBAR_SHIFT_FROM_KEYBOARD * x->r_ob.zoom_y;
					else
						x->r_ob.hscrollbar_x += CONST_UX_SCROLLBAR_SHIFT_FROM_KEYBOARD * x->r_ob.zoom_y;
					redraw_hscrollbar((t_notation_obj *)x, 0);
					return 1;
				}
		}
	}
	
	/*
	if (x->r_ob.selection_type == k_DURATION_TAIL) { // only duration tails selected!
		switch (keycode) {
            case JKEY_DELETE:
			case JKEY_BACKSPACE: {
					// return to normal position!
					if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_PITCH)) return 0;
					reset_selection_tail_gliss((t_notation_obj *) x);
					handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_RESET_TAIL_SLOPE); 
					return 1;
				}
			case JKEY_UPARROW:
			case JKEY_DOWNARROW:
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_PITCH)) return 0;
				change_selection_breakpoint_pitch((t_notation_obj *)x, (keycode == JKEY_UPARROW ? 1 : -1) * (modifiers & eShiftKey ? 1200 : 200 / x->r_ob.tone_division));
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, keycode == JKEY_UPARROW ? k_UNDO_OP_SHIFT_PITCH_UP_FOR_SELECTION : k_UNDO_OP_SHIFT_PITCH_DOWN_FOR_SELECTION); 
				return 1;
		}
	} else */
	if (x->r_ob.selection_type == k_PITCH_BREAKPOINT) { // only bpts selected!
		switch (keycode) {
            case JKEY_DELETE:
			case JKEY_BACKSPACE:
				// delete breakpoints
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_DELETION)) 
					return 0;
				reset_selection_tail_gliss((t_notation_obj *) x);
				delete_breakpoints_in_selection((t_notation_obj *) x);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_PITCH_BREAKPOINTS_IN_SELECTION); 
				return 1;
			case JKEY_ESC:
				// put the slope at 0.
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_GENERIC)) return 0;
				reset_selection_breakpoint_slope((t_notation_obj *) x);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_RESET_PITCH_BREAKPOINTS_SLOPE_FOR_SELECTION); 
				return 1;
			case JKEY_UPARROW:
			case JKEY_DOWNARROW:
				if (!is_editable((t_notation_obj *)x, k_PITCH_BREAKPOINT, k_MODIFICATION_PITCH)) return 0;
				change_selection_breakpoint_pitch((t_notation_obj *)x, (keycode == JKEY_UPARROW ? 1 : -1) * (modifiers & eShiftKey ? 1200 : 200 / x->r_ob.tone_division));
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, keycode == JKEY_UPARROW ? k_UNDO_OP_SHIFT_PITCH_UP_FOR_SELECTION : k_UNDO_OP_SHIFT_PITCH_DOWN_FOR_SELECTION); 
				return 1;
		}
	} else if (x->r_ob.selection_type == k_MEASURE) { // only measures selected + BACKSPACE 
		if (keycode == JKEY_BACKSPACE || keycode == JKEY_DELETE) {
            if (modifiers & eShiftKey) {
                if (!is_editable((t_notation_obj *)x, k_MEASURE, k_DELETION)) return 0;
                delete_selected_measures(x);
                handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_MEASURES_IN_SELECTION);
                return 1;
            } else {
                if (!is_editable((t_notation_obj *)x, k_CHORD, k_DELETION)) return 0;
                clear_selected_measures(x);
                handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CLEAR_MEASURES_IN_SELECTION);
                return 1;
            }
		} else if ((modifiers & eCommandKey) && (keycode == 100)) { // only measures selected + Cmd+D
			if (!is_editable((t_notation_obj *)x, k_MEASURE, k_CREATION)) return 0;
			duplicate_selected_measures(x);
			handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DUPLICATE_SELECTED_MEASURES); 
			return 1;
		} else if ((keycode == JKEY_LEFTARROW || keycode == JKEY_RIGHTARROW || keycode == JKEY_UPARROW || keycode == JKEY_DOWNARROW) && x->r_ob.j_last_mousedown_obj_type == k_MEASURE){
			t_measure *meas = (t_measure *) x->r_ob.j_last_mousedown_ptr;
			if (!is_editable((t_notation_obj *)x, k_SELECTION, k_ELEMENT_ACTIONS_NONE)) return 0;
			if (meas) {
				t_measure *meas_to_add = NULL;
				switch (keycode) {
					case JKEY_LEFTARROW:
						meas_to_add = meas->prev;
						break;
					case JKEY_RIGHTARROW:
						meas_to_add = meas->next;
						break;
					case JKEY_UPARROW:
						meas_to_add = (meas->voiceparent->prev ? nth_measure_of_scorevoice(meas->voiceparent->prev, meas->measure_number) : NULL);
						break;
					case JKEY_DOWNARROW:
						meas_to_add = (meas->voiceparent->next && meas->voiceparent->next->v_ob.number < x->r_ob.num_voices ? 
									   nth_measure_of_scorevoice(meas->voiceparent->next, meas->measure_number) : NULL);
						break;
				}
				
				if (meas_to_add) {
					lock_general_mutex((t_notation_obj *)x);	
					if (!(modifiers & eShiftKey)) {
						clear_preselection((t_notation_obj *) x);
						clear_selection((t_notation_obj *) x);
					}
					notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)meas_to_add);
					move_preselecteditems_to_selection((t_notation_obj *) x, k_SELECTION_MODE_FORCE_SELECT, false, false);
					set_mousedown((t_notation_obj *) x, meas_to_add, k_MEASURE);
					invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
					unlock_general_mutex((t_notation_obj *)x);	
				}
			}
		}
	} else if (x->r_ob.selection_type == k_MEASURE_END_BARLINE && (keycode == JKEY_BACKSPACE || keycode == JKEY_DELETE) && modifiers == eShiftKey) {
		if (!is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_GENERIC)) return 0;
		reset_selected_measures_local_spacing_width_multiplier((t_notation_obj *)x);
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_RESET_LOCAL_SPACING_FOR_SELECTION); 
		return 1;
	} else if ((x->r_ob.selection_type == k_TEMPO) && (keycode == JKEY_BACKSPACE || keycode == JKEY_DELETE)) { // only tempi selected + BACKSPACE
		if (!is_editable((t_notation_obj *)x, k_TEMPO, k_DELETION)) return 0;
		delete_selected_tempi(x); 
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_SELECTED_TEMPI); 
		return 1;
	} else if ((x->r_ob.selection_type == k_TEMPO) && (modifiers & eCommandKey && modifiers & eAltKey && modifiers & eShiftKey) && (keycode == 105)) { // only tempi selected + Cmd+Alt+Shift+I 
		if (!is_editable((t_notation_obj *)x, k_TEMPO, k_MODIFICATION_GENERIC)) return 0;
		switch_interpolation_to_selected_tempi(x); 
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_TOGGLE_INTERPOLATION_FOR_SELECTED_TEMPI); 
		return 1;
	} else if (x->r_ob.selection_type == k_ARTICULATION && (keycode == JKEY_BACKSPACE || keycode == JKEY_DELETE)) { // only measures selected + BACKSPACE
		if (!is_editable((t_notation_obj *)x, k_ARTICULATION, k_DELETION)) return 0;
		delete_articulations_in_selection((t_notation_obj *) x);
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_ARTICULATIONS_FOR_SELECTION); 
		return 1;
	} else if ((x->r_ob.selection_type == k_SLUR_START_POINT || x->r_ob.selection_type == k_SLUR_END_POINT || x->r_ob.selection_type == k_SLUR) && (keycode == JKEY_BACKSPACE || keycode == JKEY_DELETE)) {
		if (!is_editable((t_notation_obj *)x, k_SLUR, k_DELETION)) return 0;
		delete_slurs_in_selection(x);
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_SLURS_FOR_SELECTION); 
		return 1;
	} 
	
	if (handle_keys_for_articulations((t_notation_obj *) x, patcherview, keycode, modifiers, textcharacter) && is_editable((t_notation_obj *)x, k_ARTICULATION, k_CREATION)) {
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_ADD_ARTICULATION_TO_SELECTION); 
		return 1;
	}
	
	if (modifiers & eControlKey && keycode >= 'a' && keycode <= 'g' && is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH)) {
		change_pitch_to_selection_from_diatonic_step((t_notation_obj *) x, keycode == 'a' ? 5 : (keycode == 'b' ? 6 : keycode - 'c'));
		handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_CHANGE_PITCH); 
		return 1;
	}

	// mixed or notes/chord selection
	switch (keycode) {
		case JKEY_UPARROW:
			// shift note up
			if (!(modifiers & (eCommandKey | eAltKey)) && is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH)) {
				change_pitch_for_selection(x, (!(modifiers & eCommandKey) && (modifiers & eShiftKey)) ? 6 * x->r_ob.tone_division : 1, 0, ((modifiers & eControlKey) && (modifiers & eShiftKey)), true);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_SHIFT_PITCH_UP_FOR_SELECTION); 
				return 1;
			}
			return 0;
			break;
		case JKEY_DOWNARROW:
			// shift note down
			if (!(modifiers & (eCommandKey | eAltKey)) && is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH)) {
				change_pitch_for_selection(x, (!(modifiers & eCommandKey) && (modifiers & eShiftKey)) ? -6 * x->r_ob.tone_division : -1, 0, ((modifiers & eControlKey) && (modifiers & eShiftKey)), true);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_SHIFT_PITCH_DOWN_FOR_SELECTION); 
				return 1;
			}
			return 0;
			break;
        case JKEY_DELETE:
		case JKEY_BACKSPACE:
			// delete notes/chords in selection
			if (x->r_ob.active_slot_num >= 0) {
				if (is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) {
					if (x->r_ob.selected_slot_items->l_size > 0) {
						create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, get_activeitem_undo_item((t_notation_obj *)x), k_CHORD, k_UNDO_MODIFICATION_CHANGE);
						delete_all_selected_function_points((t_notation_obj *)x, x->r_ob.active_slot_num);
						handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DELETE_SLOT_CONTENT);
					}
				}
			} else { 
				
				if (is_editable((t_notation_obj *)x, k_MARKER, k_DELETION))
					delete_selected_markers(x); // x->r_ob.selection_type
                t_llll *slots_to_transfer = get_default_slots_to_transfer_1based((t_notation_obj *)x);
                turn_selection_into_rests(x, is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_DELETION), is_editable((t_notation_obj *)x, k_LYRICS, k_ELEMENT_ACTIONS_NONE), is_editable((t_notation_obj *)x, k_DYNAMICS, k_ELEMENT_ACTIONS_NONE), slots_to_transfer, false);
                llll_free(slots_to_transfer);
				invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_TURN_SELECTION_INTO_RESTS); 
			}
			return 1;
			break;
		case JKEY_ESC:
			// return to normal view
            if ((x->r_ob.active_slot_notationitem || x->r_ob.active_slot_num >= 0) && is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) {
				close_slot_window((t_notation_obj *)x);
				return 1;
			}
			return 0;
			break;
		case JKEY_TAB:
			if (is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)) {
				rebeam_levels_of_selected_tree_nodes(x, modifiers & eAltKey, !(modifiers & eControlKey), k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, modifiers & eAltKey ? k_UNDO_OP_AUTO_RHYTHMIC_TREE_KEEPING_EXISTING_TUPLETS_FOR_SELECTION : k_UNDO_OP_AUTO_RHYTHMIC_TREE_IGNORING_EXISTING_TUPLETS_FOR_SELECTION); 
				return 1;
			}
			return 0;
			break;
		case 'y': // letter y: join
			if (modifiers & eCommandKey && is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_CREATION) && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE) ) {
				if (modifiers & eShiftKey){
//					rebeam_levels_of_selected_tree_nodes(x, modifiers & eAltKey);
//					handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, modifiers & eAltKey ? k_UNDO_OP_AUTO_RHYTHMIC_TREE_KEEPING_EXISTING_TUPLETS_FOR_SELECTION : k_UNDO_OP_AUTO_RHYTHMIC_TREE_IGNORING_EXISTING_TUPLETS_FOR_SELECTION); 
//				} else {
					quick_merge_selection(x);
					handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_JOIN_SELECTION); 
					return 1;
				}
			}
			return 0;
			break;
            
        case 'd':
            if (modifiers & eCommandKey && modifiers & eShiftKey) {
                if (!is_editable((t_notation_obj *)x, k_DYNAMICS, k_ELEMENT_ACTIONS_NONE))
                    return 0;
                t_chord *ch = get_first_selected_chord((t_notation_obj *) x);
                if (!ch) {
                    t_dynamics *dy = get_first_selected_dynamics((t_notation_obj *) x);
                    if (dy)
                        ch = dy->owner;
                }
                if (ch && x->r_ob.show_dynamics && x->r_ob.link_dynamics_to_slot > 0)
                    start_editing_dynamics((t_notation_obj *) x, patcherview, ch);
                invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
            }
            return 1;

            break;
            
		case 'f': // Cmd+Shift+F: Fix tree
			if (modifiers & eCommandKey && modifiers & eShiftKey && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)) {
				fix_levels_of_selected_tree_nodes_as_original(x); 
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_STICK_RHYTHMIC_TREE_FOR_SELECTION); 
				return 1;
			}
			return 0;
			break;
		case 'h': // Cmd+Shift+H: Splatter tree
			if (modifiers & eCommandKey && modifiers & eShiftKey && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)) {
				splatter_selected_tree_nodes(x, true, true, true);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DESTROY_RHYTHMIC_TREE_FOR_SELECTION); 
				return 1;
			}
			return 0;
			break;
		case 'g': // Cmd+Shift+G: Group tree
			if (modifiers & eCommandKey && modifiers & eShiftKey && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)) {
				create_level_for_selected_tree_nodes(x);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_ADD_RHYTHMIC_TREE_LEVELS_FROM_SELECTION); 
				return 1;
			}
			return 0;
			break;
		case 't': // letter t: 
			if (modifiers & eCommandKey && modifiers & eShiftKey && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE) && x->r_ob.allow_lock) { 
				if (x->r_ob.selection_type == k_MEASURE) {
					lock_unlock_rhythmic_trees_in_selection((t_notation_obj *) x);
					handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LOCK_UNLOCK_RHYTHMIC_TREES_FOR_SELECTION); 
				}
				return 1;
			} else if (modifiers & eCommandKey && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_GENERIC)) {// tie 
				tie_untie_selection(x);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_TIE_UNTIE_SELECTION); 
				return 1;
			}
			return 0;
			break;
		case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57: 
			// change slot view
			// detect the selection type
			if (modifiers & eCommandKey && keycode > 48 && is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_CREATION) && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)) {
				split_selection(x, keycode - 48, !(modifiers & eAltKey));  // split selection - ALSO WORKS WITH TREES
				if (!(modifiers & eShiftKey))
					rebeam_levels_of_selected_tree_nodes(x, false, false, k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_SPLIT_SELECTION); 
				return 1;
            } else if (!(modifiers & eCommandKey) && !(modifiers & eAltKey) && !(modifiers & eControlKey)) {
                 select_only_first_item_if_tied_sequence_is_selected(x);
                 if (((x->r_ob.num_selecteditems == 1) && ((x->r_ob.firstselecteditem->type == k_NOTE) || (x->r_ob.firstselecteditem->type == k_CHORD))) && is_editable((t_notation_obj *)x, k_SLOT, k_ELEMENT_ACTIONS_NONE)) {
                    open_slot_window((t_notation_obj *) x, (keycode == 48) ? 9 : keycode - 49, notation_item_to_notation_item_for_slot_win_opening((t_notation_obj *)x, x->r_ob.firstselecteditem));
                    return 1;
                }
			}
			return 0;
			break; 
		case 'c': // Cmd+C
			if (modifiers & eCommandKey && x->r_ob.allow_copy_paste) {
				// copy!
				if (x->r_ob.active_slot_num >= 0 && x->r_ob.active_slot_notationitem) { // we copy the slot
					if (x->r_ob.selected_slot_items->l_size > 0)
						notation_obj_copy_slot_selection((t_notation_obj *)x, &clipboard, x->r_ob.active_slot_notationitem, x->r_ob.active_slot_num, keycode == 'x');
					else
						notation_obj_copy_slot((t_notation_obj *)x, &clipboard, x->r_ob.active_slot_notationitem, (modifiers & eShiftKey) ? -1 : x->r_ob.active_slot_num, keycode == 'x');
				} else if (x->r_ob.selection_type == k_MEASURE) { // we copy the measures
					score_copy_selected_measures(x, keycode == 'x');
				}
				return 1;
			}
			return 0;
			break;
		case 'V': case 'v': // letter V or v
			if (!(modifiers & eCommandKey))
				evaluate_selection(x, modifiers, true);
			else if (modifiers & eCommandKey && clipboard.gathered_syntax && (clipboard.object == k_NOTATION_OBJECT_SCORE || clipboard.object == k_NOTATION_OBJECT_ANY) && clipboard.gathered_syntax->l_size > 0 && x->r_ob.allow_copy_paste) {
				// paste!
				if (clipboard.type == k_SLOT) {
					notation_obj_paste_slot((t_notation_obj *) x, &clipboard, (x->r_ob.active_slot_num < 0 || clipboard.gathered_syntax->l_size > 1) ? -1 : x->r_ob.active_slot_num);
				} else if (clipboard.type == k_SLOT_SELECTION) {
					notation_obj_paste_slot_selection_to_open_slot_window((t_notation_obj *) x, &clipboard, !(modifiers & eControlKey));
                } else if (clipboard.type == k_GATHERED_SYNTAX) {
                    if (x->r_ob.selection_type == k_MEASURE)
                        score_paste_replace_measures(x, modifiers & eShiftKey);
				} else if (clipboard.type == k_MEASURE) {
					if (x->r_ob.selection_type == k_MEASURE)
						score_paste_replace_single_measures(x, modifiers & eShiftKey); // DEPRECATED, should not be used
				}
				invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			}
			return 1;
			break;
		case 'l': // Cmd+L
			if (modifiers & eCommandKey) {
				if (x->r_ob.num_selecteditems == 0) {
					if (modifiers & eShiftKey) {
						x->r_ob.use_loop_region = x->r_ob.use_loop_region ? false : true;
						send_loop_region_on_off((t_notation_obj *)x, 7);
						if (x->r_ob.use_loop_region && x->r_ob.show_loop_region)
							send_loop_region((t_notation_obj *)x, 7);
						if (x->r_ob.playing)
							check_correct_scheduling((t_notation_obj *)x, true);
						invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
					} else {
						x->r_ob.show_loop_region = x->r_ob.show_loop_region ? false : true;
						send_loop_region_on_off((t_notation_obj *)x, 7);
						if (x->r_ob.use_loop_region && x->r_ob.show_loop_region)
							send_loop_region((t_notation_obj *)x, 7);
						if (x->r_ob.playing)
							check_correct_scheduling((t_notation_obj *)x, true);
						invalidate_notation_static_layer_and_repaint((t_notation_obj *)x);
					}
				} else {
					if (modifiers & eShiftKey) {
						if (x->r_ob.selection_type == k_MEASURE) {
							if (is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_GENERIC) && x->r_ob.allow_barline_lock) {
								fix_unfix_barlines_in_selection((t_notation_obj *) x);
								handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_FIX_UNFIX_MEASURE_WIDTH_FOR_SELECTION);
								remove_all_tuttipoints_flag_modified(x);
							}
						} else {
							// getting first selected chord
							if (is_editable((t_notation_obj *)x, k_LYRICS, k_CREATION)) {
								t_chord *ch = get_first_selected_chord((t_notation_obj *) x);
                                if (!ch) {
                                    t_lyrics *ly = get_first_selected_lyrics((t_notation_obj *) x);
                                    if (ly)
                                        ch = ly->owner;
                                }
								if (ch && ch->r_sym_duration.r_num >= 0 && x->r_ob.show_lyrics && x->r_ob.link_lyrics_to_slot > 0)
									start_editing_lyrics((t_notation_obj *) x, patcherview, ch);
							}
						}
					} else {
						if (x->r_ob.allow_lock) {
							lock_unlock_selection((t_notation_obj *) x, true);
							handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_LOCK_UNLOCK_SELECTION);
							remove_all_tuttipoints_flag_modified(x);
						}
					}
				}
			}
			invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
			return 1;
			break;
            
		case 'u': // Cmd+U
			if (modifiers & eCommandKey && x->r_ob.allow_mute) {
				mute_unmute_selection((t_notation_obj *) x, true);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_MUTE_UNMUTE_SELECTION);
				return 1;
			}
			return 0;
			break;
            
		case 'j': // Cmd+J or Cmd+Shift+J
            if (modifiers & eCommandKey && modifiers & eShiftKey && is_editable((t_notation_obj *)x, k_MEASURE, k_MODIFICATION_RHYTHMIC_TREE)) {
                destroy_selected_tree_nodes(x, true, true, true);
                handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_DESTROY_RHYTHMIC_TREE_LEVEL_FOR_SELECTION);
                return 1;
            } else if (modifiers & eCommandKey && x->r_ob.allow_solo) {
				solo_unsolo_selection((t_notation_obj *) x, true);
				handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_SOLO_UNSOLO_SELECTION);
				return 1;
			}
			return 0;
			break;
            
		case 'r': // Cmd+R
            if (modifiers & eCommandKey) {
                if (is_editable((t_notation_obj *)x, k_NOTE_OR_CHORD, k_MODIFICATION_PITCH)){
                    if (modifiers & eShiftKey) {
                        snap_pitch_to_grid_for_selection((t_notation_obj *) x);
                        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_SNAP_PITCH_TO_GRID_FOR_SELECTION);
                        return 1;
                    } else {
                        enharmonically_respell_selection((t_notation_obj *) x);
                        handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_RESET_ENHARMONICITY_FOR_SELECTION);
                        return 1;
                    }
                }
            }
            return 0;
			break;
		case 'a': // Cmd+A
			if (modifiers & eCommandKey && is_editable((t_notation_obj *)x, k_SELECTION, k_MULTIPLE_SELECTION)) {
				select_all(x);
				return 1;
			}
			return 0;
			break;
		case 'p': // Cmd+P // show-hide playcursor
			if (modifiers & eCommandKey && is_editable((t_notation_obj *)x, k_PLAYCURSOR, k_ELEMENT_ACTIONS_NONE)) {
				x->r_ob.show_playhead = (x->r_ob.show_playhead) ? false : true;
                notationobj_redraw((t_notation_obj *) x);
				return 1;
			}
			return 0;
			break;
	}
	
	if (textcharacter == 32 && x->r_ob.allow_play_from_interface) {
		if (x->r_ob.playing) {
			if (modifiers == eShiftKey) { // acts like a "pause" command: playhead stays visible where it is
				x->r_ob.show_playhead = true;
				x->r_ob.play_head_start_ms = x->r_ob.play_head_ms;
				x->r_ob.play_head_start_ux = x->r_ob.play_head_ux;
                score_stop(x, _llllobj_sym_pause, 0, NULL);
			} else
                score_stop(x, NULL, 0, NULL);
		} else {
			if (modifiers == eShiftKey)
				score_playselection(x, NULL, 0, NULL);
			else
				score_play(x, NULL, 0, NULL);
		}
		return 1;
	}
	return 0;
}

void evaluate_selection(t_score *x, long modifiers, char alsosortselectionbyonset, t_llll *forced_routers)
{
    if (alsosortselectionbyonset)
        sort_selection((t_notation_obj *)x, false);

    // detect the selection type
	if ((modifiers & eShiftKey) && (modifiers & eAltKey)) { // send all values
		send_all_values_as_llll(x, -1); // dump all
	} else if (modifiers & eShiftKey) { // send this chord values
		if ((x->r_ob.num_selecteditems == 1) && (x->r_ob.firstselecteditem->type == k_NOTE)) 
			send_chord_as_llll((t_notation_obj *) x, ((t_note *)x->r_ob.firstselecteditem)->parent, 7, k_CONSIDER_FOR_DUMPING, -1, forced_routers);
		else if ((x->r_ob.num_selecteditems == 1) && (x->r_ob.firstselecteditem->type == k_CHORD))
			send_chord_as_llll((t_notation_obj *) x, (t_chord *)x->r_ob.firstselecteditem, 7, k_CONSIDER_FOR_DUMPING, -1, forced_routers);
	} else { // send selection values
		if (standard_dump_selection((t_notation_obj *) x, 7, -1, (delete_item_fn)score_sel_delete_item, forced_routers)) {
			lock_general_mutex((t_notation_obj *)x);
			perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
			unlock_general_mutex((t_notation_obj *)x);
		}
	}
}

void selection_send_command(t_score *x, long modifiers, long command_number, char alsosortselectionbyonset)
{
    if (alsosortselectionbyonset)
        sort_selection((t_notation_obj *)x, false);

    
	if (command_number == -1 && modifiers & eShiftKey) {
        // chord-wise dump but not for commands: we might wanna define a command for Shift+Something!
		if ((x->r_ob.num_selecteditems == 1) && (x->r_ob.firstselecteditem->type == k_NOTE)) 
			send_chord_as_llll((t_notation_obj *) x, ((t_note *)x->r_ob.firstselecteditem)->parent, 7, k_CONSIDER_FOR_DUMPING, command_number);
		else if ((x->r_ob.num_selecteditems == 1) && (x->r_ob.firstselecteditem->type == k_CHORD))
			send_chord_as_llll((t_notation_obj *) x, (t_chord *)x->r_ob.firstselecteditem, 7, k_CONSIDER_FOR_DUMPING, command_number);
	} else { // send selection values
		standard_dump_selection((t_notation_obj *)x, 7, command_number, (delete_item_fn)score_sel_delete_item);
	}
}

char move_selection_breakpoint(t_score *x, double delta_x_pos, double delta_y_pos, char tail_only){
	t_notation_item *curr_it = x->r_ob.firstselecteditem;
	char changed = 0;
	lock_general_mutex((t_notation_obj *)x);	
	while (curr_it) { // cycle on the selected items
		if ((curr_it->type == k_PITCH_BREAKPOINT) && ((t_bpt *)curr_it)->next && !tail_only) { // it is a breakpoint : let's move it
			t_note *note = ((t_bpt *)curr_it)->owner;
			if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)note)) {
                double note_length;
                t_note *last_tied;
                if (x->r_ob.dl_spans_ties && (last_tied = get_last_tied_note(note)))
                    note_length = unscaled_xposition_to_xposition((t_notation_obj *)x, last_tied->parent->parent->tuttipoint_reference->offset_ux + last_tied->parent->stem_offset_ux + last_tied->parent->duration_ux) - unscaled_xposition_to_xposition((t_notation_obj *)x, note->parent->parent->tuttipoint_reference->offset_ux + note->parent->stem_offset_ux);
                else
                    note_length = unscaled_xposition_to_xposition((t_notation_obj *)x, note->parent->parent->tuttipoint_reference->offset_ux + note->parent->stem_offset_ux + note->parent->duration_ux) - unscaled_xposition_to_xposition((t_notation_obj *)x, note->parent->parent->tuttipoint_reference->offset_ux + note->parent->stem_offset_ux);
                double delta_rel_x_pos = delta_x_pos / note_length;
				
				if (!(note->parent->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
					create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)note->parent, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
				
				move_breakpoint((t_notation_obj *) x, (t_bpt *)curr_it, delta_rel_x_pos, delta_y_pos);
				recompute_all_for_measure((t_notation_obj *) x, note->parent->parent, false); // it's a bit overkilling!! TODO make it better
				changed = 1;
			}
		} else if (curr_it->type == k_PITCH_BREAKPOINT && !((t_bpt *)curr_it)->next) {	// note tail
			t_note *note = ((t_bpt *)curr_it)->owner;
			if (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)note)) {
				
				if (!(note->parent->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
					create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)note->parent, k_CHORD, k_UNDO_MODIFICATION_CHANGE);
			
				move_breakpoint((t_notation_obj *) x, note->lastbreakpoint, 0., delta_y_pos);
				recompute_all_for_measure((t_notation_obj *) x, note->parent->parent, false); // it's a bit overkilling!! TODO make it better
				changed = 1;
			}
		}
		curr_it = curr_it->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);	
	return changed;
}




char change_selection_tempo(t_score *x, double delta_tempo, char also_synchronous_tempi){ 
	t_notation_item *curr_it = x->r_ob.firstselecteditem;
	char changed = 0; 
	long i;
	
	lock_general_mutex((t_notation_obj *)x);	
	while (curr_it) { // cycle on the selected items
		if (curr_it->type == k_TEMPO) { // it is a tempo
			t_tempo *this_tempo = ((t_tempo *)curr_it);
			double new_tempo = rat2double(this_tempo->figure_tempo_value) + delta_tempo;
			t_tempo *sync_tempi[CONST_MAX_VOICES];
			long num_sync_tempi = 0;

			if (also_synchronous_tempi) {
				num_sync_tempi = get_synchronous_tempi((t_notation_obj *)x, this_tempo, sync_tempi);
				for (i = 0; i < num_sync_tempi; i++) 
					if (!(sync_tempi[i]->owner->r_it.flags & k_FLAG_MODIF_UNDO))
						create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)sync_tempi[i]->owner, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			} else {
				if (!(this_tempo->owner->r_it.flags & k_FLAG_MODIF_UNDO))
				create_simple_selected_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)this_tempo->owner, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			}
			
			if (new_tempo <= 1) 
				new_tempo = 1.;	
			if (new_tempo > CONST_MAX_TEMPO) 
				new_tempo = CONST_MAX_TEMPO;
				
			if (also_synchronous_tempi) { 
				for (i = 0; i < num_sync_tempi; i++)  {
					sync_tempi[i]->figure_tempo_value = long2rat(round(new_tempo));
					sync_tempi[i]->tempo_value = rat_rat_prod(this_tempo->figure_tempo_value, rat_long_prod(this_tempo->tempo_figure, 4));
				}
			} else {
				this_tempo->figure_tempo_value = long2rat(round(new_tempo));
				this_tempo->tempo_value = rat_rat_prod(this_tempo->figure_tempo_value, rat_long_prod(this_tempo->tempo_figure, 4));
			}
			changed = 1;
		}
		curr_it = curr_it->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);	
	recompute_all(x);
	return changed;
}


void select_all(t_score *x){
	t_scorevoice *voice;
	t_measure *meas;
	t_chord *chord;
	lock_general_mutex((t_notation_obj *)x);
	clear_selection((t_notation_obj *) x);
	clear_preselection((t_notation_obj *) x);
	voice = x->firstvoice;
	while (voice && (voice->v_ob.number < x->r_ob.num_voices)) {
		meas = voice->firstmeasure;
		while (meas) {
			chord = meas->firstchord;
			while (chord) {
				notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)chord);
				chord = chord->next;
			}
			meas = meas->next;
		}
		voice = voice->next;
	}
	lock_markers_mutex((t_notation_obj *)x);
	select_all_markers((t_notation_obj *)x, k_SELECTION_MODE_FORCE_SELECT);
	unlock_markers_mutex((t_notation_obj *)x);
	unlock_general_mutex((t_notation_obj *)x);
	handle_change_selection((t_notation_obj *)x);
//	invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
}	

void preselect_elements_in_region_for_mouse_selection(t_score *x, double ux1, double ux2, double mc1, double mc2, long v1, long v2){
	// preselect (and then, at the mouseup: select) all the elements in the region. ux1 and ux2 are the unscaled x positions

	t_scorevoice *voice;
	t_chord *curr_chord; 
	t_measure *curr_measure;
	long voicenum;

//	THIS LINE WAS WRONG! we've put it outside this function
//	clear_preselection((t_notation_obj *) x);
	
    for (voice = x->firstvoice; voice && (voice->v_ob.number < x->r_ob.num_voices); voice = voice->next) {
        
        if (voice->v_ob.hidden)
            continue;

		voicenum = voice->v_ob.number;
        
        // correcting voicenum for voiceensembles
        char same_v1_v2 = (v1 == v2);
        t_voice *v1v = nth_voice((t_notation_obj *)x, v1);
        t_voice *v2v = nth_voice((t_notation_obj *)x, v2);
        if (do_voices_belong_to_same_voiceensemble((t_notation_obj *)x, (t_voice *)voice, v1v))
            voicenum = v1;
        else if (do_voices_belong_to_same_voiceensemble((t_notation_obj *)x, (t_voice *)voice, v2v))
            voicenum = v2;
        if (do_voices_belong_to_same_voiceensemble((t_notation_obj *)x, v1v, v2v))
            same_v1_v2 = true;
        
		curr_measure = voice->firstmeasure;
		while (curr_measure) {
//			long count_notes_in_region;
			curr_chord = curr_measure->firstchord;
			//	post("----", curr_chord);
			while (curr_chord) {
				if (curr_chord->r_sym_duration.r_num < 0) { // it's a rest
					if ((curr_chord->parent->tuttipoint_reference->offset_ux + curr_chord->stem_offset_ux >= ux1)&&(curr_chord->parent->tuttipoint_reference->offset_ux + curr_chord->stem_offset_ux <= ux2)){
						long rest_note_mc = yposition_to_mc((t_notation_obj *)x, rest_get_nonfloating_yposition((t_notation_obj *) x, curr_chord, NULL, NULL) - curr_chord->float_steps * x->r_ob.step_y, (t_voice *)voice, NULL);
						if ( ((voicenum > v2) && (voicenum < v1)) || // v2 >= v1 always 
							(same_v1_v2 && (v1 == voicenum) && ((rest_note_mc >= mc1)&&(rest_note_mc <= mc2))) ||
							(!same_v1_v2 && (voicenum == v1) && (rest_note_mc >= mc1)) ||
							(!same_v1_v2 && (voicenum == v2) && (rest_note_mc <= mc2)) ) { // rest is in preselection rectangle
							notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)curr_chord);
						}
					} 
				} else {
					double ch_align_pt_ux = chord_get_alignment_ux((t_notation_obj *)x, curr_chord);
					if (ch_align_pt_ux >= ux1 && ch_align_pt_ux <= ux2){ 
						t_note *curr_nt = curr_chord->firstnote;
						while (curr_nt) { // cycle on the notes
							if ( ((voicenum > v2) && (voicenum < v1)) ||
								(same_v1_v2 && (v1 == voicenum) && ((curr_nt->midicents >= mc1)&&(curr_nt->midicents <= mc2))) ||
								(!same_v1_v2 && (voicenum == v1) && (curr_nt->midicents >= mc1)) ||
								(!same_v1_v2 && (voicenum == v2) && (curr_nt->midicents <= mc2)) ) {
								//						post("adding %lx to preselection", curr_nt);
								notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)curr_nt);
							}
							curr_nt = curr_nt->next;
						}
					} else if ((x->r_ob.show_durations) && (ch_align_pt_ux < ux1)) { // maybe the duration tail is selected...
						t_note *curr_nt = curr_chord->firstnote;
						while (curr_nt) { // cycle on the notes
							if ( ((curr_chord->parent->tuttipoint_reference->offset_ux + curr_chord->stem_offset_ux + curr_chord->duration_ux >= ux1)&&(curr_chord->parent->tuttipoint_reference->offset_ux + curr_chord->stem_offset_ux + curr_chord->duration_ux <= ux2)) &&
								(  ((voicenum > v2) && (voicenum < v1)) ||
								 (same_v1_v2 && (v1 == voicenum) && ((curr_nt->midicents + curr_nt->lastbreakpoint->delta_mc >= mc1)&&(curr_nt->midicents + curr_nt->lastbreakpoint->delta_mc <= mc2))) ||
								 (!same_v1_v2 && (voicenum == v1) && (curr_nt->midicents + curr_nt->lastbreakpoint->delta_mc >= mc1)) ||
								 (!same_v1_v2 && (voicenum == v2) && (curr_nt->midicents + curr_nt->lastbreakpoint->delta_mc <= mc2)) )) {
								notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)curr_nt->lastbreakpoint);
							}
							curr_nt = curr_nt->next;
						} 
					}
					if ((x->r_ob.show_durations) && (!(ch_align_pt_ux >= ux2)) && (!notation_item_is_preselected((t_notation_obj *)x, (t_notation_item *)curr_chord))) { // looking for breakpoint-selection
						t_note *curr_nt = curr_chord->firstnote;
						while (curr_nt) { // cycle on the notes
							// we only select breakpoints IF the whole note has NOT been selected in the region, otherwise the whole note is selected, that's all
							if (!notation_item_is_preselected((t_notation_obj *)x, (t_notation_item *)curr_nt)) {
								t_bpt *curr_bpt = curr_nt->firstbreakpoint->next;
								while (curr_bpt) { // cycle on the breakpoints
									if (curr_bpt != curr_nt->lastbreakpoint){
										if ( ((curr_chord->parent->tuttipoint_reference->offset_ux + curr_chord->stem_offset_ux + curr_bpt->rel_x_pos * curr_chord->duration_ux >= ux1)
											  && (curr_chord->parent->tuttipoint_reference->offset_ux + curr_chord->stem_offset_ux + curr_bpt->rel_x_pos * curr_chord->duration_ux <= ux2)) &&
											(  ((voicenum > v2) && (voicenum < v1)) ||
											 (same_v1_v2 && (v1 == voicenum) && ((curr_nt->midicents + curr_bpt->delta_mc >= mc1)&&(curr_nt->midicents + curr_bpt->delta_mc <= mc2))) ||
											 (!same_v1_v2 && (voicenum == v1) && (curr_nt->midicents + curr_bpt->delta_mc >= mc1)) ||
											 (!same_v1_v2 && (voicenum == v2) && (curr_nt->midicents + curr_bpt->delta_mc <= mc2)) )) {
												notation_item_add_to_preselection((t_notation_obj *) x, (t_notation_item *)curr_bpt);						
											}
									}
									curr_bpt = curr_bpt->next;
								}
							}
							curr_nt = curr_nt->next;
						}
					}
				}
				curr_chord = curr_chord->next;
			} 
			curr_measure = curr_measure->next;
		}
	}
}

char is_chord_in_selected_region(t_score *x, t_chord *chord){
// determines whether a chord is within the selected region. Return 0 if chord is completely outside the region, 1 if the entire chord is within the region, 2 if some notes are inside, some outside
	long ms1 = x->r_ob.j_selected_region_ms1;
	long ms2 = x->r_ob.j_selected_region_ms2;
	long mc1 = x->r_ob.j_selected_region_mc1; // low
	long mc2 = x->r_ob.j_selected_region_mc2; // high
	long v1 = x->r_ob.j_selected_region_voice1;
	long v2 = x->r_ob.j_selected_region_voice2;
	
	long count_notes_in_region = 0;
	long chord_voice = chord->voiceparent->v_ob.number;
//	post("°°°°°°°°°°°°°°°°° chord: %lx. Num_notes: %d", chord, chord->num_notes);
	if ((chord->onset >= ms1)&&(chord->onset <= ms2)){
		if ((chord_voice >= v1)&&(chord_voice <= v2)){ // voice can be ok
			t_note *curr_nt = chord->firstnote;
			while (curr_nt) { // cycle on the notes
				if ( ((chord_voice > v1) && (chord_voice < v2)) ||
					 ((v1 == v2) && ((curr_nt->midicents >= mc1)&&(curr_nt->midicents <= mc2))) ||
					 ((chord_voice == v1) && (curr_nt->midicents <= mc2)) ||
					 ((chord_voice == v2) && (curr_nt->midicents >= mc1)) )
					count_notes_in_region++;
				curr_nt = curr_nt->next;
			}
			if (count_notes_in_region == chord->num_notes){
				return 1; // totally inside
			} else {
				return 2; // partially inside
			}
		}
	} else {
		return 0; // totally outside
	}
	
	return 0; // BIZZARRO... da mettere
}

char is_note_in_selected_region(t_score *x, t_chord *chord, t_note *note){
// determines whether a note of a given chord is within the selected region. Return 0 if outside, 1 if inside
	long ms1 = x->r_ob.j_selected_region_ms1;
	long ms2 = x->r_ob.j_selected_region_ms2;
	long mc1 = x->r_ob.j_selected_region_mc1;
	long mc2 = x->r_ob.j_selected_region_mc2;
	long v1 = x->r_ob.j_selected_region_voice1;
	long v2 = x->r_ob.j_selected_region_voice2;

	long chord_voice = chord->voiceparent->v_ob.number;
	if ( ((chord->onset >= ms1) &&(chord->onset <= ms2)) &&
		 ( ((chord_voice > v1) && (chord_voice < v2)) ||
	     	((v1 == v2) && ((note->midicents >= mc1)&&(note->midicents <= mc2))) ||
			((chord_voice == v1) && (note->midicents <= mc2)) ||
			((chord_voice == v2) && (note->midicents >= mc1)) ) )
		return 1;
	else
		return 0;
}

void score_focusgained(t_score *x, t_object *patcherview) {
	x->r_ob.j_has_focus = true;
    notationobj_redraw((t_notation_obj *) x);
}

void score_focuslost(t_score *x, t_object *patcherview) {
    set_mousedown((t_notation_obj *)x, NULL, k_NONE, false);

	if (x->r_ob.show_dilation_rectangle) {
		x->r_ob.show_dilation_rectangle = false;
		if (x->r_ob.j_mouse_cursor == BACH_CURSOR_DRAGGINGHAND) 
			bach_set_cursor((t_object *)x, &x->r_ob.j_mouse_cursor, patcherview, BACH_CURSOR_DEFAULT);
	}
	
	if (!x->r_ob.keep_selection_if_lost_focus){
		lock_general_mutex((t_notation_obj *)x);
		clear_selection((t_notation_obj *) x);
		unlock_general_mutex((t_notation_obj *)x);
	}
	x->r_ob.j_has_focus = false;
}

char delete_selected_measures(t_score *x)
{
	char changed = 0;
	char need_check_scheduling = false;
	t_notation_item *temp = x->r_ob.firstselecteditem;
	lock_general_mutex((t_notation_obj *)x);
	while (temp) {
		t_notation_item *next = temp->next_selected;
		if (temp->type == k_MEASURE) {
			if (!notation_item_is_globally_locked((t_notation_obj *) x, temp)) {
                char need_update_solos = false;
                if (voiceensemble_delete_measure(x, (t_measure *)temp, ((t_measure *)temp)->firstchord ? get_prev_chord(((t_measure *)temp)->firstchord) : NULL, &need_update_solos, true))
                    need_check_scheduling = true;
                if (need_update_solos) update_solos((t_notation_obj *)x);
 				changed = 1;
			}
		}
		temp = next;
	}
	
	if (need_check_scheduling)
		check_correct_scheduling((t_notation_obj *)x, false);
	
    clear_selection((t_notation_obj *) x);
    
    recompute_all_except_for_beamings_and_autocompletion(x);
    perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_DONT_CHANGE_ANYTHING);
    
	unlock_general_mutex((t_notation_obj *)x);
	return changed;
}

char clear_selected_measures(t_score *x) {
    char changed = 0;
    char need_check_scheduling = false;
    t_notation_item *temp = x->r_ob.firstselecteditem;
    lock_general_mutex((t_notation_obj *)x);
    while (temp) {
        t_notation_item *next = temp->next_selected;
        if (temp->type == k_MEASURE) {
            if (!notation_item_is_globally_locked((t_notation_obj *) x, temp)) {
                if (((t_measure *)temp)->prev)
                    create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)((t_measure *)temp)->prev, k_UNDO_MODIFICATION_CHANGE); // because of ties
                create_simple_notation_item_undo_tick((t_notation_obj *)x, temp, k_UNDO_MODIFICATION_CHANGE);
                clear_measure(x, (t_measure *)temp, false, true, false);
                need_check_scheduling = true;
                changed = 1;
            }
        }
        temp = next;
    }
    
    if (need_check_scheduling)
        check_correct_scheduling((t_notation_obj *)x, false);
    
    update_solos((t_notation_obj *)x);

    clear_selection((t_notation_obj *) x);
    unlock_general_mutex((t_notation_obj *)x);
    recompute_all_except_for_beamings_and_autocompletion(x);
    return changed;
}


char delete_selected_tempi(t_score *x) {
	char changed = 0;
	t_notation_item *temp = x->r_ob.firstselecteditem;
	lock_general_mutex((t_notation_obj *)x);
	while (temp) {
		t_notation_item *next = temp->next_selected;
		t_measure *meas = ((t_tempo *)temp)->owner;
		if ((temp->type == k_TEMPO) && (!notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)meas))) {
			create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			delete_tempo((t_notation_obj *)x, (t_tempo *)temp);
			changed = 1;
		}
		temp = next;
	}
	clear_selection((t_notation_obj *) x);
	unlock_general_mutex((t_notation_obj *)x);
	recompute_all(x);
	return changed;
}

char switch_interpolation_to_selected_tempi(t_score *x) {
	char changed = 0;
	t_notation_item *temp = x->r_ob.firstselecteditem;
	lock_general_mutex((t_notation_obj *)x);	
	while (temp) {
		t_measure *meas = ((t_tempo *)temp)->owner;
		if (temp->type == k_TEMPO && !notation_item_is_globally_locked((t_notation_obj *) x, (t_notation_item *)meas)) {
			t_tempo *this_tempo = (t_tempo *)temp;
			create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas, k_MEASURE, k_UNDO_MODIFICATION_CHANGE);
			this_tempo->interpolation_type = (this_tempo->interpolation_type > 0) ? 0 : 1;
			changed = 1;
		}
		temp = temp->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);	
	recompute_all_except_for_beamings_and_autocompletion(x);
	return changed;
}

char duplicate_selected_measures(t_score *x)
{
	char changed = 0;
	t_notation_item *temp = x->r_ob.firstselecteditem;
	while (temp) {
		if (temp->type == k_MEASURE)
			((t_measure *)temp)->r_it.flags = (e_bach_internal_notation_flags) (((t_measure *)temp)->r_it.flags | k_FLAG_DUPLICATE);
		temp = temp->next_selected;
	}

	temp = x->r_ob.firstselecteditem;
	lock_general_mutex((t_notation_obj *)x);
	while (temp) {
		if ((temp->type == k_MEASURE) && (((t_measure *)temp)->r_it.flags & k_FLAG_DUPLICATE)) {
			t_measure *oldmeas, *newmeas;
//			t_tempo *tmp;
			((t_measure *)temp)->r_it.flags = (e_bach_internal_notation_flags) (((t_measure *)temp)->r_it.flags & ~k_FLAG_DUPLICATE);
			oldmeas = (t_measure *)temp;
			newmeas = clone_measure((t_notation_obj *) x, oldmeas, k_CLONE_FOR_NEW);
			newmeas->voiceparent = oldmeas->voiceparent;
			insert_measure(x, oldmeas->voiceparent, newmeas, oldmeas, 0);
			
			create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)newmeas, k_UNDO_MODIFICATION_DELETE);
            if (newmeas->prev)
                create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)newmeas->prev, k_UNDO_MODIFICATION_CHANGE);
            if (newmeas->next)
                create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)newmeas->next, k_UNDO_MODIFICATION_CHANGE);

/*			// changing measurenum at timepoints
			tmp = newmeas->firsttempo;
			for (tmp = newmeas->firsttempo; tmp; tmp = tmp->next) 
				tmp->changepoint.measure_num = newmeas->measure_number;
*/			
			notation_item_add_to_selection((t_notation_obj *) x, (t_notation_item *)newmeas);
			changed = 1;
		}
		temp = temp->next_selected;
	}
    recompute_all(x);
    perform_analysis_and_change(x, NULL, NULL, 0);
	unlock_general_mutex((t_notation_obj *)x);

	return changed;
}


t_slur *add_slur(t_score *x, t_note *start_note, t_note *end_note){
	if (start_note->num_slurs_to < CONST_MAX_SLURS_PER_NOTE - 1 && end_note->num_slurs_from < CONST_MAX_SLURS_PER_NOTE - 1) {
		t_slur *this_slur = (t_slur *)bach_newptr(sizeof(t_slur));
		
		notation_item_init(&this_slur->r_it, k_SLUR);
		
		this_slur->start_note = start_note;
		this_slur->direction = 0; // undefined: use standard
		this_slur->end_note = end_note;
		start_note->slur_to[start_note->num_slurs_to++] = this_slur;
		end_note->slur_from[end_note->num_slurs_from++] = this_slur;
		this_slur->need_recompute_position = true;
		return this_slur;
	}
	return NULL;
}

void change_slur_starting_note(t_score *x, t_slur *slur, t_note *newnote) {
	long i = 0;
	if (newnote->num_slurs_to < CONST_MAX_SLURS_PER_NOTE - 1) {
		while (i < slur->start_note->num_slurs_to) {
			if (slur->start_note->slur_to[i] == slur) {
				long j;
				for (j = i+1; j < slur->start_note->num_slurs_to; j++)
					slur->start_note->slur_to[j-1] = slur->start_note->slur_to[j];
				slur->start_note->num_slurs_to--;
			} else
				i++;
		}
		
		newnote->slur_to[newnote->num_slurs_to] = slur;
		newnote->num_slurs_to++;
		
		slur->start_note = newnote;
		slur->need_recompute_position = true;
	}
}

void change_slur_ending_note(t_score *x, t_slur *slur, t_note *newnote) {
	long i = 0;
	if (newnote->num_slurs_from < CONST_MAX_SLURS_PER_NOTE - 1) {
		while (i < slur->end_note->num_slurs_from) {
			if (slur->end_note->slur_to[i] == slur) {
				long j;
				for (j = i+1; j < slur->end_note->num_slurs_from; j++)
					slur->end_note->slur_from[j-1] = slur->end_note->slur_from[j];
				slur->end_note->num_slurs_from--;
			} else
				i++;
		}
		
		newnote->slur_from[newnote->num_slurs_from] = slur;
		newnote->num_slurs_from++;
		
		slur->end_note = newnote;
		slur->need_recompute_position = true;
	}
}

char delete_slurs_in_selection(t_score *x){ 
	char changed = 0;
	t_notation_item *curr_it;
	lock_general_mutex((t_notation_obj *)x);	
	curr_it = x->r_ob.firstselecteditem;
	while (curr_it) { // cycle on the selected items
		if (curr_it->type == k_SLUR || curr_it->type == k_SLUR_START_POINT || curr_it->type == k_SLUR_END_POINT) { 
			t_slur *slur = ((t_slur *)curr_it);
			t_notation_item *temp = curr_it->next_selected;
			changed = 1;
			notation_item_delete_from_selection((t_notation_obj *) x, (t_notation_item *)slur); // to be 100% sure, we delete all stuff
			delete_slur((t_notation_obj *) x, slur);
			curr_it = temp;
		} else {
			curr_it = curr_it->next_selected;
		}
	}
	unlock_general_mutex((t_notation_obj *)x);	
	return changed;
}


void score_fixvzoom(t_score *x){
	t_atom av[1];
	atom_setfloat(av, x->r_ob.zoom_y * 100.);
	score_setattr_vzoom(x, NULL, 1, av);
}

// PAINT FUNCTION!

void score_paint(t_score *x, t_object *view)
{
	t_jgraphics *g;
	t_rect rect;

	// getting rectangle dimensions
	g = (t_jgraphics*) patcherview_get_jgraphics(view); 
	jbox_get_rect_for_view(&x->r_ob.j_box.l_box.b_ob, view, &rect);

	paint_background((t_object *)x, g, &rect, &x->r_ob.j_background_rgba, x->r_ob.corner_roundness);

	if (x->r_ob.j_box.l_rebuild){
		invalidate_notation_static_layer_and_repaint((t_notation_obj *) x);
		x->r_ob.j_box.l_rebuild = 0;
	}
	
	scoreapi_paint(x, view, g, rect);
	
    if (x->r_ob.are_there_solos)
        paint_border((t_object *)x, g, &rect, &x->r_ob.j_solo_rgba, 2.5, x->r_ob.corner_roundness);
    else
        paint_border((t_object *)x, g, &rect, &x->r_ob.j_border_rgba, (!x->r_ob.show_border) ? 0 : ((x->r_ob.j_has_focus && x->r_ob.show_focus) ? 2.5 : 1), x->r_ob.corner_roundness);

	send_changed_bang_and_automessage_if_needed((t_notation_obj *)x);
}

char score_sel_dilate_mc(t_score *x, double mc_factor, double fixed_mc_y_pixel){
	char changed = 0;
	t_notation_item *curr_it;
	
	if (mc_factor == 1.)
		return 0;
	
	lock_general_mutex((t_notation_obj *)x);
	curr_it = x->r_ob.firstselecteditem;
	while (curr_it) {
		if (curr_it->type == k_NOTE) {
			t_note *nt = (t_note *) curr_it;
			double fixed_mc_point = yposition_to_mc((t_notation_obj *)x, fixed_mc_y_pixel, (t_voice *) nt->parent->voiceparent, NULL);

			if (!(nt->parent->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
				create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)nt->parent, k_CHORD, k_UNDO_MODIFICATION_CHANGE);

			nt->midicents = fixed_mc_point + (nt->midicents - fixed_mc_point) * mc_factor;
			if (nt->midicents < 0)
				nt->midicents = 0;
			recompute_all_for_measure((t_notation_obj *) x, nt->parent->parent, false);
			changed = 1;
		} else if (curr_it->type == k_CHORD) {
			t_chord *chord = (t_chord *) curr_it;
			double fixed_mc_point = yposition_to_mc((t_notation_obj *)x, fixed_mc_y_pixel, (t_voice *) chord->voiceparent, NULL);
			t_note *nt;

			if (!(chord->r_it.flags & k_FLAG_MODIF_UNDO_WITH_OR_WO_CHECK_ORDER))
				create_simple_selected_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)chord, k_CHORD, k_UNDO_MODIFICATION_CHANGE);

			for (nt = chord->firstnote; nt; nt = nt->next) {
				nt->midicents = fixed_mc_point + (nt->midicents - fixed_mc_point) * mc_factor;
				if (nt->midicents < 0)
					nt->midicents = 0;
			}
			recompute_all_for_measure((t_notation_obj *) x, chord->parent, false);
			changed = 1;
		}
		curr_it = curr_it->next_selected;
	}
	unlock_general_mutex((t_notation_obj *)x);
	
	return changed;
}

void score_old_undo(t_score *x){
	if (x->r_ob.old_undo_llll[1]) {
		int i;
		
#ifdef BACH_UNDO_DEBUG
		llll_post_named(x->r_ob.old_undo_llll[1], 0, 1, 2, gensym("UNDO reconstruting"), NULL); 
#endif
		
		set_score_from_llll(x, x->r_ob.old_undo_llll[1], true); // resetting score
		changed_bang(x, k_CHANGED_STANDARD_SEND_BANG);
		
		lock_general_mutex((t_notation_obj *)x);	
		for (i = CONST_MAX_UNDO_STEPS - 1; i > 0; i--) // reshifting all redo elements
			x->r_ob.old_redo_llll[i]=x->r_ob.old_redo_llll[i-1];
		x->r_ob.old_redo_llll[0] = x->r_ob.old_undo_llll[0];
		for (i = 0; i < CONST_MAX_UNDO_STEPS - 1; i++) // reshifting all undo elements
			x->r_ob.old_undo_llll[i]=x->r_ob.old_undo_llll[i+1];
		x->r_ob.old_undo_llll[CONST_MAX_UNDO_STEPS - 1] = NULL;
		unlock_general_mutex((t_notation_obj *)x);	
		
#ifdef BACH_UNDO_DEBUG
		llll_post_named(x->r_ob.old_undo_llll[1], 0, 1, 2, gensym("UNDO next step"), NULL); 
#endif
	} else {
		object_warn((t_object *)x, "Can't undo!");
	}
}

void score_old_redo(t_score *x){
	if (x->r_ob.old_redo_llll[0]) {
		int i;
		
		#ifdef BACH_UNDO_DEBUG
			llll_post_named(x->r_ob.old_redo_llll[0], 0, 1, 2, gensym("REDO reconstruting"), NULL); 
		#endif
		
		set_score_from_llll(x, x->r_ob.old_redo_llll[0], true); // resetting score
		changed_bang(x, k_CHANGED_STANDARD_SEND_BANG);
		
		lock_general_mutex((t_notation_obj *)x);	
		for (i = CONST_MAX_UNDO_STEPS - 1; i > 0; i--) // reshifting all undo elements
			x->r_ob.old_undo_llll[i]=x->r_ob.old_undo_llll[i-1];
		x->r_ob.old_undo_llll[0] = x->r_ob.old_redo_llll[0];
		for (i = 0; i < CONST_MAX_UNDO_STEPS - 1; i++) // reshifting all redo elements
			x->r_ob.old_redo_llll[i]=x->r_ob.old_redo_llll[i+1];
		x->r_ob.old_redo_llll[CONST_MAX_UNDO_STEPS - 1] = NULL;
		unlock_general_mutex((t_notation_obj *)x);	
		
	} else {
		object_warn((t_object *)x, "Can't redo!");
	}
}



// what = -1 -> undo, what = 1 -> redo
void score_new_undo_redo(t_score *x, char what)
{
	t_llll *llll = NULL;
	long undo_op = k_UNDO_OP_UNKNOWN;
	char need_perform_analysis_and_change = false;
	t_llll *measure_whose_flag_needs_to_be_cleared = llll_get();

	lock_general_mutex((t_notation_obj *)x);	
	systhread_mutex_lock(x->r_ob.c_undo_mutex);	

	if (what == k_UNDO)
		llll = x->r_ob.undo_llll;
	else if (what == k_REDO)
		llll = x->r_ob.redo_llll;
	
	if (!llll) {
		llll_free(measure_whose_flag_needs_to_be_cleared);
		systhread_mutex_unlock(x->r_ob.c_undo_mutex);	
		unlock_general_mutex((t_notation_obj *)x);	
		return;
	}
	
	while (llll->l_head && hatom_gettype(&llll->l_head->l_hatom) != H_LONG){
		object_error((t_object *) x, what == k_UNDO ? "Wrongly placed undo tick!" : "Wrongly placed redo tick!");
		llll_destroyelem(llll->l_head);
	}

	if (!llll->l_head) {
		if (!(atom_gettype(&x->r_ob.max_undo_steps) == A_LONG && atom_getlong(&x->r_ob.max_undo_steps) == 0))
			object_warn((t_object *) x, what == k_UNDO ? "Can't undo!" : "Can't redo!");
		llll_free(measure_whose_flag_needs_to_be_cleared);
		systhread_mutex_unlock(x->r_ob.c_undo_mutex);	
		unlock_general_mutex((t_notation_obj *)x);	
		return;
	}
	
	undo_op = hatom_getlong(&llll->l_head->l_hatom);
	if (x->r_ob.verbose_undo_redo) {
		char *buf = undo_op_to_string(undo_op);
		object_post((t_object *) x, "%s %s", what == k_UNDO ? "Undo" : "Redo", buf);
		bach_freeptr(buf);
	}
	
	// Destroy the marker
	if (llll->l_head == x->r_ob.last_undo_marker) 
		x->r_ob.last_undo_marker = NULL;
	llll_destroyelem(llll->l_head);
	
	if (what == k_UNDO)
		x->r_ob.num_undo_steps--;
	else
		x->r_ob.num_redo_steps--;
	
	while (llll->l_head && hatom_gettype(&llll->l_head->l_hatom) == H_OBJ){
		t_undo_redo_information *this_information = (t_undo_redo_information *)hatom_getobj(&llll->l_head->l_hatom);
		long ID = this_information->n_it_ID;
		e_element_types type = this_information->n_it_type;
		long modif_type = this_information->modification_type;
		long voice_num = this_information->voice_num;
		long meas_num = this_information->meas_num;
		e_header_elems header_info = this_information->header_info;
		t_llll *content = this_information->n_it_content;
		t_llll *newcontent = NULL;
		t_notation_item *item = (t_notation_item *) shashtable_retrieve(x->r_ob.IDtable, ID);
		t_undo_redo_information *new_information = NULL;

		if (!item && modif_type != k_UNDO_MODIFICATION_ADD && type != k_WHOLE_NOTATION_OBJECT && type != k_HEADER_DATA) {
			object_error((t_object *) x, "Wrong undo/redo data");
			llll_destroyelem(llll->l_head);
			continue;
		}
		
		if (modif_type == k_UNDO_MODIFICATION_CHANGE_FLAG) {
			newcontent = get_multiple_flags_for_undo((t_notation_obj *)x, item);
			new_information = build_undo_redo_information(ID, type, k_UNDO_MODIFICATION_CHANGE_FLAG, voice_num, meas_num, k_HEADER_NONE, newcontent);
			set_multiple_flags_from_llll_for_undo((t_notation_obj *)x, content, item);
			x->r_ob.are_there_solos = are_there_solos((t_notation_obj *)x);
		
		} else if (modif_type == k_UNDO_MODIFICATION_CHANGE_NAME) {
			newcontent = get_names_as_llll(item, false);
			new_information = build_undo_redo_information(ID, type, k_UNDO_MODIFICATION_CHANGE_NAME, voice_num, meas_num, k_HEADER_NONE, newcontent);
			notation_item_set_names_from_llll((t_notation_obj *)x, item, content);

		} else if (type == k_WHOLE_NOTATION_OBJECT){
			// need to reconstruct the whole score
			newcontent = get_score_values_as_llll(x, k_CONSIDER_FOR_UNDO, k_HEADER_ALL, true, true, false, true);
			new_information = build_undo_redo_information(0, k_WHOLE_NOTATION_OBJECT, k_UNDO_MODIFICATION_CHANGE, 0, 0, k_HEADER_NONE, newcontent);
			score_clear_all(x);
			x->r_ob.take_rhythmic_tree_for_granted = 1;
			set_score_from_llll(x, content, false);
			x->r_ob.take_rhythmic_tree_for_granted = 0;
			
		} else if (type == k_MEASURE) {
			t_measure *meas = ((t_measure *)item);
			if (modif_type == k_UNDO_MODIFICATION_CHANGE) { // measure need to be changed
                char need_update_solos = false;
				newcontent = measure_get_values_as_llll((t_notation_obj *) x, (t_measure *) item, k_CONSIDER_FOR_UNDO, true, true);
                new_information = build_undo_redo_information(ID, k_MEASURE, k_UNDO_MODIFICATION_CHANGE, voice_num, meas_num, k_HEADER_NONE, newcontent);
				clear_measure(x, meas, true, false, true);
				notation_item_get_ID_from_llll(content); // if there's an ID in the measure, we ignore it.
				set_measure_from_llll(x, meas, content, true, false, &need_update_solos);
//				meas->beaming_calculation_flags = is_measure_single_whole_rest((t_notation_obj *) x, meas) ? k_BEAMING_CALCULATION_FROM_SCRATCH : k_BEAMING_CALCULATION_DONT_CHANGE_ANYTHING;
                meas->beaming_calculation_flags = (is_measure_single_whole_rest((t_notation_obj *) x, meas) && x->r_ob.tree_handling != k_RHYTHMIC_TREE_HANDLING_TAKE_FOR_GRANTED) ? k_BEAMING_CALCULATION_FROM_SCRATCH : k_BEAMING_CALCULATION_DONT_CHANGE_ANYTHING;
				meas->need_recompute_beamings = true;
				compute_note_approximations_for_measure((t_notation_obj *)x, meas, true);
				recompute_all_except_for_beamings_and_autocompletion(x);
                if (need_update_solos) update_solos((t_notation_obj *)x);
				need_perform_analysis_and_change = true;
				llll_appendobj(measure_whose_flag_needs_to_be_cleared, meas, 0, WHITENULL_llll);
			} else if (modif_type == k_UNDO_MODIFICATION_DELETE) { // measure need to be erased
                char need_update_solos = false;
				newcontent = measure_get_values_as_llll((t_notation_obj *) x, (t_measure *) item, k_CONSIDER_FOR_UNDO, true, true);
				new_information = build_undo_redo_information(ID, k_MEASURE, k_UNDO_MODIFICATION_ADD, voice_num, meas_num, k_HEADER_NONE, newcontent);
				if (delete_measure(x, meas, meas->firstchord ? get_prev_chord(meas->firstchord) : NULL, &need_update_solos))
					check_correct_scheduling((t_notation_obj *)x, false);
                if (need_update_solos) update_solos((t_notation_obj *)x);
				recompute_all_except_for_beamings_and_autocompletion(x);
			} else if (modif_type == k_UNDO_MODIFICATION_ADD) { // measure need to be added
				t_scorevoice *voice = nth_scorevoice(x, voice_num);
				t_measure *measure;
                char need_update_solos = false;
				newcontent = llll_get();
				new_information = build_undo_redo_information(ID, k_MEASURE, k_UNDO_MODIFICATION_DELETE, voice_num, meas_num, k_HEADER_NONE, newcontent);
				measure = build_measure((t_notation_obj *) x, NULL);
				notation_item_get_ID_from_llll(content); // if there's an ID in the measure, we ignore it.
				insert_measure(x, voice, measure, nth_measure_of_scorevoice(voice, meas_num - 1), ID);
				measure->beaming_calculation_flags = k_BEAMING_CALCULATION_DONT_CHANGE_ANYTHING;
				set_measure_from_llll(x, measure, content, true, false, &need_update_solos);
				compute_note_approximations_for_measure((t_notation_obj *)x, measure, true);
				recompute_all_except_for_beamings_and_autocompletion(x);
                if (need_update_solos) update_solos((t_notation_obj *)x);
				need_perform_analysis_and_change = true;
				llll_appendobj(measure_whose_flag_needs_to_be_cleared, measure, 0, WHITENULL_llll);
			}
			
		} else if (type == k_CHORD) {
			if (modif_type == k_UNDO_MODIFICATION_CHANGE) {
				newcontent = get_scorechord_values_as_llll((t_notation_obj *) x, (t_chord *) item, k_CONSIDER_FOR_UNDO, false);
//				dev_llll_print(content, NULL, 0, 0, NULL);
				new_information = build_undo_redo_information(ID, k_CHORD, k_UNDO_MODIFICATION_CHANGE, voice_num, meas_num, k_HEADER_NONE, newcontent);
				set_scorechord_values_from_llll((t_notation_obj *) x, (t_chord *)item, content, true, false);
                compute_note_approximations_for_chord((t_notation_obj *)x, (t_chord *)item, true);
                if (is_solo_with_progeny((t_notation_obj *) x, item)) update_solos((t_notation_obj *)x);
				recompute_all_for_measure((t_notation_obj *) x, ((t_chord *)item)->parent, false);
			}
			
		} else if (type == k_HEADER_DATA) {
			if (modif_type == k_UNDO_MODIFICATION_CHANGE) { 
				newcontent = get_score_values_as_llll(x, k_CONSIDER_FOR_UNDO, header_info, true, true, false, true);
				new_information = build_undo_redo_information(0, k_HEADER_DATA, k_UNDO_MODIFICATION_CHANGE, 0, 0, header_info, newcontent);
				x->r_ob.take_rhythmic_tree_for_granted = 1;
				set_score_from_llll(x, content, false);
				x->r_ob.take_rhythmic_tree_for_granted = 0;
			}
		} 
		
		if (new_information)
			create_undo_redo_tick((t_notation_obj *) x, -what, 1, new_information, false);
		
//		#ifdef BACH_UNDO_DEBUG
//			object_post((t_object *)x, "A new undo tick has been performed, to which correspond the new redo tick:");
//			post_undo_redo_tick((t_notation_obj *) x, -what, new_information);
//		#endif
		
		llll_free(content);
		bach_freeptr(this_information);
		llll_destroyelem(llll->l_head);
	}	
	
	if (need_perform_analysis_and_change)
		perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
		
	t_llllelem *elem;
	for (elem = measure_whose_flag_needs_to_be_cleared->l_head; elem; elem = elem->l_next)
		((t_measure *)hatom_getobj(&elem->l_hatom))->beaming_calculation_flags = k_BEAMING_CALCULATION_FROM_SCRATCH;

	create_undo_redo_step_marker((t_notation_obj *) x, -what, 1, undo_op, false);
	systhread_mutex_unlock(x->r_ob.c_undo_mutex);	
	unlock_general_mutex((t_notation_obj *)x);	

	handle_change((t_notation_obj *)x, x->r_ob.send_undo_redo_bang ? k_CHANGED_STANDARD_SEND_BANG : k_CHANGED_STANDARD, k_UNDO_OP_UNKNOWN);
	
	llll_free(measure_whose_flag_needs_to_be_cleared);
}

void score_getmaxID(t_score *x){
	object_post((t_object *) x, "Last used ID: %ld", x->r_ob.IDtable->s_lastused);
}

void score_resetslotinfo(t_score *x)
{
	create_whole_score_undo_tick(x);
	notation_obj_reset_slotinfo((t_notation_obj *)x);
	handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_SLOTINFO);
}


void score_resetarticulationinfo(t_score *x)
{
    create_whole_score_undo_tick(x);
    notation_obj_reset_articulationinfo((t_notation_obj *)x);
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_CUSTOM_ARTICULATIONS_DEFINITION);
}

void score_resetnoteheadinfo(t_score *x)
{
    create_whole_score_undo_tick(x);
    notation_obj_reset_noteheadinfo((t_notation_obj *)x);
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_CHANGE_CUSTOM_NOTEHEADS_DEFINITION);
}


void score_prune_last_undo_step(t_score *x){
	prune_last_undo_step((t_notation_obj *)x, true);
}


void score_inhibit_undo(t_score *x, long val){
	x->r_ob.inhibited_undo = val;
}

void score_undo(t_score *x){
	if (USE_NEW_UNDO_SYSTEM)
		score_new_undo_redo(x, k_UNDO);
	else
		score_old_undo(x);
}


void score_redo(t_score *x){
	if (USE_NEW_UNDO_SYSTEM)
		score_new_undo_redo(x, k_REDO);
	else
		score_old_redo(x);
}


void score_mute(t_score *x)
{
	mute_selection((t_notation_obj *)x, false);
}

void score_unmute(t_score *x)
{
	unmute_selection((t_notation_obj *)x, false);
}

void score_lock(t_score *x)
{
	lock_selection((t_notation_obj *)x, false);
}

void score_unlock(t_score *x)
{
	unlock_selection((t_notation_obj *)x, false);
}

void score_solo(t_score *x)
{
	solo_selection((t_notation_obj *)x, false);
}

void score_unsolo(t_score *x)
{
	unsolo_selection((t_notation_obj *)x, false);
}


void score_sel_rebeam(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
	rebeam_levels_of_selected_tree_nodes(x, false, false, k_BEAMING_CALCULATION_DONT_CHANGE_CHORDS + k_BEAMING_CALCULATION_DONT_CHANGE_TIES + k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, k_UNDO_OP_AUTO_BEAM);
}

void score_sel_reparse(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    char type = argc && argv && atom_gettype(argv) == A_LONG && atom_getlong(argv) != 0 ? 1 : 0;
    rebeam_levels_of_selected_tree_nodes(x, type, true, k_BEAMING_CALCULATION_DONT_AUTOCOMPLETE);
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *)x, k_CHANGED_STANDARD_UNDO_MARKER_AND_BANG, type == 1 ? k_UNDO_OP_AUTO_RHYTHMIC_TREE_IGNORING_EXISTING_TUPLETS_FOR_SELECTION : k_UNDO_OP_AUTO_RHYTHMIC_TREE_KEEPING_EXISTING_TUPLETS_FOR_SELECTION);
}

void score_copyslot_fn(t_notation_obj *r_ob, t_note *nt, void *dummy){
    long slotnum = *((long *)dummy);
    notation_obj_copy_slot(r_ob, &clipboard, (t_notation_item *)nt, slotnum, false);
}

void score_cutslot_fn(t_notation_obj *r_ob, t_note *nt, void *dummy){
    long slotnum = *((long *)dummy);
    notation_obj_copy_slot(r_ob, &clipboard, (t_notation_item *)nt, slotnum, true);
}


void score_copy_or_cut(t_score *x, t_symbol *s, long argc, t_atom *argv, char cut){
	t_llll *ll = llllobj_parse_llll((t_object *)x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_RETAIN);
    if (ll->l_size >= 1 && hatom_gettype(&ll->l_head->l_hatom) == H_SYM && (hatom_getsym(&ll->l_head->l_hatom) == gensym("durationline"))) {
        t_note *note = get_leftmost_selected_note((t_notation_obj *)x);
        notation_obj_copy_durationline((t_notation_obj *)x, &clipboard, note, cut);
    } else if (ll->l_size >= 1 && hatom_gettype(&ll->l_head->l_hatom) == H_SYM && (hatom_getsym(&ll->l_head->l_hatom) == _llllobj_sym_slot || hatom_getsym(&ll->l_head->l_hatom) == _llllobj_sym_slots)) {
        long slotnum = ll->l_size >= 2 && hatom_gettype(&ll->l_head->l_next->l_hatom) == H_LONG ? hatom_getlong(&ll->l_head->l_next->l_hatom) - 1 : (ll->l_size >= 2 && hatom_gettype(&ll->l_head->l_next->l_hatom) == H_SYM && hatom_getsym(&ll->l_head->l_next->l_hatom) == _sym_all ? -1 : x->r_ob.active_slot_num);
        
		if (x->r_ob.active_slot_notationitem)
			notation_obj_copy_slot((t_notation_obj *)x, &clipboard, x->r_ob.active_slot_notationitem, slotnum, cut);
        else
            iterate_notewise_changes_on_selection((t_notation_obj *)x, cut ? (notation_obj_note_fn)score_cutslot_fn : (notation_obj_note_fn)score_copyslot_fn, &slotnum, true, k_CHORD, false);
	} else {
		score_copy_selected_measures(x, cut);
	}
	llll_free(ll);
}


void score_copy(t_score *x, t_symbol *s, long argc, t_atom *argv){
	score_copy_or_cut(x, s, argc, argv, false);
}

void score_cut(t_score *x, t_symbol *s, long argc, t_atom *argv){
	score_copy_or_cut(x, s, argc, argv, true);
}

void score_paste(t_score *x, t_symbol *s, long argc, t_atom *argv){
	t_llll *ll = llllobj_parse_llll((t_object *)x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
	
    if (ll->l_size >= 1 && hatom_gettype(&ll->l_head->l_hatom) == H_SYM && (hatom_getsym(&ll->l_head->l_hatom) == gensym("durationline"))) {
        if (clipboard.type == k_DURATION_LINE)
            notation_obj_paste_durationline((t_notation_obj *) x, &clipboard);
    } else if (ll->l_size >= 1 && hatom_gettype(&ll->l_head->l_hatom) == H_SYM && (hatom_getsym(&ll->l_head->l_hatom) == _llllobj_sym_slot || hatom_getsym(&ll->l_head->l_hatom) == _llllobj_sym_slots)) {
		if (clipboard.type == k_SLOT) 
			notation_obj_paste_slot((t_notation_obj *) x, &clipboard, 
									ll->l_size >= 2 && hatom_gettype(&ll->l_head->l_next->l_hatom) == H_LONG ? hatom_getlong(&ll->l_head->l_next->l_hatom) - 1 :
									(ll->l_size >= 2 && hatom_gettype(&ll->l_head->l_next->l_hatom) == H_SYM &&
									 hatom_getsym(&ll->l_head->l_next->l_hatom) == _llllobj_sym_active ? x->r_ob.active_slot_num : -1));
	} else if (clipboard.type == k_GATHERED_SYNTAX) {
        long tempi = 0, how_many_times = 1, ignore_copied_voice_offset = false;
        llll_parseargs_and_attrs_destructive((t_object *) x, ll, "iii", gensym("tempi"), &tempi, gensym("repeat"), &how_many_times);
        if (argc && atom_gettype(argv) == A_SYM && atom_getsym(argv) == gensym("replace")) { // paste replace
            if (x->r_ob.selection_type == k_MEASURE)
                score_paste_replace_measures(x, tempi);
        } else {
            long at_this_measure = -1;
            long starting_from_this_voice = 1;
            if (argc && atom_gettype(argv) == A_LONG) {
                at_this_measure = atom_getlong(argv);
                if (argc > 1 && atom_gettype(argv + 1) == A_LONG) {
                    starting_from_this_voice = atom_getlong(argv+1);
                    ignore_copied_voice_offset = true;
                }
            }
            score_paste_measures(x, at_this_measure, starting_from_this_voice, tempi, ignore_copied_voice_offset, how_many_times);
        }
	}
	
	llll_free(ll);
}





///// SLICING



void slice_voice_at_position(t_score *x, t_scorevoice *voice, t_timepoint tp, char put_ties)
{
    if (!voice || tp.pt_in_measure.r_num == 0 || tp.measure_num < 0 || tp.measure_num >= voice->num_measures)
        return; // nothing to do
    
    char changed = false;
    t_measure *meas = nth_measure_of_scorevoice(voice, tp.measure_num);
    
    if (!meas)
        return;
    
    for (t_chord *ch = meas->firstchord; ch; ch = ch->next) {
        char sign = ch->r_sym_duration.r_num >= 0 ? 1 : -1;
        t_rational r_sym_tail = rat_rat_sum(ch->r_sym_onset, rat_abs(ch->r_sym_duration));
        if (!ch->is_grace_chord && ch->r_sym_duration.r_num != 0 && rat_rat_cmp(ch->r_sym_onset, tp.pt_in_measure) < 0 && rat_rat_cmp(r_sym_tail, tp.pt_in_measure) > 0) {
            // slice the chord!
            if (!changed) {
                create_simple_notation_item_undo_tick((t_notation_obj *)x, (t_notation_item *)meas, k_UNDO_MODIFICATION_CHANGE);
                if (meas->prev)
                create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas->prev, k_UNDO_MODIFICATION_CHANGE);
                if (meas->next)
                create_simple_notation_item_undo_tick((t_notation_obj *) x, (t_notation_item *)meas->next, k_UNDO_MODIFICATION_CHANGE);
                changed = true;
                recompute_all_for_measure((t_notation_obj *) x, meas, true);
            }
            
            t_rational left_dur = rat_rat_diff(tp.pt_in_measure, ch->r_sym_onset);
            t_rational right_dur = rat_rat_diff(r_sym_tail, tp.pt_in_measure);
            double cut_rel_pos = rat2double(left_dur)/rat2double(rat_abs(ch->r_sym_duration));
            
            t_chord *new_chord = clone_chord((t_notation_obj *) x, ch, k_CLONE_FOR_NEW);
            new_chord->parent = ch->parent;

            ch->r_sym_duration = rat_long_prod(left_dur, sign);
            new_chord->r_sym_duration = rat_long_prod(right_dur, sign);
            new_chord->rhythmic_tree_elem = llll_insertobj_after(new_chord, ch->rhythmic_tree_elem);

            insert_chord_in_measure((t_notation_obj *)x, meas, new_chord, ch, 0);
            
            t_note *nt, *new_nt;
            for (nt = ch->firstnote, new_nt = new_chord->firstnote; nt && new_nt; nt = nt->next, new_nt = new_nt->next) {
                
                if (are_note_breakpoints_nontrivial((t_notation_obj *)x, nt)) {
                    // split breakpoints
                    double new_midicents = nt->midicents;
                    t_llll *left_bpt = get_partialnote_breakpoint_values_as_llll((t_notation_obj *)x, nt, 0., cut_rel_pos, NULL);
                    t_llll *right_bpt = get_partialnote_breakpoint_values_as_llll((t_notation_obj *)x, nt, cut_rel_pos, 1., &new_midicents);
                    set_breakpoints_values_to_note_from_llll((t_notation_obj *)x, nt, left_bpt);
                    set_breakpoints_values_to_note_from_llll((t_notation_obj *)x, new_nt, right_bpt);
                    new_nt->midicents = new_midicents;
                    llll_free(left_bpt);
                    llll_free(right_bpt);
                }
                
                for (long i = 0; i < CONST_MAX_SLOTS; i++) {
                    if (x->r_ob.slotinfo[i].slot_uwidth < 0) { // temporal slots
                        // need to split!
                        t_llll *left_slot = get_partialnote_single_slot_values_as_llll((t_notation_obj *)x, nt, k_CONSIDER_FOR_SAVING, i, 0., cut_rel_pos);
                        t_llll *right_slot = get_partialnote_single_slot_values_as_llll((t_notation_obj *)x, nt, k_CONSIDER_FOR_SAVING, i, cut_rel_pos, 1.);
                        llll_wrap_once(&left_slot);
                        llll_wrap_once(&right_slot);
                        set_slots_values_to_note_from_llll((t_notation_obj *)x, nt, left_slot);
                        set_slots_values_to_note_from_llll((t_notation_obj *)x, new_nt, right_slot);
                    }
                }
            }
            
        }
    }
}


void slice(t_score *x, t_llll *timepoints, char put_ties)
{
    if (!timepoints)
        return;
    
    for (t_llllelem *tp_elem = timepoints->l_head; tp_elem; tp_elem = tp_elem->l_next) {
        if (is_hatom_number(&tp_elem->l_hatom)){
            for (t_scorevoice *voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next) {
                t_timepoint tp = ms_to_timepoint((t_notation_obj *)x, hatom_getdouble(&tp_elem->l_hatom), voice->v_ob.number, k_MS_TO_TP_RETURN_INTERPOLATION);
                slice_voice_at_position(x, voice, tp, put_ties);
            }
        } else if (hatom_gettype(&tp_elem->l_hatom) == H_LLLL) {
            char is_voice_defined = false;
            t_timepoint tp = llll_to_timepoint((t_notation_obj *)x, hatom_getllll(&tp_elem->l_hatom), &is_voice_defined, true);
            if (is_voice_defined) {
                slice_voice_at_position(x, nth_scorevoice(x, tp.voice_num), tp, put_ties);
            } else {
                // slice all voices
                for (t_scorevoice *voice = x->firstvoice; voice && voice->v_ob.number < x->r_ob.num_voices; voice = voice->next)
                    slice_voice_at_position(x, voice, tp, put_ties);
            }
        }
    }
    perform_analysis_and_change(x, NULL, NULL, k_BEAMING_CALCULATION_FROM_SCRATCH);
}




void score_slice(t_score *x, t_symbol *s, long argc, t_atom *argv)
{
    t_llll *arguments = llllobj_parse_llll((t_object *) x, LLLL_OBJ_UI, NULL, argc, argv, LLLL_PARSE_CLONE);
    
    lock_general_mutex((t_notation_obj *)x);
    slice(x, arguments, false);
    unlock_general_mutex((t_notation_obj *)x);
    
    llll_free(arguments);
    
    handle_change_if_there_are_free_undo_ticks((t_notation_obj *) x, k_CHANGED_STANDARD_UNDO_MARKER, k_UNDO_OP_SLICE);
}