package com.feifei.car.control;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.feifei.car.User.User;
import com.feifei.car.User.UserLogin;
import com.feifei.car.common.R;
import com.feifei.car.mapper.UserLoginMapper;
import lombok.val;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

@CrossOrigin
@RestController
@RequestMapping("/user")
public class RegisterUser {
    boolean matches;

    String reg = "^1[3,4,5,6,7,8,9]\\d{9}$";

    // 插入新的数据
    private UserLogin userLoginData;

    // 用户查询数据返回
    private UserLogin userLogin;

    @Autowired
    private UserLoginMapper userLoginMapper;

    /**
     * @function 验证用户信息
     * @param username 账号名
     * @param password 密码
     * @param email 邮箱
     * @return error 失败 success 获取成功
     */
    @PostMapping( "/getUserInfo")
    public R<UserLogin> getUserFlag(@RequestParam(value = "username",required = false) String username,
                                    @RequestParam(value = "password",required = false) String password,
                                    @RequestParam(value = "email",required = false) String email){

        QueryWrapper<UserLogin> userLoginQueryWrapper = new QueryWrapper<>();
        System.out.println(username);
        System.out.println(password);
        System.out.println(email);
        if(username == null){
            return R.error("手机号不能为空");
        }
        matches = Pattern.matches(reg, username);
        if(!matches){
            return R.error("手机号格式错误");
        }

        if(password == null){
            return R.error("密码不能为空");
        }

        if(password.length() < 6 || password.length() > 16){
            return R.error("密码长度错误");
        }

        userLoginQueryWrapper.eq("user_name",username);
        userLogin = userLoginMapper.selectOne(userLoginQueryWrapper);

        if(StringUtils.isEmpty(userLogin)){
            return R.error("用户不存在");
        }

        if(!password.equals(userLogin.getPassword())){
            return R.error("密码错误");
        }
        return R.success(userLogin);
   }

    /**
     * @function 注册新用户
     * @param username 账号名
     * @param password 密码
     * @param email 邮箱
     * @return error 失败 success 注册成功
     */
    @PostMapping( "/registerUser")
    public R<UserLogin> registerUser(@RequestParam(value = "username",required = false) String username,
                                    @RequestParam(value = "password",required = false) String password,
                                    @RequestParam(value = "email",required = false) String email){

        QueryWrapper<UserLogin> userLoginQueryWrapper = new QueryWrapper<>();

        matches = Pattern.matches(reg, username);
        if(!matches){
            return R.error("手机号格式错误");
        }

        if(password == null){
            return R.error("密码不能为空");
        }

        if(password.length() < 6 || password.length() > 16){
            return R.error("密码长度错误");
        }


        userLoginQueryWrapper.eq("user_name",username);
        userLogin = userLoginMapper.selectOne(userLoginQueryWrapper);

        if(StringUtils.isEmpty(userLogin)){
            userLoginData = new UserLogin(username,password,email);
            userLoginData.toString();
            userLoginMapper.insert(userLoginData);
            return R.success(userLoginData);
        }
        return R.error("用户已存在");
    }

    /**
     *  获取用户的车牌信息
     * @param phone
     * @return
     */
    @RequestMapping("/getCarPlate")
    public R<String> getCarPlate(@RequestParam("phone") String phone){
        QueryWrapper<UserLogin> queryWrapper = new QueryWrapper<UserLogin>();

        if(phone == null){
            return R.error("手机号不能为空");
        }
        matches = Pattern.matches(reg, phone);
        if(!matches){
            return R.error("手机号格式错误");
        }
        queryWrapper.eq("user_name",phone);
        UserLogin user = userLoginMapper.selectOne(queryWrapper);
        if(user.getCarPlste() == null){
            return R.error("未注册车牌");
        }
        return R.success(user.getCarPlste());
    }

    @RequestMapping("/updateCarPlate")
    public  R<String> isCarNo(@RequestParam("phone")String phone,@RequestParam("carPlate")String carPlate){
        QueryWrapper<UserLogin> queryWrapper = new QueryWrapper<>();

        if(phone == null){
            return R.error("手机号不能为空");
        }
        matches = Pattern.matches(reg, phone);
        if(!matches){
            return R.error("手机号格式错误");
        }

        if (carPlate.length() >= 7 && carPlate.length() <= 8){
            Pattern p = Pattern.compile("^([京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[a-zA-Z](([DF]((?![IO])[a-zA-Z0-9](?![IO]))[0-9]{4})|([0-9]{5}[DF]))|[京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[A-Z]{1}[A-Z0-9]{4}[A-Z0-9挂学警港澳]{1})$");
            Matcher m = p.matcher(carPlate);
            if (!m.matches()){
                return R.error("error");
            }
            UserLogin userLogin = new UserLogin(carPlate);
            queryWrapper.eq("user_name",phone);
            int flag = userLoginMapper.update(userLogin,queryWrapper);
            if( flag == 1){
                return R.success("修改成功");
            }
            return R.error("未知错误");
        }else{
            return R.error("车牌号位数错误");
        }
    }

}
