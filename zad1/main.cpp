#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "tcpConnection.h"
#include "menu.h"
#include "action.h"
#include "telnet.h"


using namespace std;

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

State prepareState() {
    auto sub_menu = make_shared<vector<Option>>(vector<Option>{
            Option("Opcja B1", make_shared<AddText>("B1")),
            Option("Opcja B2", make_shared<AddText>("B2")),
            Option("Wstecz")
    });

    auto main_menu = make_shared<vector<Option>>(vector<Option>{
            Option("Opccja A", make_shared<AddText>("A")),
            Option("Opcja B", make_shared<EnterSubmenu>(sub_menu))
    });
    (*sub_menu)[2].setAction(make_shared<EnterSubmenu>(main_menu));

    auto menu = make_shared<Menu>(main_menu);

    State state(menu, 1);
    return state;
}


void loop(State state, Telnet telnet) {
    bool client_connected = true;
    while (client_connected) {
        try {
            auto key = telnet.readCommand();
            switch (key) {
                case Keys::UP:
                    break;
                case Keys::DOWN:
                    break;
                case Keys::ENTER:
                    state = state.getOption().action->execute(state);
                    break;
                case Keys::STREAM_END:
                    client_connected = false;
                    break;
                default:
                    ;
            }
        } catch (const Eof& eof) {
            client_connected = false;
        }

        if (client_connected) {
            //  connectionBuffer.send(state.toMessage());
            //  connectionBuffer.flush();
        } else {
            std::cout << "Client has disconnected." << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {

    try {
        tcpConnection connection(readPortNumber(argc, argv));
        while (true) {
            auto clientConnection = connection.handleClinet();
            cout << "Client connection from address " << clientConnection.toString() << endl;
            try {
                State state = prepareState();
                Telnet telnet(clientConnection);
                clientConnection.send(telnet.init());
                clientConnection.send(state.initial());
                clientConnection.send(state.toMessage());
                clientConnection.flush();

                loop(state, telnet);

            } catch (exception& io_exception) {
                cout << "Disconnecting client due to IO error." << endl;
                cerr << "IOException: " << io_exception.what() << endl;
            }
        }
    } catch (TcpException& tcp_exception) {
        cout << "Exiting due to connection error." << endl;
        cerr << "Exception: " << tcp_exception.what() << endl;
    }

}