SOURCES	+= main.cpp
HEADERS	+= highlighter.h \
	common.h \
	decl.h
















unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= ide.ui \
	welcome.ui \
	rules.ui \
	reg.ui \
	score.ui \
	result.ui \
	test.ui \
	options.ui
IMAGES	= images/gnome-gemvt.png \
	images/procman.png
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
