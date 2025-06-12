#include "scanner.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

PortScanner::PortScanner(const std::string& target) : targetIP(target) {}

bool PortScanner::isPortOpen(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, targetIP.c_str(), &serverAddr.sin_addr);

    // Set non-blocking
    fcntl(sock, F_SETFL, O_NONBLOCK);

    connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    fd_set fdset;
    struct timeval tv;
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);
    tv.tv_sec = 1; // 1 second timeout
    tv.tv_usec = 0;

    if (select(sock + 1, nullptr, &fdset, nullptr, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
        close(sock);
        return (so_error == 0);
    }

    close(sock);
    return false;
}

void PortScanner::scanPorts(int start, int end) {
    openPorts.clear();
    for (int port = start; port <= end; port++) {
        if (isPortOpen(port)) {
            openPorts.push_back(port);
            std::cout << "[+] Port " << port << " open\n";
        }
    }
}

std::vector<int> PortScanner::getOpenPorts() const {
    return openPorts;
}