cmake_minimum_required(VERSION 3.10.2)
project("fileclassifier")
add_library(fileclassifier SHARED
        fileclassifier/traverseFilesNR.cpp
        fileclassifier/classifyType.cpp
        fileclassifier/classifier.cpp
        utils/StringUtils.cpp
        utils/FileUtils.cpp
        trie/SuffixTrie.cpp)

find_library(log-lib
        log)

target_link_libraries(
        fileclassifier
        ${log-lib})