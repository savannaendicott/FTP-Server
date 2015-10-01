import socket
import subprocess
import os

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 5000))

s.listen(5)
print("Listening on port 5000")
c, addr = s.accept()

def start_server():
    while True:
        message = str(c.recv(1024))[2:] # First 2 chars are stream headers
        command = ""
        parameter = ""
        if " " in message:
            parts = message.split(" ")
            command = parts[0]
            parameter = parts[1]
        else:
            command = message

        if(len(command) > 0):
            print(message)
            handle_command(command, parameter)

def handle_command(command, parameter):

    if command == "ls":
        proc = subprocess.Popen(["ls"], stdout=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()
        c.send(out)


    elif command == "cd":
        if os.path.exists(parameter):
            os.chdir(parameter)
            c.send("ok")
        else:
            c.send("nok")


    elif command == "mkdir":
        if not os.path.exists(parameter):
            os.makedirs(parameter)
            c.send("ok")
        else:
            c.send("nok")


    elif command == "get":
        size = os.path.getsize(parameter)
        

        with open(parameter,'rb') as f:
            c.sendall(f.read())
        print "sent"

    elif command == "put":
        chunks = []

        while "&endm;" not in chunks:
            chunk = c.recv(4096)
            chunks += chunk
            if "&endm;" in chunk:
                break

        msg = chunks[:-6]

        with open(parameter,'wb') as f:
            f.write(b''.join(msg))
        print "end put"


    elif command == "exit":
        s.close()

    else:
        c.send("command: '%s' not recognized" % command)

start_server()
