#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

#define GI_DFS 0
#define GI_DFS_WITH_DEGREE 1
#define GI_HASH 2

struct Hash
{
	int num;
	std::string hash;
	Hash(std::string hash, int num)
	{
		this->hash = hash;
		this->num = num;
	}
	bool operator==(Hash &b)
	{
		return hash == b.hash;
	}
};

class graphMat
{
private:
	//矩阵
	int *degree_in;
	int *degree_out;
	int *number;
	int **mat;
	int numberOfPnt;
	//搜索方式
	int mode;
	//DFS
	int *numberResult;
	int ifIsomorphism;
	//内部操作
	void isomorphismDfs(graphMat &g2, int layer);
	void isomorphismDfsWithDegree(graphMat &g2, int layer);
	void isomorphismHash(graphMat &g2);//hash + DFS
	void isomorphismHashDfs(graphMat &g2, int layer, std::vector<Hash> &g1Hash, std::vector<Hash> &g2Hash);
public:
	//构造析构
	graphMat(int np, int mode = 0);
	graphMat(const graphMat &g2);
	graphMat();
	~graphMat();
	//操作
	int* getResult();
	void resize(int np);
	void setMode(int mode);
	void swapPointFromFollowingTable(int p1, int p2);
	void addEdge(int u, int v, int w = 1);
	void addEdgeWithoutDirection(int u, int v);
	friend std::ostream& operator<<(std::ostream& out, graphMat &g);
	graphMat& operator=(graphMat &g2);
	bool operator==(graphMat &g2);
	bool isomorphism(graphMat &g2);
};