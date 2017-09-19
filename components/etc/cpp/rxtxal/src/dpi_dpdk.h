#ifndef _DPI_PFRING_H_
#define _DPI_PFRING_H_

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
  MSTS open() {
    cout << "dpdkDPI" << "::open()" << endl;
    return 0;
  }
  MSTS write() {
    cout << "dpdkDPI" << "::write()" << endl;
    return 0;
  }
  MSTS read() {
    cout << "dpdkDPI" << "::read()" << endl;
    return 0;
  }
  MSTS close() {
    cout << "dpdkDPI" << "::close()" << endl;
    return 0;
  }
  MSTS    init();
};

#endif /* _DPI_PFRING_H_ */
