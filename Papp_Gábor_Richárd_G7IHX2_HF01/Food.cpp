#include "Food.h"
#include<time.h>
#include<cmath>


void Food::regenerate(std::deque<std::pair<float,float>> excludePositions, float tolerance)
{
    bool isOk = true;
    float new_x;
    float new_y;
    do{
        srand(time(nullptr) * (long)this);
        new_x = (rand() % 19 - 9) / 10.0;
        new_y = (rand() % 19 - 9) / 10.0;
        for (std::pair<float, float> point : excludePositions) {
            float distance = sqrt(pow(new_x - point.first, 2) + pow(new_y - point.second, 2));
            if (distance < tolerance)
            {
                isOk = false;
            }
        }
    } while (!isOk);

    pos_x = new_x;
    pos_y = new_y;
    if (bodyParts.empty()) {
        bodyParts.push_back(std::pair<float,float>(pos_x, pos_y));
    }
    else {
        bodyParts[0].first = new_x;
        bodyParts[0].second = new_y;
    }

}

void Food::generateColor() {
    r = poisoned ? 0.9f : 0.1f;
    g = poisoned ? 0.1f : 0.9f;
    b = poisoned ? 0.1f : 0.1f;
}

Food::Food(bool _poisoned) :poisoned(_poisoned) {
    regenerate(std::deque<std::pair<float, float>>(),SCALE);
    generateColor();
}


void Food::draw() {
    ourShader.use();
    glBindVertexArray(VAO);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(pos_x, pos_y, 0.0));
    ourShader.setMat4("trans", trans);
    int vertexColorLocation = glGetUniformLocation(ourShader.ID, "ourColor");
    glUniform4f(vertexColorLocation,r,g,b, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}