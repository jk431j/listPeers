#include <iostream>
#include "dsTest.h"

using namespace std;

void print_usage(char* execName) {
    cerr << "Usage: " << basename(execName) << " [-d] [-h] server node name\n"
         << "List active Diameter peers on the dsTest server\n\n"
         << " -d\t debug mode, more verbose information\n"
         << " -h\t print diameter host too, not only realm\n"
         << "\n"
         << " server\t dsTest server name or IP\n"
         << " node\t type of the node (tdf, ocs, pcef)\n"
         << " name\t name of the node (e.g. tdf_1)\n"
         << endl;
}

int main(int argc, char* argv[]) {
    bool printHost {false},
         debugEnabled {false};
    int opt;

    while ((opt = getopt(argc, argv, "dh")) != -1) {
       switch (opt) {
       case 'd':
           debugEnabled = true;
           break;
       case 'h':
           printHost = true;
           break;
       default: /* '?' */
           print_usage(argv[0]);
           exit(EXIT_FAILURE);
       }
    }

    if (optind + 3 > argc) {
       print_usage(argv[0]);
       exit(EXIT_FAILURE);
    }

    dsTest dst(argv[optind]);

    stringstream cmd;
    cmd << "<" << argv[optind + 1] << " name=\"" << argv[optind + 2] << "\"" << " />";
    if (debugEnabled) {
        cerr << "Command to be run: " << cmd.str() << endl;
    }

    if (!dst.runCommand(cmd.str()))
        exit( EXIT_FAILURE );

    if (debugEnabled)
        dst.getResponse().print(cerr);

    pugi::xpath_node_set diameters = dst.getResponse().select_nodes("//diameter");

    for (auto &dstNode: diameters) {
        auto socket = dstNode.node().child("socket");
        if (strcmp(socket.child("status").child_value(), "Connected") == 0) {
            if (printHost)
                cout << dstNode.node().child("dest_host").child_value() << "\t";
            cout << dstNode.node().child("dest_realm").child_value() << "\t"
                 << socket.child("dest_ip_address").child_value();
            cout << endl;
        }
    }

    return 0;
}
