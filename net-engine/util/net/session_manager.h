#ifndef __SESSION_MANAGER_INC_H__
#define __SESSION_MANAGER_INC_H__

#include "singleton.h"
#include "net/session.h"

#include <list>
#include <mutex>
#include <unordered_map>

typedef std::function<void(int32_t)> HandleProc;

class SessionManager
{
public:
  SessionManager();
  ~SessionManager();

  // ע��ҵ���߼���ص�
  void registerProc(HandleProc on_connection, HandleProc on_close);

  // bool active����session���Ƴ����ڵ�session
  void update(bool active);

  // �µ�����
  void onAcceptSession(Session::PTR session);
  // ���ӹر�
  void onCloseSession(Session::PTR session);

private:
  // ����accept�Ự
  void swapAcceptSessions(std::list<Session::WPTR>& swap_accept_seesion);
  // Ӧ�ò㼤��Ự
  void activeAcceptSessions(std::list<Session::WPTR>& swap_accept_seesion);

  // ����close�Ự
  void swapCloseSessions(std::list<int>& swap_close_seesion);
  // Ӧ�ò�رջỰ
  void detachCloseSessions(std::list<int>& swap_close_seesion);

  // tick��Ч�ĻỰ
  void tickActiveSessions();

  // ���session���������
  bool addSession(Session::PTR session);
  // �ӻ�Ự���Ƴ�session
  bool detachSession(int id);

private:
  std::unordered_map<int, Session::WPTR> active_sessions_;

  std::mutex accept_mutex_;
  std::unordered_map<int, Session::WPTR> accepted_sessions_;

  std::mutex close_mutex_;
  std::list<int> close_sessions_;

  // ҵ���߼���ص��ӿ�
  HandleProc on_connection_;
  HandleProc on_close_;
};

typedef util::singleton<SessionManager> gSessionManager;
#define gSessionManagerPtr gSessionManager::instance()

#endif // __SESSION_MANAGER_INC_H__
