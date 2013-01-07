Welcome to "StreamingViz"!

This is a visualization tool dedicated to display networks in tree style. In order to use "StreamingViz", you need to establish a TCP or UDP connection. "StreamingViz" is a server application. Start the program and look up the name or IP address and port you need to connect to.
In the file config.ini you can alter the port number you want "StreamingViz" to listen on and the protocol type. In addition, you can modify the language, display of the welcome screen and some colors, for further details have a look at the documentation. Ctrl-H turns off the high quality display.

To change language open config.ini and modify the value of "lang" as follows:
0 -> German
1 -> English

If you want the splash screen to disappear modify the value of "splash" as follows:
0 -> no splash screen
1 -> show splash screen

Modify "protocol" to switch between TCP and UDP connection.
tcp
udp

Specify "port" number to listen on, default is: 3003

Colors of "background", "nodes" and "text" are in RGB format (0-255)