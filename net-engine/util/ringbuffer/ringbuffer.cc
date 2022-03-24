#include "ringbuffer.h"
#include <cassert>

RingBuffer::RingBuffer(int buf_size)
  : m_buffer_size(buf_size)
  , m_circle_buffer(NULL)
  , m_write_p(0)
  , m_read_p(0)
{
  assert(is_power_of_2(buf_size)); // size must be 2^n
  m_circle_buffer = new char[m_buffer_size];
}

RingBuffer::~RingBuffer()
{
  if (m_circle_buffer)
    delete[] m_circle_buffer;

  m_circle_buffer = NULL;
}

unsigned int RingBuffer::__kfifo_put(const char* buffer, unsigned int len)
{
  if (len == 0)
    return 0;

  unsigned int iLen;
  len = _min(len, m_buffer_size - m_write_p + m_read_p);

  /*
  * Ensure that we sample the fifo->out index -before- we
  * start putting bytes into the kfifo.
  */
  smp_mb();                                 /*===>B2*/

  /* first put the data starting from fifo->in to buffer end */
  iLen = _min(len, m_buffer_size - (m_write_p & (m_buffer_size - 1)));
  memcpy(m_circle_buffer + (m_write_p & (m_buffer_size - 1)), buffer, iLen);

  /* then put the rest (if any) at the beginning of the buffer */
  memcpy(m_circle_buffer, buffer + iLen, len - iLen);

  /*
  * Ensure that we add the bytes to the kfifo -before-
  * we update the fifo->in index.
  */
  smp_wmb();                                 /*===>A1*/

  m_write_p += len;

  return len;
}

unsigned int RingBuffer::__kfifo_put(const std::string& buffer, unsigned int len)
{
  if (len == 0 || len > buffer.length())
    return 0;

  unsigned int iLen;
  len = _min(len, m_buffer_size - m_write_p + m_read_p);

  /*
  * Ensure that we sample the fifo->out index -before- we
  * start putting bytes into the kfifo.
  */
  smp_mb();                                 /*===>B2*/

                                            /* first put the data starting from fifo->in to buffer end */
  iLen = _min(len, m_buffer_size - (m_write_p & (m_buffer_size - 1)));
  memcpy(m_circle_buffer + (m_write_p & (m_buffer_size - 1)), buffer.data(), iLen);

  const char* leftbuf = buffer.data() + iLen;
  /* then put the rest (if any) at the beginning of the buffer */
  memcpy(m_circle_buffer, leftbuf, len - iLen);

  /*
  * Ensure that we add the bytes to the kfifo -before-
  * we update the fifo->in index.
  */
  smp_wmb();                                 /*===>A1*/

  m_write_p += len;

  return len;
}

unsigned int RingBuffer::__peek(char* buffer, unsigned int len)
{
  if (empty() || used() < len)
    return 0;

  unsigned int iLen;
  len = _min(len, m_write_p - m_read_p);

  /*
  * Ensure that we sample the fifo->in index -before- we
  * start removing bytes from the kfifo.
  */
  smp_rmb();                   /*===>A2*/

  /* first get the data from fifo->out until the end of the buffer */
  iLen = _min(len, m_buffer_size - (m_read_p & (m_buffer_size - 1)));
  memcpy(buffer, m_circle_buffer + (m_read_p & (m_buffer_size - 1)), iLen);

  /* then get the rest (if any) from the beginning of the buffer */
  memcpy(buffer + iLen, m_circle_buffer, len - iLen);

  /*
  * Ensure that we remove the bytes from the kfifo -before-
  * we update the fifo->out index.
  */
  smp_mb();                       /*===>B2*/

  //
  //// No move m_read ////

  return len;
}

bool RingBuffer::__seekread(unsigned int len)
{
  if (used() < len)
    return false;

  m_read_p += len;
  return true;
}

unsigned int RingBuffer::__kfifo_get(char* buffer, unsigned int len)
{
  if (empty())
    return 0;

  unsigned int iLen;
  len = _min(len, m_write_p - m_read_p);

  /*
  * Ensure that we sample the fifo->in index -before- we
  * start removing bytes from the kfifo.
  */
  smp_rmb();                   /*===>A2*/

  /* first get the data from fifo->out until the end of the buffer */
  iLen = _min(len, m_buffer_size - (m_read_p & (m_buffer_size - 1)));
  memcpy(buffer, m_circle_buffer + (m_read_p & (m_buffer_size - 1)), iLen);

  /* then get the rest (if any) from the beginning of the buffer */
  memcpy(buffer + iLen, m_circle_buffer, len - iLen);

  /*
  * Ensure that we remove the bytes from the kfifo -before-
  * we update the fifo->out index.
  */
  smp_mb();                       /*===>B2*/

  m_read_p += len;
  return len;
}

unsigned int RingBuffer::__kfifo_get(std::string& buffer, unsigned int len)
{
  if (empty())
    return 0;

  unsigned int iLen;
  len = _min(len, m_write_p - m_read_p);

  /*
  * Ensure that we sample the fifo->in index -before- we
  * start removing bytes from the kfifo.
  */
  smp_rmb();                   /*===>A2*/

                                /* first get the data from fifo->out until the end of the buffer */
  iLen = _min(len, m_buffer_size - (m_read_p & (m_buffer_size - 1)));
  //memcpy(buffer, m_circle_buffer + (m_read_p & (m_buffer_size - 1)), iLen);
  buffer.assign(m_circle_buffer + (m_read_p & (m_buffer_size - 1)), iLen);

  /* then get the rest (if any) from the beginning of the buffer */
  //memcpy(buffer + iLen, m_circle_buffer, len - iLen);
  buffer.append(m_circle_buffer, len - iLen);

  /*
  * Ensure that we remove the bytes from the kfifo -before-
  * we update the fifo->out index.
  */
  smp_mb();                       /*===>B2*/

  m_read_p += len;
  return len;
}
