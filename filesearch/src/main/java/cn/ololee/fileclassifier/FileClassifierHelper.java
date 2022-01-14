package cn.ololee.fileclassifier;

import androidx.annotation.NonNull;
import cn.ololee.fileclassifier.annotation.StatisticsTypeAnnotation;
import cn.ololee.fileclassifier.classifier.FileClassifier;
import java.io.File;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 文件划分器帮助类，配合JNI使用
 * Tips:
 * 使用请参考示例程序
 * 使用请一定要初始化前缀树，使用init初始化默认的前缀树 或 使用initSuffixTrie初始化自定义的前缀树
 * 使用自定义的前缀树建议参照{@link cn.ololee.fileclassifier.DefaultConstants}设计
 * 在设计文件类型时，请勿使用0作为文件的类型，因为0被作为未知文件使用了；这样您定义的文件与位置类型将不可区分
 */
public class FileClassifierHelper {
  static {
    System.loadLibrary("fileclassifier");
  }

  public static final String TAG = FileClassifierHelper.class.getSimpleName();

  /**
   * 供jni层使用的前缀树的指针
   */
  private static long suffixTriePtr;

  /**
   * 该库默认的前缀树初始化Map
   */
  private static Map<Integer, List<String>> defaultTrieMap = new HashMap<>();

  /**
   * 默认的初始化，使用内部默认的
   */
  public static void init() {
    //todo:初始化前缀树
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_IMAGE,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.IMAGES));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_AUDIO,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.AUDIOS));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_VIDEO,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.VIDEOS));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_DOC_PDF,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.PDF));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_DOC_PPT,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.PPTS));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_DOC_EXCEL,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.EXCELS));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_DOC_WORD,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.WORDS));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_DOC_TXT,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.TXTS));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_COMPRESS,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.COMPRESS));
    defaultTrieMap.put(cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_EXECUTE,
        Arrays.asList(cn.ololee.fileclassifier.DefaultConstants.SuffixConstants.APK));
    initSuffixTrieNative(defaultTrieMap);
  }

  /**
   * 可以使用任何非0的整数初始化前缀树
   */
  public static void initSuffixTrie(Map<Integer, List<String>> suffixTrieMap) {
    if (suffixTrieMap != null) {
      initSuffixTrieNative(suffixTrieMap);
    }
  }

  /**
   * 从文件的后缀获取文件的类型
   *
   * @param suffix 后缀
   * @return 文件的类型
   */
  public static int getTypeBySuffix(String suffix) {
    if (suffix != null && suffix.length() != 0) {
      return getTypeBySuffixNative(suffix);
    }
    return 0;
  }

  /**
   * 通过文件的全名获得文件的类型
   *
   * @param name 文件的全名，包含后缀
   * @return 文件的类型
   */
  public static int getTypeByName(String name) {
    String suffix = name.substring(name.lastIndexOf(".") + 1);
    return getTypeBySuffix(suffix);
  }

  /**
   * 通过文件获取文件的类型
   */
  public static int getFileType(File file) {
    if (file.isHidden()) {
      return cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_HIDDEN;
    }
    if (file.isDirectory()) {
      return cn.ololee.fileclassifier.DefaultConstants.FileTypeConstants.TYPE_FOLDER;
    }
    return getTypeByName(file.getName().toLowerCase());
  }

  /**
   * 获取某一文件路径下的分类文件
   * Tips:获取文件列表是耗时操作，请在工作线程中执行
   *
   * @param path 文件路径
   * @param fileClassifier 文件划分器，可以定义返回什么样的文件列表，详见{@link FileClassifier}
   * @param fileInfoCallback 文件划分结束后的回调，详见{@link FileInfoCallback}
   */
  public static void getFiles(String path, @NonNull FileClassifier fileClassifier,
      @NonNull FileInfoCallback fileInfoCallback) {
    getFilesNative(path, fileClassifier, fileInfoCallback);
  }

  /**
   * 统计文件的数量，如果path只是一个文件，就没必要使用ndk去统计了，直接就返回1
   *
   * @param path 文件路径
   * @param statisticsType 统计类型
   * @return 文件数量
   */
  public static int getFileCount(String path, int statisticsType) {
    File file = new File(path);
    if (file.isFile()) {
      return 1;
    }
    return getFileCountNative(path, statisticsType);
  }

  /**
   * 获取文件/文件夹的大小，可以是文件，也可以是文件夹
   */
  public static long getFileSize(String path) {
    File file = new File(path);
    if (!file.exists()) {
      return 0;
    }
    return getFileSizeNative(path);
  }

  public static void clearTrie() {
    clearNative(suffixTriePtr);
  }

  private static native void initSuffixTrieNative(Map<Integer, List<String>> suffixTrieMap);

  private static native int getTypeBySuffixNative(String suffix);

  private static native void getFilesNative(
      String path,
      @NonNull FileClassifier fileClassifier,
      FileInfoCallback fileInfoCallback);

  private static native int getFileCountNative(String path,
      @StatisticsTypeAnnotation int statisticsType);

  /**
   * 获取文件或者文件夹的大小(size)，比java快了1.5倍
   *
   * @param path 文件路径
   * @return 件或者文件夹的大小(size)
   */
  private static native long getFileSizeNative(String path);

  /**
   * 清除native层开辟的内存空间
   */
  private static native void clearNative(long suffixTirePtr);
}