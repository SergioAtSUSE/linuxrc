/*
 * loadunimap.c - aeb
 *
 * Version 1.00
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include "paths.h"
#include "getfd.h"
#include "xmalloc.h"
#include "findfile.h"
#include "psffontop.h"
#include "loadunimap.h"
#include "psf.h"
#include "nls.h"

extern char *progname;
extern int force;

static char *unidirpath[] = { "", DATADIR "/" UNIMAPDIR "/", 0 };
static char *unisuffixes[] = { "", ".uni", 0 };

#ifdef MAIN
#include "version.h"
int verbose = 0;
int force = 0;
int debug = 0;

int
main(int argc, char *argv[]) {
	int fd;

	set_progname(argv[0]);

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	if (argc == 2 && !strcmp(argv[1], "-V"))
	    print_version_and_exit();

	fd = getfd();

	if (argc >= 3 && !strcmp(argv[1], "-o")) {
	    saveunicodemap(fd, argv[2]);
	    argc -= 2;
	    argv += 2;
	    if (argc == 1)
	      exit(0);
	}
		
	if (argc > 2 || (argc == 2 && argv[1][0] == '-' && argv[1][1])) {
		fprintf(stderr, _("usage: %s [-o map.orig] [map-file]\n"),
			progname);
		exit(1);
	}

	loadunicodemap(fd, (argc > 1) ? argv[1] : "def.uni");
	exit(0);
}
#endif

/*
 * Skip spaces and read U+1234 or return -1 for error.
 * Return first non-read position in *p0 (unchanged on error).
 */ 
static
int getunicode(char **p0) {
  char *p = *p0;

  while (*p == ' ' || *p == '\t')
    p++;
  if (*p != 'U' || p[1] != '+' || !isxdigit(p[2]) || !isxdigit(p[3]) ||
      !isxdigit(p[4]) || !isxdigit(p[5]) || isxdigit(p[6]))
    return -1;
  *p0 = p+6;
  return strtol(p+2,0,16);
}

struct unimapinit advice;

struct unimapdesc descr;

struct unipair *list = 0;
int listsz = 0;
int listct = 0;
int fd;

static void
outlist(void) {
    advice.advised_hashsize = 0;
    advice.advised_hashstep = 0;
    advice.advised_hashlevel = 1;
  again:
    if(ioctl(fd, PIO_UNIMAPCLR, &advice)) {
	perror("PIO_UNIMAPCLR");
	exit(1);
    }
    descr.entry_ct = listct;
    descr.entries = list;
    if(ioctl(fd, PIO_UNIMAP, &descr)) {
	if (errno == ENOMEM && advice.advised_hashlevel < 100) {
	    advice.advised_hashlevel++;
#ifdef MAIN
	    printf(_("trying hashlevel %d\n"), advice.advised_hashlevel);
#endif
	    goto again;
	}
	perror("PIO_UNIMAP");
	exit(1);
    }
    listct = 0;
}

static void
addpair(int fp, int un) {
    if (listct == listsz) {
	listsz += 4096;
	list = xrealloc((char *)list, listsz);
    }
    list[listct].fontpos = fp;
    list[listct].unicode = un;
    listct++;
}

