var http = require("http");
var url = require("url");
var Wemo = require('wemo-client');
var wemo = new Wemo();

wemo.discover(function(deviceInfo) {
    console.log("Found Wemo! Starting server...");
    // Get the client for the found device
    var client = wemo.client(deviceInfo);

    var postServer = http.createServer((req, res) => {
        var uri = url.parse(req.url, true);

        console.log("Forward: "+uri.pathname);

        if (uri.pathname == "/on")
            client.setBinaryState(1);
        else if (uri.pathname == "/off")
            client.setBinaryState(0);

        res.end();
    });

    console.log("Server started...");
    postServer.listen(80);
});
