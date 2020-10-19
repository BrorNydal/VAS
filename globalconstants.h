#ifndef CONSTANTGLOBALS_H
#define CONSTANTGLOBALS_H

#include <iostream>

namespace GlobalConstans {
    inline const std::string dir                =   "../";
    inline const std::string ProjectName        =   "GSOpenGL2020";
    inline const std::string ProjectDirectory   =   dir + ProjectName + "/";
    inline const std::string ResourceDirectory  =   ProjectDirectory + "Resources/";
    inline const std::string ObjFileDirectory   =   ResourceDirectory + "ObjFiles/";
}
//GSOpenGL2020\Resources\ObjFiles\TestObject.obj

#endif // CONSTANTGLOBALS_H
