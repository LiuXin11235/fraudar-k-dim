#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;

unordered_map<string, int> g[10];
int e[4000000][10];
double c[4000000];
bool erased[4000000];
vector<int> a[2000000];
int n, m, k;
int timeStamp[2000000];
int p[2000000]; //which pos
double w[2000000];
double numerator;
double denominator;
const double d[10] = {0, 0, 1, 1, 0, 1, 0, 0, 0, 0};
//const double d[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
double eps = 1e-3;
double lambda = 0.8;

int G(int j, string s) {
	if (g[j].find(s) == g[j].end()) {
		return g[j][s] = n++;
	} else {
		return g[j][s];
	}
}

//struct compare {
//	bool operator() (const int& lhs, const int& rhs) const {
//		if (w[lhs] * d[p[rhs]] == w[rhs] * d[p[lhs]]) {
//			return lhs < rhs;
//		} else {
//			return w[lhs] * d[p[rhs]] < w[rhs] * d[p[lhs]];
//		}
//	}
//};

struct compare {
	bool operator() (const pair<double, int> & lhs, const pair<double, int>& rhs) const {
		if (d[p[lhs.Y]] == 0 || d[p[rhs.Y]] == 0) {
			if (d[p[lhs.Y]] == 0 && d[p[rhs.Y]] == 0) {
				return lhs.Y < rhs.Y;
			}
			if (d[p[lhs.Y]] == 0) {
				return false; // lhs < rhs is false
			}
			if (d[p[rhs.Y]] == 0) {
				return true; // lhs < rhs is true;
			}
			assert(false);
		}
		if (lhs.X / d[p[lhs.Y]] == rhs.X / d[p[rhs.Y]]) {
			return lhs.Y < rhs.Y;
		}
		return lhs.X / d[p[lhs.Y]] < rhs.X / d[p[rhs.Y]];
	}
};

//set<int, compare> tree;
set<pair<double, int>, compare> tree;

void insert(int u) {
	if (d[p[u]] == 0) {
		return;
	}
	tree.insert(make_pair(w[u], u));
}

void erase(int u) {
	if (d[p[u]] == 0) {
		return;
	}
	assert(tree.erase(make_pair(w[u], u)) == 1);
}

void removeAny(int u, int currentTime) {
	for (int i = 0; i < (int)a[u].size(); i++) {
		bool flag = false;
		for (int j = 0; j < k; j++) {
			if (timeStamp[e[a[u][i]][j]] > 0) {
				flag = true;
				break;
			}
		}
		if (flag) {
			continue;
		}
		for (int j = 0; j < k; j++) {
			if (d[j] == 0) {
				continue;
			}
			erase(e[a[u][i]][j]);
			w[e[a[u][i]][j]] -= c[a[u][i]];
			insert(e[a[u][i]][j]);
		}
		numerator -= c[a[u][i]];
	}
	assert(abs(w[u]) < eps);
	denominator -= d[p[u]];
	erase(u);
	timeStamp[u] = currentTime;
}

void removeFirst(int currentTime) {
	removeAny(tree.begin()->second, currentTime);
}


int rounds;
set<int> solution[100];
set<int> solutions;

void read() {
//720208 3085471 3
//1214740 3085471 10
	n = 1214740;//点数
	m = 3085471;//边数
	k = 10;//事件维度，一行多少列
	//不同列的空格填充不同的数，每一个数只出现在一列中
	//eij 存储边, p存储点出现的列号
	//d是点的权值，每个列的权值一样，10个权值

//	scanf("%d%d%d", &n, &m, &k);
	memset(p, -1, sizeof p);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < k; j++) {
			int x;
			scanf("%d", &x);
			assert(0 <= x && x < n);
			assert(p[x] == -1 || p[x] == j);
			p[x] = j;
			e[i][j] = x;
			if (d[j] == 0) {
				continue;
			}
			a[x].push_back(i);//a存这个点有哪些邻接的边，一条记录是一个边
		}
		c[i] = 1;//边的权重
	}
}

void readstrings() {
	string line;
	k = 10;
	while (getline(cin, line)) {
		vector<string> a;
		string t;
		int last = 0;
//		for (int i = 0; i < line.size(); i++) {
//			if (line[i] == ',') {
//				a.push_back(t);
//				t = "";
//			} else {
//				t += line[i];
//			}
//		}
//		a.push_back(t);
		for (int i = 0; i < (int)line.size(); i++) {
			if (line[i] == ',') {
				a.push_back(line.substr(last, i - last));
				last = i + 1;
			} 
		}
		a.push_back(line.substr(last, line.size() - last));
		assert((int)a.size() == k);
		for (int j = 0; j < k; j++) {
			if (j == 2) {
				a[j] = to_string(stoi(a[j]) / 600);
			}
			a[j] += (char)('A' + j);
			e[m][j] = G(j, a[j]);
		}
//		for (string i: a) {
//			cout << i << endl;
//		}
		m++;
		if (m % 100000 == 0) {
			printf("%d %d %d\n", n, m, k);
		}
	}
	printf("%d %d %d\n", n, m, k);
}
bool checkDeleted(int i, int r = -1) {
	if (r == -1) {//这个边的所有点都被删除过
		for (int j = 0; j < k; j++) {
			if (solutions.find(e[i][j]) == solutions.end()) {
				return false;
			}
		}
		return true;
	} else {//这个边的所有点恰好在第r轮被删
		for (int j = 0; j < k; j++) {
			if (solution[r].find(e[i][j]) == solution[r].end()) {//solution每一轮被删除的点
				return false;
			}
		}
		return true;
	}
}

