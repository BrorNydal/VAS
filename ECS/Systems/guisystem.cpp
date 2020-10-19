#include "guisystem.h"

//engine
#include "ECS/coreengine.h"

//manager
#include "resourcemanager.h"
#include "entitymanager.h"

#include "vertex.h"


GuiSystem::GuiSystem()
{

}

GuiSystem &GuiSystem::get()
{
    static GuiSystem *mInstance = new GuiSystem();
    return *mInstance;
}

void GuiSystem::makeGUI(GuiComponent *gui, const std::string &texture)
{
    gui->texture = ResourceManager::getTexture(texture);

    gui->mat4.setToIdentity();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(Vertex(0,   -1*gui->scale.x() + gui->position.x(), -1 *gui->scale.y() + gui->position.y(),         0,0,0,  0,0));
    vertices.push_back(Vertex(0,   1 *gui->scale.x() + gui->position.x(),  -1*gui->scale.y() + gui->position.y(),         0,0,0,  1,0));
    vertices.push_back(Vertex(0,   1 *gui->scale.x() + gui->position.x(),  1 *gui->scale.y() + gui->position.y(),          0,0,0,  1,1));
    vertices.push_back(Vertex(0,   -1*gui->scale.x() + gui->position.x(), 1  *gui->scale.y() + gui->position.y(),          0,0,0,  0,1));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    gui->size = indices.size();

    get().glGenVertexArrays(1, &gui->vao);
    get().glGenBuffers(1, &gui->eab);
    get().glGenBuffers(1, &gui->vbo);

    //what object to draw
    get().glBindVertexArray(gui->vao);

    get().glBindBuffer(GL_ARRAY_BUFFER, gui->vbo);

    //Vertex Buffer Object to hold vertices - VBO
    get().glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()*sizeof( Vertex )), vertices.data(), GL_STATIC_DRAW );

    get().glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof( Vertex ), (GLvoid*)nullptr);
    get().glEnableVertexAttribArray(0);

    get().glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)(6 * sizeof(GLfloat)));
    get().glEnableVertexAttribArray(2);

    get().glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui->eab);
    get().glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(indices.size()) * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void GuiSystem::render()
{
    get().render_impl();
}

void GuiSystem::render_impl()
{

    EntityManager *em = Engine::getEditorWorld()->getEntityManager(); //if em guientitiessize == 0 return.
    Shader *guiShader = ResourceManager::getShader(EShaderType::gui);

    glUseProgram(guiShader->getID());

    for(auto entity = em->getEntities().begin(); entity != em->getEntities().end(); entity++)
    {
        if(entity->getComponent(EComponentType::gui) != nullptr)
        {
            //use map.first to access which texture it needs...
            // . . .

            GuiComponent *gc = static_cast<GuiComponent*>(entity->getComponent(EComponentType::gui));

            assert(gc->texture != nullptr);

            int id = gc->texture->ID;

            qDebug() << "TEX ID : " << id;

            guiShader->uniformInt("guiTexture", 0);
            guiShader->uniformMat4x4f("matrix", gc->mat4);

            glActiveTexture(GL_TEXTURE0 + id);
            glBindTexture(GL_TEXTURE_2D, id);

            glBindVertexArray( gc->vao );
            glDrawElements(GL_TRIANGLES, gc->size, GL_UNSIGNED_INT, NULL);

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}
