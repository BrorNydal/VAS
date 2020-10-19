#include "resourcemanager.h"

#include "ECS/coreengine.h"
#include "components.h"
#include "mainwindow.h"

//import
#include "obj.h"
#include "texture.h"
#include "terrain.h"

//qt
#include <QDirIterator>

//needed for folder paths
#include "globalconstants.h"

#include <fstream>



ResourceManager::ResourceManager()
    :
      mPlainShader((FolderPath::ShaderDirectory + "plainvertex.vert").c_str(),
                   (FolderPath::ShaderDirectory + "plainfragment.frag").c_str(),
                   EShaderType::plain),
      mTextureShader((FolderPath::ShaderDirectory + "texturevertex.vert").c_str(),
                     (FolderPath::ShaderDirectory + "texturefragment.frag").c_str(),
                     EShaderType::texture),
      mPhongShader((FolderPath::ShaderDirectory + "phongvertex.vert").c_str(),
                   (FolderPath::ShaderDirectory + "phongfragment.frag").c_str(),
                   EShaderType::phong),
      mGuiShader((FolderPath::ShaderDirectory + "guivertex.vert").c_str(),
                   (FolderPath::ShaderDirectory + "guifragment.frag").c_str(),
                    EShaderType::gui)
{    
    initializeOpenGLFunctions();
}

void ResourceManager::initialize()
{
    qDebug() << "(resourcemanager) reading files.";
    get().readMeshes();
    get().readTextures();
    get().getComponentIndex_impl(EComponentType::terrain, "perlinnoise.png");
    qDebug() << "(resourcemanager) all files read.";
}

ResourceManager &ResourceManager::get()
{
    static ResourceManager *mInstance = new ResourceManager();
    return *mInstance;
}

Shader *ResourceManager::getShader(EShaderType shaderType)
{
    switch(shaderType)
    {
        case EShaderType::plain:
            return &get().mPlainShader;

        case EShaderType::texture:
            return &get().mTextureShader;

        case EShaderType::phong:
            return &get().mPhongShader;

        case EShaderType::gui:
            return &get().mGuiShader;

        default:
            return &get().mPlainShader;
    }
}

Component *ResourceManager::getComponent(EComponentType type, int index)
{
    if(index < 0)
    {
        qDebug() << "Index " << index << " is an invalid component index";
        return nullptr;
    }

    switch(type)
    {
    case EComponentType::mesh:

        if((unsigned long long)index > get().mMeshComponents.size())
        {
            qDebug() << "Index " << index << " is an invalid meshcomponent index";
            return nullptr;
        }

        return &get().mMeshComponents.at(index);
        break;

    case EComponentType::texture:

        if((unsigned long long)index > get().mTextureComponents.size())
        {
            qDebug() << "Index " << index << " is an invalid texturecomponent index";
            return nullptr;
        }

        return &get().mTextureComponents.at(index);
        break;

    case EComponentType::terrain:

        if((unsigned long long)index > get().mTerrainComponents.size())
        {
            qDebug() << "Index " << index << " is an invalid terraincomponent index";
            return nullptr;
        }

        return &get().mTerrainComponents.at(index);
        break;

    default:
        qDebug() << "Type specified is invalid.";
        return nullptr;
    }
}

Component *ResourceManager::getComponent(EComponentType type, const std::string &name)
{
    int index = get().getComponentIndex_impl(type, name);

    if(index < 0)
    {
        qDebug() << "Component name " << name.c_str() << " is an invalid component name.";
        return nullptr;
    }

    switch(type)
    {
    case EComponentType::mesh:
        return &get().mMeshComponents.at(index);
        break;

    case EComponentType::texture:
        return &get().mTextureComponents.at(index);
        break;

    case EComponentType::terrain:
        return &get().mTerrainComponents.at(index);
        break;

    default:
        qDebug() << "Type specified is invalid.";
        return nullptr;
    }
}

int ResourceManager::getComponentIndex(EComponentType type, std::string name)
{
    return get().getComponentIndex_impl(type, name);
}

MeshComponent *ResourceManager::getMesh(int index)
{
    return &get().mMeshComponents.at(index);
}

MeshComponent *ResourceManager::getMesh(const std::string &mesh)
{
    int index = get().getMeshComponentIndex_impl(mesh);
    return &get().mMeshComponents.at(index);
}

const std::vector<MeshComponent> &ResourceManager::getMeshComponents()
{
    return get().mMeshComponents;
}

int ResourceManager::getMeshComponentIndex(std::string mesh)
{
    return get().getMeshComponentIndex_impl(mesh);
}

const TextureComponent &ResourceManager::getTexture(int index)
{
    return get().mTextureComponents.at(index);
}

