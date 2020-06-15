ESP WebSockets Sample
===

This sample shows how to create a WebSockets ESP controller.
The controller is in websockets.c. It registers one action that is run in response
to the URI: /ws/test/echo.

The HTML page web/test.html is served to the client browser to issue a web socket
request.

The server is configured to keep the web socket open for 1 minute of inactivity and
then close the connection.

Requirements
---

* [Appweb](http://embedthis.com/downloads/appweb/download.ejs)
* [Bit Build Tool](http://embedthis.com/downloads/bit/download.ejs)

To build:
---
    bit 

To run:
---
    bit run

The server listens on port 8080. Browse to: 
 
     http://localhost:8080/test.html

This opens a web socket and sends the message "Hello Server" to the server. The server responds
by echoing back the message. After one minute of inactivity, the server will close the connection.

Code:
---
* [cache](cache) - Directory for compiled ESP modules
* [appweb.conf](appweb.conf) - Appweb server configuration file
* [server.c](server.c) - Web server main program
* [websockets.c](websockets.c) - ESP WebSockets controller source
* [start.bit](start.bit) - Bit build instructions
* [web](web) - Directory containing the test.html web page

Documentation:
---
* [Appweb Documentation](http://embedthis.com/products/appweb/doc/index.html)
* [ESP Directives](http://embedthis.com/products/appweb/doc/guide/appweb/users/dir/esp.html)
* [ESP Tour](http://embedthis.com/products/appweb/doc/guide/esp/users/tour.html)
* [ESP Controllers](http://embedthis.com/products/appweb/doc/guide/esp/users/controllers.html)
* [ESP APIs](http://embedthis.com/products/appweb/doc/api/esp.html)
* [ESP Guide](http://embedthis.com/products/appweb/doc/guide/esp/users/index.html)
* [ESP Overview](http://embedthis.com/products/appweb/doc/guide/esp/users/using.html)

See Also:
---
* [esp-controller - Serving ESP controllers](../esp-controller/README.md)
* [esp-mvc - Serving ESP MVC applications](../esp-mvc/README.md)
* [esp-page - Serving ESP pages](../esp-page/README.md)
* [secure-server - Secure server](../secure-server/README.md)
* [simple-server - Simple server and embedding API](../simple-server/README.md)
* [typical-server - Fully featured server and embedding API](../typical-server/README.md)
