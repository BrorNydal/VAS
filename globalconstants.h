#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#include <QVector3D>
#include <string>

namespace Path {
    static const std::string Dir = "../";
    static const std::string ProjectDir = Dir + "VAS/";
    static const std::string Datasets = ProjectDir + "datasets/";

}

namespace WorldSettings {
static const QVector3D UP = QVector3D(0.f,0.f,1.f);
}

#endif // GLOBALCONSTANTS_H
