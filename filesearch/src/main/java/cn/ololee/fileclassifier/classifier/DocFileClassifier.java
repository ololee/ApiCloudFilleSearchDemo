package cn.ololee.fileclassifier.classifier;

import cn.ololee.fileclassifier.DefaultConstants;

public class DocFileClassifier implements FileClassifier {
  @Override public boolean useMask() {
    return true;
  }

  @Override public int mask() {
    return DefaultConstants.FileTypeConstants.PARENT_MASK;
  }

  @Override public int[] acceptTypes() {
    return new int[] { DefaultConstants.FileTypeConstants.TYPE_DOC };
  }

  @Override public boolean returnFile() {
    return false;
  }
}
