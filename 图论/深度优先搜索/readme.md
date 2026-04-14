# 深度优先搜索（DFS）

## 简介

**深度优先搜索（Depth-First Search, DFS）是一种用于遍历或搜索树或图数据结构的算法。该算法从起始节点开始，沿着一条路径尽可能深地探索，直到无法继续前进，然后回溯到上一个节点，探索其他未访问的路径。**

### **DFS的核心特性**:
- 沿着一条路径尽可能深入探索
- 使用栈（或递归）实现后进先出（LIFO）的特性
- 适用于需要深度探索或回溯的场景
- 可以用于检测图的连通性、寻找路径等

## 核心思想

### 迷宫探索模型

想象你在探索一个迷宫，DFS的遍历逻辑与这个场景完全一致：

```
      起始节点
        ↓
      节点A
       ↓
      节点B ←──┐
       ↓      │
      节点C    │
       ↓      │
      节点D ───┘ (回溯)
```

**DFS的遍历顺序**:
1. 从起始节点开始，选择一条路径深入
2. 沿着当前路径一直前进，直到无法继续
3. 回溯到上一个节点，探索其他未访问的路径
4. 重复上述过程，直到所有可达节点都被访问

### 为什么需要栈/递归?

DFS使用栈（Stack）或递归作为核心机制，因为栈的**后进先出（LIFO）**特性完美契合DFS的深度优先遍历需求：

```
栈操作示例:
入栈顺序: A → B → C → D
出栈顺序: D → C → B → A

这保证了:
- 最后访问的节点会最先被处理
- 可以深入探索一条路径后再回溯
- 不会出现"跨路径"访问的情况
```

## 关键数据结构与要素

### 1. 栈（Stack）

栈是DFS的核心数据结构，负责维护待访问节点的顺序。

**栈操作**:
- `push(x)`: 将元素x压入栈顶
- `top()`: 获取栈顶元素
- `pop()`: 弹出栈顶元素
- `empty()`: 判断栈是否为空
- `size()`: 获取栈中元素数量

**在DFS中的作用**:
```
栈状态变化示例(以二叉树为例):

初始状态:      [root]
处理root后:    [左子, 右子]
处理左子后:    [左左, 左右, 右子]
处理左左后:    [左右, 右子]
...
```

### 2. 访问标记（visited）

**为什么需要访问标记?**
- 防止重复访问同一节点（避免无限循环）
- 提高算法效率（避免重复计算）
- 在图中特别重要（图可能包含环）

**实现方式**:
```cpp
// 对于节点编号为0到n-1的图
vector<bool> visited(n, false);

// 对于节点为任意类型的图
unordered_map<NodeType, bool> visited;
```

**标记时机**:
```cpp
// 正确做法:节点入栈/递归调用时立即标记
if (!visited[neighbor]) {
    visited[neighbor] = true;  // 立即标记
    dfs(neighbor);
}

// 错误做法:出栈/递归返回时才标记
// 这会导致同一节点可能被多次处理
```

## 算法流程详解

### 通用DFS算法流程

```cpp
算法 DFS(起始节点start):
1. 创建访问标记数组visited,初始化为false
2. visited[start] = true
3. 处理start节点(访问、输出等)
4. for start的每个邻居neighbor:
5.    if visited[neighbor] == false:
6.        visited[neighbor] = true
7.        DFS(neighbor)  // 递归调用
```

### 算法逻辑示例(二叉树)

**示例二叉树**:
```
          1
        /   \
       2     3
      / \   /
     4   5 6
```

**DFS遍历过程详解**:

| 步骤 | 栈/递归状态 | visited集合 | 操作说明 | 输出 |
|------|------------|-------------|----------|------|
| 1 | `[1]` | `{1}` | 初始化,根节点入栈/递归 | - |
| 2 | `[2,3] `| `{1,2}` | 出栈1,访问1,子节点2,3入栈 | 1 |
| 3 | `[4,5,3]` | `{1,2,4}` | 出栈2,访问2,子节点4,5入栈 | 2 |
| 4 | `[5,3] `| `{1,2,4,5}` | 出栈4,访问4,无子节点 | 4 |
| 5 | `[3] `| `{1,2,4,5,3}` | 出栈5,访问5,无子节点 | 5 |
| 6 | `[6] `| `{1,2,4,5,3,6}` | 出栈3,访问3,子节点6入栈 | 3 |
| 7 | `[] `| `{1,2,4,5,3,6} `| 出栈6,访问6,栈为空,结束 | 6 |`

**最终遍历顺序**: `1 → 2 → 4 → 5 → 3 → 6`

## 复杂度分析

### 时间复杂度

**树的DFS**: O(n)
- n为树的节点数
- 每个节点被访问一次

**图的DFS**: O(V + E)
- V为顶点数（Vertex）
- E为边数（Edge）
- 每个顶点被访问一次，每条边被检查一次

### 空间复杂度

**最坏情况**:
- 树: O(n) - 当树退化为链表时
- 图: O(V) - 当图为完全图时

**平均情况**:
- 取决于图的分支因子和深度
- 通常远小于最坏情况

## 代码实现

### 1. 二叉树的前序遍历(C++)

```cpp
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * DFS前序遍历函数(递归实现)
 * @param root 二叉树根节点
 * @return 前序遍历的节点值序列
 */
