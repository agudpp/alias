#ifndef DEBUGUTIL_H
#define DEBUGUTIL_H


#ifdef DEBUG
#include <assert.h>
#include <QDebug>


#define ASSERT(x) assert(x);
#define debugERROR(x)  qDebug() << x

#else
#define ASSERT(x)
#define debugERROR(x)

#endif

#endif // DEBUGUTIL_H