TextureComponent *ResourceManager::getTexture(const std::string texture)
{
    int index = get().getTextureComponentIndex_impl(texture);
    return &get().mTextureComponents.at(index);
}

const std::vector<TextureComponent> &ResourceManager::getTextureComponents()
{
    return get().mTextureComponents;
}

int ResourceManager::getTextureComponentIndex(std::string texture)
{
    return get().getTextureComponentIndex_impl(texture);
}

const TerrainComponent &ResourceManager::getTerrain(int index)
{
    return get().mTerrainComponents.at(index);
}

TerrainComponent *ResourceManager::getTerrain(const std::string terrain)
{
    int index = get().getComponentIndex_impl(EComponentType::terrain, terrain);
    return &get().mTerrainComponents.at(index);
}

GuiComponent &ResourceManager::getGui(int index)
{
    return get().mGuiComponents.at(index);
}

const std::vector<GuiComponent> &ResourceManager::getGuiComponents()
{
    return get().mGuiComponents;
}

int ResourceManager::getGuiComponentIndex(std::string texture)
{
    return get().getGuiComponentIndex_impl(texture);
}

int ResourceManager::getComponentIndex_impl(EComponentType type, std::string name)
{
    switch(type)
    {
    case EComponentType::mesh:
        if(mMeshComponentKey.find(name) != mMeshComponentKey.end())
        {
            return mMeshComponentKey.at(name);
        }
        else
        {
            MeshComponent mc = newMeshComponent(name);

            if(mc == MeshComponent())
                return -1;

            int index = mMeshComponents.size();
            mMeshComponentKey[name] = index;

            mMeshComponents.push_back(mc);

            return index;
        }
        break;

    case EComponentType::texture:
        if(mTextureComponentKey.find(name) != mTextureComponentKey.end())
        {
            return mTextureComponentKey.at(name);
        }
        else
        {
            int index = mTextureComponents.size();
            mTextureComponentKey[name] = index;
            mTextureComponents.push_back(newTextureComponent(name));

            return index;
        }
        break;

    case EComponentType::terrain:
        if(mTerrainCompoentKey.find(name) != mTerrainCompoentKey.end())
        {
            return mTerrainCompoentKey.at(name);
        }
        else
        {
            int index = mTerrainComponents.size();
            mTerrainCompoentKey[name] = index;
            mTerrainComponents.push_back(newTerrainComponent(ETerrainType::map, QVector2D(500.f, 500.f), 10, name, 50.f));

            return index;
        }
        break;

    default:
        return -1;
    }
}

int ResourceManager::getMeshComponentIndex_impl(std::string mesh)
{
    if(mMeshComponentKey.find(mesh) != mMeshComponentKey.end())
    {
        return mMeshComponentKey.at(mesh);
    }
    else
    {
        MeshComponent mc = newMeshComponent(mesh);

        if(mc == MeshComponent())
            return -1;

        int index = mMeshComponents.size();
        mMeshComponentKey[mesh] = index;

        mMeshComponents.push_back(mc);

        return index;
    }
}

int ResourceManager::getTextureComponentIndex_impl(std::string texture)
{
    if(mTextureComponentKey.find(texture) != mTextureComponentKey.end())
    {
        return mTextureComponentKey.at(texture);
    }
    else
    {
        int index = mTextureComponents.size();
        mTextureComponentKey[texture] = index;
        mTextureComponents.push_back(newTextureComponent(texture));

        return index;
    }
}

int ResourceManager::getGuiComponentIndex_impl(std::string texture)
{
    if(mGuiComponentKey.find(texture) != mGuiComponentKey.end())
    {
        std::cout << "old gui : " + texture << std::endl;
        return mGuiComponentKey.at(texture);
    }
    else
    {
        int index = mGuiComponents.size();
        mGuiComponentKey[texture] = index;
        mGuiComponents.push_back(newGuiComponent(texture));
//        std::cout << "New gui : " + texture << std::endl;
//        std::cout << "position : " << mGuiComponents[index].position << std::endl;
//        std::cout << "scale : " << mGuiComponents[index].scale << std::endl;

        return index;
    }
}

