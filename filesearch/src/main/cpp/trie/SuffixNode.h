#ifndef FILECLASSIFIER_SUFFIXNODE_H
#define FILECLASSIFIER_SUFFIXNODE_H
#include "../common/Common.h"

/**
 * 前缀树的节点
 * ch:当前节点字母
 * type:文件类型
 * isLeaf:是否是叶子节点
 * children:子节点
 */
class SuffixNode {
public:
    char ch;
    int type;
    bool isLeaf;
    vector<SuffixNode> children;
public:
    SuffixNode() : ch(NULL), type(0), isLeaf(false),children(vector<SuffixNode>()) {
    }

    ~SuffixNode(){
    }
};

#endif
