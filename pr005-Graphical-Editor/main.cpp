//
//  main.cpp
//  pr005-Graphical-Editor
//
//  Created by Salmon on 2014. 10. 2..
//  Copyright (c) 2014년 Salmon. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>

#define MAXSIZE 250

class GraphicalEditor {
protected:
    int MaxSize;
    int x_max,y_max;
    char **image;
    void FindAndFillRegion(int,int,char,char);
public:
    GraphicalEditor(int);
    ~GraphicalEditor();
    void NewImage(int,int);
    void Clear();
    void ColorPixel(int,int,char);
    void DrawVerticalLine(int,int,int,char);
    void DrawHorizontalLine(int,int,int,char);
    void DrawRectangle(int,int,int,int,char);
    void Fill(int,int,char);
    void Save(std::string*);
};
GraphicalEditor::GraphicalEditor(int max_) {
    MaxSize = max_;
    image = new char*[MaxSize];
    for(int i=0; i<MaxSize; i++)
        image[i] = new char[MaxSize];
}
GraphicalEditor::~GraphicalEditor() {
    for(int i=0; i<MaxSize; i++)
        delete [] image[i];
    delete [] image;
}
void GraphicalEditor::NewImage(int x, int y) {
    x_max = x;
    y_max = y;
    Clear();
}
void GraphicalEditor::Clear() {
    for(int i=0; i<x_max; i++)
        for(int j=0; j<y_max; j++)
            image[i][j] = 'O';
}
void GraphicalEditor::ColorPixel(int x, int y, char color) {
    image[x][y] = color;
}
void GraphicalEditor::DrawVerticalLine(int x, int yStart, int yEnd, char color) {
    for(int y=yStart; y<=yEnd; y++)
        ColorPixel(x,y,color);
}
void GraphicalEditor::DrawHorizontalLine(int xStart, int xEnd, int y, char color) {
    for(int x=xStart; x<=xEnd; x++)
        ColorPixel(x,y,color);
}
void GraphicalEditor::DrawRectangle(int xStart, int yStart, int xEnd, int yEnd, char color) {
    for(int x=xStart; x<=xEnd; x++) {
        for(int y=yStart; y<=yEnd; y++) {
            ColorPixel(x,y,color);
        }
    }
}
void GraphicalEditor::Fill(int startX, int startY, char color) {
    FindAndFillRegion(startX,startY,image[startX][startY],color);
}
void GraphicalEditor::FindAndFillRegion(int x, int y, char regionColor, char fillColor) {
    image[x][y] = fillColor;
    if(x>0 && image[x-1][y] == regionColor)
        FindAndFillRegion(x-1,y,regionColor,fillColor);
    if(x<x_max && image[x+1][y] == regionColor)
        FindAndFillRegion(x+1,y,regionColor,fillColor);
    if(y>0 && image[x][y-1] == regionColor)
        FindAndFillRegion(x,y-1,regionColor,fillColor);
    if(y<y_max && image[x][y+1] == regionColor)
        FindAndFillRegion(x,y+1,regionColor,fillColor);
}
void GraphicalEditor::Save(std::string* nameAddress) {
    std::string name_ = nameAddress->c_str();
    std::cout << name_ << std::endl;
    for(int y=0; y<y_max; y++) {
        for(int x=0; x<x_max; x++) {
            std::cout << image[x][y];
        }
        std::cout << std::endl;
    }
}
int main()
{
    GraphicalEditor ge(MAXSIZE);
    bool bQuit = false;
    char command = 0, color;
    int x1, x2, y1, y2;
    std::string input_string,name;
    std::stringstream ss;
    while(!bQuit) {
        std::getline(std::cin,input_string,'\n');
        ss.clear();
        ss.str(std::string());
        ss << input_string;
        ss >> command;
        switch(command) {
            case 'I':
                ss >> x1 >> y1;
                ge.NewImage(x1,y1);
                break;
            case 'C':
                ge.Clear();
                break;
            case 'L':
                ss >> x1 >> y1 >> color;
                ge.ColorPixel(x1-1,y1-1,color);
                break;
            case 'V':
                ss >> x1 >> y1 >> y2 >> color;
                ge.DrawVerticalLine(x1-1,y1-1,y2-1,color);
                break;
            case 'H':
                ss >> x1 >> x2 >> y1 >> color;
                ge.DrawHorizontalLine(x1-1,x2-1,y1-1,color);
                break;
            case 'K':
                ss >> x1 >> x2 >> y1 >> y2 >> color;
                ge.DrawRectangle(x1-1,x2-1,y1-1,y2-1,color);
                break;
            case 'F':
                ss >> x1 >> y1 >> color;
                ge.Fill(x1-1,y1-1,color);
                break;
            case 'S':
                ss >> name;
                ge.Save(&name);
                break;
            case 'X':
                bQuit = true;
                break;
            default:
                break;
        }
    }
    return 0;
}