package com.feifei.carInit.carselect;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.feifei.carInit.util.SendHttpCarParking;

import java.util.HashMap;

public class AccountParkingPay {

    public static final String URL = "http://127.0.0.1:8081/pay/outCarParking";

    public Boolean accountParkingPay( String carPlate )throws Exception{
        JSONObject initData;
        JSONObject carParking;
        SendHttpCarParking sendHttpCarParking = new SendHttpCarParking();

        HashMap<String,Object> hashMap = new HashMap<>();
        hashMap.put("carPlate",carPlate);
        String response = sendHttpCarParking.sendHttpGet(URL,hashMap);
        initData = new JSONObject(JSON.parseObject(response));
        if(!((Integer)initData.get("code") == 1)){
            System.out.println(initData.get("msg"));
            return false;
        }
        carParking = (JSONObject) initData.get("data");
        if(!(Boolean)carParking.get("isAccount")){
            System.out.println("未结算");
            return false;
        }
        System.out.println("已结算");
        return true;
    }
}
