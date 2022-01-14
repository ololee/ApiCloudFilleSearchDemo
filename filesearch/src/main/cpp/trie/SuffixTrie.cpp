#include "../common/Common.h"
#include "SuffixTrie.h"


/**
 * 向前缀树插入
 * @param suffix 后缀名
 * @param type 文件类型
 */
void SuffixTrie::insert(char *suffix, int type) {
    assert(suffix != NULL);
    int suffixLen = strlen(suffix);
    assert(suffixLen != 0);
    SuffixNode *node = root;
    for (int i = 0; i < suffixLen; ++i) {
        int index = 0;
        for (; index < node->children.size(); ++index) {
            if (node->children[index].ch == suffix[i])
                break;
        }

        if (index < node->children.size()) {//找到了
            node = &(node->children[index]);
        } else if (index == node->children.size()) {//未找到节点
            SuffixNode temp;
            temp.ch = suffix[i];
            node->children.push_back(temp);
            node = &(node->children.back());
        }
    }
    node->isLeaf = true;
    node->type = type;
}

/**
 * 通过后缀获得文件的类型
 * @param suffix 后缀
 * @return 文件的类型
 */
int SuffixTrie::search(char *suffix) {
    assert(suffix != nullptr);
    int suffixLen = strlen(suffix);
    assert(suffixLen != 0);
    SuffixNode *node = root;
    for (int i = 0; i < suffixLen; ++i) {
        int index = 0;
        for (; index < node->children.size(); ++index) {
            if (node->children[index].ch == suffix[i])
                break;
        }
        if (index == node->children.size()) {//未找到
            return 0;
        }
        node = &(node->children[index]);
    }
    if (node->isLeaf)
        return node->type;
    else
        return 0;
}


