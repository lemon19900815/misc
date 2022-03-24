#ifndef __SERVER_APPLICATION_INC_H__
#define __SERVER_APPLICATION_INC_H__

#include <stdio.h>
#include <stdlib.h>

class ServerApp
{
public:
  virtual ~ServerApp() { }

  virtual bool startup(int argc, char* argv[]) = 0;
  virtual void run() = 0;
  virtual void stop() = 0;
};

#define RunApplication(ServerApplication) \
int main(int argc, char* argv[]) {\
  ServerApplication app; \
  if(app.startup(argc, argv)) { \
    app.run(); \
  } else { \
    printf("Server Application Startup Failed.\n");\
    exit(1); \
  }\
  app.stop(); \
}

#endif // __SERVER_APPLICATION_INC_H__
