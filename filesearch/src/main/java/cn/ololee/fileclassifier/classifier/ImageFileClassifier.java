package cn.ololee.fileclassifier.classifier;

import cn.ololee.fileclassifier.DefaultConstants;

public class ImageFileClassifier implements FileClassifier {
  @Override public boolean useMask() {
    return true;
  }

  @Override public int mask() {
    return DefaultConstants.FileTypeConstants.PARENT_MASK;
  }

  @Override public int[] acceptTypes() {
    return new int[] { DefaultConstants.FileTypeConstants.TYPE_IMAGE };
  }

  @Override public boolean returnFile() {
    return false;
  }
}