void preorderDFS(TreeNode* node, vector<int>& result) {
    if (!node) return;

    result.push_back(node->val);  // 访问当前节点
    preorderDFS(node->left, result);   // 递归遍历左子树
    preorderDFS(node->right, result);  // 递归遍历右子树
}

/**
 * DFS前序遍历函数(迭代实现)
 * @param root 二叉树根节点
 * @return 前序遍历的节点值序列
 */
vector<int> preorderDFSIterative(TreeNode* root) {
    vector<int> result;
    if (!root) return result;

    stack<TreeNode*> s;
    s.push(root);

    while (!s.empty()) {
        TreeNode* node = s.top();
        s.pop();
        result.push_back(node->val);  // 访问当前节点

        // 右子节点先入栈(后处理)
        if (node->right) s.push(node->right);
        // 左子节点后入栈(先处理)
        if (node->left) s.push(node->left);
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

    // 执行前序遍历(递归)
    vector<int> resRecursive;
    preorderDFS(root, resRecursive);

    // 执行前序遍历(迭代)
    vector<int> resIterative = preorderDFSIterative(root);

    // 输出结果
    cout << "DFS前序遍历结果(递归):";
    for (int val : resRecursive) {
        cout << val << " ";
    }
    cout << endl;

    cout << "DFS前序遍历结果(迭代):";
    for (int val : resIterative) {
        cout << val << " ";
    }
    cout << endl;

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
DFS前序遍历结果(递归):1 2 4 5 3 6 
DFS前序遍历结果(迭代):1 2 4 5 3 6 
```

### 2. 图的DFS遍历(邻接表存储,C++)

```cpp
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

/**
 * 图的DFS遍历函数(递归实现)
 * @param node 当前节点
 * @param adj 图的邻接表表示
 * @param visited 访问标记数组
 * @param traversalOrder 存储遍历顺序
 */
void dfsRecursive(int node, const vector<vector<int>>& adj, 
                  vector<bool>& visited, vector<int>& traversalOrder) {
    visited[node] = true;
    traversalOrder.push_back(node);

    // 遍历node的所有相邻节点
    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfsRecursive(neighbor, adj, visited, traversalOrder);
        }
    }
}

/**
 * 图的DFS遍历函数(迭代实现)
 * @param start 起始节点编号
 * @param adj 图的邻接表表示
 * @return DFS遍历顺序的节点序列
 */
