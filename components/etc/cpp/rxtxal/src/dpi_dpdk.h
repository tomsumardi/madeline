#ifndef _DPI_DPDK_H_
#define _DPI_DPDK_H_

#include "rxtxal.h"

class dpdkDPI {
  public:
  dpdkDPI(){}
  ~dpdkDPI() {
    cout << "dpdkDPI" << "::dtor" << endl;
  }
  void filter() {
    cout << "dpdkDPI" << "::filter()" << endl;
  }
  // Default operations
  MSTS open(u_int bufSize) {
    cout << "dpdkDPI" << "::open()" << endl;
    return 0;
  }
  int write(u_int size) {
    cout << "dpdkDPI" << "::write()" << endl;
    return 0;
  }
  int read(u_int bufSize) {
    cout << "dpdkDPI" << "::read()" << endl;
    return 0;
  }
  MSTS flush() {
    cout << "dpdkDPI" << "::flush()" << endl;
    return 0;
  }
  MSTS close() {
    cout << "dpdkDPI" << "::close()" << endl;
    return 0;
  }
  u_char isWaitForPacket(){
      cout << "dpdkDPI" << "::isWaitForPacket()" << endl;
      return (m_waitForPacket);
  }
  u_char*  getBufferPtr() {
      cout << "dpdkDPI" << "::getBufferPtr()" << endl;
      return NULL;
  }
  void printPacket(int32_t tzone) {
      cout << "dpdkDPI" << "::printPacket()" << endl;
  }
  private:
      unsigned char m_waitForPacket;
};

#endif /* _DPI_DPDK_H_ */
