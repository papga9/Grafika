
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void CalcLagrangePoints();
void CalcBezierPoints();
void CatmullClark();
void InverseCatmullClarkMode();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//globalis valtozok
const unsigned int MAX_POINTS = 1000;
//A vertices matrix ugy nez ki, hogy az elso 3 helyen a koordinatak vannak, az utolso 3 helyen a szinek minden vertexnek
//A vertices elso feleben vannak a pontok tarolva, a masodik feleben a vonalak
float vertices[2 * MAX_POINTS * 6];
int ptNum = 0;
unsigned int VBO, VAO;
float r = 1.0, g = 1.0, b = 1.0;
bool RMBpushed = false; //jobb egergomb lenyomasat ellenorzo logikai valtozo
bool LagrangeMode = false;//Azt ellenorzi, hogy "Lagrange modban" van-e
bool BezierMode = false;//Azt ellenorzi, hogy "Bezier modban" van-e
int CatmullClarkMode = 0;//Azt ellenoorzi, hogy "Catmull-Clark modban" van-e
int SelectedPointIDx = -1;//Ezzel szamlaljuk,hogy SelectedPointIDxadik a jobb egergombbal "megfogott" pont a vertices tombben

void clearGUI() //torlo fuggveny
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ptNum = 0;
    LagrangeMode = false;
    BezierMode = false;
    CatmullClarkMode = 0;
    return;
}

