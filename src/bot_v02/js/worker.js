self.addEventListener('message', function(evt) {

if (evt.data == "startBot") {

        var timer = setInterval(tick, 50);
        evt.data = "rip";
}

});

function tick() {

    self.postMessage("message");
}

