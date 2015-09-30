import socket
import subprocess
import os

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 5000))

s.listen(5)
print("Listening on port 5000")
c, addr = s.accept()

def recieve_reply():
    message = str(c.recv(1024))
    message_parts = message.split()
    command = message_parts[0].strip()[2:]

    print len(command)

    print(command == "ls")
    if len(message_parts) > 1:
        parameter = message_parts[1]

    if command == "ls":
        proc = subprocess.Popen(["ls"], stdout=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()
        print(out)
        c.send(out)


    elif command == "cd":
        if os.path.exists(parameter):
            os.chdir(parameter)


    elif command == "mkdir":
        if not os.path.exists(parameter):
            os.makedirs(parameter)


    elif command == "get":
        size = os.path.getsize(parameter)
        c.send(str(size))

        with open(parameter,'rb') as f:
            c.sendall(f.read())


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


recieve_reply()
