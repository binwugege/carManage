package com.feifei.carInit;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.feifei.carInit.carselect.StartCarParking;
import com.feifei.carInit.util.AuthService;
import com.feifei.carInit.util.Base64Util;
import com.feifei.carInit.util.FileUtil;
import com.feifei.carInit.util.HttpUtil;

import java.io.IOException;
import java.net.URLEncoder;
import java.util.logging.Logger;

public class LicensePlate {

    public static String filePath = "C:\\Users\\飞飞超帅的\\Desktop\\1.jpg";
    public static boolean carResult;

    public static void main(String[] args) throws Exception{
        AuthService.getAuth();
       //  carPlateInit();
    }
    public static boolean carPlateInit() throws IOException {
        Logger logger = Logger.getLogger(String.valueOf(LicensePlate.class));
        StartCarParking startCarParking = new StartCarParking();
        String ACCESS_TOKEN = "24.2834bd9b4b6986a9d11e067ca0936aef.2592000.1687010105.282335-32248453";
        // 请求url
        String url = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate";
        try {
            // 本地文件路径

            // 转化成二进制数据
            byte[] imgData = FileUtil.readFileByBytes(filePath);
            // Base64转换
            String imgStr = Base64Util.encode(imgData);
            String imgParam = URLEncoder.encode(imgStr, "UTF-8");
            String param = "image=" + imgParam;
            // 注意这里仅为了简化编码每一次请求都去获取access_token，线上环境access_token有过期时间， 客户端可自行缓存，过期后重新获取。
            String result = HttpUtil.post(url, ACCESS_TOKEN, param);
            JSONObject jsonObject = new JSONObject(JSON.parseObject(result));

            try{
                JSONObject plateData =  (JSONObject)jsonObject.get("words_result");
                Object carPlate = plateData.get("number");
                carResult = startCarParking.startParking((String)carPlate);
                System.out.println(carPlate);
                return carResult;
            }catch (Exception e){
                // e.printStackTrace();
                logger.info("未识别到车牌");
                System.out.println("未识别到车牌");;
                return false;
            }
            // System.out.println(result);
        } catch (Exception e) {
            return false;
            // e.printStackTrace();
        }
    }
}


