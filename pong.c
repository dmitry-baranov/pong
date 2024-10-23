
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

int input(int turn, char in);
void game();
int switchDirection(int ballBefore, int ballAfter);
int updateRacketPosition(int positionOfRacket, int move);
void draw(int xBallBefore, int yBallBefore, int bottomRacketLeft, int bottomRacketRight, int scoreLeft, int scoreRight);
char drawSymble(int x, int y, int xBallBefore, int yBallBefore, int bottomRacketLeft, int bottomRacketRight,
                int scoreLeft, int scoreRight);

int main() {
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    game();
    endwin();
    return 0;
}


void game(){
    int xBallAfter = 40, xBallBefore = 39, yBallAfter = 12, yBallBefore = 13, bottomRacketLeft = 11,
        bottomRacketRight = 11, scoreLeft = 0, scoreRight = 0, turn = 1;
    char in;
    while ((in = getch()) != 'q' && in != 'Q') {
        draw(xBallBefore, yBallBefore, bottomRacketLeft, bottomRacketRight, scoreLeft, scoreRight);
        if ((scoreLeft == 21) || (scoreRight == 21)) break;
        int move = input(turn, in);
        if (turn < 0)
            bottomRacketLeft = updateRacketPosition(bottomRacketLeft, move);
        else if (turn > 0)
            bottomRacketRight = updateRacketPosition(bottomRacketRight, move);
        int moveDirection = xBallAfter - xBallBefore;
        xBallBefore = xBallAfter;
        xBallAfter = xBallBefore + moveDirection;
        moveDirection = yBallAfter - yBallBefore;
        yBallBefore = yBallAfter;
        yBallAfter = yBallBefore + moveDirection;
        if ((yBallAfter == 0 || yBallAfter == 24)) {
            yBallAfter = switchDirection(yBallBefore, yBallAfter);
        }
        if (((xBallAfter == 1) &&
             (yBallBefore >= bottomRacketLeft && yBallBefore <= (bottomRacketLeft + 2))) ||
            ((xBallAfter == 78) && (yBallBefore >= bottomRacketRight &&
                                    yBallBefore <= (bottomRacketRight + 2)))) {
            xBallAfter = switchDirection(xBallBefore, xBallAfter);
        }
        if (xBallAfter == 79) {
            scoreLeft = scoreLeft + 1;
            xBallAfter = 40, xBallBefore = 39, yBallAfter = 12, yBallBefore = 13;
        } else if (xBallAfter == 0) {
            scoreRight = scoreRight + 1;
            xBallAfter = 40, xBallBefore = 39, yBallAfter = 12, yBallBefore = 13;
        }
        turn = turn * (-1);
        usleep(100000);
    }
    if (scoreLeft >= 21)
        printf("%28s!!!!WINNER PLAYER LEFT!!!!\n", " ");
    else if (scoreRight >= 21)
        printf("%28s!!!!WINNER PLAYER RIGHT!!!!\n", " ");
}

int switchDirection(int ballBefore, int ballAfter) {
    int moveDirection = ballAfter - ballBefore;
    return ballBefore - moveDirection;
}

int input(int turn, char in) {
    int result = 0;
    if (turn < 0 && (in == 'A' || in == 'a'))
        result = 1;
    else if (turn < 0 && (in == 'Z' || in == 'z'))
        result = -1;
    else if (turn > 0 && (in == 'L' || in == 'l'))
        result = 1;
    else if (turn > 0 && (in == 'M' || in == 'm'))
        result = -1;
    return result;
}

int updateRacketPosition(int positionOfRacket, int move) {
    int result = positionOfRacket;
    if (((positionOfRacket + move) >= 1) && ((positionOfRacket + 2 + move) <= 23))
        result = positionOfRacket + move;
    return result;
}

void draw(int xBallBefore, int yBallBefore, int bottomRacketLeft, int bottomRacketRight, int scoreLeft, int scoreRight) {
    clear();
    for (int y = 24; y >= 0; y--) {
        for (int x = 0; x <= 79; x++) {
            printw("%c", drawSymble(x, y, xBallBefore, yBallBefore, bottomRacketLeft, bottomRacketRight,
                                    scoreLeft, scoreRight));
        }
        printw("\n");
    }
    refresh();
}

char drawSymble(int x, int y, int xBallBefore, int yBallBefore, int bottomRacketLeft, int bottomRacketRight,
                int scoreLeft, int scoreRight) {
    char result = ' ';
    if (x == 37 && y == 24 && scoreLeft / 10 > 0)
        result = '0' + scoreLeft / 10;
    else if (x == 38 && y == 24)
        result = '0' + scoreLeft % 10;
    else if (x == 40 && y == 24 && scoreRight / 10 > 0)
        result = '0' + scoreRight / 10;
    else if (x == 41 && y == 24)
        result = '0' + scoreRight % 10;
    else if (x == 0 || x == 79)
        result = '#';
    else if (y == 24 || y == 0)
        result = '#';
    else if (x == 1 && (y >= bottomRacketLeft && y <= bottomRacketLeft + 2))
        result = ']';
    else if (x == 78 && (y >= bottomRacketRight && y <= bottomRacketRight + 2))
        result = '[';
    else if (x == xBallBefore && y == yBallBefore)
        result = '@';
    return result;
}
