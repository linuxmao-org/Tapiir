bin_PROGRAMS = tapiir tapiir_alsa tapiir_jack

tapiir_common_SOURCES = \
	FileIO.cxx \
	FileIO.hxx \
	GUI.cxx \
	GUI.hxx \
	IRWindow.cxx \
	IRWindow.hxx \
	Layout.cxx \
	Layout.hxx \
	LayoutBig.cxx \
	LayoutDefines.hxx \
	LayoutSmall.cxx \
	MTD.hxx \
	MTDBase.hxx \
	MTDdecl.hxx \
	MTDStore.hxx \
	Mixer.cxx \
	Mixer.hxx \
	Tap.cxx \
	Tap.hxx \
	TapSliderGroup.hxx \
	globals.hxx \
	tapiir_diagram.cxx \
	tapiir_diagram.hxx \
	tapiir_image.cxx

tapiir_SOURCES = $(tapiir_common_SOURCES) main.cxx alsaio.cxx jackio.cxx

tapiir_alsa_SOURCES = $(tapiir_common_SOURCES) main_alsa.cxx alsaio.cxx

tapiir_jack_SOURCES = $(tapiir_common_SOURCES) main_jack.cxx jackio.cxx

tapiir_LDADD = @LIBS@ @ALSA_LIBS@ @JACK_LIBS@

tapiir_alsa_LDADD = @LIBS@ @ALSA_LIBS@

tapiir_jack_LDADD = @LIBS@ @ALSA_LIBS@ @JACK_LIBS@

install-exec-hook:
	chown root $(DESTDIR)$(bindir)/tapiir
	chmod u+s $(DESTDIR)$(bindir)/tapiir
	chown root $(DESTDIR)$(bindir)/tapiir_alsa
	chmod u+s $(DESTDIR)$(bindir)/tapiir_alsa
	chown root $(DESTDIR)$(bindir)/tapiir_jack
	chmod u+s $(DESTDIR)$(bindir)/tapiir_jack

DISTCLEANFILES = Makefile.in

EXTRA_DIST = LayoutSet.cxx MTDConvert.cxx
