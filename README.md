# MobileInsight
local_server.c is the server side that initializes a local server of port 2048(port num can be changed)
local_client.c is the client side that inputs information to server side.

Use: (on linux server)

gcc -o local_server local_server.c
gcc -o local_client local_client.c

./local_server
./local_client
Then input on the client side will appear on server side. 

This is a simple simulation of telnet-like client-server using unix socket and poll. The client side can change the input from 
standard input to android deamon. And the server side can store the output in a file to use rather than write to standard out.
