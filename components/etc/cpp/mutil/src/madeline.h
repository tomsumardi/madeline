#ifndef _MADELINE_H_
#define _MADELINE_H_

#include <glib.h>
#include <boost/assert.hpp>
#include <boost/format.hpp>

using namespace std;

class madeline{
    protected:
        bool        bMstandalone;
        bool        bMverbose;
};

#define MDERROR -1
#define MDSUCCESS 0
typedef int  MSTS;

#endif /* _MADELINE_H_ */