void LineWidth(int v) //vastagsagot allito fuggveny
{
    glLineWidth(v);
    return;
}
int main1() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("4.0.shader.vs", "4.0.shader.fs"); // you can name your shader files however you like

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        // render
        // ------
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindVertexArray(VAO);
        glPointSize(9);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------


        // Draw line segments
        if (LagrangeMode || BezierMode)
        {
            glDrawArrays(GL_LINE_STRIP, MAX_POINTS, MAX_POINTS);
        }
        else
        {
            glDrawArrays(GL_LINE_STRIP, MAX_POINTS, ptNum);
        }
        // Draw clicked points
        glDrawArrays(GL_POINTS, 0, ptNum);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // C lenyomasara torol
    {
        clearGUI();
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) //1 lenyomasara a vonalvastagsag 1 lesz
    {
        LineWidth(1);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        LineWidth(3);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        LineWidth(4);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        LineWidth(5);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        LineWidth(6);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        LineWidth(7);
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        LineWidth(8);
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        LineWidth(9);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) //L lenyomasara "Lagrange modra" valt, ujboli lenyomasra visszavalt tort vonalakra
    {
        BezierMode = false;
        if (!LagrangeMode)
        {
            LagrangeMode = true;
            CalcLagrangePoints();
            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * MAX_POINTS * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
        }
        else
        {
            // Turn off Lagrange mode, reset the curve to line
            LagrangeMode = false;
            for (int i = 0; i < ptNum; i++)
            {
                vertices[(MAX_POINTS + i) * 6 + 0] = vertices[i * 6 + 0];
                vertices[(MAX_POINTS + i) * 6 + 1] = vertices[i * 6 + 1];
            }
            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * ptNum * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) //B lenyomasara "Bezier modra" valtunk
    {
        LagrangeMode = false;
        if (!BezierMode)
        {
            BezierMode = true;
            CalcBezierPoints();
            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * MAX_POINTS * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
        }
        else
        {
            // Turn off Bezier mode, reset the curve to line
            BezierMode = false;
            for (int i = 0; i < ptNum; i++)
            {
                vertices[(MAX_POINTS + i) * 6] = vertices[i * 6];
                vertices[(MAX_POINTS + i) * 6 + 1] = vertices[i * 6 + 1];
            }
            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * ptNum * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) //J-re simitjuk a gorbet
    {
        CatmullClarkMode++;
        BezierMode = false;
        LagrangeMode = false;
        CatmullClark();
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) //inverz Catmull-Clark
    {
        if (CatmullClarkMode != 0)
        {
            BezierMode = false;
            LagrangeMode = false;
            CatmullClarkMode--;
            InverseCatmullClarkMode();
        }
    }
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (RMBpushed == true && SelectedPointIDx != -1)
    {
        xpos = xpos / (SCR_WIDTH / 2) - 1;
        ypos = -ypos / (SCR_HEIGHT / 2) + 1;

        //Point update
        vertices[6 * SelectedPointIDx + 0] = xpos;
        vertices[6 * SelectedPointIDx + 1] = ypos;
        //Set the color of the clicked point to blue while it is moving
        vertices[6 * SelectedPointIDx + 3] = 0.0;
        vertices[6 * SelectedPointIDx + 4] = 0.0;
        vertices[6 * SelectedPointIDx + 5] = 1.0;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 6 * SelectedPointIDx * sizeof(float), 6 * sizeof(float), &vertices[6 * SelectedPointIDx + 0]);

        //Line update
        if (!LagrangeMode && !BezierMode)
        {
            vertices[6 * (MAX_POINTS + SelectedPointIDx) + 0] = xpos;
            vertices[6 * (MAX_POINTS + SelectedPointIDx) + 1] = ypos;
            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS + SelectedPointIDx) * sizeof(float), 6 * sizeof(float), &vertices[6 * (MAX_POINTS + SelectedPointIDx) + 0]);
        }
        else
        {
            if (LagrangeMode)
            {
                CalcLagrangePoints();
            }
            else
            {
                CalcBezierPoints();
            }
            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * MAX_POINTS * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
        }
    }
}
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        CatmullClarkMode = 0;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);

        // Create point from click
        xpos = xpos / (SCR_WIDTH / 2) - 1;
        ypos = -ypos / (SCR_HEIGHT / 2) + 1;
        vertices[6 * ptNum + 0] = xpos;
        vertices[6 * ptNum + 1] = ypos;
        vertices[6 * ptNum + 2] = 0;
        vertices[6 * ptNum + 3] = 1.0;
        vertices[6 * ptNum + 4] = 0.0;
        vertices[6 * ptNum + 5] = 0.0;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 6 * ptNum * sizeof(float), 6 * sizeof(float), &vertices[6 * ptNum + 0]);

        if (!LagrangeMode && !BezierMode)
        {
            // Create line from click
            vertices[(MAX_POINTS + ptNum) * 6 + 0] = xpos;
            vertices[(MAX_POINTS + ptNum) * 6 + 1] = ypos;
            vertices[(MAX_POINTS + ptNum) * 6 + 2] = 0;
            vertices[(MAX_POINTS + ptNum) * 6 + 3] = 0.0;
            vertices[(MAX_POINTS + ptNum) * 6 + 4] = 0.0;
            vertices[(MAX_POINTS + ptNum) * 6 + 5] = 0.0;
            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS + ptNum) * sizeof(float), 6 * sizeof(float), &vertices[6 * (MAX_POINTS + ptNum) + 0]);
            ptNum++;
        }
        else
        {
            ptNum++;
            if (LagrangeMode)
            {
                CalcLagrangePoints();
            }
            else
            {
                CalcBezierPoints();
            }

            glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * MAX_POINTS * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
        }

        std::cout << "Cursor Position at (" << xpos << " : " << ypos << "\n";
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        double ClickRadius = 5.0;
        RMBpushed = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos = xpos / (SCR_WIDTH / 2) - 1;
        ypos = -ypos / (SCR_HEIGHT / 2) + 1;
        for (int i = 0; i != ptNum; i++)
        {
            if (vertices[i * 6]<xpos + ClickRadius / SCR_WIDTH && vertices[i * 6]>xpos - ClickRadius / SCR_WIDTH && vertices[i * 6 + 1]<ypos + ClickRadius / SCR_HEIGHT && vertices[i * 6 + 1]>ypos - ClickRadius / SCR_HEIGHT)
            {
                SelectedPointIDx = i;
            }
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        RMBpushed = false;
        //Reset the color of the clicked point to red
        vertices[6 * SelectedPointIDx + 3] = 1.0;
        vertices[6 * SelectedPointIDx + 4] = 0.0;
        vertices[6 * SelectedPointIDx + 5] = 0.0;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 6 * SelectedPointIDx * sizeof(float), 6 * sizeof(float), &vertices[6 * SelectedPointIDx + 0]);

        SelectedPointIDx = -1;
    }
}
//Lagrange egyĂźtthatĂł szĂĄmĂ­tĂĄs
double L(int i, double tt)
{
    double Li = 1.0;
    for (int j = 0; j < ptNum; j++)
    {
        int pp = max(ptNum - 1, 1);
        if (i != j)
            Li *= (tt - (double)j / pp) / ((double)i / pp - (double)j / pp);
    }
    return Li;
}
//Lagrange gĂśrbe adott pontjanak szamÄątasa t parametererteknel:
void CalcLagrangePoints()
{
    for (int k = 0; k < MAX_POINTS; k++)
    {
        float t = (float)k / (MAX_POINTS - 1);
        vertices[(MAX_POINTS + k) * 6] = 0;
        vertices[(MAX_POINTS + k) * 6 + 1] = 0;
        for (int i = 0; i < ptNum; i++)
        {
            double Li = L(i, t);
            vertices[(MAX_POINTS + k) * 6] += vertices[i * 6] * Li;
            vertices[(MAX_POINTS + k) * 6 + 1] += vertices[i * 6 + 1] * Li;
        }
    }
}
//Bezier egyĂźtthatĂł szĂĄmĂ­tĂĄs
float B(int i, float t)
{
    float Bi = 1.0;
    int j = 1;
    for (; j <= i; j++)
    {
        Bi *= (t * (ptNum - j)) / j;
    }
    for (; j < ptNum; j++)
    {
        Bi *= (1 - t);
    }
    return Bi;
}
//Bezier gĂśrbe szĂĄmĂ­tĂĄs
void CalcBezierPoints()
{
    for (int k = 0; k < MAX_POINTS; k++)
    {
        float t = (float)k / (MAX_POINTS - 1);
        vertices[(MAX_POINTS + k) * 6] = 0;
        vertices[(MAX_POINTS + k) * 6 + 1] = 0;
        for (int i = 0; i <= ptNum; i++)
        {
            double Bi = B(i, t);
            vertices[(MAX_POINTS + k) * 6] += vertices[i * 6] * Bi;
            vertices[(MAX_POINTS + k) * 6 + 1] += vertices[i * 6 + 1] * Bi;
        }
    }
}

