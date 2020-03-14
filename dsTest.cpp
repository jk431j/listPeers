//
// Created by jan on 9/12/19.
//

#include "dsTest.h"

ba::io_service asio_service;


bool dsTest::connect() {
    boost::system::error_code ec;

    ip::tcp::resolver::iterator endp = dns.resolve(dsTserver, "9999");
    dsTsocket.connect(*endp, ec);

    return !ec && dsTsocket.is_open();
}


bool dsTest::runCommand(const string &command) {
    if (!dsTsocket.is_open()) {
        bool rslt = connect();
        if (!rslt)
            return false;
    };

    boost::system::error_code ec;

    ba::streambuf outStrBuff;
    ostream outStr(&outStrBuff);

    outStr << R"(<?xml version="1.0" encoding="UTF-8"?><dst:devsol xmlns:dst="http://www.developingsolutions.com/schema/dsTest"><command>)" << endl;
    outStr << command << endl;
    outStr << R"(</command></dst:devsol>)";

    boost::uint32_t xmlLen = htonl(outStrBuff.size());

    vector<ba::const_buffer> outBuffs {
            ba::buffer(&xmlLen, sizeof(xmlLen)),
            ba::buffer(outStrBuff.data())
    };

    outBytes = ba::write(
            dsTsocket,
            outBuffs,
            ec
    );

    if (ec)
        return false;

    boost::uint32_t recvLen;
    inBytes = ba::read(
            dsTsocket,
            ba::buffer(&recvLen, sizeof(recvLen)),
            ec
    );

    if (ec)
        return false;

    xmlBytes = ntohl(recvLen);

    ba::streambuf inStrBuf;
    ba::mutable_buffer inBuff = inStrBuf.prepare(xmlBytes);
    std::size_t bytes = ba::read(
            dsTsocket,
            inBuff,
            ec
    );

    if (ec)
        return false;

    inStrBuf.commit(bytes);
    inBytes += bytes;

    istream xmlLines(&inStrBuf);
    pugi::xml_parse_result result = xmlResponse.load (xmlLines);

    return static_cast<bool>(result);
}


pugi::xml_document &dsTest::getResponse() {
    return xmlResponse;
}


void dsTest::print_stats(std::ostream &where) {
    where << "Bytes sent: " << outBytes << " , bytes received: " << inBytes << " , XML size: " << xmlBytes << endl;
}
