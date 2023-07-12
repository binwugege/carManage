package com.feifei.car.User;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;


@TableName("car_order")
@Data
public class CarPakingPay {

    @TableId(value = "id",type = IdType.AUTO)
    private Integer id;

    @TableField("carPlate")
    private String carPlate;

    @TableField("start_timer")
    private String startTimer;

    @TableField("end_timer")
    private String endTimer;

    @TableField("isParking")
    private Boolean isParking;

    @TableField("isAccount")
    private Boolean isAccount;
}
