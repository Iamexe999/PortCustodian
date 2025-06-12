#include "scanner.h"
#include "vulnerability.h"
#include "logger.h"
#include <iostream>

int main() {
    std::string target;
    std::cout << "Enter target IP: ";
    std::cin >> target;

    // Initialize components
    PortScanner scanner(target);
    VulnerabilityChecker vulnChecker;
    Logger logger("scan_report.txt");
    
    vulnChecker.loadSignatures("data/vuln_signatures.txt");
    
    // Scan common ports
    scanner.scanPorts(20, 1000);
    
    // Check vulnerabilities
    for (int port : scanner.getOpenPorts()) {
        std::string service = "HTTP"; // Simplified for demo
        std::string vuln = vulnChecker.checkVulnerability(port, service);
        logger.log("Port " + std::to_string(port) + ": " + vuln);
    }
    
    std::cout << "Scan complete! Report saved to scan_report.txt\n";
    return 0;
}