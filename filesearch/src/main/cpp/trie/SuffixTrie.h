#ifndef FILECLASSIFIER_SUFFIXTRIE_H
#define FILECLASSIFIER_SUFFIXTRIE_H
#include "SuffixNode.h"

/**
 * 前缀树
 * search:插入操作
 * search:查询操作
 * 析构:释放操作
 */
class SuffixTrie {
private:
    SuffixNode *root;
public:
    SuffixTrie() {
        root = new SuffixNode();
    }

    /**
     * 构造字典树
     * @param suffix 后缀
     * @param type 后缀类型
     */
    void insert(char *suffix, int type);

    /**
     * 查询字典
     * @param suffix 后缀
     * @return  后缀类型
     */
    int search(char* suffix);

    ~SuffixTrie(){
        LOGD("SuffixTrie析构");
        delete root;
    }
};

#endif
