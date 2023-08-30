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
    omegaang = 0.0f;
    yperc = 0.0f;
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
//temp
void Camera::handleMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << yoffset << std::endl;
}
//good enough
glm::vec3 Camera::panDirection(double xdiff, double ydiff)
{
    xdiff *= -1;
    float xperc = sinf(omegaang * PI);
    float zperc = cosf(omegaang * PI);
    //float dumb = subtractFromOne(yperc);
    return glm::vec3({xperc * xdiff + zperc * -1.0f * yperc * ydiff,ydiff * yperc,xperc * yperc * -1.0f *ydiff + zperc * xdiff});
}
float Camera::subtractFromOne(float num) {
    if (num > 0.0f)
        return 1.0f - num;
    else
        return -1.0f - num;
}
void Camera::handleKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_EQUAL)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            total -= .05f;
            if (total < .05f)
                total = .05f;
            refresh();
        }
        else
        {
            total += .05f;
            refresh();
        }
    }
}
void Camera::handleDrag(GLFWwindow* window,double xnewpos, double ynewpos) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN)
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            auto var = panDirection(xpos-xnewpos,ypos-ynewpos);
            center += var * .001f;
            viewpoint += var * .001f;
            view = glm::lookAt(viewpoint, center, glm::vec3(0.0f, 1.0f, 0.0f));
            glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            xpos = xnewpos;
            ypos = ynewpos;
        }
        else
        {
            using namespace std;
            //rotational angles
            omegaang -= glm::radians((float)xpos - xnewpos);
            //zperc is the amount of transformation that belongs to the z axis, 0 = z parallel to camera
            yperc += glm::radians((float)ypos - ynewpos);
            if (yperc > 1.0f)
                yperc = .999f;
            if (yperc < -1.0f)
                yperc = -.999f;
            
            refresh();
            xpos = xnewpos;
            ypos = ynewpos;
        }
}
void Camera::refresh()
{
    float zperc = sinf(omegaang * PI);
    float xperc = cosf(omegaang * PI);

    viewpoint = glm::vec3(center);
    float temp = total - fabs(total * yperc);
    viewpoint[1] += total * yperc;
    viewpoint[2] += zperc * temp;
    viewpoint[0] += xperc * temp;

    view = glm::lookAt(viewpoint, center, glm::vec3(0.0f, 1.0f, 0.0f));

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}