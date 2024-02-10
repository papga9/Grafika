#include "Snake.h"



Snake::Snake(float _r, float _g, float _b){
    r = _r;
    g = _g;
    b = _b;
    createSnake();

}


void Snake::createSnake() {
    std::pair<float, float> head(pos_x, pos_y);
    std::pair<float, float> neck(pos_x - 2 * SCALE, pos_y);
    std::pair<float, float> tail(pos_x - 4 * SCALE, pos_y);

    bodyParts.push_back(head);
    bodyParts.push_back(neck);
    bodyParts.push_back(tail);
}

void Snake::setFacing(Facing newFacing) {
    switch (facing) {
    case Facing::DOWN:
    case Facing::UP:
        if (newFacing == Facing::UP || newFacing == Facing::DOWN) {
            return;
        }
        break;
    case Facing::LEFT:
    case Facing::RIGHT:
        if (newFacing == Facing::LEFT || newFacing == Facing::RIGHT) {
            return;
        }
    }

    facing = newFacing;


   
}

void Snake::move(Facing newFacing) {

    endTimer();
    if (getDifference() > referenceDuration) {
        
        setFacing(newFacing);


        float dx = 0.0f;
        float dy = 0.0f;

        switch (facing)
        {
        case Facing::UP:
            dy = 2 * SCALE;
            break;
        case Facing::DOWN:
            dy = -2 * SCALE;
            break;
        case Facing::RIGHT:
            dx = 2 * SCALE;
            break;
        case Facing::LEFT:
            dx = -2 * SCALE;
            break;
        }
        

    for (int i = bodyParts.size()-1; i >= 0 ; i--) {
        std::pair<float, float> current_body_part = bodyParts[i];

        current_body_part.first = bodyParts[i].first + dx;
        current_body_part.second = bodyParts[i].second + dy;

        if (abs(current_body_part.first) >= 1 - 2 * SCALE)
        {
            float tmp_x = current_body_part.first;
            bodyParts[i].first = -tmp_x;
        }

        if (abs(current_body_part.second) >= 1 - 2 * SCALE)
        {
            float tmp_y = current_body_part.second;
            bodyParts[i].second = -tmp_y;
        }

        if (i > 0) {
            bodyParts[i].first = bodyParts[i-1].first;
            bodyParts[i].second =bodyParts[i-1].second;
        }
        else {
            bodyParts[i].first += dx;
            bodyParts[i].second += dy;
        }
    }

    for (int j = 1; j < bodyParts.size(); j++)
    {
        if (bodyParts[0].first == bodyParts[j].first && bodyParts[0].second == bodyParts[j].second)
        {
            exit(0);
        }
    }
    startTimer();
    }
 }

void Snake::eat() {

    float dx = 0.0f;
    float dy = 0.0f;

    switch (facing)
    {
    case Facing::UP:
        dy = -2 * SCALE;
        break;
    case Facing::DOWN:
        dy = 2 * SCALE;
        break;
    case Facing::RIGHT:
        dx = -2 * SCALE;
        break;
    case Facing::LEFT:
        dx = 2 * SCALE;
        break;
    }


    std::pair<float, float> lastBodyCell = bodyParts.back();
    std::pair<float, float> newBodyCell(lastBodyCell.first+dx,lastBodyCell.second+dy);
    bodyParts.push_back(newBodyCell);



}


void Snake::draw() {
	ourShader.use();
	glBindVertexArray(VAO);
    drawBodyParts();

}

std::deque<std::pair<float, float>> Snake::getBodyParts() const {
    return bodyParts;
}



