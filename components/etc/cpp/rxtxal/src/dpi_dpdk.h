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
  int write_rx(void* pbuffer, u_int size) {
    cout << "dpdkDPI" << "::write_rx()" << endl;
    return 0;
  }
  int write_tx(void* pbuffer, u_int size) {
    cout << "dpdkDPI" << "::write_tx()" << endl;
    return 0;
  }
  int read_rx(void* pbuffer, u_int bufSize) {
    cout << "dpdkDPI" << "::read_rx()" << endl;
    return 0;
  }
  int read_tx(void* pbuffer, u_int bufSize) {
    cout << "dpdkDPI" << "::read_tx()" << endl;
    return 0;
  }
  MSTS flush_rx() {
    cout << "dpdkDPI" << "::flush_rx()" << endl;
    return 0;
  }
  MSTS flush_tx() {
    cout << "dpdkDPI" << "::flush_tx()" << endl;
    return 0;
  }
  MSTS close_rx() {
    cout << "dpdkDPI" << "::close_rx()" << endl;
    return 0;
  }
  MSTS close_tx() {
    cout << "dpdkDPI" << "::close_tx()" << endl;
    return 0;
  }
  u_char isWaitForPacket(){
      cout << "dpdkDPI" << "::isWaitForPacket()" << endl;
      return (m_waitForPacket);
  }
  void printPacket(int32_t tzone, void* pBuffer) {
      cout << "dpdkDPI" << "::printPacket()" << endl;
  }
  private:
      unsigned char m_waitForPacket;
};

#endif /* _DPI_DPDK_H_ */
