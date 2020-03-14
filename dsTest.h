//
// Created by jan on 9/12/19.
//

#ifndef XMLOTCP_DSTEST_H
#define XMLOTCP_DSTEST_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/lexical_cast.hpp>
#include <arpa/inet.h>
#include "pugixml.hpp"

namespace ba = ::boost::asio;
namespace ip = ::boost::asio::ip;
using namespace std;

extern ba::io_service asio_service;

class dsTest {
public:
    dsTest(const string& nameOrIP) : dsTserver(nameOrIP), dns(asio_service), dsTsocket(asio_service)  {}

    ~dsTest() {
        if (dsTsocket.is_open())
            dsTsocket.close();
    }

    bool connect();
    bool runCommand(const string& command);
    pugi::xml_document& getResponse();
    void print_stats(std::ostream& where);

private:
    ip::tcp::resolver dns;
    string dsTserver;
    ip::tcp::socket dsTsocket;
    pugi::xml_document xmlResponse;
    std::size_t  inBytes{0}, outBytes{0}, xmlBytes{0};
};

#endif //XMLOTCP_DSTEST_H
