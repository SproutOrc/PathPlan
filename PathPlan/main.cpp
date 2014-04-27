//
//  main.cpp
//  PathPlan
//
//  Created by Atropos on 14-4-23.
//  Copyright (c) 2014年 Atropos. All rights reserved.
//


#include <iostream>
#include <vector>
#include <list>

using namespace std;


#define XMAX 6
#define YMAX 6

class Point{
public:
    int x;
    int y;
    
    enum direction{
        xNegative,
        xPositive,
        yNegative,
        yPositive,
        noDir
    };
    
    direction dir;
    Point(const Point& setPoint) {
        x = setPoint.x;
        y = setPoint.y;
        dir = setPoint.dir;
    }
    
    Point(int inX, int inY) {
        x = inX;
        y = inY;
    }
    
    Point() {
        x = 0;
        y = 0;
    }
    
    bool isEqual (const Point& thisPoint) const{
        if (this->x == thisPoint.x && this->y == thisPoint.y) {
            return true;
        }
        return false;
    }
    
    inline void setToNil () {
        x = -1;
        y = -1;
        dir = noDir;
    }
    
    inline bool isNil() const{
        if (x == -1 && y == -1) {
            return true;
        }
        return false;
    }
    
    void setPoint (int inX, int inY, direction inDir) {
        x = inX;
        y = inY;
        dir = inDir;
    }
    
    void setPoint (const Point& thisPoint) {
        x = thisPoint.x;
        y = thisPoint.y;
        dir = thisPoint.dir;
    }
};


class Node {
public:
    Node (Point nodePoint) {
        coord.setPoint(nodePoint.x, nodePoint.y, nodePoint.dir);
        pathNode.push_back(nodePoint);
        nextPathNum = 4;
        rootNode = NULL;
        nextNode = NULL;
        allNode[0].setToNil();
        allNode[1].setToNil();
        allNode[2].setToNil();
        allNode[3].setToNil();
    }
    
    ~Node () {
        //delete nextNode;
    }
    // 生成下一个节点
    bool create () {
        //deleteNextNode();
        nextNode->~Node();
        if (4 > nextPathNum) {
            allNode[nextPathNum].setToNil();
        }
        
        int i;
        int min = 4;
        for (i = 0; i < 4; ++i) {
            if ((min > nextNodePriority[i]) && (!allNode[i].isNil())) {
                min = nextNodePriority[i];
                nextPathNum = i;
            }
        }
        //nextPathNum = i;
        if (4 == min) {
            return false;
        } else {
            nextNode = new Node(allNode[nextPathNum]);
            nextNode->rootNode = this;
            return true;
        }
    }
    
    // 释放下一个节点
    void deleteNextNode () {
        delete nextNode;
        nextNode = NULL;
    }
    
