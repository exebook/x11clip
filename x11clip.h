#include <stdio.h> // printf
#include <string.h> // strndup
#include <stdlib.h> // free
#include <X11/Xlib.h>

static Display * display;
static Window window;
int quit = 0;
Atom targets_atom, text_atom, UTF8, XA_ATOM = 4, XA_STRING = 31, XSEL_DATA, CLIPBOARD;
unsigned char * copy_text = 0; char * paste_text = 0; 
int copy_size, paste_size;

bool XStep() {
	XEvent event;
	int format;
	unsigned long N, size;
	char * data;
	Atom target;
   if (quit) return false;
	if (!XPending(display)) return false;
	XNextEvent(display, &event);
	if (event.type == SelectionNotify) {
		if (event.xselection.selection != CLIPBOARD) return true;
		if (event.xselection.property) {
			XGetWindowProperty(event.xselection.display, event.xselection.requestor,
				event.xselection.property, 0L, (~0L), 0, AnyPropertyType, &target,
				&format, &size, &N,(unsigned char**)&data);
			if (target == UTF8 || target == XA_STRING) {
				paste_text = strndup(data, size);
				paste_size = size;
				XFree(data);
			}
			XDeleteProperty(event.xselection.display, event.xselection.requestor, event.xselection.property);
		} 
	} else if (event.type == SelectionRequest) {
		if (event.xselectionrequest.selection != CLIPBOARD) return true;
		XSelectionRequestEvent * xsr = &event.xselectionrequest;
		XSelectionEvent ev = {0};
		int R = 0;
		ev.type = SelectionNotify, ev.display = xsr->display, ev.requestor = xsr->requestor,
		ev.selection = xsr->selection, ev.time = xsr->time, 
		ev.target = xsr->target, ev.property = xsr->property;
		
		if (ev.target == targets_atom) R = XChangeProperty(
			ev.display, ev.requestor, ev.property, XA_ATOM, 32,
			PropModeReplace, (unsigned char*)&UTF8, 1);
		else if (ev.target == XA_STRING || ev.target == text_atom) 
			R = XChangeProperty(ev.display, ev.requestor, ev.property, 
			XA_STRING, 8, PropModeReplace, copy_text, copy_size);
		else if (ev.target == UTF8)
			R = XChangeProperty(ev.display, ev.requestor, ev.property, 
			UTF8, 8, PropModeReplace, copy_text, copy_size);
		else ev.property = None;
		if ((R & 2) == 0) {
			XSendEvent (display, ev.requestor, 0, 0, (XEvent *)&ev);
		}
	}
	return true;
}

void XCopy(char * text, int size) {
	if (copy_text) free(copy_text);
	XSetSelectionOwner (display, CLIPBOARD, window, 0);
	if (XGetSelectionOwner (display, CLIPBOARD) != window) return;
	copy_text = (unsigned char*) strndup(text, size);
	copy_size = size;
}

char * XPaste() {
	if (paste_text) free(paste_text);
	paste_text = 0;
	paste_size = 0;
	XConvertSelection(display, CLIPBOARD, UTF8, XSEL_DATA, window, CurrentTime);
	XSync(display, 0);
}

void XClipStart() {
	display = XOpenDisplay(0);
	int N = DefaultScreen(display);
	window = XCreateSimpleWindow(display, RootWindow(display, N), 0, 0, 1, 1, 0,
		BlackPixel(display, N), WhitePixel(display, N));	
	targets_atom = XInternAtom(display, "TARGETS", 0);
	text_atom = XInternAtom(display, "TEXT", 0);
	UTF8 = XInternAtom(display, "UTF8_STRING", 1);
	if (UTF8 == None) UTF8 = XA_STRING;
	Atom selection = XInternAtom(display, "CLIPBOARD", 0);
	CLIPBOARD = XInternAtom(display, "CLIPBOARD", 0),
	XSEL_DATA = XInternAtom(display, "XSEL_DATA", 0);
}

