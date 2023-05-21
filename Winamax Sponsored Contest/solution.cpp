#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

// solution.h
//---------------------------------------------------------------
enum direction 
{   north = 1, 
    east = 2, 
    south = 3,
    west = 4
};

struct StackData
{
    int row;
    int col;
    int mag;
    int dir;
    int ballNum;
};

struct Coord
{
    int row;
    int col;
};

class GolfCourse
{
    private:
        int height;
        int width;
        int numBalls;
        char **inputMatrix;
        char **outputMatrix;
        vector<Coord> ballCoords;

    public:
        GolfCourse(int height, int width, char **matrix);
        ~GolfCourse();
        void depthFirstSearch();
        void printSolution();
};

char **parseInput(int &height, int &width);
char **deepCopyMatrix(int height, int width, char **matrix);


// solution.cpp
//---------------------------------------------------------------
GolfCourse::GolfCourse(int height, int width, char **matrix)
{
    this->height = height;
    this->width = width;
    inputMatrix = matrix;
    outputMatrix = deepCopyMatrix(height, width, matrix);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(isdigit(this->inputMatrix[i][j]))
            {
                ballCoords.push_back({i, j});
            }
        }
    }

    numBalls = ballCoords.size();
}

GolfCourse::~GolfCourse()
{
    for(int i = 0; i < height; i++)
    {
        delete inputMatrix[i];
        delete outputMatrix[i];
    }

    delete inputMatrix;
    delete outputMatrix;
}

void GolfCourse::depthFirstSearch()
{
    vector<StackData> stack;

    Coord ballCoord = ballCoords[0];
    int startRow = ballCoord.row;
    int startCol = ballCoord.col;
    int startMag = inputMatrix[startRow][startCol] - '0';
    int startDir = 0;

    stack.push_back({startRow, startCol, startMag, startDir, 1});

    while(!stack.empty())
    {
        StackData d = stack.back();
        stack.pop_back();

        int row = d.row;
        int col = d.col;
        int mag = d.mag;
        int dir = d.dir;
        int ballNum = d.ballNum;

        // Going back up recursion tree
        if(ballNum < 0)
        {
            if(outputMatrix[row][col] == 'h')
            {
                outputMatrix[row][col] = 'H';
            }

            switch(dir)
            {
                case north:
                    for(int i = 0; i <= mag; i++)
                    {
                        outputMatrix[row + 1 + i][col] = inputMatrix[row + 1 + i][col];
                    }
                    break;
                case east:
                    for(int i = 0; i <= mag; i++)
                    {
                        outputMatrix[row][col - 1 - i] = inputMatrix[row][col - 1 - i];
                    }
                    break;
                case south:
                    for(int i = 0; i <= mag; i++)
                    {
                        outputMatrix[row - 1 - i][col] = inputMatrix[row - 1 - i][col];
                    }
                    break;
                case west:
                    for(int i = 0; i <= mag; i++)
                    {
                        outputMatrix[row][col + 1 + i] = inputMatrix[row][col + 1 + i];
                    }
                    break;
                default:
                    break;
            }

            continue;
        }

        stack.push_back({row, col, mag, dir, -ballNum});

        switch(dir)
        {
            case north:
                for(int i = 0; i <= mag; i++)
                {
                    outputMatrix[row + 1 + i][col] = '^';
                }
                break;
            case east:
                for(int i = 0; i <= mag; i++)
                {
                    outputMatrix[row][col - 1 - i] = '>';
                }
                break;
            case south:
                for(int i = 0; i <= mag; i++)
                {
                    outputMatrix[row - 1 - i][col] = 'v';
                }
                break;
            case west:
                for(int i = 0; i <= mag; i++)
                {
                    outputMatrix[row][col + 1 + i] = '<';
                }
                break;
            default:
                break;
        }

        if(outputMatrix[row][col] == 'H')
        {
            outputMatrix[row][col] = 'h';

            if(ballNum == numBalls)
            {
                break;
            }
            
            ballCoord = ballCoords[ballNum];
            startRow = ballCoord.row;
            startCol = ballCoord.col;
            startMag = inputMatrix[startRow][startCol] - '0';

            stack.push_back({startRow, startCol, startMag, startDir, ballNum + 1});
            continue;
        }

        if(dir != south && row - mag >= 0)
        {
            char endGridValue = outputMatrix[row - mag][col];
            if((endGridValue == '.' && mag > 1) || endGridValue == 'H')
            {
                bool validPath = true;
                for(int i = 1; i < mag; i++)
                {
                    char gridValue = outputMatrix[row - i][col];
                    if(gridValue != '.' && gridValue != 'X')
                    {
                        validPath = false;
                        break;
                    }
                }
                
                if(validPath)
                {
                    stack.push_back({row - mag, col, mag - 1, north, ballNum});
                }
            }
        }

        if(dir != west && col + mag < width)
        {
            char endGridValue = outputMatrix[row][col + mag];
            if((endGridValue == '.' && mag > 1) || endGridValue == 'H')
            {
                bool validPath = true;
                for(int i = 1; i < mag; i++)
                {
                    char gridValue = outputMatrix[row][col + i];
                    if(gridValue != '.' && gridValue != 'X')
                    {
                        validPath = false;
                        break;
                    }
                }
                
                if(validPath)
                {
                    stack.push_back({row, col + mag,  mag - 1, east, ballNum});
                }
            }
        }

        if(dir != north && row + mag < height)
        {
            char endGridValue = outputMatrix[row + mag][col];
            if((endGridValue == '.' && mag > 1) || endGridValue == 'H')
            {
                bool validPath = true;
                for(int i = 1; i < mag; i++)
                {
                    char gridValue = outputMatrix[row + i][col];
                    if(gridValue != '.' && gridValue != 'X')
                    {
                        validPath = false;
                        break;
                    }
                }
                
                if(validPath)
                {
                    stack.push_back({row + mag, col, mag - 1, south, ballNum});
                }
            }
        }

        if(dir != east && col - mag >= 0)
        {
            char endGridValue = outputMatrix[row][col - mag];
            if((endGridValue == '.' && mag > 1) || endGridValue == 'H')
            {
                bool validPath = true;
                for(int i = 1; i < mag; i++)
                {
                    char gridValue = outputMatrix[row][col - i];
                    if(gridValue != '.' && gridValue != 'X')
                    {
                        validPath = false;
                        break;
                    }
                }
                
                if(validPath)
                {
                    stack.push_back({row, col - mag, mag - 1, west, ballNum});
                }
            }
        }
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(outputMatrix[i][j] == 'h' || outputMatrix[i][j] == 'X')
            {
                outputMatrix[i][j] = '.';
            }
        }
    }
}

void GolfCourse::printSolution()
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            cout << outputMatrix[i][j];
        }
        cout << endl;
    }
}

char **parseInput(int &height, int&width)
{
    cin >> width >> height; 
    cin.ignore();

    char **matrix = new char*[height];

    for (int i = 0; i < height; i++) 
    {
        string row;
        cin >> row; 
        cin.ignore();

        matrix[i] = new char[width];
        memcpy(matrix[i], &row[0], width*sizeof(char));
    }

    return matrix;
}

char **deepCopyMatrix(int height, int width, char **matrix)
{
    char **copy = new char*[height];
    for (int i = 0; i < height; i++) 
    {
        copy[i] = new char[width];
        memcpy(copy[i], matrix[i], width*sizeof(char));
    }

    return copy;
}

int main()
{
    int height, width;
    GolfCourse problem = GolfCourse(height, width, parseInput(height, width));
    problem.depthFirstSearch();
    problem.printSolution();
}
