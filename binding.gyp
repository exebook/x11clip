{
  "targets": [
    {
	"target_name": "x11clip",
	"include_dirs": [],
	"sources": [ "x11clip.cpp" ],
	"cflags": ["-O0 -w -Wfatal-errors -fshort-wchar -g"],
	"defines": ["UNIX"],
	"libraries": ["-lv8", "-lX11"]
    }
  ],"variables" : { "clang" : 1 }
}
