//
//  main.cpp
//  Network
//
//  Created by Pranav Prakash on 1/27/15.
//  Copyright (c) 2015 Pranav Prakash. All rights reserved.
//

#include <iostream>
#include <boost/asio.hpp>

using namespace std;

class webpage
{
    typedef boost::asio::ip::tcp::iostream serveriostream;

private:
    serveriostream webstream;

public:
    string serverURL;
    string pageName;
    
    /**
     * Open a web connection to the server and attach the page/file to the webstream
     */
    webpage(string url, string page) : serverURL(url), pageName(page), webstream(url, "http")
    {
        webstream << "GET " << "http://" + url + "/" + page << " HTTP/1.0\r\n";
        webstream << "Host: " << url << "\r\n";
        webstream << "Accept: */*\r\n";
        webstream << "Connection: close\r\n\r\n";

        string http_version;
        unsigned int status_code;
        webstream >> http_version >> status_code;

        string status_message = getNext();

        if (!webstream || http_version.substr(0, 5) != "HTTP/")
        {
            cout << http_version << " " << status_code << " " << status_message << '\n';
            throw runtime_error{"Invalid response\n"};
        }

        if (status_code != 200)
        {
            cout << http_version << " " << status_code << " " << status_message << '\n';
            throw runtime_error{"Response returned with status code"};
        }

        string header;
        while (header != "\r" && !eof())
        {
            header = getNext();
        }
    }

    string getNext()
    {
        string toRet;

        if (!webstream.eof())
            getline(webstream, toRet);

        return toRet;
    }

    bool eof()
    {
        return webstream.eof();
    }

    friend ostream &operator<<(ostream &out, webpage &page)
    {
        out << page.getNext();
        return out;
    }
};

int main(int argc, const char *argv[])
{

    webpage page("pranav-prakash.github.io", "treehouse_HTML_project/index.html");
    while (!page.eof())
    {
        cout << page << '\n';
    }

    return 0;
}
