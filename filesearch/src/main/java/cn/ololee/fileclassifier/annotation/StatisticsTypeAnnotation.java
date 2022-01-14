package cn.ololee.fileclassifier.annotation;

import androidx.annotation.IntDef;
import cn.ololee.fileclassifier.DefaultConstants;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.SOURCE)
@IntDef({
    DefaultConstants.StatisticsType.onlyFiles,
    DefaultConstants.StatisticsType.filesAndDirectorys
})
public @interface StatisticsTypeAnnotation {
}
