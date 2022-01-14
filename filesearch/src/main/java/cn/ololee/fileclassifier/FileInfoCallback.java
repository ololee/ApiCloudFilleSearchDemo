package cn.ololee.fileclassifier;

import cn.ololee.fileclassifier.classifier.FileClassifier;
import java.io.File;
import java.util.List;

/**
 * 获取文件信息的回调；
 * updateFileInfoList和updateFileList只会执行其中之一，取决于
 * {@link FileClassifier} 中的returnFile()的返回值，当返回true时，updateFileList会被回调
 * 返回false时updateFileInfoList会被回调
 */
public interface FileInfoCallback {
  /**
   * 返回FileInfo的列表
   *
   * @param fileInfoList {@link cn.ololee.fileclassifier.FileInfo}
   */
  default void updateFileInfoList(List<cn.ololee.fileclassifier.FileInfo> fileInfoList) {
  }

  /**
   * 返回File的列表
   *
   * @param fileList {@link File}
   */
  default void updateFileList(List<File> fileList) {
  }
}
