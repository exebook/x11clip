var x11clip = require('./build/Release/x11clip.node')

exports.pasteCallback = function() {}
exports.start = x11clip.start
exports.paste = x11clip.paste
exports.copy = x11clip.copy

exports.mainLoop = function() {
	var speed = 100, timer
	function slowDown() {
		speed = 100
		timer = undefined
	}
	function go() {
		// для интеграции с основным приложением
		if (typeof glxwin != 'undefined' && glxwin.x11quit()) return
		var event = x11clip.step()
		if (event) {
			speed = 1
			if (timer) clearTimeout(timer)
			timer = setTimeout(slowDown, 1000)
			var s = x11clip.get()
			if (s) exports.pasteCallback(s)
		}
		setTimeout(function() { setImmediate(go) }, speed)
	}
	setImmediate(go)
}


