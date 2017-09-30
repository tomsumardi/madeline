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
    virtual int     write(u_int size) = 0;
    virtual int     read(u_int bufSize) = 0;
    virtual MSTS    flush() = 0;
    virtual MSTS    close() = 0;
    virtual u_char  isWaitForPacket() = 0;
    virtual u_char* getBufferPtr() = 0;
    virtual void    printPacket(int32_t tzone) = 0;
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
    int write(u_int size) {
      return m_object->write(size);
    }
    int read(u_int bufSize) {
      return m_object->read(bufSize);
    }
    MSTS flush() {
      return m_object->flush();
    }
    MSTS close() {
      return m_object->close();
    }
    u_char isWaitForPacket() {
      return m_object->isWaitForPacket();
    }
    u_char* getBufferPtr() {
      return m_object->getBufferPtr();
    }
    void printPacket(int32_t tzone){
      m_object->printPacket(tzone);
    }

  private:
    TYPE *m_object;
    rxtxal_pkthdr*(TYPE:: *m_method)();
};

#include "dpi_pfring.h"
#include "dpi_dpdk.h"

#endif /* _RXTXAL_H_ */
