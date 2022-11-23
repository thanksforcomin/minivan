#include "controls.h"

using namespace std::placeholders;

Controls::Controls()
{
    std::cout << "Cursor image is NOT loaded, yay!\n";
    glfwSetInputMode(glfwGetCurrentContext(), GLFW_STICKY_KEYS, GLFW_FALSE);
    glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(glfwGetCurrentContext(), key_callback);
    glfwSetCursorPosCallback(glfwGetCurrentContext(), mouse_position_callback);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

Controls::~Controls() {
    std::cout << "Killed controls" << std::endl;
}

void Controls::processFastInput() {
    if (keyboard_down_observer.size() == 0) return;
    for (auto&& item : keyboard_down_observer) {
        if (glfwGetKey(glfwGetCurrentContext(), item.first) == GLFW_PRESS) {
            for (auto&& func : item.second) func();
        }
    }
}

bool Controls::getKeyPressed(const int& key) {
    //std::cout << "a key pressed" << std::endl;
    if (glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS) return 1; else return 0;
}

bool Controls::getKeyReleased(const int& key) {
    if(glfwGetKey(glfwGetCurrentContext(), key) == GLFW_RELEASE) return 1; else return 0;
}

//statics
std::map<int, std::vector<std::function<void()>>> Controls::keyboard_press_observer;
std::map<int, std::vector<std::function<void()>>> Controls::keyboard_down_observer;
std::vector<std::function<void(float, float, bool)>> Controls::cursor_observer;

float Controls::m_lastx ;
float Controls::m_lasty;
float Controls::m_xpos;
float Controls::m_ypos;
float Controls::m_xoffset;
float Controls::m_yoffset;
