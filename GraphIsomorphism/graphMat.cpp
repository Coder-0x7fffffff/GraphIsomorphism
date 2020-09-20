#include "stdafx.h"
#include "graphMat.h"

void graphMat::isomorphismDfs(graphMat &g2, int layer)
{
	//点数剪枝
	if (layer == g2.numberOfPnt)
		return;
	for (int i = layer; i < numberOfPnt; i++)
	{
		g2.swapPointFromFollowingTable(layer, i);
		//等号同形判断
		if (!(*this == g2))
		{
			if (!ifIsomorphism)
				isomorphismDfs(g2, layer + 1);
		}
		else
		{
			if (!ifIsomorphism)
			{
				numberResult = new int[numberOfPnt];
				memcpy_s(numberResult, sizeof(int)*numberOfPnt, g2.number, sizeof(int)*numberOfPnt);
				std::cout << *this << std::endl << g2;
			}
			ifIsomorphism = true;
		}
		g2.swapPointFromFollowingTable(i, layer);
	}
}

void graphMat::isomorphismDfsWithDegree(graphMat & g2, int layer)
{
	if (layer == g2.numberOfPnt)
		return;
	for (int i = layer; i < numberOfPnt; i++)
	{
		if (g2.degree_in[i] != degree_in[layer] || g2.degree_out[i] != degree_out[layer])
			continue;
		g2.swapPointFromFollowingTable(layer, i);
		if (!(*this == g2))
		{
			if (!ifIsomorphism)
				isomorphismDfsWithDegree(g2, layer + 1);
		}
		else
		{
			if (!ifIsomorphism)
			{
				numberResult = new int[numberOfPnt];
				memcpy_s(numberResult, sizeof(int)*numberOfPnt, g2.number, sizeof(int)*numberOfPnt);
			}
			ifIsomorphism = true;
		}
		g2.swapPointFromFollowingTable(i, layer);
	}
}

//deg_in & deg_out & last_deg_in_sum & last_deg_out_sum & next_deg_in_sum & next_deg_out_sum
void graphMat::isomorphismHash(graphMat & g2)
{
	//生产hash
	std::vector<Hash> g1Hash,g2Hash;
	//g1 hash
	std::string hash;
	char buffer[1024];
	for (int i = 0; i < numberOfPnt; i++)
	{
		//now
		hash.clear();
		_itoa_s(degree_in[i], buffer, 10);
		hash.append(buffer);
		_itoa_s(degree_out[i], buffer, 10);
		hash.append(buffer);
		//last
		int last_in_sum = 0;
		int last_out_sum = 0;
		for (int l = 0; l < numberOfPnt; l++)
		{
			if (mat[l][i])
			{
				last_in_sum += degree_in[l];
				last_out_sum += degree_out[l];
			}
		}
		_itoa_s(last_in_sum, buffer, 10);
		hash.append(buffer);
		_itoa_s(last_out_sum, buffer, 10);
		hash.append(buffer);
		//next	
		int next_in_sum = 0;
		int next_out_sum = 0;
		for (int l = 0; l < numberOfPnt; l++)
		{
			if (mat[i][l])
			{
				next_in_sum += degree_in[l];
				next_out_sum += degree_out[l];
			}
		}
		_itoa_s(next_in_sum, buffer, 10);
		hash.append(buffer);
		_itoa_s(next_out_sum, buffer, 10);
		hash.append(buffer);
		g1Hash.push_back(Hash(hash, i));
	}
	for (int i = 0; i < g2.numberOfPnt; i++)
	{
		//now
		hash.clear();
		_itoa_s(g2.degree_in[i], buffer, 10);
		hash.append(buffer);
		_itoa_s(g2.degree_out[i], buffer, 10);
		hash.append(buffer);
		//last
		int last_in_sum = 0;
		int last_out_sum = 0;
		for (int l = 0; l < g2.numberOfPnt; l++)
		{
			if (g2.mat[l][i])
			{
				last_in_sum += g2.degree_in[l];
				last_out_sum += g2.degree_out[l];
			}
		}
		_itoa_s(last_in_sum, buffer, 10);
		hash.append(buffer);
		_itoa_s(last_out_sum, buffer, 10);
		hash.append(buffer);
		//next	
		int next_in_sum = 0;
		int next_out_sum = 0;
		for (int l = 0; l < g2.numberOfPnt; l++)
		{
			if (g2.mat[i][l])
			{
				next_in_sum += g2.degree_in[l];
				next_out_sum += g2.degree_out[l];
			}
		}
		_itoa_s(next_in_sum, buffer, 10);
		hash.append(buffer);
		_itoa_s(next_out_sum, buffer, 10);
		hash.append(buffer);
		g2Hash.push_back(Hash(hash, i));
	}
	isomorphismHashDfs(g2, 0, g1Hash, g2Hash);
	return;
}

