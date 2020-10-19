#include "rendersystem.h"

#include "enumclasses.h"
#include "globalconstants.h"

#include "ECS/coreengine.h"
#include "world.h"
#include "components.h"
#include "shader.h"
#include "ECS/coreengine.h"
#include "resourcemanager.h"
#include "entitymanager.h"

#include <QMatrix4x4>

RenderSystem::RenderSystem()
{

}

RenderSystem &RenderSystem::get()
{
    static RenderSystem *mInstance = new RenderSystem();
    return *mInstance;
}

void RenderSystem::renderSimulation()
{
    get().renderSimulation_impl();
}

void RenderSystem::renderEditor()
{
    get().renderEditor_impl();
}

void RenderSystem::renderSimulation_impl()
{
    /**
        For each meshindex: we try render.
        The 'first'variable holds the entity ID.
        We can find the Entity by accessing the vector of entities with the ID.
        Then check if the entity is active.
    **/

    Shader &shader = *ResourceManager::getShader(EShaderType::plain);
    EntityManager &em = *Engine::getSimulationWorld()->getEntityManager();

    glUseProgram(shader.getID()); //program

    cameraLookAt(&Engine::EditorCamera());
    shader.uniformMat4x4f("vmatrix", Engine::EditorCamera().viewMatrix);
    shader.uniformMat4x4f("pmatrix", Engine::EditorCamera().projectionMatrix);

    QMatrix4x4 mat4x4;

    for(auto map = em.getComponentIndexMap(EComponentType::mesh).begin(); map != em.getComponentIndexMap(EComponentType::mesh).end(); map++)
    {
        //If the entity is active we render, if not we do nothing
        if(em.getEntities().at((*map).first).isActive() == true)
        {
            //unsigned int key = em->getTextureMapKey((*map).first);
            //int id = ResourceManager::getMesh((*map).second)->texture->id();
            //unsigned int target = ResourceManager::getTexture(key).target;

            //glActiveTexture(GL_TEXTURE0 + id);
            //glBindTexture(GL_TEXTURE_2D, id);

            const TransformComponent &tc = static_cast<TransformComponent&>(Engine::getSimulationWorld()->getPhysicsComponent(map->first, EComponentType::transform));
            mat4x4.setToIdentity();

            //translate
            mat4x4.translate(tc.location);

            //scale
            mat4x4.scale(tc.scale);

            //rotate
            mat4x4.rotate(tc.rotation.x(), QVector3D(1, 0, 0));
            mat4x4.rotate(tc.rotation.y(), QVector3D(0, 1, 0));
            mat4x4.rotate(tc.rotation.z(), QVector3D(0, 0, 1));

            shader.uniformMat4x4f("matrix", mat4x4); //link transform to entity
            //shader->uniformInt("textureSampler", id);

            glBindVertexArray(ResourceManager::getMesh((*map).second)->vao );
            glDrawElements(GL_TRIANGLES, ResourceManager::getMesh((*map).second)->indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }



    for(auto map = em.getComponentIndexMap(EComponentType::terrain).begin(); map != em.getComponentIndexMap(EComponentType::terrain).end(); map++)
    {
        if(em.getEntities().at((*map).first).isActive() == true)
        {
            mat4x4.setToIdentity();

            shader.uniformMat4x4f("matrix", mat4x4);

            glBindVertexArray(ResourceManager::getTerrain((*map).second).vao);
            glDrawElements(GL_TRIANGLES, ResourceManager::getTerrain((*map).second).indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }
}

void RenderSystem::renderEditor_impl()
{
    Shader *shader = ResourceManager::getShader(EShaderType::plain);
    EntityManager *em = Engine::getEditorWorld()->getEntityManager();

    glUseProgram(shader->getID()); //program

    cameraLookAt(&Engine::EditorCamera());

    shader->uniformMat4x4f("vmatrix", Engine::EditorCamera().viewMatrix);
    shader->uniformMat4x4f("pmatrix", Engine::EditorCamera().projectionMatrix);



    QMatrix4x4 mat4x4;

    for(auto entity = em->getEntities().begin(); entity != em->getEntities().end(); entity++)
    {
        if(entity->getComponent(EComponentType::mesh) != nullptr &&
            entity->getComponent(EComponentType::transform) != nullptr)
        {
            MeshComponent *mc = static_cast<MeshComponent*>(entity->getComponent(EComponentType::mesh));
            TransformComponent *tc = static_cast<TransformComponent*>(entity->getComponent(EComponentType::transform));

            mat4x4.setToIdentity();

            //translate
            mat4x4.translate(tc->location);

            //scale
            mat4x4.scale(tc->scale);

            //rotate
            mat4x4.rotate(tc->rotation.x(), QVector3D(1, 0, 0));
            mat4x4.rotate(tc->rotation.y(), QVector3D(0, 1, 0));
            mat4x4.rotate(tc->rotation.z(), QVector3D(0, 0, 1));

    //        std::cout << "mc vao = " << mc->vao << std::endl;
    //        std::cout << "mc indexcount = " << mc->indexCount << std::endl;
    //        std::cout << "tc pos = " << tc->position << std::endl<<std::endl;

            //int id = static_cast<TextureComponent*>(entity->getComponent(EComponentType::texture))->ID;
            //int id = mc->texture->id();
            //
            //glActiveTexture(GL_TEXTURE0 + id);
            //glBindTexture(GL_TEXTURE_2D, id);

            shader->uniformMat4x4f("matrix", mat4x4); //link transform to entity
            //shader->uniformInt("textureSampler", id);

            glBindVertexArray(mc->vao);
            glDrawElements(GL_TRIANGLES, mc->indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
        else if(entity->getComponent(EComponentType::terrain) != nullptr)
        {
            TerrainComponent *tc = static_cast<TerrainComponent*>(entity->getComponent(EComponentType::terrain));

            mat4x4.setToIdentity();

            shader->uniformMat4x4f("matrix", mat4x4);

            glBindVertexArray(tc->vao);
            glDrawElements(GL_TRIANGLES, tc->indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

        }
    }

    //cameraUpdate(em, cameraIndex);
}

void RenderSystem::cameraLookAt(CameraComponent *cam)
{
    cam->viewMatrix.setToIdentity();

    QVector3D world = cam->worldPosition;
    QVector3D relative = cam->relativePosition.normalized() * cam->offset;

    cam->relativePosition = relative;

    QVector3D finalPosition = world + relative;

    cam->finalPosition = finalPosition;

    cam->forward = ((world + relative) - world).normalized();
    cam->right = QVector3D::crossProduct(cam->forward, WorldSettings::WorldUpVector).normalized();
    cam->up = QVector3D::crossProduct(cam->right, cam->forward).normalized();

    cam->viewMatrix.lookAt(finalPosition, QVector3D(world.x(), world.y(), world.z() + 0.0001f), QVector3D(0,0,1));
}

void RenderSystem::cameraUpdate(EntityManager *em, int cameraIndex)
{
    em->getCameraMap().at(cameraIndex).viewMatrix.setToIdentity();
}
