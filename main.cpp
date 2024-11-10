#include <bits/stdc++.h>
using namespace std;

#define MAX_CHAR 256

struct SuffixTreeNode {
    struct SuffixTreeNode *children[MAX_CHAR];
    struct SuffixTreeNode *suffixLink;
    int start;
    int *end;
    int suffixIndex;
};

typedef struct SuffixTreeNode Node;

char text[100];
Node *root = NULL;
Node *lastNewNode = NULL;
Node *activeNode = NULL;
int activeEdge = -1;
int activeLength = 0;
int remainingSuffixCount = 0;
int leafEnd = -1;
int *rootEnd = NULL;
int *splitEnd = NULL;
int textSize = -1;
int size1 = 0;

Node *newNode(int start, int *end) {
    Node *node = (Node*) malloc(sizeof(Node));
    for (int i = 0; i < MAX_CHAR; i++) {
        node->children[i] = NULL;
    }
    node->suffixLink = root;
    node->start = start;
    node->end = end;
    node->suffixIndex = -1;
    return node;
}

int edgeLength(Node *n) {
    if (n == root) {
        return 0;
    }
    return *(n->end) - (n->start) + 1;
}

int walkDown(Node *currNode) {
    if (activeLength >= edgeLength(currNode)) {
        activeEdge += edgeLength(currNode);
        activeLength -= edgeLength(currNode);
        activeNode = currNode;
        return 1;
    }
    return 0;
}

void extendSuffixTree(int pos) {
    leafEnd = pos;
    remainingSuffixCount++;
    lastNewNode = NULL;

    while (remainingSuffixCount > 0) {
        if (activeLength == 0) {
            activeEdge = pos;
        }
        if (activeNode->children[(unsigned char) text[activeEdge]] == NULL) {
            activeNode->children[(unsigned char) text[activeEdge]] = newNode(pos, &leafEnd);
            if (lastNewNode != NULL) {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = NULL;
            }
        } else {
            Node *next = activeNode->children[(unsigned char) text[activeEdge]];
            if (walkDown(next)) {
                continue;
            }
            if (text[next->start + activeLength] == text[pos]) {
                if (lastNewNode != NULL && activeNode != root) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = NULL;
                }
                activeLength++;
                break;
            }
            splitEnd = (int*) malloc(sizeof(int));
            *splitEnd = next->start + activeLength - 1;
            Node *split = newNode(next->start, splitEnd);
            activeNode->children[(unsigned char) text[next->start]] = split;
            split->children[(unsigned char) text[pos]] = newNode(pos, &leafEnd);
            next->start += activeLength;
            split->children[(unsigned char) text[next->start]] = next;
            if (lastNewNode != NULL) {
                lastNewNode->suffixLink = split;
            }
            lastNewNode = split;
        }
        remainingSuffixCount--;
        if (activeNode == root && activeLength > 0) {
            activeLength--;
            activeEdge = pos - remainingSuffixCount + 1;
        } else if (activeNode != root) {
            activeNode = activeNode->suffixLink;
        }
    }
}

void setSuffixIndexByDFS(Node *n, int labelHeight) {
    if (n == NULL) return;
    bool leaf = true;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (n->children[i] != NULL) {
            leaf = false;
            setSuffixIndexByDFS(n->children[i], labelHeight + edgeLength(n->children[i]));
        }
    }
    if (leaf) {
        for (int i = n->start; i <= *(n->end); i++) {
            if (text[i] == '#') {
                n->end = (int*) malloc(sizeof(int));
                *(n->end) = i;
            }
        }
        n->suffixIndex = textSize - labelHeight;
    }
}

void freeSuffixTreeByPostOrder(Node *n) {
    if (n == NULL) return;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (n->children[i] != NULL) {
            freeSuffixTreeByPostOrder(n->children[i]);
        }
    }
    if (n->suffixIndex == -1)
        free(n->end);
    free(n);
}

int levenshteinDistance(const string &str1, const string &str2) {
    int len1 = str1.size();
    int len2 = str2.size();
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    for (int i = 0; i <= len1; ++i) dp[i][0] = i;
    for (int j = 0; j <= len2; ++j) dp[0][j] = j;

    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
        }
    }

    return dp[len1][len2];
}

