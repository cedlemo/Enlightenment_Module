ACLOCAL_AMFLAGS = -I m4
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
		       config.sub configure depcomp install-sh ltmain.sh \
		       missing module.desktop config.rpath mkinstalldirs

SUBDIRS = src

if HAVE_PO

SUBDIRS += po

endif

EDJE_FLAGS = -v \
	     -id $(top_srcdir)/images

filesdir = $(datadir)
files_DATA = module.desktop e-module-skel.edj skel.edj

EXTRA_DIST = module.desktop.in \
	     e_modules-skel.spec.in \
	     e-module-skel.edc \
	     skel.edc \
	     images/skel.png \
	     images/module_icon.png

%.edj:  %.edc
	$(EDJE_CC) $(EDJE_FLAGS) $< $@

clean-local:
	 rm -rf skel.edj e-module-skel.edj module.desktop e_modules-skel.spec *~

uninstall:
	rm -rf $(DESTDIR)$(datadir)
