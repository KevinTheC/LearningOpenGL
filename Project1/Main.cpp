#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "OpenGLIncludes.h"
#include "Camera.h"
#include "InputController.h"
#include "CubeFactory.h"
#define red 1.0f, 0.0f, 0.0f,
#define green 0.0f, 1.0f, 0.0f,
#define blue 0.0f, 0.0f, 1.0f,
#define gray .5f, .5f, .5f,
#define white 1.0f, 1.0f, 1.0f,

int height = 900;
int width = 800;

using namespace glm;
int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        return -1;
    }
    glViewport(0, 0, width, height);
    Camera::instance()->updateProjection(width, height);
    

    //vertices for triangle
    //Shader shaderprogram("default.vert","default.frag");
    std::shared_ptr<Shader> sh(new Shader("color.vert","color.frag"));
    sh->activate();
    Object o = Object(sh, std::shared_ptr<std::vector<GLfloat>>(new std::vector<GLfloat>({
        -.2f, 0.4f, -.2f,        gray
        -.2f,  0.4f, .2f,      gray
        .2f,  0.4f, .2f,       gray
        .2f, 0.4f, -.2f, 		gray
        0.0f, 0.8f, 0.0f,		gray
        })), 
        std::shared_ptr<std::vector<GLuint>>(new std::vector<GLuint>({
        0U, 1U, 2U,
        0U, 2U, 3U,
        0U, 1U, 4U,
        1U, 2U, 4U,
        2U, 3U, 4U,
        3U, 0U, 4U,
            })));
    Camera::instance()->setFocus(&o);
    Camera::instance()->linkShader(sh.get());
    //textures
    /*int widthImg, heightImg, numColCh;
    unsigned char* bytes = stbi_load("Untitled.png", &widthImg, &heightImg, &numColCh, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(shaderprogram.getID(), "tex0");
    shaderprogram.Activate();
    glUniform1i(tex0Uni, 0);
    */
    //textures
    o.setContext([](){glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);});
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    //Object* object = CubeFactory::generate(glm::vec3(0.0f, 0.0f, 0.0f), .2f, {red},sh);
    //object->setContext();
    InputController::addObserver(Camera::instance().get());
    glfwSetMouseButtonCallback(window, InputController::GLFWmouseButtonCB);
    glfwSetCursorPosCallback(window, InputController::GLFWmouseMoveCB);
    glfwSetWindowSizeCallback(window, InputController::GLFWresizeCB);
    glfwSetKeyCallback(window, InputController::GLFWkeyCB);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Render here */
        //glBindTexture(GL_TEXTURE_2D, texture);
        //o.transform(glm::vec3(0.0f,0.0f,0.001f));
        Renderable::drawAll();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    sh->erase();
    //glDeleteTextures(1,&texture);
    glfwTerminate();


    return 0;
}