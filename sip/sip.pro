SOURCES	+= main.cpp
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= sip.ui
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