MeshComponent ResourceManager::newMeshComponent(std::string mesh)
{
    std::cout << "New mesh : " + mesh << std::endl;

    MeshComponent mc(mesh);
    OBJ obj;
    bool success = obj.readOBJ(mesh);

    if(success == false)
        return MeshComponent();

    mc.indexCount = obj.indexCount();

    glGenVertexArrays(1, &mc.vao);
    glGenBuffers(1, &mc.eab);
    glGenBuffers(1, &mc.vbo);

    //what object to draw
    glBindVertexArray(mc.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mc.vbo);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(obj.vertexCount()*sizeof( Vertex )), obj.getVertexData(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof( Vertex ), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mc.eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(obj.indexCount()) * sizeof(GLuint), obj.getIndexData(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return mc;
}

void ResourceManager::readMeshes()
{
    //Creates an iterator through given directory
    QDirIterator it(FolderPath::ObjFileDirectory.c_str(), QDirIterator::Subdirectories);

    //iterate through directory of meshes and create the meshes
    while (it.hasNext()) {
        if(it.fileName().contains(".obj"))
        {
            qDebug() << "(read mesh) Opening file : " << it.fileName();
            getComponentIndex_impl(EComponentType::mesh, it.fileName().toStdString());
        }

        it.next();
    }
}

TextureComponent ResourceManager::newTextureComponent(std::string texture)
{
    TextureComponent tc;
    Texture tex(texture, GL_TEXTURE_2D);
    tex.readFile();

    tc.ID = tex.id();
    tc.data = tex.getData();
    tc.target = tex.getTarget();
    tc.width = tex.getWidth();
    tc.height = tex.getHeight();
    tc.channelCount = tex.getChannelCount();

    return tc;
}

void ResourceManager::readTextures()
{
    //Creates an iterator through given directory
    QDirIterator it(FolderPath::TextureFileDirectory.c_str(), QDirIterator::Subdirectories);

    //iterate through directory of meshes and create the meshes
    while (it.hasNext()) {
        if(it.fileName().contains(".bmp") || it.fileName().contains(".png") || it.fileName().contains(".jpg"))
        {
            qDebug() << "(read textures) Opening file : " << it.fileName();
            getComponentIndex_impl(EComponentType::texture, it.fileName().toStdString());
        }

        it.next();
    }
}

TerrainComponent ResourceManager::newTerrainComponent(ETerrainType type, QVector2D size, unsigned int accuracy, const std::string &mapFile, float rougness)
{
    TerrainComponent tc;
    Terrain terrain(type, size, accuracy, mapFile, rougness);

    tc.mCol = terrain.colCount();
    tc.mRow = terrain.rowCount();
    tc.tag = mapFile;

    tc.indexCount = terrain.indexCount();
    tc.vertexCount = terrain.vertexCount();

    glGenVertexArrays(1, &tc.vao);
    glGenBuffers(1, &tc.eab);
    glGenBuffers(1, &tc.vbo);

    //what object to draw
    glBindVertexArray(tc.vao);

    glBindBuffer(GL_ARRAY_BUFFER, tc.vbo);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(terrain.vertexCount()*sizeof( Vertex )), terrain.getVertexData(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof( Vertex ), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tc.eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(terrain.indexCount()) * sizeof(GLuint), terrain.getIndexData(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return tc;
}

GuiComponent ResourceManager::newGuiComponent(std::string texture)
{
    GuiComponent gc(QVector2D(-0.9,-0.9), QVector2D(0.1f,0.1f));
    gc.texture = getTexture(texture);

    gc.mat4.setToIdentity();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(Vertex(0,   -1*gc.scale.x() + gc.position.x(), -1 *gc.scale.y() + gc.position.y(),         0,0,0,  0,0));
    vertices.push_back(Vertex(0,   1 *gc.scale.x() + gc.position.x(),  -1*gc.scale.y() + gc.position.y(),         0,0,0,  1,0));
    vertices.push_back(Vertex(0,   1 *gc.scale.x() + gc.position.x(),  1 *gc.scale.y() + gc.position.y(),          0,0,0,  1,1));
    vertices.push_back(Vertex(0,   -1*gc.scale.x() + gc.position.x(), 1  *gc.scale.y() + gc.position.y(),          0,0,0,  0,1));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    gc.size = indices.size();

    glGenVertexArrays(1, &gc.vao);
    glGenBuffers(1, &gc.eab);
    glGenBuffers(1, &gc.vbo);

    //what object to draw
    glBindVertexArray(gc.vao);

    glBindBuffer(GL_ARRAY_BUFFER, gc.vbo);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()*sizeof( Vertex )), vertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof( Vertex ), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gc.eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(indices.size()) * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    return gc;
}

void ResourceManager::readGuis()
{
//    //Creates an iterator through given directory
//    QDirIterator it(FolderPath::ObjFileDirectory.c_str(), QDirIterator::Subdirectories);

//    //iterate through directory of meshes and create the meshes
//    while (it.hasNext()) {
//        if(it.fileName().contains(".bmp"))
//        {
//            qDebug() << "Opening file : " << it.fileName();
//            getMeshComponentIndex_impl(it.fileName().toStdString());
//        }

//        it.next();
//    }
}
