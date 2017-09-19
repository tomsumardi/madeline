#include "rxtxal.h"

using namespace std;

/*
int main() {
  rxtxInterface* pPfring;
  pPfring = new rxtxAdapter < pfringDPI > (new pfringDPI(), &pfringDPI::exec);
  pPfring->open();
  pPfring->read();
  pPfring->write();
  pPfring->execute();
  pPfring->close();
  cout << "" << endl;

  rxtxInterface* pDpdk;
  pDpdk = new rxtxAdapter < dpdkDPI > (new dpdkDPI(), &dpdkDPI::filter);
  pDpdk->open();
  pDpdk->read();
  pDpdk->write();
  pDpdk->execute();
  pDpdk->close();
  cout << "" << endl;

  vector<rxtxInterface*> rxtxLst;
  rxtxLst.push_back(new rxtxAdapter < pfringDPI > (new pfringDPI(), &pfringDPI::exec));
  rxtxLst.push_back(new rxtxAdapter < dpdkDPI > (new dpdkDPI(), NULL));
  for (std::vector<rxtxInterface*>::iterator it = rxtxLst.begin(); it != rxtxLst.end(); ++it) {
      (*it)->open();
      (*it)->read();
      (*it)->write();
      (*it)->execute();
      (*it)->close();
  }
}

*/