void graphMat::isomorphismHashDfs(graphMat & g2, int layer, std::vector<Hash> &g1Hash, std::vector<Hash> &g2Hash)
{
	Hash th("",0);
	if (layer == g2.numberOfPnt)
		return;
	for (int i = layer; i < numberOfPnt; i++)
	{
		if (!(g2Hash[i] == g1Hash[layer]))
			continue;
		g2.swapPointFromFollowingTable(layer, i);
		//交换hash
		th = g2Hash[layer];
		g2Hash[layer] = g2Hash[i];
		g2Hash[i] = th;
		if (!(*this == g2))
		{
			if (!ifIsomorphism)
				isomorphismHashDfs(g2, layer + 1,g1Hash,g2Hash);
		}
		else
		{
			if (!ifIsomorphism)
			{
				numberResult = new int[numberOfPnt];
				memcpy_s(numberResult, sizeof(int)*numberOfPnt, g2.number, sizeof(int)*numberOfPnt);
			}
			ifIsomorphism = true;
		}
		g2.swapPointFromFollowingTable(i, layer);
		th = g2Hash[layer];
		g2Hash[layer] = g2Hash[i];
		g2Hash[i] = th;
	}
}

graphMat::graphMat(int x, int mode)
{
	numberOfPnt = x;
	this->mode = mode;
	mat = new int*[x];
	for (int i = 0; i < x; ++i)
	{
		mat[i] = new int[x];
		memset(mat[i], 0, sizeof(int)*x);
	}
	number = new int[x];
	degree_in = new int[x];
	degree_out = new int[x];
	for (int i = 0; i < x; ++i)
	{
		number[i] = i;
		degree_in[i] = degree_out[i] = 0;
	}
}

graphMat::graphMat(const graphMat & g2)
{
	numberOfPnt = g2.numberOfPnt;
	this->mode = g2.mode;
	mat = new int*[numberOfPnt];
	for (int i = 0; i < numberOfPnt; ++i)
	{
		mat[i] = new int[numberOfPnt];
		memcpy_s(mat[i], sizeof(int)*numberOfPnt, g2.mat[i], sizeof(int)*numberOfPnt);
	}
	number = new int[g2.numberOfPnt];
	memcpy_s(number, sizeof(int)*numberOfPnt, g2.number, sizeof(int)*numberOfPnt);
	memcpy_s(degree_in, sizeof(int)*numberOfPnt, g2.degree_in, sizeof(int)*numberOfPnt);
	memcpy_s(degree_out, sizeof(int)*numberOfPnt, g2.degree_out, sizeof(int)*numberOfPnt);
}

graphMat::graphMat()
{
	numberOfPnt = 0;
	mode = 0;
	mat = nullptr;
	number = degree_in = degree_out = nullptr;
}

graphMat::~graphMat()
{
	for (int i = 0; i < numberOfPnt; ++i)
		delete mat[i];
	delete mat;
	delete number;
	delete degree_in;
	delete degree_out;
}

int * graphMat::getResult()
{
	return numberResult;
}

void graphMat::resize(int np)
{
	for (int i = 0; i < numberOfPnt; ++i)
		delete mat[i];
	delete mat;
	delete number;
	delete degree_in;
	delete degree_out;
	numberOfPnt = np;
	mat = new int*[np];
	for (int i = 0; i < np; ++i)
	{
		mat[i] = new int[np];
		memset(mat[i], 0, sizeof(int)*np);
	}
	number = new int[np];
	degree_in = new int[np];
	degree_out = new int[np];
	for (int i = 0; i < np; ++i)
	{
		number[i] = i;
		degree_in[i] = degree_out[i] = 0;
	}
}

void graphMat::setMode(int mode)
{
	this->mode = mode;
}

/* 64bit error
void graphMat::swapPointFromFollowingTable(int p1, int p2)
{
	if (p1 == p2)
		return;
	for (int i = 0; i < numberOfPnt; ++i)
	{
		mat[i][p1] = mat[i][p1] ^ mat[i][p2];
		mat[i][p2] = mat[i][p2] ^ mat[i][p1];
		mat[i][p1] = mat[i][p2] ^ mat[i][p1];
	}
	mat[p1] = (int*)((int)mat[p1] ^ (int)mat[p2]);
	mat[p2] = (int*)((int)mat[p2] ^ (int)mat[p1]);
	mat[p1] = (int*)((int)mat[p2] ^ (int)mat[p1]);
	number[p1] = number[p1] ^ number[p2];
	number[p2] = number[p2] ^ number[p1];
	number[p1] = number[p2] ^ number[p1];
	degree_in[p1] = degree_in[p1] ^ degree_in[p2];
	degree_in[p2] = degree_in[p2] ^ degree_in[p1];
	degree_in[p1] = degree_in[p2] ^ degree_in[p1];
	degree_out[p1] = degree_out[p1] ^ degree_out[p2];
	degree_out[p2] = degree_out[p2] ^ degree_out[p1];
	degree_out[p1] = degree_out[p2] ^ degree_out[p1];
}
*/
void graphMat::swapPointFromFollowingTable(int p1, int p2)
{
	int t, *tp;
	if (p1 == p2)
		return;
	for (int i = 0; i < numberOfPnt; ++i)
	{
		t = mat[i][p1];
		mat[i][p1] = mat[i][p2];
		mat[i][p2] = t;
	}
	tp = mat[p1];
	mat[p1] = mat[p2];
	mat[p2] = tp;
	t = number[p1];
	number[p1] = number[p2];
	number[p2] = t;
	t = degree_in[p1];
	degree_in[p1] = degree_in[p2];
	degree_in[p2] = t;
	t = degree_out[p1];
	degree_out[p1] = degree_out[p2];
	degree_out[p2] = t;
}

