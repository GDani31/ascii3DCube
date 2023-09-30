#include <stdio.h>
#include <math.h>
#include <windows.h>

const int width = 160;
const int height = 44;
char buffer[160 * 44];

float a = 0.0, b = 0.0, c = 0.0;

struct Vertex
{
    float x;
    float y;
    float z;
};

float calculateX(float x, float y, float z, float a, float b, float c)
{
    return cos(c) * (x * cos(b) - sin(b) * (z * cos(a) - y * sin(a))) + sin(c) * (y * cos(a) + z * sin(a));
}

float calculateY(float x, float y, float z, float a, float b, float c)
{
    return cos(c) * (y * cos(a) + z * sin(a)) - sin(c) * (x * cos(b) - sin(b) * (z * cos(a) - y * sin(a)));
}

float calculateZ(float x, float y, float z, float a, float b)
{
    return x * sin(b) + cos(b) * (z * cos(a) - y * sin(a));
}

void clearBuffer()
{
    for (int i = 0; i < width * height; ++i)
    {
        buffer[i] = ' ';
    }
}

void setPixel(int x, int y, char c)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        buffer[y * width + x] = c;
    }
}

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = (float)dx / (float)steps;
    float yIncrement = (float)dy / (float)steps;
    float x = (float)x1;
    float y = (float)y1;

    for (int i = 0; i <= steps; i++)
    {
        setPixel(round(x), round(y), '#');
        x += xIncrement;
        y += yIncrement;
    }
}

void drawCube(struct Vertex cube[8])
{
    struct Vertex rotatedCube[8];
    for (int i = 0; i < 8; i++)
    {
        float x = cube[i].x;
        float y = cube[i].y;
        float z = cube[i].z;

        float xRotated = calculateX(x, y, z, a, b, c);
        float yRotated = calculateY(x, y, z, a, b, c);
        float zRotated = calculateZ(x, y, z, a, b);

        xRotated *= 10;
        yRotated *= 10;

        xRotated += width / 2;
        yRotated += height / 2;

        rotatedCube[i].x = xRotated;
        rotatedCube[i].y = yRotated;
    }

    drawLine(rotatedCube[0].x, rotatedCube[0].y, rotatedCube[1].x, rotatedCube[1].y);
    drawLine(rotatedCube[1].x, rotatedCube[1].y, rotatedCube[3].x, rotatedCube[3].y);
    drawLine(rotatedCube[3].x, rotatedCube[3].y, rotatedCube[2].x, rotatedCube[2].y);
    drawLine(rotatedCube[2].x, rotatedCube[2].y, rotatedCube[0].x, rotatedCube[0].y);

    drawLine(rotatedCube[4].x, rotatedCube[4].y, rotatedCube[5].x, rotatedCube[5].y);
    drawLine(rotatedCube[5].x, rotatedCube[5].y, rotatedCube[7].x, rotatedCube[7].y);
    drawLine(rotatedCube[7].x, rotatedCube[7].y, rotatedCube[6].x, rotatedCube[6].y);
    drawLine(rotatedCube[6].x, rotatedCube[6].y, rotatedCube[4].x, rotatedCube[4].y);

    drawLine(rotatedCube[0].x, rotatedCube[0].y, rotatedCube[4].x, rotatedCube[4].y);
    drawLine(rotatedCube[1].x, rotatedCube[1].y, rotatedCube[5].x, rotatedCube[5].y);
    drawLine(rotatedCube[2].x, rotatedCube[2].y, rotatedCube[6].x, rotatedCube[6].y);
    drawLine(rotatedCube[3].x, rotatedCube[3].y, rotatedCube[7].x, rotatedCube[7].y);
	
	for (int i = 0; i < 4; i++)
    {
        drawLine(rotatedCube[i].x, rotatedCube[i].y, rotatedCube[i + 4].x, rotatedCube[i + 4].y);
    }
	
}

void printBuffer()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { width, height };
    COORD characterPos = { 0, 0 };
    SMALL_RECT writeArea = { 0, 0, width - 1, height - 1 };

    CHAR_INFO* charBuffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * width * height);
    for (int i = 0; i < width * height; i++)
    {
        charBuffer[i].Char.UnicodeChar = buffer[i];
        charBuffer[i].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
    }

    WriteConsoleOutput(hConsole, charBuffer, bufferSize, characterPos, &writeArea);
    free(charBuffer);
}


int main()
{
    struct Vertex cube[8];

    cube[0].x = -1;
    cube[0].y = -1;
    cube[0].z = -1;

    cube[1].x = 1;
    cube[1].y = -1;
    cube[1].z = -1;

    cube[2].x = -1;
    cube[2].y = 1;
    cube[2].z = -1;

    cube[3].x = 1;
    cube[3].y = 1;
    cube[3].z = -1;

    cube[4].x = -1;
    cube[4].y = -1;
    cube[4].z = 1;

    cube[5].x = 1;
    cube[5].y = -1;
    cube[5].z = 1;

    cube[6].x = -1;
    cube[6].y = 1;
    cube[6].z = 1;

    cube[7].x = 1;
    cube[7].y = 1;
    cube[7].z = 1;

    while (1)
    {
        clearBuffer();
        drawCube(cube);
        printBuffer();

        a += 0.001;
        b += 0.001;
        c += 0.001;
    }
    return 0;
}
