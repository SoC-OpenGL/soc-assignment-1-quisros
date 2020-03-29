//
//  ring.cpp
//
//  Created by Shabnam Sahay on 28/03/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<math.h>
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float degtorad(int degrees) {
    const float pi = 3.14159;
    return (degrees*pi)/180;
}



// settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

//vertex shader code

const char *vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main ()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


//fragment shader code

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main ()\n"
    "{\n"
    "    FragColor = vec4(0.0f, 0.0f, 0.5f, 1.0f);\n"
        //1.0 alpha value means completely opaque
    "}\0";



int main () {
    
    
    //glfw: initialize and configure
    
    glfwInit();
    if (!glfwInit ()) { // glfwInit() attempts to initialize GLFW lib
        cerr << "ERROR: could not start GLFW3" << endl;
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ring", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
 
    
    //start GLEW extension handler
    glewExperimental = GL_TRUE; //enables experimental extensions
    glewInit();
    
    
    
    //creating vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //attach shader source code to object and compile shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    //checking if vertex shader compilation was successful

    int success;

    //storage container for error msgs (if any)
    char infoLog[512];

    //check if compilation successful
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);

    if(!success) {
        //retrieve error message with given fn and print the error msg
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    
    //creating and compiling fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    //checking if fragment shader compilation was successful
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);

    if(!success) {
        //retrieve error message with given fn and print the error msg
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //creating shader program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
        
    //attaching previously compiled shaders to it, then linking them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
        
    //checking if linking failed
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    

    //deleting initial shaders which are now redundant
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    
    float r = 0.75;
    
    float vertices[361*3];
    
    for(int i = 0; i<361; i++) {
        int crindex = i*3;
        float theta = degtorad(i);
        vertices[crindex] = r*cos(theta);
        vertices[crindex+1] = r*sin(theta);
        vertices[crindex+2] = 0.0;
    }
    
    
    //generating vao and vbo
    unsigned int VAO, VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
 
    
    //uncomment below call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //change GL_LINE to GL_FILL to get default mode again

    
    
    // render loop
    
    while(!glfwWindowShouldClose(window)) {
        
        //input
        processInput(window);
        
        //render
        
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        //activating shader program
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        //drawing the ring
        for (int i = 0; i<360; i++) {
            glDrawArrays(GL_LINES, i, 2);
        }
        
        glBindVertexArray(0);
        
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //clear all previously allocated GLFW resources
    glfwTerminate();
    return 0;
    
}
