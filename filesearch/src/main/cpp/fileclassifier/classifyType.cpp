#include "classifyType.h"

/**
 * 使用遍历的方式获得可以接受的文件类型
 * @param childType 子类型
 * @param acceptType 接受类型数组
 * @param acceptTypeCount 接受类型数组长度
 * @return
 */
bool acceptTypeByTraverse(int childType, int *acceptType, int acceptTypeCount) {
    for (int i = 0; i < acceptTypeCount; i++) {
        if (acceptType[i] == childType)
            return true;
    }
    return false;
}

/**
 * 使用掩码的方式获得接受文件的类型
 * @param childType 子类型
 * @param acceptType 接受类型数组，0位置为掩码，1位置为接受的父类型
 * @param acceptTypeCount 接受类型数组长度 长度为2，未使用
 * @return
 */
bool acceptTypesByMask(int childType, int *acceptType, int acceptTypeCount) {
    bool  accept = (childType & acceptType[0]) == acceptType[1];
    //delete acceptType;
    return accept;
}
