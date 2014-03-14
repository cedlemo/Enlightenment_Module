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
files_DATA = module.desktop e-module-hello_world.edj hello_world.edj

EXTRA_DIST = module.desktop.in \
	     e_modules-hello_world.spec.in \
	     e-module-hello_world.edc \
	     hello_world.edc \
	     images/hello_world.png \
	     images/module_icon.png

%.edj:  %.edc
	$(EDJE_CC) $(EDJE_FLAGS) $< $@

clean-local:
	 rm -rf hello_world.edj e-module-hello_world.edj module.desktop e_modules-hello_world.spec *~

uninstall:
	rm -rf $(DESTDIR)$(datadir)
