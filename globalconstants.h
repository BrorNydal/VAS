#ifndef CONSTANTGLOBALS_H
#define CONSTANTGLOBALS_H

#include <iostream>
#include "vec3.h"

namespace  {
    enum class ShaderType{
        PLAIN_SHADER, TEXTURE_SHADER, PHONG_SHADER
    };
}

namespace FolderPath {
    inline const std::string dir                =   "../";
    inline const std::string ProjectName        =   "GEA2020";
    inline const std::string ProjectDirectory   =   dir + ProjectName + "/";
    inline const std::string ShaderDirectory    =   ProjectDirectory + "shaders/";
    inline const std::string ResourceDirectory  =   ProjectDirectory + "resources/";
    inline const std::string ObjFileDirectory   =   ResourceDirectory + "objfiles/";
    inline const std::string HeightmapDirectory =   ResourceDirectory + "heightmaps/";
    inline const std::string TextureFileDirectory = ResourceDirectory + "textures/";
    inline const std::string WaveFileDirectory  =   ResourceDirectory + "wavefiles/";

    inline const bool PrintInfo = true;
    inline const bool PrintTimerInfo = false;
}

namespace WorldSettings {
    inline const QVector3D WorldUpVector = QVector3D(0,0,1);
}
namespace MathConstants {
    inline const long double PIld = 3.141592653589793238L;
    inline const double PId = 3.141592653589793;
    inline const float PIf = 3.1415927;

}


//GSOpenGL2020\Resources\ObjFiles\TestObject.obj

#endif // CONSTANTGLOBALS_H
