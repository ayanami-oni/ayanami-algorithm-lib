# 广度优先搜索（BFS）

## 简介

**广度优先搜索（Breadth-First Search, BFS）是一种用于遍历或搜索树或图数据结构的算法。该算法从起始节点开始,首先访问所有相邻节点（第一层）,然后再访问这些相邻节点的相邻节点（第二层）,以此类推,逐层向外扩展,直到所有可达节点都被访问。**

### **BFS的核心特性**:
- 保证按层序访问节点
- 在无权图中,BFS可以找到从起点到其他所有节点的最短路径
- 适用于需要逐层处理或寻找最短路径的场景

## 核心思想

### 波纹扩散模型

想象你往平静的池塘中扔下一块石头,波纹会从落点开始,一圈一圈向外扩散。BFS的遍历逻辑与这个物理现象完全一致:

```
      起始节点
    /    |    \      ← 第1层:距离起点1步的所有节点
   /     |     \
  /      |      \
 /       |       \
●--------●--------●  ← 第2层:距离起点2步的所有节点
```

**BFS的遍历顺序**:
1. 访问起始节点(距离为0)
2. 访问所有距离为1的节点
3. 访问所有距离为2的节点
4. 以此类推,直到所有可达节点都被访问

### 为什么需要队列?

BFS使用队列(Queue)作为核心数据结构,因为队列的**先进先出(FIFO)**特性完美契合BFS的层序遍历需求:

```
队列操作示例:
入队顺序: A → B → C → D
出队顺序: A → B → C → D

这保证了:
- 先访问的节点的邻居会先被处理
- 同一层的节点会被连续处理
- 不会出现"跨层"访问的情况
```

## 关键数据结构与要素

### 1. 队列(Queue)

队列是BFS的核心数据结构,负责维护待访问节点的顺序。

**队列操作**:
- `push(x)`: 将元素x加入队列尾部
- `front()`: 获取队列头部元素
- `pop()`: 移除队列头部元素
- `empty()`: 判断队列是否为空
- `size()`: 获取队列中元素数量

**在BFS中的作用**:
```
队列状态变化示例(以二叉树为例):

初始状态:      [root]
处理root后:    [左子, 右子]
处理左子后:    [右子, 左左, 左右]
处理右子后:    [左左, 左右, 右左, 右右]
...
```

### 2. 访问标记(visited)

**为什么需要访问标记?**
- 防止重复访问同一节点(避免无限循环)
- 提高算法效率(避免重复计算)
- 在图中特别重要(图可能包含环)

**实现方式**:
```cpp
// 对于节点编号为0到n-1的图
vector<bool> visited(n, false);

// 对于节点为任意类型的图
unordered_map<NodeType, bool> visited;
```

**标记时机**:
```cpp
// 正确做法:节点入队时立即标记
if (!visited[neighbor]) {
    visited[neighbor] = true;  // 立即标记
    q.push(neighbor);
}

// 错误做法:出队时才标记
// 这会导致同一节点可能被多次加入队列
```

## 算法流程详解

### 通用BFS算法流程

```cpp
算法 BFS(起始节点start):
1. 创建空队列q
2. 创建访问标记数组visited,初始化为false
3. visited[start] = true
4. q.push(start)
5. while q不为空:
6.     node = q.front()
7.     q.pop()
8.     处理node(访问、输出等)
9.     for node的每个邻居neighbor:
10.        if visited[neighbor] == false:
11.            visited[neighbor] = true
12.            q.push(neighbor)
```

### 算法逻辑示例(二叉树)

**示例二叉树**:
```
          1
        /   \
       2     3
      / \   /
     4   5  6
```

**BFS遍历过程详解**:

| 步骤 | 队列状态 | visited集合 | 操作说明 | 输出 |
|------|---------|-------------|----------|------|
| 1 | `[1]` | `{1}` | 初始化,根节点入队 | - |
| 2 | `[2,3] `| `{1,2,3}` | 出队1,访问1,子节点2,3入队 | 1 |
| 3 | `[3,4,5]` | `{1,2,3,4,5} `| 出队2,访问2,子节点4,5入队 | 2 |
| 4 | `[4,5,6]` | `{1,2,3,4,5,6}` | 出队3,访问3,子节点6入队 | 3 |
| 5 | `[5,6] `| `{1,2,3,4,5,6}` | 出队4,访问4,无子节点 | 4 |
| 6 | `[6] `| `{1,2,3,4,5,6}` | 出队5,访问5,无子节点 | 5 |
| 7 | `[] `| `{1,2,3,4,5,6} `| 出队6,访问6,队列为空,结束 | 6 |

