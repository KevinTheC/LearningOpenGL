#include "Camera.h"
std::shared_ptr<Camera> Camera::camera;
#define PI 3.1416f
#define MAX_FLOAT 3.4028235E38f
#define displayc(sargs, args) std::cout << sargs << args << "\n"
Camera::Camera()
{
    pmm = nullptr;
    viewpoint = {1.0f,0.0f,0.0f};
    total = 1.0f;
    center = {0.0f,0.0f,0.0f};
    height = 0;
    width = 0;
    focus = nullptr;
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
glm::vec2 Camera::toXY(glm::mat4 model, glm::vec4 point) {
    glm::vec4 vec = proj * view * focus->getModel() * point;
    return glm::vec2((1 + vec[0] / vec[3]) * width / 2, (1 + vec[1] / vec[3]) * height / 2);
}
Object::VertIterator Camera::getClosestTo(std::pair<float,float> pos, Object& o)
{
    float distance = MAX_FLOAT;
    auto ret = o.vertend();
    for (auto itr = o.vertbegin(); itr != o.vertend(); ++itr)
    {
        glm::vec2 xy = toXY(focus->getModel(), glm::vec4((*itr)[0], (*itr)[1], (*itr)[2], 1.0f));
        float newdist = Camera::distance(pos, std::pair<float, float>(xy[0], xy[1]));
        if (newdist < distance)
        {
            ret = itr;
            distance = newdist;
        }
    }
    return ret;
}

float Camera::distance(std::pair<float, float>& p1, std::pair<float, float> p2)
{
    return std::sqrtf(
        std::powf(p1.first - p2.first, 2.0f) +
        std::powf(p1.second - p2.second, 2.0f));
}
void Camera::handleResize(GLFWwindow* window,int w, int h)
{
    camera->updateProjection(w,h);
}
void Camera::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    glfwGetCursorPos(window, &xpos, &ypos);
    if (button == GLFW_MOUSE_BUTTON_1)
        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwGetCursorPos(window, &xpos, &ypos);
        }
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS && GLFW_PRESS == glfwGetKey(window,GLFW_KEY_LEFT_CONTROL))
    {
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);
        y = height - y;
        auto point = instance()->getClosestTo(std::pair<float, float>((float)x, (float)y), *focus);
        if (pmm)
            delete(pmm);
        pmm = new PointMovementManager(point,focus);
        
    }
}
void Camera::handleMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << yoffset << std::endl;
}
//good enough
void Camera::handleDrag(GLFWwindow* window,double xnewpos, double ynewpos) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN)
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            

            view = glm::lookAt(viewpoint, center, glm::vec3(0.0f, 1.0f, 0.0f));
            xpos = xnewpos;
            ypos = ynewpos;
        }
        else
        {
            using namespace std;
            center = focus->center();
            //rotational angles
            thetaang -= glm::radians((float)ypos - ynewpos);
            omegaang -= glm::radians((float)xpos - xnewpos);
            //reset view matrix
            view = glm::mat4(1.0);
            //the x z spin makes sense, y is a construct that is against god
            float zperc = sinf(omegaang*PI);
            float xperc = cosf(omegaang*PI);
            float yperc = cosf(thetaang*PI);
            //this works but its kinda ugly and doesn't give the illusion of full rotation because the x and y stay distant
            //just basically flips the x and z values at a high enough absolute y value
            float fabsy = fabs(yperc);
            if (fabsy>.995f&&
                (inc && prev > yperc)||(!inc && prev < yperc))
            {
                inc = !inc;
                flip *= -1;
            }
            prev = yperc;
            viewpoint = glm::vec3(center);
            
            viewpoint[1] += total * yperc;
            viewpoint[2] += ((1-fabsy) * zperc * total * flip);
            viewpoint[0] += ((1-fabsy) * xperc * total * flip);

            view = glm::lookAt(viewpoint, center, glm::vec3(0.0f,1.0f,0.0f));

            glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            xpos = xnewpos;
            ypos = ynewpos;
        }
}