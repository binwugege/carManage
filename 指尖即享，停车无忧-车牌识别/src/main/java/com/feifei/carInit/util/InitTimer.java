package com.feifei.carInit.util;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * 获得上传的时间
 */
public class InitTimer {

    public String geUploadtTimer(){
        Date date = new Date();
        String strDateFormat = "yyyy-MM-dd-HH-mm";
        SimpleDateFormat sdf = new SimpleDateFormat(strDateFormat);
        return sdf.format(date);
    }
}
