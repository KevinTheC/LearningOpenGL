#include "Camera.h"

std::shared_ptr<Camera> Camera::camera = {};
#define PI 3.1416f
#define MAX_FLOAT 3.4028235E38f
#define displayc(sargs, args) std::cout << sargs << args << "\n"
Camera::Camera()
{
    thetaang = 0;
    omegaang = 0;
    flip = 1;
    prev = 0;
    inc = false;
    xpos = 0;
    ypos = 0;
    uboMatrices = 0;
    proj = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, -1.0f));
}
void Camera::linkShader(Shader* sh)
{
    sh->activate();
    GLuint ufBlockIndex = glGetUniformBlockIndex(sh->ID, "Matrices");
    glUniformBlockBinding(sh->ID, ufBlockIndex, 0);

    glGenBuffers(1, &uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Camera::updateProjection(int w, int h)
{
    proj = glm::perspective(glm::radians(90.0f), (float)(h * 1.0 / w * 1.0), 0.1f, 100.0f);
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}
void Camera::setFocus(Object* focus)
{
    Camera::focus = focus;
}
const glm::mat4& Camera::getProj()
{
    return proj;
}
const glm::mat4& Camera::getView()
{
    return view;
}

//static functions
std::array<GLfloat,3> Camera::getClosestTo(std::pair<float,float> pos, Object& o)
{
    float distance = MAX_FLOAT;
    std::array<GLfloat, 3> ret = {};
    for (auto itr = o.cbegin(); itr != o.cend(); ++itr)
    {
        glm::vec4 vec = camera->proj * camera->view * camera->focus->getModel() * glm::vec4(itr->at(0),itr->at(1),itr->at(2), 1.0f);
        float x = (1 + vec[0] / vec[3]) * instance()->width / 2;
        float y = (1 + vec[1] / vec[3]) * instance()->height / 2;
        float newdist = Camera::distance(pos, std::pair<float, float>(x, y));
        if (newdist < distance)
        {
            ret = *itr;
            distance = newdist;
        }
    }
    return ret;
}

float Camera::distance(std::pair<float, float>& p1, std::pair<float, float> p2)
{
    return std::sqrtf(std::powf(p1.first-p2.first,2.0f)+std::powf(p1.second - p2.second,2.0f));
}

void Camera::GLFWresizeCB(GLFWwindow* window,int w, int h)
{
    camera->updateProjection(w,h);
}
void Camera::GLFWmouseButtonCB(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        if (action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwGetCursorPos(window, &(camera->xpos), &(camera->ypos));
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
    {
        double x;
        double y;
        glfwGetCursorPos(window,&x,&y);
        y = camera->height - y;
        auto point = getClosestTo(std::pair<float,float>((float)x,(float)y), *camera->focus);
        //unfinished
    }
}
//this seriously should be redone but i don't have any more ideas on how to do it
void Camera::GLFWmouseMoveCB(GLFWwindow* window, double xnewpos, double ynewpos) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN)
    {
        auto center = camera->focus->center();
        //rotational angles
        camera->thetaang += glm::radians((float)camera->ypos - ynewpos);
        camera->omegaang += glm::radians((float)camera->xpos - xnewpos);
        using namespace std;
        //reset view matrix
        camera->view = glm::mat4(1.0);
        //the x z spin makes sense, y is a construct that is against god
        float zperc = sinf(camera->omegaang*PI);
        float xperc = cosf(camera->omegaang*PI);
        float yperc = cosf(camera->thetaang*PI);
        //this works but its kinda ugly and doesn't give the illusion of full rotation because the x and y stay distant
        //just basically flips the x and z values at a high enough absolute y value
        if (fabs(yperc)>.995f&&
            (camera->inc && camera->prev > yperc)||(!camera->inc && camera->prev < yperc))
        {
            camera->inc = !camera->inc;
            camera->flip *= -1;
        }
        camera->prev = yperc;
        auto temp = glm::vec3(center);
        float total = 1.5f;
        temp[1] += (total * yperc);
        total -= fabs(total * yperc);
        temp[2] += (zperc * total * camera->flip);
        temp[0] += (xperc * total * camera->flip);

        camera->view = glm::lookAt(temp, center, glm::vec3(0.0f,1.0f,0.0f));
        //view = glm::translate(view, glm::vec3(cosO*cosf(thetaang),cosO*sinf(thetaang)-.5f,sinf(omegaang)-1.5f));
        //view = glm::rotate(view, original[2], glm::vec3(0.0f, 0.0f, 1.0f));


        glBindBuffer(GL_UNIFORM_BUFFER, camera->uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        camera->xpos = xnewpos;
        camera->ypos = ynewpos;
    }
}