#ifndef __GAME_SERVER_INC_H__
#define __GAME_SERVER_INC_H__

#include "serverapp.h"

#include <memory>

class NetCore;

class GameServer : public ServerApp
{
public:
  GameServer();
  ~GameServer() override;

  bool startup(int argc, char* argv[]) override;
  void run() override;
  void stop() override;

private:
  void schema();

private:
  bool stoped_;
  std::shared_ptr<NetCore> netcore_;
};

#endif // __GAME_SERVER_INC_H__
