x11clip
=======

Дополнение к Node.js для доступа к буферу обмена X-Windows


```javascript
var x11clip = require('./x11clip')

x11clip.start()
x11clip.mainLoop()
x11clip.copy('пример')
x11clip.pasteCallback = function(txt){ console.log(txt) } // пример
x11clip.paste()

```

