//
// Created by Tom Butcher on 01/03/2021.
//

#ifndef HORIZON_BOG_H
#define HORIZON_BOG_H

#include <QFile>

class Bog : public QFile {
public:
    explicit Bog(QString fileName);
};


#endif //HORIZON_BOG_H
