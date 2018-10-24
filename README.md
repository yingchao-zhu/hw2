# hw2

You may want to open two terminals(t1 and t2) in order to send the receive the message between client and server.
In t1(server):
>>> make all
//to run udp server and client
>>>./server_c_udp [port number]		//eg. (./server_c_udp 4455)

then open t2
In t2(client):
//to run udp client
>>> ./client_c_udp [IP address] [port number]		//eg. (./client_c_udp 127.0.0.1 4455)


//to run tcp server and client
In t1(server):
>>> ./server_c_tcp [port number]		//eg.(./server_c_tcp 4455)

then open t2
In t2(client):
>>> ./client_c_tcp [IP address] [the same port number]	//eg.(./client_c_tcp 127.0.0.1 4455)
