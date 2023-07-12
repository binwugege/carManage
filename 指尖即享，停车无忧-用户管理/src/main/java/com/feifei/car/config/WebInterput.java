package com.feifei.car.config;

import com.feifei.car.interrepurt.LoginInterreput;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

/**
 * 静态资源的处理拦截 允许访问的资源路径
 */

// @Configuration
public class WebInterput implements WebMvcConfigurer {
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(new LoginInterreput()).
                addPathPatterns("/**").
                excludePathPatterns("/login","/css/**","/car/**","/user/**");
    }
}