**最终遍历顺序**: `1 → 2 → 3 → 4 → 5 → 6`

## 复杂度分析

### 时间复杂度

**树的BFS**: O(n)
- n为树的节点数
- 每个节点被访问一次

**图的BFS**: O(V + E)
- V为顶点数(Vertex)
- E为边数(Edge)
- 每个顶点被访问一次,每条边被检查一次

### 空间复杂度

**最坏情况**:
- 树: O(n) - 当树退化为链表时
- 图: O(V) - 当图为完全图时

**平均情况**:
- 取决于图的分支因子和深度
- 通常远小于最坏情况

## 代码实现

### 1. 二叉树的层序遍历(C++)

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * BFS层序遍历函数
 * @param root 二叉树根节点
 * @return 二维数组,每个子数组代表一层的节点值
 */
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;  // 存储每层的节点值
    if (!root) return result;    // 空树直接返回

    queue<TreeNode*> q;
    q.push(root);  // 根节点入队

    while (!q.empty()) {
        int levelSize = q.size();  // 当前层的节点数
        vector<int> currentLevel;  // 存储当前层的节点值

        // 遍历当前层的所有节点
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();
            currentLevel.push_back(node->val);  // 记录当前节点值

            // 左右子节点入队(若存在)
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(currentLevel);  // 记录当前层
    }
    return result;
}

// 测试代码
int main() {
    // 构建示例二叉树
    //       1
    //      / \
    //     2   3
    //    / \ /
    //   4  5 6
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);

    // 执行层序遍历
    vector<vector<int>> res = levelOrder(root);

    // 输出结果
    cout << "BFS层序遍历结果:" << endl;
    for (const auto& level : res) {
        for (int val : level) {
            cout << val << " ";
        }
        cout << endl;
    }

    // 释放内存(实际项目中应使用智能指针)
    delete root->left->left;
    delete root->left->right;
    delete root->right->left;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}
```

**输出结果**:
```
BFS层序遍历结果:
1 
2 3 
4 5 6 
```

### 2. 图的BFS遍历(邻接表存储,C++)

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

/**
 * 图的BFS遍历函数
 * @param start 起始节点编号
 * @param adj 图的邻接表表示
 * @return BFS遍历顺序的节点序列
 */
vector<int> bfsGraph(int start, const vector<vector<int>>& adj) {
    int n = adj.size();                    // 节点总数
    vector<bool> visited(n, false);        // 标记已访问节点
    vector<int> traversalOrder;            // 存储遍历顺序
    queue<int> q;

    // 起始节点入队并标记
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        traversalOrder.push_back(u);  // 记录访问顺序

        // 遍历u的所有相邻节点v
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;    // 入队前立即标记
                q.push(v);
            }
        }
    }
    return traversalOrder;
}

// 测试代码
int main() {
    // 构建邻接表(无向图)
    // 示例图结构:
    //     0
    //    / \
    //   1---2
    //    \ /
    //     3
    vector<vector<int>> adj = {
        {1, 2},   // 节点0的相邻节点
        {0, 3},   // 节点1的相邻节点
        {0, 3},   // 节点2的相邻节点
        {1, 2}    // 节点3的相邻节点
    };

    // 从节点0开始BFS遍历
    vector<int> result = bfsGraph(0, adj);

    // 输出结果
    cout << "图的BFS遍历结果:";
    for (int node : result) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}
```

**输出结果**:
```
图的BFS遍历结果:0 1 2 3 
```

### 3. 带路径记录的BFS(寻找最短路径)

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

/**
 * 使用BFS寻找从start到end的最短路径
 * @param start 起始节点
 * @param end 目标节点
 * @param adj 图的邻接表
 * @return 最短路径(节点序列),如果不存在路径则返回空
 */
vector<int> bfsShortestPath(int start, int end, const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    unordered_map<int, int> parent;  // 记录每个节点的前驱节点
    queue<int> q;

    visited[start] = true;
    q.push(start);
    parent[start] = -1;  // 起点没有前驱

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        // 找到目标节点
        if (u == end) {
            // 回溯构建路径
            vector<int> path;
            int current = end;
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // 遍历相邻节点
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;  // 记录前驱节点
                q.push(v);
            }
        }
    }

    return {};  // 没有找到路径
}

