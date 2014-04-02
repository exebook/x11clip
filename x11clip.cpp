#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "x11clip.h"
using namespace v8;

Handle<Value> start(const Arguments& a) {
	HandleScope scope;
	XClipStart();
	return scope.Close(String::New("started"));
}

Handle<Value> stop(const Arguments& a) {
	quit = true;
	return Undefined();
}

Handle<Value> get(const Arguments& a) {
	HandleScope scope;
	if (paste_text == 0) return Undefined();
	Handle<String> s = String::New(paste_text, paste_size);
	paste_text = 0; paste_size = 0;
	return scope.Close(s);
}

Handle<Value> step(const Arguments& a) {
	HandleScope scope;
	bool ret = XStep();
	return Boolean::New(ret);
}

Handle<Value> copy(const Arguments& a) {
	HandleScope scope;
	String::Utf8Value s(a[0]);
	XCopy(*s, s.length());
	return Undefined();
}

Handle<Value> paste(const Arguments& a) {
	XPaste();
	return Undefined();
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("start"), FunctionTemplate::New(start)->GetFunction());
	exports->Set(String::NewSymbol("stop"), FunctionTemplate::New(stop)->GetFunction());
	exports->Set(String::NewSymbol("step"), FunctionTemplate::New(step)->GetFunction());
	exports->Set(String::NewSymbol("get"), FunctionTemplate::New(get)->GetFunction());
	exports->Set(String::NewSymbol("copy"), FunctionTemplate::New(copy)->GetFunction());
	exports->Set(String::NewSymbol("paste"), FunctionTemplate::New(paste)->GetFunction());
}

NODE_MODULE(x11clip, init)

