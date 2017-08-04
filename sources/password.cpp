#include "password.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace ns_cipher {
    std::string get_password() {
        termios tty;
        tcgetattr(STDIN_FILENO, &tty);
        tty.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);

        std::cout << "password: " << std::flush;
        std::string password;
        std::cin >> password;
        std::cout << std::endl;

        tty.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        return password;
    }
}