    void getAllRoundNode (const int& num, const Point obstacle[]) {
        Point nextPoint(0, 0);
        nextPathNum = 4;
        
        // y轴负方向
        nextPoint.x = coord.x;
        nextPoint.y = coord.y - 1;
        if (canToNextNode(nextPoint, num, obstacle)) {
            allNode[0].setPoint(coord.x, coord.y - 2, Point::yPositive);
            if (isRootNode(allNode[0]) || isInPathNode(allNode[0])) {
                allNode[0].setToNil();
            }
        } else {
            allNode[0].setToNil();
        }
        
        // x轴正方向
        nextPoint.x = coord.x + 1;
        nextPoint.y = coord.y;
        if (canToNextNode(nextPoint, num, obstacle)) {
            allNode[1].setPoint(coord.x + 2, coord.y, Point::xNegative);
            if (isRootNode(allNode[1]) || isInPathNode(allNode[1])) {
                allNode[1].setToNil();
            }
        } else {
            allNode[1].setToNil();
        }

        // y轴正方向
        nextPoint.x = coord.x;
        nextPoint.y = coord.y + 1;
        if (canToNextNode(nextPoint, num, obstacle)) {
            allNode[2].setPoint(coord.x, coord.y + 2, Point::yNegative);
            if (isRootNode(allNode[2]) || isInPathNode(allNode[2])) {
                allNode[2].setToNil();
            }
        } else {
            allNode[2].setToNil();
        }

        // x轴负方向
        nextPoint.x = coord.x - 1;
        nextPoint.y = coord.y;
        if (canToNextNode(nextPoint, num, obstacle)) {
            allNode[3].setPoint(coord.x - 2, coord.y, Point::xPositive);
            if (isRootNode(allNode[3]) || isInPathNode(allNode[3])) {
                allNode[3].setToNil();
            }
        } else {
            allNode[3].setToNil();
        }
        if (!haveRootNode) {
            haveRootNode = 1;
            switch (coord.dir) {
                case Point::xNegative:
                    allNode[3].setToNil();
                    break;
                    
                case Point::xPositive:
                    allNode[1].setToNil();
                    break;
                    
                case Point::yNegative:
                    allNode[0].setToNil();
                    break;
                    
                case Point::yPositive:
                    allNode[2].setToNil();
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // 存储所有经过的节点
    static std::vector<Point> pathNode;
    // 当前节点坐标
    Point coord;
    // 父节点
    Node *rootNode;
    // 下一个节点
    Node *nextNode;
    
private:
    // 上一个路径的序号
    int nextPathNum;
    
    // 所有的节点
    
    Point allNode[4];
    
    // 设置优先级
    int nextNodePriority[4];
    
    //
    static bool haveRootNode;
    
    // 判断两个节点之间是否有障碍物和节点是否出了边界
    inline bool canToNextNode (Point& pathPoint, const int& num, const Point obstacle[]) const{
        int i;
        for (i = 0; i < num; ++i) {
            if ((pathPoint.x == obstacle[i].x && pathPoint.y == obstacle[i].y)
                || pathPoint.x < 0 || pathPoint.x > 6
                || pathPoint.y < 0 || pathPoint.y > 6) {
                return false;
            }
        }
        return true;
    }
    
    //  判断下一个节点是否是父节点
    inline bool isRootNode (const Point& thisNode) const{
        if (this->rootNode != NULL) {
            if (this->rootNode->coord.isEqual(thisNode)){
                return true;
            }
        }
        return false;
    }
    
    // 判断节点是否在pathNode中
    
    bool isInPathNode (const Point& thisPoint) const{
        for (std::vector<Point>::iterator iter = pathNode.begin() ; iter != pathNode.end() ; iter++ )
        {
            if (iter->isEqual(thisPoint)) {
                return true;
            }
        }
        return false;
    }
    
public:
    void setNextNodePriority (const Point& targetPoint) {
        Point::direction xDir;
        Point::direction yDir;
        
        if (targetPoint.x > coord.x) {
            xDir = Point::xNegative;
        } else if (targetPoint.x < coord.x) {
            xDir = Point::xPositive;
        } else {
            xDir = Point::noDir;
        }
        
        if (targetPoint.y > coord.y) {
            yDir = Point::yNegative;
        } else if (targetPoint.y < coord.y) {
            yDir = Point::yPositive;
        } else {
            yDir = Point::noDir;
        }
        
        for (int i = 0; i < 4; ++i) {
            if (!allNode[i].isNil()) {
                if ((xDir == coord.dir || yDir == coord.dir) && (xDir == allNode[i].dir || yDir == allNode[i].dir)) {
                    nextNodePriority[i] = 0;
                } else if (xDir == allNode[i].dir || yDir == allNode[i].dir) {
                    nextNodePriority[i] = 1;
                } else if ((allNode[i].dir == coord.dir) || (allNode[i].dir == coord.dir)) {
                    nextNodePriority[i] = 2;
                } else {
                    nextNodePriority[i] = 3;
                }
            } else {
                nextNodePriority[i] = 4;
            }
        }
    }
};

std::vector<Point> Node::pathNode;
bool Node::haveRootNode = 0;

bool notToNext(const Point& thisPoint, const Point& nextPoint) {
    if (nextPoint.x > thisPoint.x && thisPoint.dir == Point::xPositive) {
        return true;
    }
    
    if (nextPoint.x < thisPoint.x && thisPoint.dir == Point::xNegative) {
        return true;
    }
    
    if (nextPoint.y > thisPoint.y && thisPoint.dir == Point::yPositive) {
        return true;
    }
    
    if (nextPoint.y < thisPoint.y && thisPoint.dir == Point::yNegative) {
        return true;
    }

    return false;
}

bool pathPlan (Point& nextPoint,
               const Point& originNode,
               const Point& targetNode,
               const int& obstacleNum,
               const Point obstacle[]) {
    
    if (originNode.isEqual(targetNode)) {
        return false;
    }
    Point pathPoint[16];
    int num = 0;
    Node fatherNode(originNode);
    fatherNode.getAllRoundNode(obstacleNum, obstacle);
    fatherNode.setNextNodePriority(targetNode);
    Node *next;
    next = &fatherNode;
    while (true) {
        bool canCreate = next->create();
        if (!canCreate) {
            if (next->rootNode == NULL) {
                return false;
            } else {
                next = next->rootNode;
                next->nextNode->~Node();
                
            }
        } else {
            next = next->nextNode;
            next->getAllRoundNode(obstacleNum, obstacle);
            next->setNextNodePriority(targetNode);
        }
        
        if (next->coord.isEqual(targetNode)) {
            next = &fatherNode;
            for (int i = 0;; ++i) {
                pathPoint[i].setPoint(next->coord);
                if (next->nextNode == NULL) {
                    num = i + 1;
                    break;
                } else {
                    next = next->nextNode;
                }
            }
            if (num > 3) {
                for (int i = 0; i < num - 3; ++i) {
                    for (int j = i + 3; j < num; ++j) {
                        if (((pathPoint[i].x + 2 == pathPoint[j].x) && (pathPoint[i].y == pathPoint[j].y))
                            || ((pathPoint[i].x - 2 == pathPoint[j].x) && (pathPoint[i].y == pathPoint[j].y))
                            || ((pathPoint[i].y + 2 == pathPoint[j].y) && (pathPoint[i].x == pathPoint[j].x))
                            || ((pathPoint[i].y - 2 == pathPoint[j].y) && (pathPoint[i].x == pathPoint[j].x))) {
                            int a;
                            for (a = 0; a < obstacleNum; a++) {
                                if (((pathPoint[i].x + pathPoint[j].x) / 2 == obstacle[a].x
                                    && (pathPoint[i].y + pathPoint[j].y) / 2 == obstacle[a].y)
                                    || notToNext(pathPoint[i], pathPoint[j])
                                ) {
                                    break;
                                }
                            }
                            
                            if (a == obstacleNum) {
                                for (int z = 0; z + j < num; ++z) {
                                    pathPoint[z + 1 + i].setPoint(pathPoint[j + z]);
                                }
                                num = num - (j - i) + 1;
                            }
                            
                        }
                    }
                }
            }
            for (int i = 0; i < num; ++i) {
                cout << i << " (" << pathPoint[i].x << ", " << pathPoint[i].y << ")" << endl;
            }
            nextPoint.x = int((pathPoint[0].x + pathPoint[1].x) / 2);
            nextPoint.y = int((pathPoint[0].y + pathPoint[1].y) / 2);
            
            break;
        }
    }
    
    return true;
}

int main(int argc, const char * argv[])
{
    Point nextPoint;
    Point originNode(6, 0);
    originNode.dir = Point::xNegative;
    Point targetNode(6, 6);
    int obstacleNum = 2;
    Point obstacle[] = {
        Point(6, 1),
        Point(4, 1)
    };
    while (true) {
        cout << "Origin" << endl;
        std::cout << "x = ";
        std::cin >> originNode.x;
        std::cout << "y = ";
        std::cin >> originNode.y;
        std::cout << std::endl << endl;
        
        cout << "target" << endl;
        cout << "x = ";
        cin >> targetNode.x;
        cout << "y = ";
        cin >> targetNode.y;
        cout << endl << endl;
        
        cout << "obstacle" << endl;
        cout << "x = ";
        cin >> obstacle[0].x;
        cout << "y = ";
        cin >> obstacle[0].y;
        cout << endl;
        cout << "x = ";
        cin >> obstacle[1].x;
        cout << "y = ";
        cin >> obstacle[1].y;
        cout << endl << endl;
     
        bool canPath = pathPlan(nextPoint, originNode, targetNode, obstacleNum, obstacle);
        if (canPath) {
            std::cout << "NEXTPOINT (" << nextPoint.x << ", " << nextPoint.y << ")" << std::endl;
        } else {
            std::cout << "pathPlan fail" << std::endl;
        }
 
    }
    return 0;
}