void updateWeight(int r) {
	for (int i = 0; i < m; i++) {
		if (checkDeleted(i, r)) {
			c[i] *= lambda;
		}
	}
}

void construct() {
	if (rounds > 0) {
		updateWeight(rounds - 1);
	}
	numerator = 0;//所有的边权的和
	for (int i = 0; i < m; i++) {
		numerator += c[i];
		for (int j = 0; j < k; j++) {
			w[e[i][j]] += c[i];//W是记录与某个点相邻的边的边权
		}
	}
	tree.clear();//快速找最小值
	for (int i = 0; i < n; i++) {//枚举每个点，加入到tree里
		insert(i);
		denominator += d[p[i]];//算分母，所有点的点权之和
		timeStamp[i] = 0;//点：没有被删除是0，如果删除了记录是第几轮被删除的
	}
}

//int whichColumn(int x) {
//	for (int i = 0; i < k; i++) {
//		if (e[a[x][0]][i] == x) {
//			return i;
//		}
//	}
//	assert(false);
//}
int countDeleted() {
	int cnt = 0;
	for (int i = 0; i < m; i++) {
		if (c[i] < 1) {
			cnt++;
		}
	}
	return cnt;
}
bool solve() {
	fprintf(stderr, "rounds: %d\n", rounds + 1);
	double startNumerator = numerator;//没有用
	double bestFraction = 0;
	double bestNumerator = -1;
	double bestDenominator = -1;
	int bestTime = -1;//记录哪一轮是最优的
	fprintf(stderr, "Deleted Edges %d\n", countDeleted());//枚举所有的边，看边权是否小于1，如果小于1就被删除过
	for (int i = 0; i < n; i++) {
		if (numerator < eps && denominator < eps) {//说明删除完了
			break;
		}
		if (denominator < eps) {//只剩下分子，说明程序错了  分子是边数的权值之和，分母是点数的权值之和
			assert(false);
		}
		double fration = numerator / denominator;
		if (bestFraction < fration) {
			bestFraction = fration;
			bestNumerator = numerator;
			bestDenominator = denominator;
			bestTime = i;
		}
		removeFirst(i + 1);//从tree删除最小的点权
	}
//	printf("%d\n", (int)tree.size());
//	assert(tree.size() == 0);
	if (!(abs(numerator) < eps)) {
		cout << abs(numerator) << endl;
	}
	assert(abs(numerator) < eps);
	assert(abs(denominator) < eps);
	fprintf(stderr, "bestFraction: %.6f\n", bestFraction);
	fprintf(stderr, "bestNumerator: %.6f\n", bestNumerator);
	fprintf(stderr, "bestDenominator: %.6f\n", bestDenominator);
//	fprintf(stderr, "bestTime: %d\n", bestTime);
//	fprintf(stderr, "TheNumberOfLeftNodes: %d\n", n - bestTime);
	if (rounds == 21) {
		return false;
	}
	for (int i = 0; i < n; i++) {
		if (timeStamp[i] > bestTime || timeStamp[i] == 0) {
			solution[rounds].insert(i);
			solutions.insert(i);
		}
	}
	rounds++;
	return true;
}
int main() {
//	readstrings();
//	return 0;
	read();
	fprintf(stderr, "Configration:\n");
	fprintf(stderr, "eps\t%.6f\n", eps);//判断为0
	fprintf(stderr, "lambda\t%.6f\n", lambda);//边的衰减系数，一开始权值都是1
	fprintf(stderr, "n\t%d\n", n);
	fprintf(stderr, "m\t%d\n", m);
	fprintf(stderr, "k\t%d\n", k);
	fprintf(stderr, "d\t");
	for (int i = 0; i < k; i++) {
		fprintf(stderr, "%.2f\n", d[i]);
	}
	while (true) {
		construct();//准备性的工作
		bool flag = solve();//求解
		if (!flag) {//返回FALSE，说明返回的边数超过阈值，大概10万，说明不是虚假
			break;
		}
	}
	construct();
	set<int> users[10];
	for (int i = 0; i < m; i++) {
		if (c[i] < 1) {
			for (int j = 0; j < k; j++) {
				users[j].insert(e[i][j]);//第j列被删的点有哪些
			}
		}
	}
	for (int j = 0; j < k; j++) {
		fprintf(stderr, "Different users[%d]: %d\n", j, (int)users[j].size());//输出第j列有多少可疑的点
	}

/*
	FILE *userA = fopen("userA.txt", "w");
	for (int x: users[1]) {
		fprintf(userA, "%d\n", x);
	}
	fclose(userA);

	FILE *userF = fopen("userF.txt", "w");
	for (int x: users[5]) {
		fprintf(userF, "%d\n", x);
	}
	fclose(userF);
*/
//	for (int i = 0; i < rounds; i++) {
//		for (int j: solution[i]) {
//			printf("%d\n", j);
//		}
//		printf("\n\n");
//	}
	return 0;
}