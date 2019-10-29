//
// Created by sanch on 10/28/2019.
//

#ifndef INC_28_10_19_DRIVER_H
#define INC_28_10_19_DRIVER_H

#include "Token_Stream.h"
#include "Biblios.h"
void calculate()
{
    for (;;) {
        ts.get();
        if (ts.current().kind == Kind::end) break;
        if (ts.current().kind == Kind::print) continue;
        cout << expr(false) << '\n';
    }
}
#endif //INC_28_10_19_DRIVER_H
