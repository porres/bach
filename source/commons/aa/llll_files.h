#ifndef _LLLL_FILES_H_
#define _LLLL_FILES_H_

#include "llllobj.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
BEGIN_CHECK_LINKAGE
#endif	




t_max_err bach_openfile_read(t_object * x, t_symbol *filename_sym, t_filehandle *fh, t_fourcc *types, long numtypes, t_fourcc *outtype);
t_max_err bach_openfile_for_read(t_object *x, t_symbol *filename_sym, short *path, t_fourcc *types, long numtypes, t_fourcc *outtype, char *filename);
t_max_err bach_readfile(t_object *x, const char *filename, short path, t_filehandle *fh);

t_max_err bach_openfile_write(t_symbol *filename_sym, const char *default_filename, t_filehandle *fh, 
							  t_fourcc *types, long numtypes, t_fourcc *outtype, char *output_filename, short *output_path);


typedef void (*read_fn)(t_object *x, t_llll *ll);


// write a llll as text
// s contains the file name, or NULL to open a save dialog
void llll_write(t_object *x, t_llll *ll, t_llll *msg, long default_maxdecimals = 10, long default_wrap = 0, const char *default_indent = "\t", long default_maxdepth = -1);

// write a llll as text
// ll is the llll to write
// arguments is everything that follows the "writetxt" message, including the file name
// llll_writetxt takes ownership of ll and arguments, and destroys them (possibly asynchronously)
void llll_writetxt(t_object *x, t_llll *ll, t_llll *arguments, long default_maxdecimals = 10, long default_wrap = 0, const char *default_indent = "\t", long default_maxdepth = -1);

// write a llll in native (binary) format
// s contains the file name, or NULL to open a save dialog
void llll_writenative(t_object *x, t_symbol *s, t_llll *ll);

// read a llll
// s contains the file name, or NULL to open a save dialog
// after read is finished, outfn is called with x and the newly created ll as its arguments
// the ll is not destroyed thereafter - outfn owns it
void llll_read(t_object *x, t_symbol *s, read_fn outfn);

t_llll *llll_readfile(t_object *x, t_filehandle fh);

t_max_err bach_write_binary_file(t_symbol *filename_sym, const char *default_filename, t_fourcc filetype, t_ptr_size *count, const void *buffer);
t_max_err llll_write_text_file(t_symbol *filename_sym, t_ptr_size *count, const void *buffer);
t_max_err bach_write_dictionary(t_symbol *filename_sym, const char *default_filename, t_fourcc filetype, t_dictionary *dict);

// if *s is not empty and has no specified extension, add .<ext> to it - that is, s will point to a new symbol
// return 1 if *s has changed, 0 if not
long bach_fix_filename_extension(t_symbol **s, const char *ext);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
END_CHECK_LINKAGE
#endif

#endif // _LLLL_FILES_H_