#include <vector>
using namespace std;

class Snake {
public:
	Snake(vector<int>, vector<int>);
	void setPos(vector<int>);
	vector<int> getPos();
	void setDir(vector<int>);
	vector<int> getDir();
	void move();
private:
	vector<int> pos;
	vector<int> dir;
};

