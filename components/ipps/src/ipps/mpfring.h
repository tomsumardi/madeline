#ifndef _MPFRING_H_
#define _MPFRING_H_

class mpfring {

  public:
    mpfring(Document *pInDoc) //, pd(NULL)
    {
        //Do deep copy here.
        ippsDoc.CopyFrom(*pInDoc, ippsDoc.GetAllocator());
    }
    MSTS    init()
    {
        return MDSUCCESS;
    }
    //init()
   // log()

  private:
    Document         ippsDoc;
    //pfring*         pd;
};

#endif /* _MPFRING_H_ */
