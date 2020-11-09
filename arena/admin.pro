SOURCES	+= main.cpp
HEADERS	+= highlighter.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= admin.ui \
	io.ui \
	addrem.ui
IMAGES	= images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/undo \
	images/redo \
	images/editcut \
	images/editcopy \
	images/editpaste \
	images/searchfind
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
