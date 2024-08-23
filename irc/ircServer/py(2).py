import socket
import threading
import time

class IRCClient:
    def __init__(self, server, port, password, nickname, realname, hostname, channel):
        self.server = server
        self.port = port
        self.password = password
        self.nickname = nickname
        self.realname = realname
        self.hostname = hostname
        self.channel = channel
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.sock.connect((self.server, self.port))
        self.sock.send(f"PASS {self.password}\r\n".encode('utf-8'))
        self.sock.send(f"NICK {self.nickname}\r\n".encode('utf-8'))
        self.sock.send(f"USER {self.nickname} {self.hostname} * :{self.realname}\r\n".encode('utf-8'))
        self.sock.send(f"JOIN {self.channel}\r\n".encode('utf-8'))
        threading.Thread(target=self.receive_messages).start()
        threading.Thread(target=self.send_periodic_messages).start()

    def send_message(self, message):
        self.sock.send(f"PRIVMSG {self.channel} :{message}\r\n".encode('utf-8'))

    def send_periodic_messages(self):
        while True:
            self.send_message("Hello from " + self.nickname)
            time.sleep(10)

    def receive_messages(self):
        while True:
            response = self.sock.recv(2048).decode('utf-8')
            if response.startswith("PING"):
                self.sock.send(f"PONG {response.split()[1]}\r\n".encode('utf-8'))
            else:
                print(response)

def start_clients(server, port, password, channel, nicknames, realnames, hostnames):
    clients = []
    for nickname, realname, hostname in zip(nicknames, realnames, hostnames):
        client = IRCClient(server, port, password, nickname, realname, hostname, channel)
        client.connect()
        clients.append(client)

if __name__ == "__main__":
    server = "localhost"
    port = 6667
    password = "1234"
    channel = "#test"
    nicknames = ["client1", "client2", "client3"]
    # realnames = ["Real Name 1", "Real Name 2", "Real Name 3"]
    # hostnames = ["host1", "host2", "host3"]
    start_clients(server, port, password, channel, nicknames, realnames, hostnames)
