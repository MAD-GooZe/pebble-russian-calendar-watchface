Pebble.addEventListener("ready",
    function(e) {
        Pebble.addEventListener('showConfiguration', function(e) {
            // Show config page
            Pebble.openURL('http://mad-gooze.github.io/pebble-russian-calendar-watchface/config.html');
        });
    }
);

