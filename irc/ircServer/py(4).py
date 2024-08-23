import socket
import threading

class IRCClient:
    def __init__(self, server, port, password, nickname, channel):
        self.server = server
        self.port = port
        self.password = password
        self.nickname = nickname
        self.channel = channel
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.sock.connect((self.server, self.port))
        self.sock.send(f"PASS {self.password}\r\n".encode('utf-8'))
        self.sock.send(f"NICK {self.nickname}\r\n".encode('utf-8'))
        self.sock.send(f"USER {self.nickname} 0 * :{self.nickname}\r\n".encode('utf-8'))
        self.sock.send(f"JOIN {self.channel}\r\n".encode('utf-8'))
        threading.Thread(target=self.receive_messages).start()

    # def send_message(self, message):
        # self.sock.send(f"PRIVMSG {self.channel} :{message}\r\n".encode('utf-8'))

    def receive_messages(self):
        while True:
            response = self.sock.recv(2048).decode('utf-8')
            if response.startswith("PING"):
                self.sock.send(f"PONG {response.split()[1]}\r\n".encode('utf-8'))
            else:
                print(response)

def start_clients(server, port, password, channel, nicknames):
    clients = []
    for nickname in nicknames:
        client = IRCClient(server, port, password, nickname, channel)
        client.connect()
        clients.append(client)

if __name__ == "__main__":
    server = "localhost"
    port = 6667
    password = "1234"
    channel = "#test"
    nicknames = ["client1", "client2", "client3"]
    start_clients(server, port, password, channel, nicknames)
