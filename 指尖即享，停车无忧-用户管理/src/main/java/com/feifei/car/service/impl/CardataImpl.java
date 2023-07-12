package com.feifei.car.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.feifei.car.User.User;
import com.feifei.car.mapper.UserMapper;
import com.feifei.car.service.CarData;
import org.springframework.stereotype.Service;

@Service
public class CardataImpl extends ServiceImpl<UserMapper,User> implements CarData {}
