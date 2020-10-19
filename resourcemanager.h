#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>

#include "enumclasses.h"
#include "components.h"
#include "shader.h"

#include <map>
#include <vector>


/**
    Holds onto all resources.
    May seperate this into three classes:
    MeshManager, ShaderManager, AudioManager.
 **/
class ResourceManager : public QOpenGLFunctions_4_1_Core
{
private://variables

    //shader
    static const unsigned int TotalShaders = 3;
    Shader mPlainShader;
    Shader mTextureShader;
    Shader mPhongShader;
    Shader mGuiShader;

    //mesh
    std::map<std::string, int> mMeshComponentKey;
    std::vector<MeshComponent> mMeshComponents; //array?

    //texture
    std::map<std::string, int> mTextureComponentKey;
    std::vector<TextureComponent> mTextureComponents;

    std::map<std::string, int> mTerrainCompoentKey;
    std::vector<TerrainComponent> mTerrainComponents;

    //gui - will be not be kept in resourcemanager
    std::map<std::string, int> mGuiComponentKey;
    std::vector<GuiComponent> mGuiComponents;

    //audio


public:
    ResourceManager();
    static void initialize();

    static ResourceManager &get();

    /**
     * Gets shader of given shader type.
     * @param shaderType is an enum that describes which shader to return
     * @return returns shader of given shader type
     */
    static Shader* getShader(EShaderType shaderType);

    static Component *getComponent(EComponentType type, int index);
    static Component *getComponent(EComponentType type, const std::string &name);
    static int getComponentIndex(EComponentType type, std::string name);



    //Mesh

    /**
     * Gets the mesh at given index to mesh vector.
     * @param index is the position of the mesh in the mesh vector
     * @return returns the mesh component at given index
     */
    static MeshComponent *getMesh(int index);
    /**
     * Gets the mesh of given name.
     * @param mesh is the name.
     * @return a pointer to the mesh in its vector.
     */
    static MeshComponent *getMesh(const std::string &mesh);

    /**
     * Gets vector of meshes.
     * @return returns a vector of meshes imported
     */
    static const std::vector<MeshComponent> &getMeshComponents();
    /**
     * Gets the index of meshcomponent.
     * @param mesh is the tag of the mesh component
     * @return returns index to vector of mesh components in resourcemanager
     */
    static int getMeshComponentIndex(std::string mesh);



    //texture

    /**
     * Gets the texture at given index.
     * @param index is the position of the texture in the texture vector
     * @return returns the texture component at given index
     */
    static const TextureComponent &getTexture(int index);

    static TextureComponent *getTexture(const std::string texture);

    /**
     * Gets vector of textures.
     * @return returns a vector of textures imported
     */
    static const std::vector<TextureComponent> &getTextureComponents();
    /**
     * Gets the index of texture component.
     * @param texture is the tag of the texture component
     * @return returns index to vector of texture components in resourcemanager
     */
    static int getTextureComponentIndex(std::string texture);


    //terrain
    /**
     * Gets the texture at given index.
     * @param index is the position of the texture in the texture vector
     * @return returns the texture component at given index
     */
    static const TerrainComponent &getTerrain(int index);

    static TerrainComponent *getTerrain(const std::string terrain);

    /**
     * Gets vector of textures.
     * @return returns a vector of textures imported
     */
    static const std::vector<TerrainComponent> &getTerrainComponents();
    /**
     * Gets the index of texture component.
     * @param texture is the tag of the texture component
     * @return returns index to vector of texture components in resourcemanager
     */
    static int getTerrainComponentIndex(std::string texture);


    //gui
    static GuiComponent &getGui(int index);
    static const std::vector<GuiComponent> &getGuiComponents();
    static int getGuiComponentIndex(std::string texture);

    //audio


private://functions

    //Implementations

    int getComponentIndex_impl(EComponentType type, std::string name);

    //Mesh
    int getMeshComponentIndex_impl(std::string mesh);

    //texture
    int getTextureComponentIndex_impl(std::string texture);

    //gui
    int getGuiComponentIndex_impl(std::string texture);

    //other functions

    //mesh

    /**
     * Creates a new mesh component.
     * Utilizes the obj import class to create a new
     * mesh component based on an obj file
     * @param mesh is the name of the obj file
     * @return returns a mesh component.
     */
    MeshComponent newMeshComponent(std::string mesh);

    void readMeshes();

    //texture

    /**
     * Creates a new texture component.
     * Utilizes the texture import class to create a new
     * texture component based on a bmp file.
     * May also work with png or jpg, but may fail
     * @param texture is the name of the texture file
     * @return returns a texture component
     */
    TextureComponent newTextureComponent(std::string texture);

    void readTextures();

    TerrainComponent newTerrainComponent(ETerrainType type, QVector2D size, unsigned int accuracy = 0, const std::string &mapFile = "", float rougness = 1.f);

    //gui
    GuiComponent newGuiComponent(std::string texture);
    void readGuis();
};

#endif // RESOURCEMANAGER_H
