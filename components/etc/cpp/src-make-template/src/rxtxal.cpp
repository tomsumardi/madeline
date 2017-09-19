#include <iostream>
#include <vector>
#include "rxtxal.h"

using namespace std;

class rxtxInterface {
  public:
    virtual void execute() = 0;
    // Default operations
    virtual int open() = 0;
    virtual int write(int x) = 0;
    virtual int read() = 0;
    virtual int close() = 0;
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
    void execute() {
      if (method){
          (object->*method)();
      }
    }
    // Default operations
    int open() {
      return object->open();
    }
    int write(int x) {
      return object->write(x);
    }
    int read() {
      return object->read();
    }
    int close() {
      return object->close();
    }
  private:
    TYPE *object;
    void(TYPE:: *method)();
};

class pfringDPI {
  public:
  pfringDPI(){}
  pfringDPI(string name){
      _name = name;
  }
  ~pfringDPI() {
    cout << _name << "::dtor" << endl;
  }
  void exec() {
    cout << _name << "::exec()" << endl;
  }
  // Default operations
  int open() {
    cout << _name << "::open()" << endl;
    return 0;
  }
  int write(int x) {
    cout << _name << "::write()" << endl;
    return 0;
  }
  int read() {
    cout << _name << "::read()" << endl;
    return 0;
  }
  int close() {
    cout << _name << "::close()" << endl;
    return 0;
  }
  private:
      string _name;
};

class dpdkDPI {
  public:
  dpdkDPI(){}
  dpdkDPI(string name){
      _name = name;
  }
  ~dpdkDPI() {
    cout << _name << "::dtor" << endl;
  }
  void filter() {
    cout << _name << "::filter()" << endl;
  }
  // Default operations
  int open() {
    cout << _name << "::open()" << endl;
    return 0;
  }
  int write(int x) {
    cout << _name << "::write()" << endl;
    return 0;
  }
  int read() {
    cout << _name << "::read()" << endl;
    return 0;
  }
  int close() {
    cout << _name << "::close()" << endl;
    return 0;
  }
  private:
      string _name;
};

int main() {
  rxtxInterface* pPfring;
  pPfring = new rxtxAdapter < pfringDPI > (new pfringDPI("pfring"), &pfringDPI::exec);
  pPfring->open();
  pPfring->read();
  pPfring->write(1);
  pPfring->execute();
  pPfring->close();
  cout << "" << endl;

  rxtxInterface* pDpdk;
  pDpdk = new rxtxAdapter < dpdkDPI > (new dpdkDPI("dpdk"), &dpdkDPI::filter);
  pDpdk->open();
  pDpdk->read();
  pDpdk->write(2);
  pDpdk->execute();
  pDpdk->close();
  cout << "" << endl;

  vector<rxtxInterface*> rxtxLst;
  rxtxLst.push_back(new rxtxAdapter < pfringDPI > (new pfringDPI("pfring"), &pfringDPI::exec));
  rxtxLst.push_back(new rxtxAdapter < dpdkDPI > (new dpdkDPI("dpdk"), NULL));
  for (std::vector<rxtxInterface*>::iterator it = rxtxLst.begin(); it != rxtxLst.end(); ++it) {
      (*it)->open();
      (*it)->read();
      (*it)->write(2);
      (*it)->execute();
      (*it)->close();
  }
}

