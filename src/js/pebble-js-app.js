var KEY_INVERT = 0;

Pebble.addEventListener("ready",
    function(e) {
        Pebble.addEventListener('showConfiguration', function(e) {
            // Show config page
            Pebble.openURL('http://mad-gooze.github.io/pebble-russian-calendar-watchface/config.html');
        });

        Pebble.addEventListener("webviewclosed", function(e) {
            //Get JSON dictionary
            var configuration = JSON.parse(decodeURIComponent(e.response));
            console.log("Configuration window returned: " + JSON.stringify(configuration));

            //Send to Pebble, persist there
            Pebble.sendAppMessage(
                {0: configuration.invert},
                function(e) {
                    console.log("Sending settings data...");
                },
                function(e) {
                    console.log("Settings feedback failed!");
                }
            );
        });
    }
);