int main() {
    // 示例图
    vector<vector<int>> adj = {
        {1, 2},     // 0
        {0, 3, 4},  // 1
        {0, 4},     // 2
        {1, 5},     // 3
        {1, 2, 5},  // 4
        {3, 4}      // 5
    };

    // 寻找从节点0到节点5的最短路径
    vector<int> path = bfsShortestPath(0, 5, adj);

    if (!path.empty()) {
        cout << "最短路径: ";
        for (int node : path) {
            cout << node;
            if (node != path.back()) {
                cout << " -> ";
            }
        }
        cout << endl;
        cout << "路径长度: " << path.size() - 1 << endl;
    } else {
        cout << "不存在从起点到终点的路径" << endl;
    }

    return 0;
}
```

**输出结果**:
```
最短路径: 0 -> 1 -> 3 -> 5
路径长度: 3
```

## BFS与DFS对比

| 特性 | BFS | DFS |
|------|-----|-----|
| 数据结构 | 队列(先进先出) | 栈(后进先出) |
| 遍历顺序 | 层序遍历 | 深度优先 |
| 最短路径 | 可找到无权图的最短路径 | 不保证最短路径 |
| 空间复杂度 | O(V) | O(h),h为树/图的深度 |
| 适用场景 | 最短路径、层序遍历 | 拓扑排序、连通性检测 |

## 应用场景

### 1. 树相关应用
- **层序遍历**: 按层输出树的所有节点
- **求树的最大深度/最小深度**: 统计层数
- **树的序列化/反序列化**: 按层序存储和恢复树结构
- **二叉树的锯齿形层序遍历**: 奇偶层交替方向遍历

### 2. 图相关应用
- **无权图最短路径**: 找到两点间的最短路径
- **连通分量检测**: 找出图中所有连通分量
- **拓扑排序**: 对有向无环图进行排序
- **二分图检测**: 判断图是否为二分图

### 3. 网格问题
- **岛屿数量**: 计算网格中连通的陆地数量
- **迷宫最短路径**: 在网格中找到从起点到终点的最短路径
- **单词接龙**: 找到单词变换的最短序列
- **腐烂的橘子**: 计算所有橘子腐烂所需的最短时间

### 4. 其他应用
- **社交网络分析**: 计算用户之间的最短距离
- **网络爬虫**: 按层次遍历网页
- **游戏AI**: 寻找最短移动路径
- **推荐系统**: 基于用户关系图进行推荐

## 常见问题与注意事项

### 1. 什么时候使用BFS?

**使用BFS的场景**:
- 需要找到最短路径(无权图)
- 需要按层处理节点
- 需要遍历所有节点且希望按距离排序

**不使用BFS的场景**:
- 图/树非常深,空间复杂度会很高
- 不需要最短路径,只需要找到任意路径
- 需要深度优先处理的场景(如回溯)

### 2. 如何优化BFS的空间复杂度?

- **双向BFS**: 同时从起点和终点开始搜索,在中间相遇
- **A*算法**: 使用启发式函数指导搜索方向
- **迭代加深**: 限制搜索深度,逐步增加

### 3. 常见错误

**错误1: 出队时才标记visited**
```cpp
// 错误做法
while (!q.empty()) {
    int u = q.front();
    q.pop();
    visited[u] = true;  // 错误:应该在入队时标记
    // ...
}
```

**错误2: 忘记处理不连通图**
```cpp
// 只处理一个连通分量
bfs(start);  // 错误:图中可能有多个连通分量

// 正确做法
for (int i = 0; i < n; i++) {
    if (!visited[i]) {
        bfs(i);
    }
}
```

**错误3: 队列大小使用不当**
```cpp
while (!q.empty()) {
    // 错误:在循环中修改q.size()
    for (int i = 0; i < q.size(); i++) {  // q.size()会变化!
        // ...
    }
}

// 正确做法
while (!q.empty()) {
    int levelSize = q.size();  // 先保存当前层的大小
    for (int i = 0; i < levelSize; i++) {
        // ...
    }
}
```

## 总结

BFS是一种**简单而强大**的图遍历算法,其核心思想是:
1. 使用队列维护待访问节点
2. 按层序访问节点
3. 使用visited数组避免重复访问
4. 在无权图中可找到最短路径

掌握BFS对于解决许多算法问题非常重要,特别是在需要寻找最短路径或按层处理节点的场景中。通过理解BFS的核心思想和正确实现,你可以高效地解决各种图论问题。



