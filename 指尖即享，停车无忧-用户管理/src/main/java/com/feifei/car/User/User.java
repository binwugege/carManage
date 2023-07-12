package com.feifei.car.User;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

@TableName("account")
@Data
public class User {
    private Integer id;
    private String name;
    private String phone;
    private String start_now_time;
    private String start_time;
    private String end_time;
    private Boolean booking;
    private Boolean status;

    public User() {
        this.id = id;
    }

    public User(String phone, Boolean booking) {
        this.phone = phone;
        this.booking = booking;
    }
}
