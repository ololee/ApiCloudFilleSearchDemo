package cn.ololee.fileclassifier;

/**
 * 建议的文件信息类
 * 只包含文件名、路径、文件类型
 */
public class FileInfo {
  private String path;
  private String name;
  private int type;

  public FileInfo() {
  }

  public FileInfo(String path, String name, int type) {
    this.path = path;
    this.name = name;
    this.type = type;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public String getPath() {
    return path;
  }

  public void setPath(String path) {
    this.path = path;
  }

  public int getType() {
    return type;
  }

  public void setType(int type) {
    this.type = type;
  }

  @Override public String toString() {
    return "FileInfo{" +
        "name='" + name + '\'' +
        ", path='" + path + '\'' +
        ", type=" + type +
        '}';
  }
}
