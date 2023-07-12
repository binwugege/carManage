package com.feifei.carInit.carselect;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.feifei.carInit.util.InitTimer;
import com.feifei.carInit.util.SendHttpCarParking;

import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class StartCarParking {

    public static final String Url = "http://127.0.0.1:8081/pay/startOrder";

    public boolean checkStartParking(String carPlate) throws Exception{
        StartCarParking startCarParking = new StartCarParking();

        if (carPlate.length() >= 7 && carPlate.length() <= 8){
            Pattern p = Pattern.compile("^([京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[a-zA-Z](([DF]((?![IO])[a-zA-Z0-9](?![IO]))[0-9]{4})|([0-9]{5}[DF]))|[京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[A-Z]{1}[A-Z0-9]{4}[A-Z0-9挂学警港澳]{1})$");
            Matcher m = p.matcher(carPlate);
            if (!m.matches()){
                System.out.println("车牌数目错误");
                return false;
            }
            startCarParking.startParking(carPlate);
            System.out.println("111");
            return true;
        }else{
            System.out.println("车牌位数错误");
            return false;
        }
    }

    public boolean startParking( String carPlate ) throws Exception{
        JSONObject initData;    // 接收初始数据
        // JSONObject parkingData; // 接收停车数据
        HashMap<String,Object> hashMap = new HashMap<>();

        InitTimer initTimer =  new InitTimer();
        String result = initTimer.geUploadtTimer();

        hashMap.put("carPlate",carPlate);
        hashMap.put("start_timer",result);

        SendHttpCarParking sendHttpCarParking = new SendHttpCarParking();
        String response = sendHttpCarParking.sendHttpGet(Url,hashMap);
        initData = new JSONObject(JSON.parseObject(response));
        // parkingData = (JSONObject)initData.get("data");  // 获取data字段 没有不可以用这一行
        System.out.println("22");
        if(!((Integer) initData.get("code") == 1)){
            System.out.println("请求错误");
            return false;
        }
        System.out.println("停车成功");
        return true;
    }
}


