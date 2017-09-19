#ifndef _RXTXAL_H_
#define _RXTXAL_H_

#include <iostream>
#include <vector>
#include "mutil/src/madeline.h"
#include "mutil/src/mutil.h"

// abstract adapter class
class rxtxInterface {
  public:
    // Default operations
    virtual void execute() = 0;
    virtual MSTS open() = 0;
    virtual MSTS write() = 0;
    virtual MSTS read() = 0;
    virtual MSTS close() = 0;
    virtual MSTS init() = 0;
    // destructor
    virtual ~rxtxInterface(){}
};

// RXTX adapter class
template <class TYPE>
class rxtxAdapter: public rxtxInterface {
  public:
    rxtxAdapter(TYPE *o, void(TYPE:: *m)()) {
      object = o;
      method = m;
    }
    ~rxtxAdapter() {
      delete object;
    }
    // Default operations
    void execute() {
      if (method){
          (object->*method)();
      }
    }
    MSTS init() {
      return object->init();
    }
    MSTS open() {
      return object->open();
    }
    MSTS write() {
      return object->write();
    }
    MSTS read() {
      return object->read();
    }
    MSTS close() {
      return object->close();
    }
  private:
    TYPE *object;
    void(TYPE:: *method)();
};

#include "dpi_pfring.h"
#include "dpi_dpdk.h"

#endif /* _RXTXAL_H_ */
