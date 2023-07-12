package com.feifei.car.control;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.feifei.car.User.User;
import com.feifei.car.common.R;
import com.feifei.car.mapper.UserMapper;
import com.feifei.car.service.CarData;
import lombok.val;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

@ResponseBody
@Controller
@RequestMapping("/car")
public class GetCarStatus {

    @Autowired
    private UserMapper userMapper;

    @Autowired
    private CarData cardata;

    // 是否匹配
    boolean matches;
    // 匹配手机号
    String reg = "^1[3,4,5,6,7,8,9]\\d{9}$";


    @RequestMapping(value = "/getData",method = RequestMethod.GET)
    public R<List<User>> getCarData(){
        QueryWrapper<User> queryWrapper = new QueryWrapper<>();
        queryWrapper.isNotNull("id");
        List<User> users = userMapper.selectList(queryWrapper);
        return R.success(users);
    }

    @RequestMapping(value = "/getOneStatus",method = RequestMethod.GET)
    public R<User> getOneCarStatus(@RequestParam("id") Integer id, HttpServletRequest httpServletRequest){
        QueryWrapper<User> queryWrapper = new QueryWrapper<>();
        queryWrapper.eq("id",id);
        User users = userMapper.selectOne(queryWrapper);
        if(StringUtils.isEmpty(users)){
            return R.error("未查到车位信息");
        }
        return R.success(users);
    }

    @RequestMapping("/booking")
    public R<User> applicationBooking(@RequestParam("id") String id,@RequestParam("phone") String phone){
        QueryWrapper<User> queryWrapper = new QueryWrapper<>();

        if(phone == null){
            return R.error("手机号不能为空");
        }

        matches = Pattern.matches(reg, phone);
        if(!matches){
            return R.error("手机号格式错误");
        }

        queryWrapper.eq("id",id);
        User users = userMapper.selectOne(queryWrapper);
        if(StringUtils.isEmpty(users)){
            return R.error("未查到车位信息");
        }
        if(users.getStatus() == true){
            return R.error("车辆运行中不能预约");
        }
        if(users.getBooking() == true){
            return R.error("预约中");
        }

        User user = new User(phone,true);

        int flag = userMapper.update(user,queryWrapper);
        if(flag == 1){
            return R.success(user);
        }
        return R.error("更改信息失败");
    }

    @RequestMapping("/cancel")
    public R<User> cancleBooking(@RequestParam("id") String id,@RequestParam("phone") String phone){
        QueryWrapper<User> queryWrapper = new QueryWrapper<>();

        if(phone == null){
            return R.error("手机号不能为空");
        }

        matches = Pattern.matches(reg, phone);
        if(!matches){
            return R.error("手机号格式错误");
        }

        queryWrapper.eq("id",id);
        User users = userMapper.selectOne(queryWrapper);
        if(StringUtils.isEmpty(users)){
            return R.error("未查到车位信息");
        }

        if(!phone.equals(users.getPhone())){
            return R.error("您未预约该车位");
        }

        User user = new User("",false);
        int flag = userMapper.update(user,queryWrapper);
        if(flag == 1){
            return R.success(user);
        }
        return R.error("更改信息失败");
    }

    @RequestMapping("/getUserBooking")
    public R<List<User>> getUserBooking(@RequestParam("phone")String phone){

        QueryWrapper<User> queryWrapper = new QueryWrapper<User>();

        if(phone == null){
            return R.error("手机号不能为空");
        }

        matches = Pattern.matches(reg, phone);
        if(!matches){
            return R.error("手机号格式错误");
        }

        queryWrapper.eq("phone",phone).eq("booking",true);
        List<User> users = userMapper.selectList(queryWrapper);

        return R.success(users);
    }

}