void graphMat::addEdge(int u, int v, int w)
{
	mat[u][v] = w;
	degree_in[v]++;
	degree_out[u]++;
}

void graphMat::addEdgeWithoutDirection(int u, int v)
{
	addEdge(u, v);
	addEdge(v, u);
}

std::ostream & operator<<(std::ostream & out, graphMat &g)
{
	out << std::setw(5) << " ";
	for (int i = 0; i < g.numberOfPnt; ++i)
	{
		out << std::setw(5) << g.number[i] + 1;
	}
	out << std::endl;
	for (int i = 0; i < g.numberOfPnt; ++i)
	{
		out << std::setw(5) << g.number[i] + 1;
		for (int j = 0; j < g.numberOfPnt; ++j)
		{
			out << std::setw(5) << g.mat[i][j];
		}
		out << std::endl;
	}
	return out;
}

graphMat & graphMat::operator=(graphMat & g2)
{
	for (int i = 0; i < numberOfPnt; ++i)
		delete mat[i];
	delete mat;
	numberOfPnt = g2.numberOfPnt;
	mat = new int*[numberOfPnt];
	for (int i = 0; i < numberOfPnt; ++i)
	{
		mat[i] = new int[numberOfPnt];
		memcpy_s(mat[i], sizeof(int)*numberOfPnt, g2.mat[i], sizeof(i)*numberOfPnt);
	}
	number = new int[g2.numberOfPnt];
	memcpy_s(number, sizeof(int)*numberOfPnt, g2.number, sizeof(int)*numberOfPnt);
	return *this;
}

bool graphMat::operator==(graphMat & g2)
{
	int isSame = true;
	if (numberOfPnt != g2.numberOfPnt)
		return false;
	for (int i = 0; i < numberOfPnt; i++)
	{
		if (memcmp(mat[i], g2.mat[i], sizeof(int)*numberOfPnt) != 0)
		{
			isSame = false;
			break;
		}
	}
	return isSame;
}

bool graphMat::isomorphism(graphMat &g2)
{
	numberResult = nullptr;
	ifIsomorphism = false;
	//判断图的度构成是否相同
	bool ifSubDegreeSame;
	int *degreeCount = new int[numberOfPnt];
	int *degreeCount2 = new int[numberOfPnt];
	ifSubDegreeSame = true;
	memcpy_s(degreeCount, sizeof(int)*numberOfPnt, degree_in, sizeof(int)*numberOfPnt);
	memcpy_s(degreeCount2, sizeof(int)*numberOfPnt, g2.degree_in, sizeof(int)*numberOfPnt);
	std::sort(degreeCount, degreeCount + numberOfPnt);
	std::sort(degreeCount2, degreeCount2 + numberOfPnt);
	if (memcmp(degreeCount, degreeCount2, sizeof(int)*numberOfPnt) != 0)
		ifSubDegreeSame = false;
	memcpy_s(degreeCount, sizeof(int)*numberOfPnt, degree_out, sizeof(int)*numberOfPnt);
	memcpy_s(degreeCount2, sizeof(int)*numberOfPnt, g2.degree_out, sizeof(int)*numberOfPnt);
	std::sort(degreeCount, degreeCount + numberOfPnt);
	std::sort(degreeCount2, degreeCount2 + numberOfPnt);
	if (memcmp(degreeCount, degreeCount2, sizeof(int)*numberOfPnt) != 0)
		ifSubDegreeSame = false;
	delete degreeCount;
	delete degreeCount2;
	if (!ifSubDegreeSame)
		return false;
	//判断
	switch (mode)
	{
	case GI_DFS:
		isomorphismDfs(g2, 0);
		break;
	case GI_DFS_WITH_DEGREE:
		isomorphismDfsWithDegree(g2, 0);
		break;
	case GI_HASH:
		isomorphismHash(g2);
		break;
	}
	return ifIsomorphism;
}
