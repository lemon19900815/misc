#ifndef __RUNABLE_INC_H__
#define __RUNABLE_INC_H__

/*
* thread runable interface
*/
class IRunable
{
public:
  typedef std::shared_ptr<IRunable> PTR;

  virtual~IRunable() {}

  /*
  * thread work function
  */
  virtual void run(void* arg) = 0;
};

#endif // __RUNABLE_INC_H__
