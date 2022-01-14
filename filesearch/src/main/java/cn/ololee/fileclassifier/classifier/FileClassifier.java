package cn.ololee.fileclassifier.classifier;

/**
 * 文件划分器 FileClassifier
 */
public interface FileClassifier {
  /**
   * 是否使用mask进行与操作，这样的同类型的划分比遍历快，但不具有通用性；
   * 配合parentMask()传递 要进行与操作的掩码。acceptTypes代表使用
   * 优点：速度快
   * 缺点：通用性差
   *
   * @return true 代表使用掩码方式，false 代表使用遍历方式
   */
  boolean useMask();

  /**
   * 掩码&子类型=>父类型
   */
  int mask();

  /**
   * 接受文件的类型
   */
  int[] acceptTypes();

  /**
   * 返回文件形式还是FileInfo形式
   *
   * @return true 返回文件File类型的链表，false返回FileInfo类型的链表
   */
  boolean returnFile();
}
