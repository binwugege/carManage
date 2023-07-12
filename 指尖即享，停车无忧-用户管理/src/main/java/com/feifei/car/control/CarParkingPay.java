package com.feifei.car.control;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.feifei.car.User.CarPakingPay;
import com.feifei.car.common.R;
import com.feifei.car.mapper.CarParkingPayMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Controller
@ResponseBody
@RequestMapping("/pay")
public class CarParkingPay {

    public static final String reg = "^([京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[a-zA-Z](([DF]((?![IO])[a-zA-Z0-9](?![IO]))[0-9]{4})|([0-9]{5}[DF]))|[京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[A-Z]{1}[A-Z0-9]{4}[A-Z0-9挂学警港澳]{1})$";

    @Autowired
    CarParkingPayMapper carParkingPayMapper;

    /**
     * @function:获取用户订单
     * @param carPlate
     * @return
     */
    @RequestMapping("/getOrder")
    public R<CarPakingPay> getCarOrder(@RequestParam("carPlate") String carPlate){
        QueryWrapper<CarPakingPay> queryWrapper = new QueryWrapper<>();

        if (carPlate.length() >= 7 && carPlate.length() <= 8){
            Pattern p = Pattern.compile(reg);
            Matcher m = p.matcher(carPlate);
            if (!m.matches()){
                return R.error("error");
            }
            queryWrapper.eq("carPlate",carPlate);
            CarPakingPay carPakingPay =  carParkingPayMapper.selectOne(queryWrapper);
            if(StringUtils.isEmpty(carPakingPay)){
                return R.error("未查询到车牌停车信息");
            }
            return R.success(carPakingPay);
        }else{
            return R.error("车牌号位数错误");
        }
    }

    /**
     * 结算用户订单
     * @param carPlate
     * @return
     */
    @RequestMapping("/accountOrder")
    public R<String> accountCarOrder(@RequestParam("carPlate") String carPlate) {
        QueryWrapper<CarPakingPay> queryWrapper = new QueryWrapper<>();

        if (carPlate.length() >= 7 && carPlate.length() <= 8){
            Pattern p = Pattern.compile(reg);
            Matcher m = p.matcher(carPlate);
            if (!m.matches()){
                return R.error("error");
            }
            queryWrapper.eq("carPlate",carPlate);
            CarPakingPay carPakingPay =  carParkingPayMapper.selectOne(queryWrapper);

            if(StringUtils.isEmpty(carPakingPay)){
                return R.error("未查询到车牌停车信息");
            }

            // 修改用户订单
            CarPakingPay carPakingPay1 = new CarPakingPay();
            carPakingPay1.setIsAccount(true);
            carPakingPay1.setStartTimer("");
            carPakingPay1.setEndTimer("");

            int flag = carParkingPayMapper.update(carPakingPay1,queryWrapper);
            if(flag == 1){
                return R.success("缴费成功");
            }
            return R.error("未知错误");
        }else{
            return R.error("车牌号位数错误");
        }
    }

    /**
     * @function: 用户准备进入车位
     * @param carPlate
     * @param startTimer
     * @return
     */
    @RequestMapping("/startOrder")
    public R<String> startCarOrder(@RequestParam("carPlate") String carPlate,@RequestParam("start_timer") String startTimer) {
        // 操作标识
        int flag;
        QueryWrapper<CarPakingPay> queryWrapper = new QueryWrapper<>();

        if (carPlate.length() >= 7 && carPlate.length() <= 8){
            Pattern p = Pattern.compile(reg);
            Matcher m = p.matcher(carPlate);
            if (!m.matches()){
                return R.error("error");
            }
            queryWrapper.eq("carPlate",carPlate);
            CarPakingPay carPakingPay =  carParkingPayMapper.selectOne(queryWrapper);

            CarPakingPay carPakingPay1 = new CarPakingPay();

            carPakingPay1.setIsParking(true);
            carPakingPay1.setIsAccount(false);
            carPakingPay1.setStartTimer(startTimer);
            carPakingPay1.setEndTimer("");

            if(StringUtils.isEmpty(carPakingPay)){
                carPakingPay1.setCarPlate(carPlate);
                flag = carParkingPayMapper.insert(carPakingPay1);
                if(flag == 1){
                    return R.success("添加成功");
                }
                return R.error("添加错误");
            }
             flag = carParkingPayMapper.update(carPakingPay1,queryWrapper);
            if(flag == 1){
                return R.success("进入车位成功");
            }
            return R.error("未知错误");
        }else{
            return R.error("车牌号位数错误");
        }
    }

    /**
     * @function: 用户准备出车位
     * @param carPlate
     * @return
     */
    @RequestMapping("/outCarParking")
    public R<CarPakingPay> outCarParking(@RequestParam("carPlate") String carPlate) {
        // 操作标识
        QueryWrapper<CarPakingPay> queryWrapper = new QueryWrapper<>();

        if (carPlate.length() >= 7 && carPlate.length() <= 8){
            Pattern p = Pattern.compile(reg);
            Matcher m = p.matcher(carPlate);
            if (!m.matches()){
                return R.error("error");
            }
            queryWrapper.eq("carPlate",carPlate);
            CarPakingPay carPakingPay =  carParkingPayMapper.selectOne(queryWrapper);

            if(StringUtils.isEmpty(carPakingPay)){
                return R.error("查询错误");
            }

            if(carPakingPay.getIsAccount()){
                return R.success(carPakingPay);
            }
            return R.error("未结算");
        }else{
            return R.error("车牌号位数错误");
        }
    }
}
