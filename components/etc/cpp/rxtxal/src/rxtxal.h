#ifndef _RXTXAL_H_
#define _RXTXAL_H_

#include <iostream>
#include <vector>
#include "mutil/src/madeline.h"
#include "mutil/src/mutil.h"

typedef struct
{
    u_int caplen;
    u_int eth_type;
} rxtxal_pkthdr;

// abstract adapter class
class rxtxInterface {
  public:
    // Specific operation, function pointer
    virtual rxtxal_pkthdr* getPktHeader() = 0;
    // Default operations
    virtual MSTS    open(u_int bufSize) = 0;
    virtual int     write_rx(void* pBuffer, u_int size) = 0;
    virtual int     write_tx(void* pBuffer, u_int size) = 0;
    virtual int     read_rx(void* pBuffer, u_int bufSize) = 0;
    virtual int     read_tx(void* pBuffer, u_int bufSize) = 0;
    virtual MSTS    flush_rx() = 0;
    virtual MSTS    flush_tx() = 0;
    virtual MSTS    close_rx() = 0;
    virtual MSTS    close_tx() = 0;
    virtual u_char  isWaitForPacket() = 0;
    virtual void    printPacket(int32_t tzone, void* pBuffer) = 0;
    // destructor
    virtual ~rxtxInterface(){}
};

// RXTX adapter class
template <class TYPE>
class rxtxAdapter: public rxtxInterface {
  public:
    rxtxAdapter(TYPE *object, rxtxal_pkthdr*(TYPE:: *method)()) {
      m_object = object;
      m_method = method;
    }
    ~rxtxAdapter() {
      delete m_object;
    }
    // Default operations
    rxtxal_pkthdr* getPktHeader() {
      rxtxal_pkthdr* _pHdr = NULL;
      if (m_method){
          _pHdr = ((m_object->*m_method)());
      }
      return _pHdr;
    }
    MSTS open(u_int bufSize) {
      return m_object->open(bufSize);
    }
    int write_rx(void* pBuffer, u_int size) {
      return m_object->write_rx(pBuffer, size);
    }
    int write_tx(void* pBuffer, u_int size) {
      return m_object->write_tx(pBuffer, size);
    }
    int read_rx(void* pBuffer, u_int bufSize) {
      return m_object->read_rx(pBuffer, bufSize);
    }
    int read_tx(void* pBuffer, u_int bufSize) {
      return m_object->read_tx(pBuffer, bufSize);
    }
    MSTS flush_rx() {
      return m_object->flush_rx();
    }
    MSTS flush_tx() {
      return m_object->flush_tx();
    }
    MSTS close_rx() {
      return m_object->close_rx();
    }
    MSTS close_tx() {
      return m_object->close_tx();
    }
    u_char isWaitForPacket() {
      return m_object->isWaitForPacket();
    }
    void printPacket(int32_t tzone, void* pBuffer){
      m_object->printPacket(tzone, pBuffer);
    }

  private:
    TYPE *m_object;
    rxtxal_pkthdr*(TYPE:: *m_method)();
};

#include "dpi_pfring.h"
#include "dpi_dpdk.h"

#endif /* _RXTXAL_H_ */
