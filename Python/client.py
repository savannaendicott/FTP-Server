import socket
import subprocess
import os

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#s.connect(("127.0.0.1", 5000))
s.connect(("159.203.82.155", 5000))


while True:
    message = raw_input("prompt: ")
    s.send(message)
    
    message_parts = message.split()
    command = message_parts[0].strip()
    
    if len(message_parts) > 1:
        parameter = message_parts[1]
    
    
    
    if command == "ls":
        data = s.recv(4096)
        print data
        
    elif command == "cd":
        pass
        
    elif command == "mkdir":
        pass
        
    elif command == "get":
        size = int(s.recv(4))
        
        chunks = []
        while len(chunks) < size:
            chunk = s.recv(4096)
            chunks += chunk
            
        with open(parameter,'wb') as f:
            f.write(b''.join(chunks))
        
    elif command == "put":
        #size = os.path.getsize(parameter)
        #s.send(str(size))
        
        with open(parameter,'rb') as f:
            s.sendall(f.read())
        s.send("&endm;")
    
    elif command == "exit":
        s.close()
        break
    
    else:
        print "that command wasn't recognized"

