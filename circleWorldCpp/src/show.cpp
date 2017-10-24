#include "scene.cpp"
using namespace Eigen;

// Global Variables
std::string action;
Vector2f force;


void initializeRendering()
{
    glfwInit();
}


static void key_callback(GLFWwindow* window, int key, int scancode, int keypress, int mods)
{
    switch (key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_LEFT :
            if (keypress) action = "left";break;
        case GLFW_KEY_RIGHT:
            if (keypress) action = "right"; break;
        case GLFW_KEY_UP   :
            if (keypress) action = "up"; break;
        case GLFW_KEY_DOWN :
            if (keypress) action = "down"; break;
        case GLFW_KEY_R :
            if (keypress) action = "rotate"; break;
        case GLFW_KEY_SPACE: 
            if (keypress) action = "space"; break;
        case GLFW_KEY_Z: 
            if (keypress) action = "stop"; break;
        default: break;
    }
}

void run_display(std::string display_type, Scene* scene) {
	initializeRendering();
    
	int Width_global = int(scene->radii[0] * 2.0f/scene->dxy) + 20;
	int Height_global = int(scene->radii[1] * 2.0f/scene->dxy) + 20;

    GLFWwindow* window = glfwCreateWindow( Width_global, Height_global, "circleworld", NULL, NULL );
    if ( !window )
    {
        std::cerr << "Error on window creating" << std::endl;
        glfwTerminate();
        return;
    }

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if ( !mode )
    {
        std::cerr << "Error on getting monitor" << std::endl;
        glfwTerminate();
        return;
    }

	glfwMakeContextCurrent( window );



    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    // glfwGetFramebufferSize(window, &Width_global, &Height_global);
    


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Width_global, 0, Height_global, 1, -1);
    
    glfwSetWindowTitle(window, "Demonstration");
    // glfwSetWindowSizeCallback(window, size_callback);
    glfwSetKeyCallback(window, key_callback);

    double xpos, ypos;
    float newx, newy, mindist, distance;
    float threshold = 0;
    int counter = 0;

    while( !glfwWindowShouldClose( window ) ) // infinite loop to draw object again and again
    {   // because once object is draw then window is terminated

        // int start = 0;
        // if (timer_on){
        //     if (timer_up){
        //         timer+=0.1;
        //     }
        //     else{
        //         timer-=0.1;
        //     }
        //     sphere_timer+=0.01;
        // }
        // int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    	// std::cout << scene->agent->location;
        scene->display( window );
        if (scene->terminate) {
        	glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        scene->updateScene();
        Vector2f force(0.0f, 0.0f); // currently undefined
        scene->takeAction(action, force);
        counter++;
        // std::cout << "Reward: " << scene->agent->reward << std::endl;
        if(counter == 10) {
        	counter = 0;
        }
        action = "";
  

        glfwPollEvents();

        // In displayWindow, if screenshotting, then instead of displaying the window, make the screenshot        
    }
    return;

}