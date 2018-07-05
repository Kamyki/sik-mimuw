#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

#include "tcpConnection.h"
#include <vector>
#include <string>
#include <functional>


#define IAC 255
#define DO 253
#define WILL 251

#define ECHO 1
#define LINEMODE 34

#define LF 10
#define CR 13

using namespace std;

bool connection_active = true;


static const vector<uint8_t> ENTER_BYTES() {
    vector<uint8_t> a{13, 0};
    return a;
}

static const vector<uint8_t> UP_BYTES() {
    return {27, 91, 65};
}

static const vector<uint8_t> DOWN_BYTES() {
    return {27, 91, 66};
}

static const vector<uint8_t> INIT() {
    return {IAC, WILL, ECHO, IAC, DO, LINEMODE};
}

enum Command {
    DUMMY,
    ENTER,
    UP,
    DOWN,
    STREAM_END,
};

string csi(const string& value) {
    return "\033[" + value;
}

void clear_lines(ConnectionBuffer& connectionBuffer, int n) {
    string to_send = csi("2K") + csi("A");
    for (int i = 0; i < n - 1; ++i) {
        connectionBuffer.write(to_send);
    }
    connectionBuffer.write(csi("2K"));
}

struct Menu {
    vector<string> options;
    vector<std::function<void(ConnectionBuffer&)>> actions;
    unsigned long active = 0;

    Menu() = default;

    Menu(const vector<string>& options, vector<function<void(ConnectionBuffer&)>> actions, unsigned int acitve)
            : options(
            options), actions(std::move(actions)), active(acitve) {}

    void render(ConnectionBuffer& connectionBuffer) {
        for (unsigned long i = 0; i < options.size(); ++i) {
            if (this->active == i) {
                connectionBuffer.write(csi("7m"));
                connectionBuffer.write(options[i]);
                connectionBuffer.write(csi("0m"));
                connectionBuffer.write(CR);
                connectionBuffer.write(LF);
            } else {
                connectionBuffer.write(options[i]);
                connectionBuffer.write(CR);
                connectionBuffer.write(LF);
            }
            connectionBuffer.flush();
        }
    }

    void execute(Command command, ConnectionBuffer& connectionBuffer) {
        switch (command) {
            case STREAM_END:
                connection_active = false;
                return;
            case ENTER:
                actions[active](connectionBuffer);
                break;
            case UP:
                active = (active - 1 + options.size()) % options.size();
                clear_lines(connectionBuffer, 4);
                render(connectionBuffer);
                break;
            case DOWN:
                active = (active + 1) % options.size();
                clear_lines(connectionBuffer, 4);
                render(connectionBuffer);
                break;
            default:
                break;
        }
    }
};


long readPortNumber(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "USAGE: ./server port" << endl;
        exit(-1);
    }
    long port_number;
    try {
        port_number = std::stoi(argv[1]);
    } catch (invalid_argument& exc) {
        cerr << "Argument should be a valid number" << endl;
        exit(-1);
    } catch (out_of_range& exc) {
        cerr << "Out of range number" << endl;
        exit(-1);
    }
    if (port_number < 0 || port_number > std::numeric_limits<uint16_t>::max()) {
        cerr << "Port number should be between 0 and " << numeric_limits<uint16_t>::max() << endl;
        exit(-1);
    }
    return port_number;
}

Command recv_command(ConnectionBuffer& connectionBuffer) {
    vector<uint8_t> bytes = connectionBuffer.recieve(3);
    vector<uint8_t> a;
    a.push_back(1);
    printf("LINE: ");
    for(int k=0; k<bytes.size(); ++k) {
        printf("%x ", bytes[k]);
    }
    printf("\n");
    if (bytes.empty())
        return STREAM_END;
    else if (ENTER_BYTES() == bytes)
        return ENTER;
    else if (UP_BYTES() == bytes)
        return UP;
    else if (DOWN_BYTES() == bytes)
        return DOWN;
    else
        return DUMMY;
}

Menu menu;

void actionB1(ConnectionBuffer& connectionBuffer) {
    connectionBuffer.write("B1");
    connectionBuffer.write(csi("2D"));
}

void actionB2(ConnectionBuffer& connectionBuffer) {
    connectionBuffer.write("B2");
    connectionBuffer.write(csi("2D"));
}

void actionA(ConnectionBuffer& connectionBuffer) {
    connectionBuffer.write("A");
    connectionBuffer.write(csi("1D"));
}

void actionB(ConnectionBuffer& connectionBuffer);

void actionK(ConnectionBuffer& connectionBuffer) {
    connection_active = false;
}

void actionW(ConnectionBuffer& connectionBuffer) {
    vector<string> options1{
            "Opcja A",
            "Opcja B",
            "Koniec"
    };
    vector<function<void(ConnectionBuffer&)>> acitons1{
            actionA,
            actionB,
            actionK
    };
    menu = Menu(options1, acitons1, 1);
    clear_lines(connectionBuffer, 4);
    menu.render(connectionBuffer);

};


void actionB(ConnectionBuffer& connectionBuffer) {
    vector<string> option2{
            "Opcka B1",
            "Opcja B2",
            "Wstecz"
    };

    vector<function<void(ConnectionBuffer&)>> actions2{
            actionB1,
            actionB2,
            actionW
    };
    menu = Menu(option2, actions2, 1);
    clear_lines(connectionBuffer, 4);
    menu.render(connectionBuffer);
}


int main(int argc, char *argv[]) {
    long port = readPortNumber(argc, argv);


    vector<string> options1{
            "Opcja A-----",
            "Opcja B-----\n",
            "Koniec"
    };
    vector<function<void(ConnectionBuffer&)>> acitons1{
            actionA,
            actionB,
            actionK
    };


    try {
        tcpConnection connection(port);
        while (true) {
            auto clientConnection = connection.handleClinet();
            connection_active = true;
            menu = Menu(options1, acitons1, 0);
            cout << "Client connection from address " << clientConnection.toString() << endl;
            clientConnection.write(INIT());
            menu.render(clientConnection);
            while (connection_active) {
                Command command = recv_command(clientConnection);
                menu.execute(command, clientConnection);


            }
            clientConnection.write(csi("?25h"));
            cout << "Connection terminated" << endl;
        }


    } catch (TcpException& tcp_exception) {
        cout << "Exiting due to connection error." << endl;
        cerr << "Exception: " << tcp_exception.what() << endl;
    }
}


