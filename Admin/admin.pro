TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release


HEADERS	+= highlighter.h
SOURCES	+= main.cpp
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

























unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
