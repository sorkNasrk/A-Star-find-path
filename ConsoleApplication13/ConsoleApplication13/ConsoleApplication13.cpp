// ConsoleApplication13.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <math.h>
#include <algorithm>
#include <memory>
#include <iostream>

using namespace std;

struct Node {
	public:
		Node(pair<int, int> beginPoint) :point(beginPoint){

		}
		pair<int, int> point;
		int F = 0;
		int G = 0;
		int H = 0;
		Node * parent = nullptr;
};


int getH(pair<int, int> point, pair<int, int> endPoint) {
	return abs(point.first - endPoint.first) +
		abs(point.second - endPoint.second);
}

int getG(Node &node) {
	if (node.parent) {
		int parentG = (*(node.parent)).G;
		if (abs((*(node.parent)).point.first - node.point.first) == 0 ||
			abs((*(node.parent)).point.second - node.point.second) == 0) {
			return parentG + 10;

		}
		else {
			return parentG + 14;
		}

	}
	else
	{
		return 0;
	}
}

bool isAccessible(pair<int, int>  beginPoint, pair<int, int> endPoint, vector<vector<int>> &mapData) {
	if (endPoint.first < 0 || endPoint.second < 0 || endPoint.first >= (int)(mapData.size()) || endPoint.second >= (int)(mapData[1].size())) {
		return false;
	}
	else if (mapData[endPoint.first][endPoint.second] == 1) {
		return false;
	}
	else if (beginPoint.first != endPoint.first && beginPoint.second != endPoint.second) {
		if (mapData[beginPoint.first][endPoint.second] == 0 && mapData[endPoint.first][beginPoint.second] == 0){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return true;
	}

}
vector<pair<int, int>> findPath(pair<int, int>  beginPoint, pair<int ,int> endPoint, vector<vector<int>> &mapData) {
	auto  beginNode = make_shared<Node>(beginPoint), endNode = make_shared<Node>(endPoint);
	vector<shared_ptr<Node>> open;
	vector<shared_ptr<Node>> close;
	vector<pair<int, int>> neighbourArray = {
		{1,0},
		{1,1},
		{0,1},
		{-1,1},
		{-1,0},
		{-1,-1},
		{0,-1},
		{1,-1}
	};

	int neighbourSize = neighbourArray.size();
	
	beginNode->H = getH(beginPoint, endPoint);
	beginNode->G = getG(*beginNode);
	beginNode->F = beginNode->G + beginNode->H;
	open.push_back(beginNode);
	bool isFind = false;
	while (!(open.empty() || isFind))
	{
		sort(open.begin(), open.end(), [](shared_ptr<Node> &a, shared_ptr<Node> &b)->bool {
			return a->F > b->F;
		});
		shared_ptr<Node> minFNode = open.back();
		open.pop_back();
		close.push_back(minFNode);
		for (auto neighbour : neighbourArray) {
			auto neighbourNode = make_shared<Node>(make_pair(minFNode->point.first + neighbour.first, minFNode->point.second + neighbour.second ));
			if (isAccessible(minFNode->point, neighbourNode->point, mapData)
				&& find_if(close.begin(), close.end(), [&](shared_ptr<Node> & node) {
														return node->point.first == neighbourNode->point.first && 
															node->point.second == neighbourNode->point.second; 
														}
				) == close.end())
			{
				auto iterator =  find_if(open.begin(), open.end(),
					[&](shared_ptr<Node> & node) {
					return node->point.first == neighbourNode->point.first &&
						node->point.second == neighbourNode->point.second;
				});
				if (iterator== open.end()) {

					neighbourNode->parent = minFNode.get();
					neighbourNode->G = getG(*neighbourNode);
					neighbourNode->H = getH(neighbourNode->point, endPoint);
					neighbourNode->F = neighbourNode->G + neighbourNode->H;
					open.push_back(neighbourNode);

					if (neighbourNode->point == endPoint) {
						isFind = true;
						break;
					}
				}
				else {
					int G = 0;
					
					if (abs((*iterator)->point.first - minFNode->point.first) == 0 ||
						abs((*iterator)->point.second - minFNode->point.second) == 0) {
						G = minFNode->G + 10;

					}
					else {
						G = minFNode->G + 14;
					}

					if (G < (*iterator)->G) {
						(*iterator)->parent = minFNode.get();
						(*iterator)->G = getG(*(*iterator));
						(*iterator)->F = (*iterator)->G + (*iterator)->H;
					}
				}
			}
		}
	}
	vector<pair<int, int>> path;
	if (isFind)
	{
		auto end = open.back().get();
		 
		while (end)
		{
			path.push_back(end->point);
			end = end->parent;
		}
	}
	return path;
}



int main()
{
	vector<vector<int>> mapData =
	{ 
		{ 0,0,0,0,1,0,0,0,0,0 },
		{ 0,0,0,0,1,0,0,0,0,0 },
		{ 1,1,1,1,1,0,0,0,0,0 },
		{ 0,0,0,0,1,0,0,0,0,0 },
		{ 0,0,0,0,1,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0 },
	};

	vector<pair<int, int>> path = findPath({ 0,2 }, { 0,2 }, mapData);

	for (auto i : path) {
		cout << "(" << i.first << "," << i.second << ")" << endl;
		mapData[i.first][i.second] = 5;
	}

	for (auto i : mapData) {
		for (auto j : i) {
			cout << j << ",";
		}
		cout << endl;
	}
	
	system("pause");
	
    return 0;
}