void buildSuffixTree() {
    textSize = strlen(text);
    rootEnd = (int*) malloc(sizeof(int));
    *rootEnd = -1;
    root = newNode(-1, rootEnd);
    activeNode = root;
    for (int i = 0; i < textSize; i++)
        extendSuffixTree(i);
    int labelHeight = 0;
    setSuffixIndexByDFS(root, labelHeight);
}

int doTraversal(Node *n, int labelHeight, int* maxHeight, int* substringStartIndex) {
    if (n == NULL) {
        return 0;
    }
    int ret = -1;
    if (n->suffixIndex < 0) {
        for (int i = 0; i < MAX_CHAR; i++) {
            if (n->children[i] != NULL) {
                ret = doTraversal(n->children[i], labelHeight + edgeLength(n->children[i]), maxHeight, substringStartIndex);
                if (n->suffixIndex == -1)
                    n->suffixIndex = ret;
                else if ((n->suffixIndex == -2 && ret == -3) || (n->suffixIndex == -3 && ret == -2) || n->suffixIndex == -4) {
                    n->suffixIndex = -4;
                    if (*maxHeight < labelHeight) {
                        *maxHeight = labelHeight;
                        *substringStartIndex = *(n->end) - labelHeight + 1;
                    }
                }
            }
        }
    } else if (n->suffixIndex > -1 && n->suffixIndex < size1)
        return -2;
    else if (n->suffixIndex >= size1)
        return -3;
    return n->suffixIndex;
}

int getLongestCommonSubstring(string &lcsStr) {
    int maxHeight = 0;
    int substringStartIndex = 0;
    doTraversal(root, 0, &maxHeight, &substringStartIndex);
    if (maxHeight > 0) {
        lcsStr = string(text + substringStartIndex, maxHeight);
    }
    return maxHeight;
}

void text_filter(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file!" << endl;
        return;
    }
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file!" << endl;
        return;
    }

    char ch;
    while (inputFile.get(ch)) {
        // Write alphabetic characters and spaces only
        if (isalpha(ch) || isspace(ch)) {
            outputFile.put(ch);
        }
    }

    inputFile.close();
    outputFile.close();
}

double similarityScore(string str1, string str2, double alpha = 0.5) {
    int levDist = levenshteinDistance(str1, str2);
    double maxLen = max(str1.size(), str2.size());
    double levSimilarity = 1.0 - (double) levDist / maxLen;
    cout << "Levenshtein similarity score: " << levSimilarity << endl;
    cout << "Levenshtein distance: " << levDist << endl;

    strcpy(text, (str1 + "#" + str2 + "$").c_str());
    size1 = str1.size();
    buildSuffixTree();
    
    string lcsStr;
    int lcsLength = getLongestCommonSubstring(lcsStr);
    double lcsSimilarity = (double) lcsLength / maxLen;
    cout << "Longest Common Substring similarity score: " << lcsSimilarity << endl;
    cout << "Longest Common Substring: " << lcsStr << " (length: " << lcsLength << ")" << endl;

    double similarity = alpha * lcsSimilarity + (1 - alpha) * levSimilarity;
    cout << "Combined similarity score: " << similarity * 100 << "%" << endl;

    freeSuffixTreeByPostOrder(root);
    return similarity;
}

string readFileContent(const string& filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    string str1, str2;
    string inputType;

    cout << "Enter 'f' for file input or 't' for text input: ";
    cin >> inputType;

    if (inputType == "f") {
        string inputFile1, inputFile2, filteredFile1, filteredFile2;
        
        cout << "Enter path for the first file: ";
        cin >> inputFile1;
        cout << "Enter path for the second file: ";
        cin >> inputFile2;
        
        filteredFile1 = "filtered1.txt";
        filteredFile2 = "filtered2.txt";
        text_filter(inputFile1.c_str(), filteredFile1.c_str());
        text_filter(inputFile2.c_str(), filteredFile2.c_str());

        str1 = readFileContent(filteredFile1);
        str2 = readFileContent(filteredFile2);
    } else if (inputType == "t") {
        cout << "Enter the first text: ";
        cin.ignore();
        getline(cin, str1);
        cout << "Enter the second text: ";
        getline(cin, str2);
    } else {
        cout << "Invalid input type. Exiting.";
        return 1;
    }

    double alpha;
    cout << "Enter the alpha value (between 0 and 1): ";
    cin >> alpha;

    similarityScore(str1, str2, alpha);
    return 0;
}
