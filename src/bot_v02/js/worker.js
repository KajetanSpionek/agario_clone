self.addEventListener('message', function(evt) {

if (evt.data == "startBot") {

        var timer = setInterval(tick, 1000/60);
        evt.data = "rip";
}

});

function tick() {

    self.postMessage("message");
}

