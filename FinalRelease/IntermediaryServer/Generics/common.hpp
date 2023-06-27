#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>
#include <string>

#include "Socket/Socket.hpp"


enum serverAction {
  requestingFigures,
  requestingParts,
  requestingAssembly
};

struct Request {
  std::shared_ptr<Socket> socket;
  std::string figure;
  serverAction requestType;

  Request(Socket* socket, std::string figure, serverAction requestType)
      : socket()
      , figure(figure)
      , requestType(requestType) {
        this->socket.reset(socket);
      }
};

struct Response {
  std::shared_ptr<Socket> socket;
  std::string response;
  serverAction requestType;

  Response(Socket* socket, std::string response, serverAction requestType)
      : socket()
      , response(response)
      , requestType(requestType) {
    this->socket.reset(socket);
  }

  Response(std::shared_ptr<Socket> socket, std::string response, serverAction requestType)
      : socket(socket)
      , response(response)
      , requestType(requestType) {
  }
};

#endif