void
loadunicodemap(int fd, char *tblname) {
    FILE *mapf;
    char buffer[65536];
    int fontlen = 512;
    int i;
    int fp0, fp1, un0, un1;
    char *p, *p1;

    mapf = findfile(tblname, unidirpath, unisuffixes);
    if ( !mapf ) {
	perror(tblname);
	exit(EX_NOINPUT);
    }

    if (verbose)
      printf(_("Loading unicode map from file %s\n"), pathname);

    while ( fgets(buffer, sizeof(buffer), mapf) != NULL ) {
	if ( (p = strchr(buffer, '\n')) != NULL )
	  *p = '\0';
	else
	  fprintf(stderr, _("%s: %s: Warning: line too long\n"), progname, tblname);

	p = buffer;

/*
 * Syntax accepted:
 *	<fontpos>	<unicode> <unicode> ...
 *	<range>		idem
 *	<range>		<unicode range>
 *
 * where <range> ::= <fontpos>-<fontpos>
 * and <unicode> ::= U+<h><h><h><h>
 * and <h> ::= <hexadecimal digit>
 */

	while (*p == ' ' || *p == '\t')
	  p++;
	if (!*p || *p == '#')
	  continue;	/* skip comment or blank line */

	fp0 = strtol(p, &p1, 0);
	if (p1 == p) {
	    fprintf(stderr, _("Bad input line: %s\n"), buffer);
	    exit(EX_DATAERR);
	}
	p = p1;

	while (*p == ' ' || *p == '\t')
	  p++;
	if (*p == '-') {
	    p++;
	    fp1 = strtol(p, &p1, 0);
	    if (p1 == p) {
		fprintf(stderr, _("Bad input line: %s\n"), buffer);
		exit(EX_DATAERR);
	    }
	    p = p1;
	}
	else
	  fp1 = 0;

	if ( fp0 < 0 || fp0 >= fontlen ) {
	    fprintf(stderr,
		    _("%s: Glyph number (0x%x) larger than font length\n"),
		    tblname, fp0);
	    exit(EX_DATAERR);
	}
	if ( fp1 && (fp1 < fp0 || fp1 >= fontlen) ) {
	    fprintf(stderr,
		    _("%s: Bad end of range (0x%x)\n"),
		    tblname, fp1);
	    exit(EX_DATAERR);
	}

	if (fp1) {
	    /* we have a range; expect the word "idem" or a Unicode range
	       of the same length */
	    while (*p == ' ' || *p == '\t')
	      p++;
	    if (!strncmp(p, "idem", 4)) {
		for (i=fp0; i<=fp1; i++)
		  addpair(i,i);
		p += 4;
	    } else {
		un0 = getunicode(&p);
		while (*p == ' ' || *p == '\t')
		  p++;
		if (*p != '-') {
		    fprintf(stderr,
			    _("%s: Corresponding to a range of font positions, "
			    "there should be a Unicode range\n"),
			    tblname);
		    exit(EX_DATAERR);
		}
		p++;
		un1 = getunicode(&p);
		if (un0 < 0 || un1 < 0) {
		    fprintf(stderr,
			    _("%s: Bad Unicode range corresponding to "
			    "font position range 0x%x-0x%x\n"),
			    tblname, fp0, fp1);
		    exit(EX_DATAERR);
		}
		if (un1 - un0 != fp1 - fp0) {
		    fprintf(stderr,
			    _("%s: Unicode range U+%x-U+%x not of the same"
			      " length as font position range 0x%x-0x%x\n"),
			    tblname, un0, un1, fp0, fp1);
		    exit(EX_DATAERR);
		}
		for(i=fp0; i<=fp1; i++)
		  addpair(i,un0-fp0+i);
	    }
	} else {
	    /* no range; expect a list of unicode values
	       for a single font position */

	    while ( (un0 = getunicode(&p)) >= 0 )
	      addpair(fp0, un0);
	}
	while (*p == ' ' || *p == '\t')
	  p++;
	if (*p && *p != '#')
	  fprintf(stderr, _("%s: trailing junk (%s) ignored\n"), tblname, p);
    }

    fpclose(mapf);

    if (listct == 0 && !force)
      fprintf(stderr, _("%s: not loading empty unimap\n"
	              "(if you insist: use option -f to override)\n"),
	      progname);
    else
      outlist();
}

static struct unimapdesc
getunicodemap(int fd) {
  struct unimapdesc descr;

  descr.entry_ct = 0;
  descr.entries = 0;
  if(ioctl(fd, GIO_UNIMAP, (unsigned long) &descr)) {
      if(errno != ENOMEM || descr.entry_ct == 0) {
	  perror("GIO_UNIMAP");
	  exit(1);
      }
      descr.entries = (struct unipair *)
	malloc(descr.entry_ct * sizeof(struct unipair));
      if (descr.entries == NULL) {
	  fprintf(stderr, _("%s: out of memory?\n"), progname);
	  exit(1);
      }
      if(ioctl(fd, GIO_UNIMAP, (unsigned long) &descr)) {
	  perror("GIO_UNIMAP(2)");
	  exit(1);
      }
  }
#ifdef MAIN
  fprintf(stderr, "# %d %s\n", descr.entry_ct,
	 (descr.entry_ct == 1) ? _("entry") : _("entries"));
#endif

  return descr;
}

void
saveunicodemap(int fd, char *oufil) {
  FILE *fpo;
  struct unimapdesc descr;
  struct unipair *list;
  int i;

  if ((fpo = fopen(oufil, "w")) == NULL) {
      perror(oufil);
      exit(1);
  }

  descr = getunicodemap(fd);
  list = descr.entries;

  for(i=0; i<descr.entry_ct; i++)
      fprintf(fpo, "0x%02x\tU+%04x\n", list[i].fontpos, list[i].unicode);
  fclose(fpo);

  if (verbose)
    printf(_("Saved unicode map on `%s'\n"), oufil);
}

void
appendunicodemap(int fd, FILE *fp, int fontsize, int utf8) {
	struct unimapdesc descr;
	struct unipair *list;
	int i, j;

	descr = getunicodemap(fd);
	list = descr.entries;

		
	for(i=0; i<fontsize; i++) {
#if 0
		/* More than one mapping is not a sequence! */
		int no = 0;
		for(j=0; j<descr.entry_ct; j++) 
			if (list[j].fontpos == i)
				no++;
		if (no > 1)
			appendseparator(fp, 1, utf8);
#endif		
		if (debug) printf ("\nchar %03x: ", i);
		for(j=0; j<descr.entry_ct; j++)
			if (list[j].fontpos == i) {
				if (debug)
					printf ("%04x ", list[j].unicode);
				appendunicode(fp, list[j].unicode, utf8);
			}
		appendseparator(fp, 0, utf8);
	}


	if (debug) printf ("\n");
	if (verbose)
		printf(_("Appended Unicode map\n"));
}
