package cn.ololee.fileclassifier;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;
import cn.ololee.fileclassifier.classifier.FileClassifier;
import com.uzmap.pkg.uzcore.UZWebView;
import com.uzmap.pkg.uzcore.uzmodule.UZModule;
import com.uzmap.pkg.uzcore.uzmodule.UZModuleContext;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class FileModule extends UZModule {
  public static final String TAG = "FileModule";

  public FileModule(UZWebView webView) {
    super(webView);
  }

  public void jsmethod_init(UZModuleContext uzModuleContext) {
    FileClassifierHelper.init();
  }

  public void jsmethod_initSuffixTrie(UZModuleContext uzModuleContext) throws JSONException {
    Log.d(TAG, "jsmethod_initSuffixTrie");
    Map<Integer, List<String>> suffixTrieMap4Helper = new HashMap<>();
    JSONObject suffixTrieMap = uzModuleContext.optJSONObject("suffixTrieMap");
    Iterator<String> keys = suffixTrieMap.keys();
    while (keys.hasNext()) {
      String key = keys.next();
      final Integer intKey = Integer.parseInt(key);
      JSONArray suffixListJsonArray = suffixTrieMap.getJSONArray(key);
      int suffixsLength = suffixListJsonArray.length();
      for (int i = 0; i < suffixsLength; i++) {
        String suffix = suffixListJsonArray.getString(i);
        if (suffixTrieMap4Helper.get(intKey) == null) {
          suffixTrieMap4Helper.put(intKey, new ArrayList<>());
        }
        suffixTrieMap4Helper.get(intKey).add(suffix);
      }
    }
    //验证
    Log.d(TAG, suffixTrieMap4Helper.toString());
    FileClassifierHelper.initSuffixTrie(suffixTrieMap4Helper);
  }

  public void jsmethod_getTypeBySuffix(UZModuleContext uzModuleContext) {
    String jsSuffix = uzModuleContext.optString("suffix");
    Log.d(TAG, "jsmethod_getTypeBySuffix:suffix:" + jsSuffix);
    int type = FileClassifierHelper.getTypeBySuffix(jsSuffix);
    Log.d(TAG, "jsmethod_getTypeBySuffix:type:" + type);
    uzModuleContext.success(type + "", true, false);
  }

  public void jsmethod_getTypeByName(UZModuleContext uzModuleContext) {
    String jsName = uzModuleContext.optString("fileName");
    Log.d(TAG, "jsmethod_getTypeByName: fileName:" + jsName);
    int type = FileClassifierHelper.getTypeByName(jsName);
    Log.d(TAG, "jsmethod_getTypeByName:type: " + type);
    uzModuleContext.success(type + "", true, false);
  }

  public void jsmethod_getFileType(UZModuleContext uzModuleContext) throws JSONException {
    String jsPath = uzModuleContext.optString("filePath");
    JSONObject result = new JSONObject();
    JSONObject failedReason = new JSONObject();
    Log.d(TAG, "jsmethod_getFileType: filePath:" + jsPath);
    File file = new File(jsPath);
    if (!file.exists()) {
      failedReason.put("error", "failed!!! because file didn't exist. please check your path");
      uzModuleContext.error(result, failedReason, false);
      return;
    }
    int type = FileClassifierHelper.getFileType(file);
    Log.d(TAG, "jsmethod_getFileType:type: " + type);
    uzModuleContext.success(type + "", true, false);
  }

  public void jsmethod_getFiles(UZModuleContext uzModuleContext) throws JSONException {
    String jsPath = uzModuleContext.optString("filePath");
    Log.d(TAG, "jsmethod_getFileType: filePath:" + jsPath);
    JSONObject jsFileClassifier = uzModuleContext.optJSONObject("fileClassifier");
    Log.d(TAG, "jsmethod_getFileType: jsFileClassifier:" + jsFileClassifier.toString());
    boolean jsUseMask = jsFileClassifier.optBoolean("useMask");
    Log.d(TAG, "jsmethod_getFileType: jsUseMask:" + jsUseMask);
    int jsMask = jsFileClassifier.optInt("mask");
    Log.d(TAG, "jsmethod_getFileType: jsMask:" + jsMask);
    JSONArray jsAcceptTypes = jsFileClassifier.optJSONArray("acceptTypes");
    Log.d(TAG, "jsmethod_getFileType: jsAcceptTypes:" + jsAcceptTypes.toString());
    boolean jsReturnFile = jsFileClassifier.optBoolean("returnFile");
    Log.d(TAG, "jsmethod_getFileType: jsReturnFile:" + jsReturnFile);
    int acceptTypesLength = jsAcceptTypes.length();
    int[] acceptTypesArray = new int[acceptTypesLength];
    for (int i = 0; i < acceptTypesLength; i++) {
      acceptTypesArray[i] = jsAcceptTypes.getInt(i);
    }
    FileClassifier fileClassifier = new FileClassifier() {
      @Override public boolean useMask() {
        return jsUseMask;
      }

      @Override public int mask() {
        return jsMask;
      }

      @Override public int[] acceptTypes() {
        return acceptTypesArray;
      }

      @Override public boolean returnFile() {
        return jsReturnFile;
      }
    };
    FileClassifierHelper.getFiles(jsPath, fileClassifier, new FileInfoCallback() {
      @Override public void updateFileInfoList(List<FileInfo> fileInfoList) {
        JSONObject result = new JSONObject();
        JSONArray array = new JSONArray();
        JSONObject failedReason = new JSONObject();
        try {
          for (FileInfo fileInfo : fileInfoList) {
            JSONObject fileInfoJsonObject = new JSONObject();
            fileInfoJsonObject.put("name", fileInfo.getName());
            fileInfoJsonObject.put("path", fileInfo.getPath());
            fileInfoJsonObject.put("type", fileInfo.getType());
            array.put(fileInfoJsonObject);
          }
          result.put("result", array);
          uzModuleContext.success(result, false);
        } catch (JSONException e) {
          e.printStackTrace();
          try {
            failedReason.put("error", e.getMessage());
          } catch (JSONException jsonException) {
            jsonException.printStackTrace();
          }
          uzModuleContext.error(result, failedReason, true);
        }
      }

      @Override public void updateFileList(List<File> fileList) {
        JSONObject result = new JSONObject();
        JSONArray array = new JSONArray();
        JSONObject failedReason = new JSONObject();
        try {
          for (File fileInfo : fileList) {
            JSONObject fileInfoJsonObject = new JSONObject();
            fileInfoJsonObject.put("name", fileInfo.getName());
            fileInfoJsonObject.put("path", fileInfo.getPath());
            array.put(fileInfoJsonObject);
          }
          result.put("result", array);
          uzModuleContext.success(result, false);
        } catch (JSONException e) {
          e.printStackTrace();
          try {
            failedReason.put("error", e.getMessage());
          } catch (JSONException jsonException) {
            jsonException.printStackTrace();
          }
          uzModuleContext.error(result, failedReason, true);
        }
      }
    });
  }



  public void jsmethod_getFileSize(UZModuleContext uzModuleContext) {
    String filePath = uzModuleContext.optString("filePath");
    Log.d(TAG, "jsmethod_getFileSize:filePath:" + filePath);
    long fileSize = FileClassifierHelper.getFileSize(filePath);
    Log.d(TAG, "jsmethod_getFileSize:size:" + fileSize);
    uzModuleContext.success(fileSize+"",false,false);
  }

  public void jsmethod_getFileCount(UZModuleContext uzModuleContext){
    String filePath = uzModuleContext.optString("filePath");
    int statisticWay = uzModuleContext.optInt("statisticWay");
    Log.d(TAG, "jsmethod_getFileCount:filePath:" + filePath);
    int fileCount = FileClassifierHelper.getFileCount(filePath,statisticWay);
    Log.d(TAG, "jsmethod_getFileCount:count:" + fileCount);
    uzModuleContext.success(fileCount+"",false,false);
  }


  public void jsmethod_androidLog(UZModuleContext uzModuleContext) {
    String value = uzModuleContext.optString("value");
    Log.d("ololeeTAG", value);
    uzModuleContext.success(null, false);
  }

  public void jsmethod_release(UZModuleContext uzModuleContext) {
    FileClassifierHelper.clearTrie();
  }
}