void CatmullClark()
{
    vector<float> old;
    for (int j = 0; j < ptNum; j++)
    {
        old.push_back(vertices[j * 6]);
        old.push_back(vertices[j * 6 + 1]);
    }
    vertices[6] = (old[0] + old[2]) / 2;
    vertices[7] = (old[1] + old[3]) / 2;
    for (int i = 1; i < ptNum - 1; i++)
    {
        vertices[6 * (2 * i + 1)] = (old[i * 2] + old[i * 2 + 2]) / 2;
        vertices[6 * (2 * i + 1) + 1] = (old[i * 2 + 1] + old[i * 2 + 2 + 1]) / 2;
        vertices[6 * 2 * i] = (0.25 * vertices[6 * (2 * i - 1)]) + (0.5 * old[i * 2]) + (0.25 * vertices[6 * (2 * i + 1)]);
        vertices[6 * 2 * i + 1] = (0.25 * vertices[6 * (2 * i - 1) + 1]) + (0.5 * old[i * 2 + 1]) + (0.25 * vertices[6 * (2 * i + 1) + 1]);

        vertices[6 * (2 * i + 1) + 3] = 1.0;
        vertices[6 * 2 * i + 3] = 1.0;
    }
    ptNum += (ptNum - 1);
    vertices[(ptNum - 1) * 6] = old[old.size() - 2];
    vertices[(ptNum - 1) * 6 + 1] = old[old.size() - 1];

    vertices[(ptNum - 1) * 6 + 3] = 1.0;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * ptNum * sizeof(float), &vertices[0]);
    for (int i = 0; i < ptNum; i++)
    {
        vertices[(MAX_POINTS + i) * 6] = vertices[i * 6];
        vertices[(MAX_POINTS + i) * 6 + 1] = vertices[i * 6 + 1];
    }
    glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * ptNum * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
}
void InverseCatmullClarkMode()
{
    vector<float> oldx, oldy;
    for (int j = 0; j < ptNum; j++)
    {
        oldx.push_back(vertices[j * 6]);
        oldy.push_back(vertices[j * 6 + 1]);
    }
    ptNum = (ptNum + 1) / 2;
    for (int i = 1; i < ptNum - 1; i++)
    {
        vertices[6 * i] = (oldx[(2 * i)] * 2) - 0.5 * (oldx[(2 * i - 1)] + oldx[(2 * i + 1)]);
        vertices[6 * i + 1] = oldy[(2 * i)] * 2 - 0.5 * (oldy[(2 * i - 1)] + oldy[(2 * i + 1)]);
    }
    vertices[(ptNum - 1) * 6] = oldx[oldx.size() - 1];
    vertices[(ptNum - 1) * 6 + 1] = oldy[oldy.size() - 1];

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * ptNum * sizeof(float), &vertices[0]);
    for (int i = 0; i < ptNum; i++)
    {
        vertices[(MAX_POINTS + i) * 6] = vertices[i * 6];
        vertices[(MAX_POINTS + i) * 6 + 1] = vertices[i * 6 + 1];
    }
    glBufferSubData(GL_ARRAY_BUFFER, 6 * (MAX_POINTS) * sizeof(float), 6 * ptNum * sizeof(float), &vertices[6 * (MAX_POINTS)+0]);
}

