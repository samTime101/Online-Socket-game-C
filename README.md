# Online-Socket-game-C
A simple online game in C made using sockets

```
Author:samTime101
Date:Aug 12,2024 2081/4/28
```
# preview
[preview.webm](https://github.com/user-attachments/assets/33cb299f-534e-4050-8f82-5ff5dd940e88)


# Prerequisites
- Linux socket API
- online TCP server (Use NGROK because this code is based on ngrok)
# Compile
compile the server side code on your computer you want to host and play , *remember the host also plays*, you can just only download *server.c* and compile
`gcc server.c -o server`

and run server

`./server` remember the default port is 8080 , i have hardcoded it in my program , i might make it more dynamic but currently it is what it is
now open another terminal in host machine and run

`ngrok tcp 8080`

now we will get output like this

```
ngrok                                                                                          (Ctrl+C to quit)
                                                                                                               
Try our new Traffic Inspector: https://ngrok.com/r/ti                                                          
                                                                                                               
Session Status                online                                                                           
Account                       ********************************                                              
Version                       3.14.0                                                                           
Region                        ********************************                                                              
Latency                       2ms                                                                              
Web Interface                 http://127.0.0.1:4040                                                            
Forwarding                    tcp://0.tcp.ap.ngrok.io:<ngrokport> -> localhost:8080                                  
                                                                                                               
Connections                   ttl     opn     rt1     rt5     p50     p90                                      
                              0       0       0.00    0.00    65.22   65.22                                    
                                                                                 
```
now copy the ngrok port

now In Player 2 computer

compile the *clientn.c*

`gcc clientn.c -o client`

and run the client code

`./client <ngrokport>`
now you will be connected to host computer and both can play
