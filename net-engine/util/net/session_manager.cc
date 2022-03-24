#include "session_manager.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

// ע��ҵ���߼���ص�
void SessionManager::registerProc(HandleProc on_connection, HandleProc on_close)
{
  on_connection_ = on_connection;
  on_close_ = on_close;
}

// ����session���Ƴ����ڵ�session
void SessionManager::update(bool active)
{
  if (active)
  {
    std::list<Session::WPTR> swap_accepted_sessions;
    swapAcceptSessions(swap_accepted_sessions);

    activeAcceptSessions(swap_accepted_sessions);

    std::list<int> swap_close_sessions;
    swapCloseSessions(swap_close_sessions);

    detachCloseSessions(swap_close_sessions);
  }

  tickActiveSessions();
}

// tick��Ч�ĻỰ
void SessionManager::tickActiveSessions()
{
  std::list<int> need_close_sessions;

  for (auto& info : active_sessions_)
  {
    auto session = info.second.lock();
    if (session)
      session->dispatch();
    else
      need_close_sessions.push_back(info.first);
  }

  detachCloseSessions(need_close_sessions);
}

// �µ�����
void SessionManager::onAcceptSession(Session::PTR session)
{
  std::lock_guard<std::mutex> guard(accept_mutex_);
  accepted_sessions_[session->fd()] = session;
}

// ���ӹر�
void SessionManager::onCloseSession(Session::PTR session)
{
  std::lock_guard<std::mutex> guard(close_mutex_);
  close_sessions_.push_back(session->fd());
}

// ����accept�Ự
void SessionManager::swapAcceptSessions(std::list<Session::WPTR>& swap_accept_seesion)
{
  std::lock_guard<std::mutex> guard(accept_mutex_);

  for (auto& info : accepted_sessions_)
  {
    swap_accept_seesion.push_back(info.second);
  }

  accepted_sessions_.clear();
}

// Ӧ�ò㼤��Ự
void SessionManager::activeAcceptSessions(std::list<Session::WPTR>& swap_accept_seesion)
{
  for (auto& info : swap_accept_seesion)
  {
    auto session = info.lock();
    if (session)
      addSession(session);
  }
}

// ����close�Ự
void SessionManager::swapCloseSessions(std::list<int>& swap_close_seesion)
{
  std::lock_guard<std::mutex> guard(close_mutex_);
  swap_close_seesion = std::move(close_sessions_);
}

// Ӧ�ò�رջỰ
void SessionManager::detachCloseSessions(std::list<int>& swap_close_seesion)
{
  for (auto& id : swap_close_seesion)
  {
    detachSession(id);
  }
}

// ���session���������
bool SessionManager::addSession(Session::PTR session)
{
  auto ret = active_sessions_.insert(std::make_pair(session->fd(), session));
  return ret.second;
}

// �ӻ�Ự���Ƴ�session
bool SessionManager::detachSession(int id)
{
  return active_sessions_.erase(id) > 0;
}