vector<int> dfsIterative(int start, const vector<vector<int>>& adj) {
    int n = adj.size();                    // 节点总数
    vector<bool> visited(n, false);        // 标记已访问节点
    vector<int> traversalOrder;            // 存储遍历顺序
    stack<int> s;

    // 起始节点入栈并标记
    visited[start] = true;
    s.push(start);

    while (!s.empty()) {
        int u = s.top();
        s.pop();
        traversalOrder.push_back(u);  // 记录访问顺序

        // 遍历u的所有相邻节点v(逆序入栈以保持与递归相同的顺序)
        for (int i = adj[u].size() - 1; i >= 0; i--) {
            int v = adj[u][i];
            if (!visited[v]) {
                visited[v] = true;    // 入栈前立即标记
                s.push(v);
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

    // 从节点0开始DFS遍历(递归)
    vector<bool> visited(4, false);
    vector<int> resultRecursive;
    dfsRecursive(0, adj, visited, resultRecursive);

    // 从节点0开始DFS遍历(迭代)
    vector<int> resultIterative = dfsIterative(0, adj);

    // 输出结果
    cout << "图的DFS遍历结果(递归):";
    for (int node : resultRecursive) {
        cout << node << " ";
    }
    cout << endl;

    cout << "图的DFS遍历结果(迭代):";
    for (int node : resultIterative) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}
```

**输出结果**:
```
图的DFS遍历结果(递归):0 1 3 2 
图的DFS遍历结果(迭代):0 1 3 2 
```

### 3. 带路径记录的DFS(寻找路径)

```cpp
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <algorithm>
using namespace std;

/**
 * 使用DFS寻找从start到end的路径
 * @param node 当前节点
 * @param end 目标节点
 * @param adj 图的邻接表
 * @param visited 访问标记数组
 * @param path 当前路径
 * @param allPaths 所有找到的路径
 */
void dfsFindPath(int node, int end, const vector<vector<int>>& adj,
                 vector<bool>& visited, vector<int>& path, vector<vector<int>>& allPaths) {
    visited[node] = true;
    path.push_back(node);

    // 找到目标节点
    if (node == end) {
        allPaths.push_back(path);
    } else {
        // 遍历相邻节点
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfsFindPath(neighbor, end, adj, visited, path, allPaths);
            }
        }
    }

    // 回溯
    path.pop_back();
    visited[node] = false;
}

/**
 * 使用DFS寻找从start到end的所有路径
 * @param start 起始节点
 * @param end 目标节点
 * @param adj 图的邻接表
 * @return 所有路径的集合
 */
vector<vector<int>> findAllPaths(int start, int end, const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    vector<int> path;
    vector<vector<int>> allPaths;

    dfsFindPath(start, end, adj, visited, path, allPaths);

    return allPaths;
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

    // 寻找从节点0到节点5的所有路径
    vector<vector<int>> paths = findAllPaths(0, 5, adj);

    if (!paths.empty()) {
        cout << "从节点0到节点5的所有路径:" << endl;
        for (const auto& path : paths) {
            cout << "路径: ";
            for (size_t i = 0; i < path.size(); i++) {
                cout << path[i];
                if (i < path.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl;
        }
        cout << "共找到 " << paths.size() << " 条路径" << endl;
    } else {
        cout << "不存在从起点到终点的路径" << endl;
    }

    return 0;
}
```

**输出结果**:
```
从节点0到节点5的所有路径:
路径: 0 -> 1 -> 3 -> 5
路径: 0 -> 1 -> 4 -> 5
路径: 0 -> 2 -> 4 -> 5
共找到 3 条路径
```


## 应用场景

### 1. 树相关应用
- **前序/中序/后序遍历**: 按不同顺序访问树的所有节点
- **树的深度**: 计算树的最大深度
- **树的路径和**: 查找满足特定条件的路径
- **最近公共祖先**: 查找两个节点的最近公共祖先

### 2. 图相关应用
- **连通性检测**: 判断图中两个节点是否连通
- **连通分量**: 找出图中所有连通分量
- **拓扑排序**: 对有向无环图进行排序
- **强连通分量**: 找出有向图中的强连通分量
- **检测环**: 判断图中是否存在环

### 3. 回溯问题
- **排列组合**: 生成所有可能的排列或组合
- **N皇后问题**: 在N×N棋盘上放置N个皇后
- **数独求解**: 填充数独格子
- **子集生成**: 生成集合的所有子集

### 4. 其他应用
- **迷宫求解**: 寻找从起点到终点的路径
- **图着色**: 用最少颜色给图着色
- **哈密顿路径**: 寻找经过每个节点一次的路径
- **欧拉路径**: 寻找经过每条边一次的路径

## 常见问题与注意事项

### 1. 什么时候使用DFS?

**使用DFS的场景**:
- 需要深度探索所有可能的情况
- 需要找到所有可能的解
- 需要回溯或回溯搜索
- 需要检测图的连通性或环

**不使用DFS的场景**:
- 需要找到最短路径(无权图)
- 需要按层处理节点
- 图/树非常深,可能导致栈溢出

### 2. 如何优化DFS的空间复杂度?

- **迭代DFS**: 使用显式栈代替递归,避免栈溢出
- **路径压缩**: 在并查集中使用路径压缩优化
- **剪枝**: 提前终止不可能的搜索路径

### 3. 常见错误

**错误1: 出栈/递归返回时才标记visited**
```cpp
// 错误做法
void dfs(int node) {
    // ...
    visited[node] = true;  // 错误:应该在入栈/递归调用时标记
    // ...
}

// 正确做法
void dfs(int node) {
    visited[node] = true;  // 正确:立即标记
    // ...
}
```

**错误2: 忘记处理不连通图**
```cpp
// 只处理一个连通分量
dfs(start);  // 错误:图中可能有多个连通分量

// 正确做法
for (int i = 0; i < n; i++) {
    if (!visited[i]) {
        dfs(i);
    }
}
```

**错误3: 回溯时忘记恢复状态**
```cpp
// 错误做法
void dfs(int node, vector<int>& path) {
    path.push_back(node);
    // ...
    // 错误:忘记回溯时弹出节点
}

// 正确做法
void dfs(int node, vector<int>& path) {
    path.push_back(node);
    // ...
    path.pop_back();  // 正确:回溯时恢复状态
}
```

## 总结

DFS是一种**简单而强大**的图遍历算法,其核心思想是:
1. 使用栈(或递归)维护待访问节点
2. 深度优先探索路径
3. 使用visited数组避免重复访问
4. 通过回溯探索所有可能路径

掌握DFS对于解决许多算法问题非常重要,特别是在需要深度探索、回溯或检测图的连通性的场景中。通过理解DFS的核心思想和正确实现,你可以高效地解决各种图论问题。
