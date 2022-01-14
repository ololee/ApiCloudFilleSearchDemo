package cn.ololee.fileclassifier;

/**
 * 内置默认常量
 * 包含文件类型，文件后缀合集；
 * 你可以使用该库，也可以按需制作自己的常量库，然后给按照init的方式构造自己的Map后传递给initSuffixTrie方法初始化前缀树即可；
 * 推荐使用FileTypeConstants划分的方式
 */
public interface DefaultConstants {
  /**
   * 这个常量的划分很巧妙，它使用16进制作为常量的值，而后每一种类型的第9-16位作为父类型
   * 第1-8位作为子类型，而父类型与mask【0xff00】进行与操作后即可得到父类型
   * 这样，当你要doc合集时，你只需要将子类型与父类型进行mask的与操作即可得到所有的doc类型
   */
  interface FileTypeConstants {
    int TYPE_UNKNOWN = 0x00;
    int TYPE_FOLDER = 0x0100;
    int TYPE_IMAGE = 0x0200;
    int TYPE_VIDEO_OR_AUDIO = 0x0300;
    int TYPE_VIDEO = 0x0301;
    int TYPE_AUDIO = 0x0302;
    int TYPE_DOC = 0x0400;
    int TYPE_DOC_PDF = 0x0401;
    int TYPE_DOC_PPT = 0x0402;
    int TYPE_DOC_EXCEL = 0x0403;
    int TYPE_DOC_WORD = 0x0404;
    int TYPE_DOC_TXT = 0x0405;
    int TYPE_DOC_WEB = 0x0406;
    int TYPE_COMPRESS = 0x0500;
    int TYPE_EXECUTE = 0x0600;
    int TYPE_HIDDEN = 0x0700;
    int PARENT_MASK = 0xff00;
  }

  interface SuffixConstants {
    String IMAGES[] = { "png", "gif", "jpe", "jpg", "jpeg", "bmp", "heic", "hif", "jfif", "dib" };
    String AUDIOS[] = {
        "aac", "wav", "mid", "midi", "mp2",
        "mp3", "m4a"
    };
    String VIDEOS[] = {
        "mkv", "mov", "mp4", "mpeg", "mpg", "3gp",
        "avi", "flv", "webm"
    };
    String PDF[] = { "pdf" };
    String PPTS[] = { "ppt", "pptx" };
    String EXCELS[] = { "xls", "xlsx" };
    String WORDS[] = { "doc", "docx" };
    String TXTS[] = { "txt", "log", "rtf", "xml" };
    String COMPRESS[] = { "zip", "rar" };
    String APK[] = { "apk" };
  }

  /**
   * 统计文件的类型
   * onlyFiles：仅文件
   * filesAndDirectorys：文件或文件夹
   */
  interface StatisticsType {
    int onlyFiles = 1;
    int filesAndDirectorys = 2;
  }
}
