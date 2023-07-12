package com.feifei.car.interrepurt;

import org.springframework.web.servlet.HandlerInterceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

// 用户登录的处理
public class LoginInterreput implements HandlerInterceptor {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        String userInfo = request.getParameter("userInfo");
        if( userInfo == null) {
            return false;
        }
        return true;
    }
}
